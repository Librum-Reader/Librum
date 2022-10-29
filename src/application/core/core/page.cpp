/*
    SPDX-FileCopyrightText: 2004 Enrico Ros <eros.kde@email.it>

    Work sponsored by the LiMux project of the city of Munich:
    SPDX-FileCopyrightText: 2017 Klarälvdalens Datakonsult AB a KDAB Group
   company <info@kdab.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "page.h"
#include "page_p.h"

// qt/kde includes
#include <QDebug>
#include <QDomDocument>
#include <QDomElement>
#include <QHash>
#include <QPixmap>
#include <QSet>
#include <QString>
#include <QUuid>
#include <QVariant>

// local includes
#include <limits>
#include "action.h"
#include "annotations.h"
#include "annotations_p.h"
#include "debug_p.h"
#include "document.h"
#include "document_p.h"
#include "form.h"
#include "form_p.h"
#include "observer.h"
#include "pagecontroller_p.h"
#include "pagesize.h"
#include "pagetransition.h"
#include "rotationjob_p.h"
#include "textpage_p.h"
#include "tile.h"
#include "tilesmanager_p.h"
#include "utils_p.h"

#ifdef PAGE_PROFILE
    #include <QTime>
#endif

using namespace Okular;

static const double distanceConsideredEqual = 25;  // 5px

static void deleteObjectRects(QList<ObjectRect*>& rects,
                              const QSet<ObjectRect::ObjectType>& which)
{
    QList<ObjectRect*>::iterator it = rects.begin();
    for(; it != rects.end();)
    {
        if(which.contains((*it)->objectType()))
        {
            delete *it;
            it = rects.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

PagePrivate::PagePrivate(Page* page, uint n, double w, double h, Rotation o) :
    m_page(page),
    m_number(n),
    m_orientation(o),
    m_width(w),
    m_height(h),
    m_doc(nullptr),
    m_boundingBox(0, 0, 1, 1),
    m_rotation(Rotation0),
    m_text(nullptr),
    m_transition(nullptr),
    m_textSelections(nullptr),
    m_openingAction(nullptr),
    m_closingAction(nullptr),
    m_duration(-1),
    m_isBoundingBoxKnown(false)
{
    // avoid Division-By-Zero problems in the program
    if(m_width <= 0)
    {
        m_width = 1;
    }

    if(m_height <= 0)
    {
        m_height = 1;
    }
}

PagePrivate::~PagePrivate()
{
    qDeleteAll(formfields);
    delete m_openingAction;
    delete m_closingAction;
    delete m_text;
    delete m_transition;
}

PagePrivate* PagePrivate::get(Page* page)
{
    return page ? page->d : nullptr;
}

void PagePrivate::imageRotationDone(RotationJob* job)
{
    TilesManager* tm = tilesManager(job->observer());
    if(tm)
    {
        QPixmap* pixmap = new QPixmap(QPixmap::fromImage(job->image()));
        tm->setPixmap(pixmap, job->rect(), job->isPartialUpdate());
        delete pixmap;
        return;
    }

    QMap<DocumentObserver*, PixmapObject>::iterator it =
        m_pixmaps.find(job->observer());
    if(it != m_pixmaps.end())
    {
        PixmapObject& object = it.value();
        (*object.m_pixmap) = QPixmap::fromImage(job->image());
        object.m_rotation = job->rotation();
        object.m_isPartialPixmap = job->isPartialUpdate();
    }
    else
    {
        PixmapObject object;
        object.m_pixmap = new QPixmap(QPixmap::fromImage(job->image()));
        object.m_rotation = job->rotation();
        object.m_isPartialPixmap = job->isPartialUpdate();

        m_pixmaps.insert(job->observer(), object);
    }
}

QTransform PagePrivate::rotationMatrix() const
{
    return Okular::buildRotationMatrix(m_rotation);
}

/** class Page **/

Page::Page(uint pageNumber, double w, double h, Rotation o) :
    d(new PagePrivate(this, pageNumber, w, h, o))
{
}

Page::~Page()
{
    if(d)
    {
        deletePixmaps();
        deleteRects();
        d->deleteHighlights();
        deleteAnnotations();
        d->deleteTextSelections();
        deleteSourceReferences();

        delete d;
    }
}

int Page::number() const
{
    return d->m_number;
}

Rotation Page::orientation() const
{
    return d->m_orientation;
}

Rotation Page::rotation() const
{
    return d->m_rotation;
}

Rotation Page::totalOrientation() const
{
    return (Rotation)(((int)d->m_orientation + (int)d->m_rotation) % 4);
}

double Page::width() const
{
    return d->m_width;
}

double Page::height() const
{
    return d->m_height;
}

double Page::ratio() const
{
    return d->m_height / d->m_width;
}

NormalizedRect Page::boundingBox() const
{
    return d->m_boundingBox;
}

bool Page::isBoundingBoxKnown() const
{
    return d->m_isBoundingBoxKnown;
}

void Page::setBoundingBox(const NormalizedRect& bbox)
{
    if(d->m_isBoundingBoxKnown && d->m_boundingBox == bbox)
    {
        return;
    }

    // Allow tiny rounding errors (happens during rotation)
    static const double epsilon = 0.00001;
    Q_ASSERT(bbox.left >= -epsilon && bbox.top >= -epsilon &&
             bbox.right <= 1 + epsilon && bbox.bottom <= 1 + epsilon);

    d->m_boundingBox = bbox & NormalizedRect(0., 0., 1., 1.);
    d->m_isBoundingBoxKnown = true;
}

bool Page::hasPixmap(DocumentObserver* observer, int width, int height,
                     const NormalizedRect& rect) const
{
    TilesManager* tm = d->tilesManager(observer);
    if(tm)
    {
        if(width != tm->width() || height != tm->height())
        {
            // FIXME hasPixmap should not be calling setSize on the TilesManager
            // this is not very "const" as this function claims to be
            if(width != -1 && height != -1)
            {
                tm->setSize(width, height);
            }
            return false;
        }

        return tm->hasPixmap(rect);
    }

    QMap<DocumentObserver*, PagePrivate::PixmapObject>::const_iterator it =
        d->m_pixmaps.constFind(observer);
    if(it == d->m_pixmaps.constEnd())
    {
        return false;
    }

    if(width == -1 || height == -1)
    {
        return true;
    }

    if(it.value().m_isPartialPixmap)
    {
        return false;
    }

    const QPixmap* pixmap = it.value().m_pixmap;

    return (pixmap->width() == width && pixmap->height() == height);
}

bool Page::hasTextPage() const
{
    return d->m_text != nullptr;
}

RegularAreaRect* Page::wordAt(const NormalizedPoint& p, QString* word) const
{
    if(d->m_text)
    {
        return d->m_text->wordAt(p, word);
    }

    return nullptr;
}

RegularAreaRect* Page::textArea(TextSelection* selection) const
{
    if(d->m_text)
    {
        return d->m_text->textArea(selection);
    }

    return nullptr;
}

bool Page::hasObjectRect(double x, double y, double xScale, double yScale) const
{
    if(m_rects.isEmpty())
    {
        return false;
    }

    for(ObjectRect* rect : m_rects)
    {
        if(rect->distanceSqr(x, y, xScale, yScale) < distanceConsideredEqual)
        {
            return true;
        }
    }

    return false;
}

bool Page::hasHighlights(int s_id) const
{
    // simple case: have no highlights
    if(m_highlights.isEmpty())
    {
        return false;
    }
    // simple case: we have highlights and no id to match
    if(s_id == -1)
    {
        return true;
    }
    // iterate on the highlights list to find an entry by id
    for(HighlightAreaRect* highlight : m_highlights)
    {
        if(highlight->s_id == s_id)
        {
            return true;
        }
    }
    return false;
}

bool Page::hasTransition() const
{
    return d->m_transition != nullptr;
}

bool Page::hasAnnotations() const
{
    return !m_annotations.isEmpty();
}

RegularAreaRect* Page::findText(int id, const QString& text,
                                SearchDirection direction,
                                Qt::CaseSensitivity caseSensitivity,
                                const RegularAreaRect* lastRect) const
{
    RegularAreaRect* rect = nullptr;
    if(text.isEmpty() || !d->m_text)
    {
        return rect;
    }

    rect = d->m_text->findText(id, text, direction, caseSensitivity, lastRect);
    return rect;
}

QString Page::text(const RegularAreaRect* area) const
{
    return text(area, TextPage::AnyPixelTextAreaInclusionBehaviour);
}

QString Page::text(const RegularAreaRect* area,
                   TextPage::TextAreaInclusionBehaviour b) const
{
    QString ret;

    if(!d->m_text)
    {
        return ret;
    }

    if(area)
    {
        RegularAreaRect rotatedArea = *area;
        rotatedArea.transform(d->rotationMatrix().inverted());

        ret = d->m_text->text(&rotatedArea, b);
    }
    else
    {
        ret = d->m_text->text(nullptr, b);
    }

    return ret;
}

TextEntity::List Page::words(const RegularAreaRect* area,
                             TextPage::TextAreaInclusionBehaviour b) const
{
    TextEntity::List ret;

    if(!d->m_text)
    {
        return ret;
    }

    if(area)
    {
        RegularAreaRect rotatedArea = *area;
        rotatedArea.transform(d->rotationMatrix().inverted());

        ret = d->m_text->words(&rotatedArea, b);
    }
    else
    {
        ret = d->m_text->words(nullptr, b);
    }

    for(auto& retI : ret)
    {
        const TextEntity* orig = retI;
        retI = new TextEntity(orig->text(),
                              new Okular::NormalizedRect(
                                  orig->transformedArea(d->rotationMatrix())));
        delete orig;
    }

    return ret;
}

void PagePrivate::rotateAt(Rotation orientation)
{
    if(orientation == m_rotation)
    {
        return;
    }

    deleteTextSelections();

    if(((int)m_orientation + (int)m_rotation) % 2 !=
       ((int)m_orientation + (int)orientation) % 2)
    {
        qSwap(m_width, m_height);
    }

    Rotation oldRotation = m_rotation;
    m_rotation = orientation;

    /**
     * Rotate the images of the page.
     */
    QMapIterator<DocumentObserver*, PagePrivate::PixmapObject> it(m_pixmaps);
    while(it.hasNext())
    {
        it.next();

        const PagePrivate::PixmapObject& object = it.value();

        RotationJob* job =
            new RotationJob(object.m_pixmap->toImage(), object.m_rotation,
                            m_rotation, it.key());
        job->setPage(this);
        m_doc->m_pageController->addRotationJob(job);
    }

    /**
     * Rotate tiles manager
     */
    QMapIterator<const DocumentObserver*, TilesManager*> i(m_tilesManagers);
    while(i.hasNext())
    {
        i.next();

        TilesManager* tm = i.value();
        if(tm)
        {
            tm->setRotation(m_rotation);
        }
    }

    /**
     * Rotate the object rects on the page.
     */
    const QTransform matrix = rotationMatrix();
    for(ObjectRect* objRect : qAsConst(m_page->m_rects))
    {
        objRect->transform(matrix);
    }

    const QTransform highlightRotationMatrix = Okular::buildRotationMatrix(
        (Rotation)(((int)m_rotation - (int)oldRotation + 4) % 4));
    for(HighlightAreaRect* hlar : qAsConst(m_page->m_highlights))
    {
        hlar->transform(highlightRotationMatrix);
    }
}

void PagePrivate::changeSize(const PageSize& size)
{
    if(size.isNull() || (size.width() == m_width && size.height() == m_height))
    {
        return;
    }

    m_page->deletePixmaps();
    //    deleteHighlights();
    //    deleteTextSelections();

    m_width = size.width();
    m_height = size.height();
    if(m_rotation % 2)
    {
        qSwap(m_width, m_height);
    }
}

const ObjectRect* Page::objectRect(ObjectRect::ObjectType type, double x,
                                   double y, double xScale, double yScale) const
{
    // Walk list in reverse order so that annotations in the foreground are
    // preferred
    QListIterator<ObjectRect*> it(m_rects);
    it.toBack();
    while(it.hasPrevious())
    {
        const ObjectRect* objrect = it.previous();
        if((objrect->objectType() == type) &&
           objrect->distanceSqr(x, y, xScale, yScale) < distanceConsideredEqual)
        {
            return objrect;
        }
    }

    return nullptr;
}

QList<const ObjectRect*> Page::objectRects(ObjectRect::ObjectType type,
                                           double x, double y, double xScale,
                                           double yScale) const
{
    QList<const ObjectRect*> result;

    QListIterator<ObjectRect*> it(m_rects);
    it.toBack();
    while(it.hasPrevious())
    {
        const ObjectRect* objrect = it.previous();
        if((objrect->objectType() == type) &&
           objrect->distanceSqr(x, y, xScale, yScale) < distanceConsideredEqual)
        {
            result.append(objrect);
        }
    }

    return result;
}

const ObjectRect* Page::nearestObjectRect(ObjectRect::ObjectType type, double x,
                                          double y, double xScale,
                                          double yScale, double* distance) const
{
    ObjectRect* res = nullptr;
    double minDistance = std::numeric_limits<double>::max();

    for(ObjectRect* rect : m_rects)
    {
        if(rect->objectType() == type)
        {
            double d = rect->distanceSqr(x, y, xScale, yScale);
            if(d < minDistance)
            {
                res = rect;
                minDistance = d;
            }
        }
    }

    if(distance)
    {
        *distance = minDistance;
    }
    return res;
}

const PageTransition* Page::transition() const
{
    return d->m_transition;
}

QList<Annotation*> Page::annotations() const
{
    return m_annotations;
}

Annotation* Page::annotation(const QString& uniqueName) const
{
    for(Annotation* a : m_annotations)
    {
        if(a->uniqueName() == uniqueName)
        {
            return a;
        }
    }
    return nullptr;
}

const Action* Page::pageAction(PageAction action) const
{
    switch(action)
    {
    case Page::Opening:
        return d->m_openingAction;
        break;
    case Page::Closing:
        return d->m_closingAction;
        break;
    }

    return nullptr;
}

QList<FormField*> Page::formFields() const
{
    return d->formfields;
}

void Page::setPixmap(DocumentObserver* observer, QPixmap* pixmap,
                     const NormalizedRect& rect)
{
    d->setPixmap(observer, pixmap, rect, false /*isPartialPixmap*/);
}

void PagePrivate::setPixmap(DocumentObserver* observer, QPixmap* pixmap,
                            const NormalizedRect& rect, bool isPartialPixmap)
{
    if(m_rotation == Rotation0)
    {
        TilesManager* tm = tilesManager(observer);
        if(tm)
        {
            tm->setPixmap(pixmap, rect, isPartialPixmap);
            delete pixmap;
            return;
        }

        QMap<DocumentObserver*, PagePrivate::PixmapObject>::iterator it =
            m_pixmaps.find(observer);
        if(it != m_pixmaps.end())
        {
            delete it.value().m_pixmap;
        }
        else
        {
            it = m_pixmaps.insert(observer, PagePrivate::PixmapObject());
        }
        it.value().m_pixmap = pixmap;
        it.value().m_rotation = m_rotation;
        it.value().m_isPartialPixmap = isPartialPixmap;
    }
    else
    {
        // it can happen that we get a setPixmap while closing and thus the page
        // controller is gone
        if(m_doc->m_pageController)
        {
            RotationJob* job = new RotationJob(pixmap->toImage(), Rotation0,
                                               m_rotation, observer);
            job->setPage(this);
            job->setRect(TilesManager::toRotatedRect(rect, m_rotation));
            job->setIsPartialUpdate(isPartialPixmap);
            m_doc->m_pageController->addRotationJob(job);
        }

        delete pixmap;
    }
}

void Page::setTextPage(TextPage* textPage)
{
    delete d->m_text;

    d->m_text = textPage;
    if(d->m_text)
    {
        d->m_text->d->m_page = this;
        // Correct/optimize text order for search and text selection
        d->m_text->d->correctTextOrder();
    }
}

void Page::setObjectRects(const QList<ObjectRect*>& rects)
{
    QSet<ObjectRect::ObjectType> which;
    which << ObjectRect::Action << ObjectRect::Image;
    deleteObjectRects(m_rects, which);

    /**
     * Rotate the object rects of the page.
     */
    const QTransform matrix = d->rotationMatrix();

    for(ObjectRect* objectRect : rects)
    {
        objectRect->transform(matrix);
    }

    m_rects << rects;
}

const QList<ObjectRect*>& Page::objectRects() const
{
    return m_rects;
}

void PagePrivate::setHighlight(int s_id, RegularAreaRect* rect,
                               const QColor& color)
{
    HighlightAreaRect* hr = new HighlightAreaRect(rect);
    hr->s_id = s_id;
    hr->color = color;

    m_page->m_highlights.append(hr);
}

void PagePrivate::setTextSelections(RegularAreaRect* r, const QColor& color)
{
    deleteTextSelections();
    if(r)
    {
        HighlightAreaRect* hr = new HighlightAreaRect(r);
        hr->s_id = -1;
        hr->color = color;
        m_textSelections = hr;
        delete r;
    }
}

void Page::setSourceReferences(const QList<SourceRefObjectRect*>& refRects)
{
    deleteSourceReferences();
    for(SourceRefObjectRect* rect : refRects)
    {
        m_rects << rect;
    }
}

void Page::setDuration(double seconds)
{
    d->m_duration = seconds;
}

double Page::duration() const
{
    return d->m_duration;
}

void Page::setLabel(const QString& label)
{
    d->m_label = label;
}

QString Page::label() const
{
    return d->m_label;
}

const RegularAreaRect* Page::textSelection() const
{
    return d->m_textSelections;
}

QColor Page::textSelectionColor() const
{
    return d->m_textSelections ? d->m_textSelections->color : QColor();
}

void Page::addAnnotation(Annotation* annotation)
{
    // Generate uniqueName: okular-{UUID}
    if(annotation->uniqueName().isEmpty())
    {
        QString uniqueName =
            QStringLiteral("okular-") + QUuid::createUuid().toString();
        annotation->setUniqueName(uniqueName);
    }
    annotation->d_ptr->m_page = d;
    m_annotations.append(annotation);

    AnnotationObjectRect* rect = new AnnotationObjectRect(annotation);

    // Rotate the annotation on the page.
    const QTransform matrix = d->rotationMatrix();
    annotation->d_ptr->annotationTransform(matrix);

    m_rects.append(rect);
}

bool Page::removeAnnotation(Annotation* annotation)
{
    if(!d->m_doc->m_parent->canRemovePageAnnotation(annotation))
    {
        return false;
    }

    QList<Annotation*>::iterator aIt = m_annotations.begin();
    for(; aIt != m_annotations.end(); ++aIt)
    {
        if((*aIt) && (*aIt)->uniqueName() == annotation->uniqueName())
        {
            int rectfound = false;
            QList<ObjectRect*>::iterator it = m_rects.begin();
            for(; it != m_rects.end() && !rectfound; ++it)
            {
                if(((*it)->objectType() == ObjectRect::OAnnotation) &&
                   ((*it)->object() == (*aIt)))
                {
                    delete *it;
                    it = m_rects.erase(it);
                    rectfound = true;
                }
            }
            qCDebug(OkularCoreDebug)
                << "removed annotation:" << annotation->uniqueName();
            annotation->d_ptr->m_page = nullptr;
            m_annotations.erase(aIt);
            break;
        }
    }

    return true;
}

void Page::setTransition(PageTransition* transition)
{
    delete d->m_transition;
    d->m_transition = transition;
}

void Page::setPageAction(PageAction action, Action* link)
{
    switch(action)
    {
    case Page::Opening:
        delete d->m_openingAction;
        d->m_openingAction = link;
        break;
    case Page::Closing:
        delete d->m_closingAction;
        d->m_closingAction = link;
        break;
    }
}

void Page::setFormFields(const QList<FormField*>& fields)
{
    qDeleteAll(d->formfields);
    d->formfields = fields;
    for(FormField* ff : qAsConst(d->formfields))
    {
        ff->d_ptr->setDefault();
        ff->d_ptr->m_page = this;
    }
}

void Page::deletePixmap(DocumentObserver* observer)
{
    TilesManager* tm = d->tilesManager(observer);
    if(tm)
    {
        delete tm;
        d->m_tilesManagers.remove(observer);
    }
    else
    {
        PagePrivate::PixmapObject object = d->m_pixmaps.take(observer);
        delete object.m_pixmap;
    }
}

void Page::deletePixmaps()
{
    QMapIterator<DocumentObserver*, PagePrivate::PixmapObject> it(d->m_pixmaps);
    while(it.hasNext())
    {
        it.next();
        delete it.value().m_pixmap;
    }

    d->m_pixmaps.clear();

    qDeleteAll(d->m_tilesManagers);
    d->m_tilesManagers.clear();
}

void Page::deleteRects()
{
    // delete ObjectRects of type Link and Image
    QSet<ObjectRect::ObjectType> which;
    which << ObjectRect::Action << ObjectRect::Image;
    deleteObjectRects(m_rects, which);
}

void PagePrivate::deleteHighlights(int s_id)
{
    // delete highlights by ID
    QList<HighlightAreaRect*>::iterator it = m_page->m_highlights.begin();
    while(it != m_page->m_highlights.end())
    {
        HighlightAreaRect* highlight = *it;
        if(s_id == -1 || highlight->s_id == s_id)
        {
            it = m_page->m_highlights.erase(it);
            delete highlight;
        }
        else
        {
            ++it;
        }
    }
}

void PagePrivate::deleteTextSelections()
{
    delete m_textSelections;
    m_textSelections = nullptr;
}

void Page::deleteSourceReferences()
{
    deleteObjectRects(m_rects, QSet<ObjectRect::ObjectType>()
                                   << ObjectRect::SourceRef);
}

void Page::deleteAnnotations()
{
    // delete ObjectRects of type Annotation
    deleteObjectRects(m_rects, QSet<ObjectRect::ObjectType>()
                                   << ObjectRect::OAnnotation);
    // delete all stored annotations
    qDeleteAll(m_annotations);
    m_annotations.clear();
}

bool PagePrivate::restoreLocalContents(const QDomNode& pageNode)
{
    bool loadedAnything = false;  // set if something actually gets loaded

    // iterate over all children (annotationList, ...)
    QDomNode childNode = pageNode.firstChild();
    while(childNode.isElement())
    {
        QDomElement childElement = childNode.toElement();
        childNode = childNode.nextSibling();

        // parse annotationList child element
        if(childElement.tagName() == QLatin1String("annotationList"))
        {
#ifdef PAGE_PROFILE
            QTime time;
            time.start();
#endif
            // Clone annotationList as root node in restoredLocalAnnotationList
            const QDomNode clonedNode =
                restoredLocalAnnotationList.importNode(childElement, true);
            restoredLocalAnnotationList.appendChild(clonedNode);

            // iterate over all annotations
            QDomNode annotationNode = childElement.firstChild();
            while(annotationNode.isElement())
            {
                // get annotation element and advance to next annot
                QDomElement annotElement = annotationNode.toElement();
                annotationNode = annotationNode.nextSibling();

                // get annotation from the dom element
                Annotation* annotation =
                    AnnotationUtils::createAnnotation(annotElement);

                // append annotation to the list or show warning
                if(annotation)
                {
                    m_doc->performAddPageAnnotation(m_number, annotation);
                    qCDebug(OkularCoreDebug)
                        << "restored annot:" << annotation->uniqueName();
                    loadedAnything = true;
                }
                else
                {
                    qCWarning(OkularCoreDebug).nospace()
                        << "page (" << m_number
                        << "): can't restore an annotation from XML.";
                }
            }
#ifdef PAGE_PROFILE
            qCDebug(OkularCoreDebug).nospace()
                << "annots: XML Load time: " << time.elapsed() << "ms";
#endif
        }
        // parse formList child element
        else if(childElement.tagName() == QLatin1String("forms"))
        {
            // Clone forms as root node in restoredFormFieldList
            const QDomNode clonedNode =
                restoredFormFieldList.importNode(childElement, true);
            restoredFormFieldList.appendChild(clonedNode);

            if(formfields.isEmpty())
            {
                continue;
            }

            QHash<int, FormField*> hashedforms;
            for(FormField* ff : qAsConst(formfields))
            {
                hashedforms[ff->id()] = ff;
            }

            // iterate over all forms
            QDomNode formsNode = childElement.firstChild();
            while(formsNode.isElement())
            {
                // get annotation element and advance to next annot
                QDomElement formElement = formsNode.toElement();
                formsNode = formsNode.nextSibling();

                if(formElement.tagName() != QLatin1String("form"))
                {
                    continue;
                }

                bool ok = true;
                int index =
                    formElement.attribute(QStringLiteral("id")).toInt(&ok);
                if(!ok)
                {
                    continue;
                }

                QHash<int, FormField*>::const_iterator wantedIt =
                    hashedforms.constFind(index);
                if(wantedIt == hashedforms.constEnd())
                {
                    continue;
                }

                QString value = formElement.attribute(QStringLiteral("value"));
                (*wantedIt)->d_ptr->setValue(value);
                loadedAnything = true;
            }
        }
    }

    return loadedAnything;
}

void PagePrivate::saveLocalContents(QDomNode& parentNode,
                                    QDomDocument& document,
                                    PageItems what) const
{
    // create the page node and set the 'number' attribute
    QDomElement pageElement = document.createElement(QStringLiteral("page"));
    pageElement.setAttribute(QStringLiteral("number"), m_number);

    // add annotations info if has got any
    if((what & AnnotationPageItems) && (what & OriginalAnnotationPageItems))
    {
        const QDomElement savedDocRoot =
            restoredLocalAnnotationList.documentElement();
        if(!savedDocRoot.isNull())
        {
            // Import and append node in target document
            const QDomNode importedNode =
                document.importNode(savedDocRoot, true);
            pageElement.appendChild(importedNode);
        }
    }
    else if((what & AnnotationPageItems) && !m_page->m_annotations.isEmpty())
    {
        // create the annotationList
        QDomElement annotListElement =
            document.createElement(QStringLiteral("annotationList"));

        // add every annotation to the annotationList
        for(const Annotation* a : qAsConst(m_page->m_annotations))
        {
            // only save okular annotations (not the embedded in file ones)
            if(!(a->flags() & Annotation::External))
            {
                // append an filled-up element called 'annotation' to the list
                QDomElement annElement =
                    document.createElement(QStringLiteral("annotation"));
                AnnotationUtils::storeAnnotation(a, annElement, document);
                annotListElement.appendChild(annElement);
                qCDebug(OkularCoreDebug)
                    << "save annotation:" << a->uniqueName();
            }
        }

        // append the annotationList element if annotations have been set
        if(annotListElement.hasChildNodes())
        {
            pageElement.appendChild(annotListElement);
        }
    }

    // add forms info if has got any
    if((what & FormFieldPageItems) && (what & OriginalFormFieldPageItems))
    {
        const QDomElement savedDocRoot =
            restoredFormFieldList.documentElement();
        if(!savedDocRoot.isNull())
        {
            // Import and append node in target document
            const QDomNode importedNode =
                document.importNode(savedDocRoot, true);
            pageElement.appendChild(importedNode);
        }
    }
    else if((what & FormFieldPageItems) && !formfields.isEmpty())
    {
        // create the formList
        QDomElement formListElement =
            document.createElement(QStringLiteral("forms"));

        // add every form data to the formList
        for(const FormField* f : formfields)
        {
            QString newvalue = f->d_ptr->value();
            if(f->d_ptr->m_default == newvalue)
            {
                continue;
            }

            // append an filled-up element called 'annotation' to the list
            QDomElement formElement =
                document.createElement(QStringLiteral("form"));
            formElement.setAttribute(QStringLiteral("id"), f->id());
            formElement.setAttribute(QStringLiteral("value"), newvalue);
            formListElement.appendChild(formElement);
        }

        // append the annotationList element if annotations have been set
        if(formListElement.hasChildNodes())
        {
            pageElement.appendChild(formListElement);
        }
    }

    // append the page element only if has children
    if(pageElement.hasChildNodes())
    {
        parentNode.appendChild(pageElement);
    }
}

const QPixmap* Page::_o_nearestPixmap(DocumentObserver* observer, int w,
                                      int h) const
{
    Q_UNUSED(h)

    const QPixmap* pixmap = nullptr;

    // if a pixmap is present for given id, use it
    QMap<DocumentObserver*, PagePrivate::PixmapObject>::const_iterator
        itPixmap = d->m_pixmaps.constFind(observer);
    if(itPixmap != d->m_pixmaps.constEnd())
    {
        pixmap = itPixmap.value().m_pixmap;
    }
    else if(!d->m_pixmaps.isEmpty())
    {
        // else find the closest match using pixmaps of other IDs (great optim!)
        int minDistance = -1;
        QMap<DocumentObserver*, PagePrivate::PixmapObject>::const_iterator
            it = d->m_pixmaps.constBegin(),
            end = d->m_pixmaps.constEnd();
        for(; it != end; ++it)
        {
            int pixWidth = (*it).m_pixmap->width(),
                distance = pixWidth > w ? pixWidth - w : w - pixWidth;
            if(minDistance == -1 || distance < minDistance)
            {
                pixmap = (*it).m_pixmap;
                minDistance = distance;
            }
        }
    }

    return pixmap;
}

bool Page::hasTilesManager(const DocumentObserver* observer) const
{
    return d->tilesManager(observer) != nullptr;
}

const QPixmap* Page::getPixmap(DocumentObserver* observer, int width,
                               int height) const
{
    return _o_nearestPixmap(observer, width, height);
}

QList<Tile> Page::tilesAt(const DocumentObserver* observer,
                          const NormalizedRect& rect) const
{
    TilesManager* tm = d->m_tilesManagers.value(observer);
    if(tm)
    {
        return tm->tilesAt(rect, TilesManager::PixmapTile);
    }
    else
    {
        return QList<Tile>();
    }
}

TilesManager* PagePrivate::tilesManager(const DocumentObserver* observer) const
{
    return m_tilesManagers.value(observer);
}

void PagePrivate::setTilesManager(const DocumentObserver* observer,
                                  TilesManager* tm)
{
    TilesManager* old = m_tilesManagers.value(observer);
    delete old;

    m_tilesManagers.insert(observer, tm);
}

void PagePrivate::adoptGeneratedContents(PagePrivate* oldPage)
{
    rotateAt(oldPage->m_rotation);

    m_pixmaps = oldPage->m_pixmaps;
    oldPage->m_pixmaps.clear();

    m_tilesManagers = oldPage->m_tilesManagers;
    oldPage->m_tilesManagers.clear();

    m_boundingBox = oldPage->m_boundingBox;
    m_isBoundingBoxKnown = oldPage->m_isBoundingBoxKnown;
    m_text = oldPage->m_text;
    oldPage->m_text = nullptr;

    m_textSelections = oldPage->m_textSelections;
    oldPage->m_textSelections = nullptr;

    restoredLocalAnnotationList = oldPage->restoredLocalAnnotationList;
    restoredFormFieldList = oldPage->restoredFormFieldList;
}

FormField* PagePrivate::findEquivalentForm(const Page* p, FormField* oldField)
{
    // given how id is not very good of id (at least for pdf) we do a few passes
    // same rect, type and id
    for(FormField* f : qAsConst(p->d->formfields))
    {
        if(f->rect() == oldField->rect() && f->type() == oldField->type() &&
           f->id() == oldField->id())
        {
            return f;
        }
    }
    // same rect and type
    for(FormField* f : qAsConst(p->d->formfields))
    {
        if(f->rect() == oldField->rect() && f->type() == oldField->type())
        {
            return f;
        }
    }
    // fuzzy rect, same type and id
    for(FormField* f : qAsConst(p->d->formfields))
    {
        if(f->type() == oldField->type() && f->id() == oldField->id() &&
           qFuzzyCompare(f->rect().left, oldField->rect().left) &&
           qFuzzyCompare(f->rect().top, oldField->rect().top) &&
           qFuzzyCompare(f->rect().right, oldField->rect().right) &&
           qFuzzyCompare(f->rect().bottom, oldField->rect().bottom))
        {
            return f;
        }
    }
    // fuzzy rect and same type
    for(FormField* f : qAsConst(p->d->formfields))
    {
        if(f->type() == oldField->type() &&
           qFuzzyCompare(f->rect().left, oldField->rect().left) &&
           qFuzzyCompare(f->rect().top, oldField->rect().top) &&
           qFuzzyCompare(f->rect().right, oldField->rect().right) &&
           qFuzzyCompare(f->rect().bottom, oldField->rect().bottom))
        {
            return f;
        }
    }
    return nullptr;
}

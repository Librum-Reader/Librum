/*
    SPDX-FileCopyrightText: 2006 Pino Toscano <toscano.pino@tiscali.it>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generator_djvu.h"
#include <core/action.h>
#include <core/annotations.h>
#include <core/area.h>
#include <core/document.h>
#include <core/fileprinter.h>
#include <core/page.h>
#include <core/textpage.h>
#include <core/utils.h>
#include <KAboutData>
#include <KLocalizedString>
#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QMutex>
#include <QPixmap>
#include <QPrinter>
#include <QString>
#include <QTemporaryFile>
#include <QUuid>

static void recurseCreateTOC(QDomDocument& maindoc, const QDomNode& parent,
                             QDomNode& parentDestination, KDjVu* djvu)
{
    QDomNode n = parent.firstChild();
    while(!n.isNull())
    {
        QDomElement el = n.toElement();

        QDomElement newel =
            maindoc.createElement(el.attribute(QStringLiteral("title")));
        parentDestination.appendChild(newel);

        QString dest;
        if(!(dest = el.attribute(QStringLiteral("PageNumber"))).isEmpty())
        {
            Okular::DocumentViewport vp;
            vp.pageNumber = dest.toInt() - 1;
            newel.setAttribute(QStringLiteral("Viewport"), vp.toString());
        }
        else if(!(dest = el.attribute(QStringLiteral("PageName"))).isEmpty())
        {
            Okular::DocumentViewport vp;
            vp.pageNumber = djvu->pageNumber(dest);
            newel.setAttribute(QStringLiteral("Viewport"), vp.toString());
        }
        else if(!(dest = el.attribute(QStringLiteral("URL"))).isEmpty())
        {
            newel.setAttribute(QStringLiteral("URL"), dest);
        }

        if(el.hasChildNodes())
        {
            recurseCreateTOC(maindoc, n, newel, djvu);
        }
        n = n.nextSibling();
    }
}

OKULAR_EXPORT_PLUGIN(DjVuGenerator, "libokularGenerator_djvu.json")

DjVuGenerator::DjVuGenerator(QObject* parent, const QVariantList& args) :
    Okular::Generator(parent, args),
    m_docSyn(nullptr)
{
    setFeature(TextExtraction);
    setFeature(Threaded);
    setFeature(PrintPostscript);
    if(Okular::FilePrinter::ps2pdfAvailable())
    {
        setFeature(PrintToFile);
    }

    m_djvu = new KDjVu();
    m_djvu->setCacheEnabled(false);
}

DjVuGenerator::~DjVuGenerator()
{
    delete m_djvu;
}

bool DjVuGenerator::loadDocument(const QString& fileName,
                                 QVector<Okular::Page*>& pagesVector)
{
    QMutexLocker locker(userMutex());
    if(!m_djvu->openFile(fileName))
    {
        return false;
    }

    locker.unlock();

    loadPages(pagesVector, 0);

    return true;
}

bool DjVuGenerator::doCloseDocument()
{
    userMutex()->lock();
    m_djvu->closeFile();
    userMutex()->unlock();

    delete m_docSyn;
    m_docSyn = nullptr;

    return true;
}

QImage DjVuGenerator::image(Okular::PixmapRequest* request)
{
    userMutex()->lock();
    QImage img = m_djvu->image(request->pageNumber(), request->width(),
                               request->height(), request->page()->rotation());
    userMutex()->unlock();
    return img;
}

Okular::DocumentInfo DjVuGenerator::generateDocumentInfo(
    const QSet<Okular::DocumentInfo::Key>& keys) const
{
    Okular::DocumentInfo docInfo;

    if(keys.contains(Okular::DocumentInfo::MimeType))
    {
        docInfo.set(Okular::DocumentInfo::MimeType,
                    QStringLiteral("image/vnd.djvu"));
    }

    if(m_djvu)
    {
        // compile internal structure reading properties from KDjVu
        if(keys.contains(Okular::DocumentInfo::Author))
        {
            docInfo.set(Okular::DocumentInfo::Title,
                        m_djvu->metaData(QStringLiteral("title")).toString());
        }
        if(keys.contains(Okular::DocumentInfo::Author))
        {
            docInfo.set(Okular::DocumentInfo::Author,
                        m_djvu->metaData(QStringLiteral("author")).toString());
        }
        if(keys.contains(Okular::DocumentInfo::CreationDate))
        {
            docInfo.set(Okular::DocumentInfo::CreationDate,
                        m_djvu->metaData(QStringLiteral("year")).toString());
        }
        if(keys.contains(Okular::DocumentInfo::CustomKeys))
        {
            docInfo.set(QStringLiteral("editor"),
                        m_djvu->metaData(QStringLiteral("editor")).toString(),
                        i18n("Editor"));
            docInfo.set(
                QStringLiteral("publisher"),
                m_djvu->metaData(QStringLiteral("publisher")).toString(),
                i18n("Publisher"));
            docInfo.set(QStringLiteral("volume"),
                        m_djvu->metaData(QStringLiteral("volume")).toString(),
                        i18n("Volume"));
            docInfo.set(
                QStringLiteral("documentType"),
                m_djvu->metaData(QStringLiteral("documentType")).toString(),
                i18n("Type of document"));
            QVariant numcomponents =
                m_djvu->metaData(QStringLiteral("componentFile"));
            docInfo.set(
                QStringLiteral("componentFile"),
                numcomponents.type() != QVariant::Int
                    ? i18nc("Unknown number of component files", "Unknown")
                    : numcomponents.toString(),
                i18n("Component Files"));
        }
    }

    return docInfo;
}

const Okular::DocumentSynopsis* DjVuGenerator::generateDocumentSynopsis()
{
    QMutexLocker locker(userMutex());
    if(m_docSyn)
    {
        return m_docSyn;
    }

    const QDomDocument* doc = m_djvu->documentBookmarks();
    if(doc)
    {
        m_docSyn = new Okular::DocumentSynopsis();
        recurseCreateTOC(*m_docSyn, *doc, *m_docSyn, m_djvu);
    }
    locker.unlock();

    return m_docSyn;
}

Okular::Document::PrintError DjVuGenerator::print(QPrinter& printer)
{
    // Create tempfile to write to
    QTemporaryFile tf(QDir::tempPath() + QLatin1String("/okular_XXXXXX.ps"));
    if(!tf.open())
    {
        return Okular::Document::TemporaryFileOpenPrintError;
    }
    const QString fileName = tf.fileName();

    QMutexLocker locker(userMutex());
    QList<int> pageList = Okular::FilePrinter::pageList(
        printer, m_djvu->pages().count(), document()->currentPage() + 1,
        document()->bookmarkedPageList());

    if(m_djvu->exportAsPostScript(&tf, pageList))
    {
        tf.setAutoRemove(false);
        tf.close();
        return Okular::FilePrinter::printFile(
            printer, fileName, document()->orientation(),
            Okular::FilePrinter::SystemDeletesFiles,
            Okular::FilePrinter::ApplicationSelectsPages,
            document()->bookmarkedPageRange());
    }

    return Okular::Document::UnknownPrintError;
}

QVariant DjVuGenerator::metaData(const QString& key,
                                 const QVariant& option) const
{
    Q_UNUSED(option)
    if(key == QLatin1String("DocumentTitle"))
    {
        return m_djvu->metaData(QStringLiteral("title"));
    }
    return QVariant();
}

Okular::TextPage* DjVuGenerator::textPage(Okular::TextRequest* request)
{
    userMutex()->lock();
    const Okular::Page* page = request->page();
    QList<KDjVu::TextEntity> te;
    if(te.isEmpty())
    {
        te = m_djvu->textEntities(page->number(), QStringLiteral("word"));
    }
    if(te.isEmpty())
    {
        te = m_djvu->textEntities(page->number(), QStringLiteral("line"));
    }
    userMutex()->unlock();
    QList<KDjVu::TextEntity>::ConstIterator it = te.constBegin();
    QList<KDjVu::TextEntity>::ConstIterator itEnd = te.constEnd();
    QList<Okular::TextEntity*> words;
    const KDjVu::Page* djvupage = m_djvu->pages().at(page->number());
    for(; it != itEnd; ++it)
    {
        const KDjVu::TextEntity& cur = *it;
        words.append(new Okular::TextEntity(
            cur.text(),
            new Okular::NormalizedRect(cur.rect(), djvupage->width(),
                                       djvupage->height())));
    }
    Okular::TextPage* textpage = new Okular::TextPage(words);
    return textpage;
}

void DjVuGenerator::loadPages(QVector<Okular::Page*>& pagesVector, int rotation)
{
    const QVector<KDjVu::Page*>& djvu_pages = m_djvu->pages();
    int numofpages = djvu_pages.count();
    pagesVector.resize(numofpages);

    for(int i = 0; i < numofpages; ++i)
    {
        const KDjVu::Page* p = djvu_pages.at(i);
        if(pagesVector[i])
        {
            delete pagesVector[i];
        }
        int w = p->width();
        int h = p->height();
        if(rotation % 2 == 1)
        {
            qSwap(w, h);
        }
        Okular::Page* page = new Okular::Page(
            i, w, h, (Okular::Rotation)(p->orientation() + rotation));
        pagesVector[i] = page;

        QList<KDjVu::Annotation*> annots;
        QList<KDjVu::Link*> links;
        userMutex()->lock();
        m_djvu->linksAndAnnotationsForPage(i, &links, &annots);
        userMutex()->unlock();
        if(!links.isEmpty())
        {
            QList<Okular::ObjectRect*> rects;
            QList<KDjVu::Link*>::ConstIterator it = links.constBegin();
            QList<KDjVu::Link*>::ConstIterator itEnd = links.constEnd();
            for(; it != itEnd; ++it)
            {
                KDjVu::Link* curlink = (*it);
                Okular::ObjectRect* newrect = convertKDjVuLink(i, curlink);
                if(newrect)
                {
                    rects.append(newrect);
                }
                // delete the links as soon as we process them
                delete curlink;
            }
            if(rects.count() > 0)
            {
                page->setObjectRects(rects);
            }
        }
        if(!annots.isEmpty())
        {
            QList<KDjVu::Annotation*>::ConstIterator it = annots.constBegin();
            QList<KDjVu::Annotation*>::ConstIterator itEnd = annots.constEnd();
            for(; it != itEnd; ++it)
            {
                KDjVu::Annotation* ann = (*it);
                Okular::Annotation* newann = convertKDjVuAnnotation(w, h, ann);
                if(newann)
                {
                    page->addAnnotation(newann);
                }
                // delete the annotations as soon as we process them
                delete ann;
            }
        }
    }
}

Okular::ObjectRect* DjVuGenerator::convertKDjVuLink(int page,
                                                    KDjVu::Link* link) const
{
    Okular::Action* newlink = nullptr;
    Okular::ObjectRect* newrect = nullptr;
    switch(link->type())
    {
    case KDjVu::Link::PageLink:
    {
        KDjVu::PageLink* l = static_cast<KDjVu::PageLink*>(link);
        bool ok = true;
        QString target = l->page();
        if((target.length() > 0) && target.at(0) == QLatin1Char('#'))
        {
            target.remove(0, 1);
        }
        int tmppage = target.toInt(&ok);
        if(ok || target.isEmpty())
        {
            Okular::DocumentViewport vp;
            if(!target.isEmpty())
            {
                vp.pageNumber = (target.at(0) == QLatin1Char('+') ||
                                 target.at(0) == QLatin1Char('-'))
                                    ? page + tmppage
                                    : tmppage - 1;
            }
            newlink = new Okular::GotoAction(QString(), vp);
        }
        break;
    }
    case KDjVu::Link::UrlLink:
    {
        KDjVu::UrlLink* l = static_cast<KDjVu::UrlLink*>(link);
        QString url = l->url();
        newlink = new Okular::BrowseAction(QUrl(url));
        break;
    }
    }
    if(newlink)
    {
        const KDjVu::Page* p = m_djvu->pages().at(page);
        int width = p->width();
        int height = p->height();
        bool scape_orientation =
            false;  // hack by tokoe, should always create default page
        if(scape_orientation)
        {
            qSwap(width, height);
        }
        switch(link->areaType())
        {
        case KDjVu::Link::RectArea:
        case KDjVu::Link::EllipseArea:
        {
            QRect r(QPoint(link->point().x(), p->height() - link->point().y() -
                                                  link->size().height()),
                    link->size());
            bool ellipse = (link->areaType() == KDjVu::Link::EllipseArea);
            newrect = new Okular::ObjectRect(
                Okular::NormalizedRect(
                    Okular::Utils::rotateRect(r, width, height, 0), width,
                    height),
                ellipse, Okular::ObjectRect::Action, newlink);
            break;
        }
        case KDjVu::Link::PolygonArea:
        {
            QPolygon poly = link->polygon();
            QPolygonF newpoly;
            for(int i = 0; i < poly.count(); ++i)
            {
                int x = poly.at(i).x();
                int y = poly.at(i).y();
                if(scape_orientation)
                {
                    qSwap(x, y);
                }
                else
                {
                    y = height - y;
                }
                newpoly << QPointF((double)(x) / width, (double)(y) / height);
            }
            if(!newpoly.isEmpty())
            {
                newpoly << newpoly.first();
                newrect = new Okular::ObjectRect(
                    newpoly, Okular::ObjectRect::Action, newlink);
            }
            break;
        }
        default:;
        }
        if(!newrect)
        {
            delete newlink;
        }
    }
    return newrect;
}

Okular::Annotation* DjVuGenerator::convertKDjVuAnnotation(
    int w, int h, KDjVu::Annotation* ann) const
{
    Okular::Annotation* newann = nullptr;
    switch(ann->type())
    {
    case KDjVu::Annotation::TextAnnotation:
    {
        KDjVu::TextAnnotation* txtann =
            static_cast<KDjVu::TextAnnotation*>(ann);
        Okular::TextAnnotation* newtxtann = new Okular::TextAnnotation();
        // boundary
        QRect rect(QPoint(txtann->point().x(),
                          h - txtann->point().y() - txtann->size().height()),
                   txtann->size());
        newtxtann->setBoundingRectangle(Okular::NormalizedRect(
            Okular::Utils::rotateRect(rect, w, h, 0), w, h));
        // type
        newtxtann->setTextType(txtann->inlineText()
                                   ? Okular::TextAnnotation::InPlace
                                   : Okular::TextAnnotation::Linked);
        newtxtann->style().setOpacity(txtann->color().alphaF());
        // FIXME remove once the annotation text handling is fixed
        newtxtann->setContents(ann->comment());
        newann = newtxtann;
        break;
    }
    case KDjVu::Annotation::LineAnnotation:
    {
        KDjVu::LineAnnotation* lineann =
            static_cast<KDjVu::LineAnnotation*>(ann);
        Okular::LineAnnotation* newlineann = new Okular::LineAnnotation();
        // boundary
        QPoint a(lineann->point().x(), h - lineann->point().y());
        QPoint b(lineann->point2().x(), h - lineann->point2().y());
        QRect rect = QRect(a, b).normalized();
        newlineann->setBoundingRectangle(Okular::NormalizedRect(
            Okular::Utils::rotateRect(rect, w, h, 0), w, h));
        // line points
        QList<Okular::NormalizedPoint> points;
        points.append(Okular::NormalizedPoint(a.x(), a.y(), w, h));
        points.append(Okular::NormalizedPoint(b.x(), b.y(), w, h));
        newlineann->setLinePoints(points);
        // arrow?
        if(lineann->isArrow())
        {
            newlineann->setLineEndStyle(Okular::LineAnnotation::OpenArrow);
        }
        // width
        newlineann->style().setWidth(lineann->width());
        newann = newlineann;
        break;
    }
    }
    if(newann)
    {
        // setting the common parameters
        newann->style().setColor(ann->color());
        newann->setContents(ann->comment());
        // creating an id as name for the annotation
        QString uid = QUuid::createUuid().toString();
        uid.remove(0, 1);
        uid.chop(1);
        uid.remove(QLatin1Char('-'));
        newann->setUniqueName(uid);
        // is external
        newann->setFlags(newann->flags() | Okular::Annotation::External);
    }
    return newann;
}

#include "generator_djvu.moc"

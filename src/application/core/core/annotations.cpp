/*
    SPDX-FileCopyrightText: 2005 Enrico Ros <eros.kde@email.it>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "annotations.h"
#include "annotations_p.h"

// qt/kde includes
#include <QApplication>
#include <QColor>
#include <QIcon>
#include <QPainter>
#include <QStandardPaths>
#include <QSvgRenderer>

// DBL_MAX
#include <float.h>

// local includes
#include "action.h"
#include "document.h"
#include "document_p.h"
#include "movie.h"
#include "page_p.h"
#include "sound.h"

using namespace Okular;

/**
 * True, if point @p c lies to the left of the vector from @p a to @p b
 * @internal
 */
static bool isLeftOfVector(const NormalizedPoint &a, const NormalizedPoint &b, const NormalizedPoint &c)
{
    // cross product
    return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)) > 0;
}

/**
 * @brief Calculates distance of the given point @p x @p y @p xScale @p yScale to the @p path
 *
 * Does piecewise comparison and selects the distance to the closest segment
 */
static double distanceSqr(double x, double y, double xScale, double yScale, const QList<NormalizedPoint> &path)
{
    double distance = DBL_MAX;
    double thisDistance;
    QList<NormalizedPoint>::const_iterator i = path.constBegin();
    NormalizedPoint lastPoint = *i;

    for (++i; i != path.constEnd(); ++i) {
        thisDistance = NormalizedPoint::distanceSqr(x, y, xScale, yScale, lastPoint, (*i));

        if (thisDistance < distance) {
            distance = thisDistance;
        }

        lastPoint = *i;
    }
    return distance;
}

/**
 * Given the squared @p distance from the idealized 0-width line and a pen width @p penWidth,
 * (not squared!), returns the final distance
 *
 * @warning The returned distance is not exact:
 * We calculate an (exact) squared distance to the ideal (centered) line, and then subtract
 * the squared width of the pen:
 * a^2 - b^2 where a = "distance from idealized 0-width line" b = "pen width"
 * For an exact result, we would want to calculate "(a - b)^2" but that would require
 * a square root operation because we only know the squared distance a^2.
 *
 * However, the approximation is feasible, because:
 * error = (a-b)^2 - (a^2 - b^2) = -2ab + 2b^2 = 2b(b - a)
 * Therefore:
 * lim_{a->b} a^2 - b^2 - a^2 + 2ab - b^2 --> 0
 *
 * In other words, this approximation will estimate the distance to be slightly more than it actually is
 * for as long as we are far "outside" the line, becoming more accurate the closer we get to the line
 * boundary. Trivially, it also fulfils (a1 < a2) => ((a1^2 - b^2) < (a2^2 - b^2)) making it monotonic.
 * "Inside" of the drawn line, the distance is 0 anyway.
 */
static double strokeDistance(double distance, double penWidth)
{
    return fmax(distance - pow(penWidth, 2), 0);
}

// BEGIN AnnotationUtils implementation
Annotation *AnnotationUtils::createAnnotation(const QDomElement &annElement)
{
    // safety check on annotation element
    if (!annElement.hasAttribute(QStringLiteral("type"))) {
        return nullptr;
    }

    // build annotation of given type
    Annotation *annotation = nullptr;
    int typeNumber = annElement.attribute(QStringLiteral("type")).toInt();
    switch (typeNumber) {
    case Annotation::AText:
        annotation = new TextAnnotation(annElement);
        break;
    case Annotation::ALine:
        annotation = new LineAnnotation(annElement);
        break;
    case Annotation::AGeom:
        annotation = new GeomAnnotation(annElement);
        break;
    case Annotation::AHighlight:
        annotation = new HighlightAnnotation(annElement);
        break;
    case Annotation::AStamp:
        annotation = new StampAnnotation(annElement);
        break;
    case Annotation::AInk:
        annotation = new InkAnnotation(annElement);
        break;
    case Annotation::ACaret:
        annotation = new CaretAnnotation(annElement);
        break;
    }

    // return created annotation
    return annotation;
}

void AnnotationUtils::storeAnnotation(const Annotation *ann, QDomElement &annElement, QDomDocument &document)
{
    // save annotation's type as element's attribute
    annElement.setAttribute(QStringLiteral("type"), (uint)ann->subType());

    // append all annotation data as children of this node
    ann->store(annElement, document);
}

QDomElement AnnotationUtils::findChildElement(const QDomNode &parentNode, const QString &name)
{
    // loop through the whole children and return a 'name' named element
    QDomNode subNode = parentNode.firstChild();
    while (subNode.isElement()) {
        QDomElement element = subNode.toElement();
        if (element.tagName() == name) {
            return element;
        }
        subNode = subNode.nextSibling();
    }
    // if the name can't be found, return a dummy null element
    return QDomElement();
}

QRect AnnotationUtils::annotationGeometry(const Annotation *annotation, double scaleX, double scaleY)
{
    const QRect rect = annotation->transformedBoundingRectangle().geometry((int)scaleX, (int)scaleY);
    if (annotation->subType() == Annotation::AText && (((TextAnnotation *)annotation)->textType() == TextAnnotation::Linked)) {
        // To be honest i have no clue of why the 24,24 is here, maybe to make sure it's not too small?
        // But why only for linked text?
        const QRect rect24 = QRect((int)(annotation->transformedBoundingRectangle().left * scaleX), (int)(annotation->transformedBoundingRectangle().top * scaleY), 24, 24);
        return rect24.united(rect);
    }

    return rect;
}

QPixmap AnnotationUtils::loadStamp(const QString &nameOrPath, int size, bool keepAspectRatio)
{
    const QString name = nameOrPath.toLower();

    static std::unique_ptr<QSvgRenderer> svgStampFile;
    if (!svgStampFile.get()) {
        const QString stampFile = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("okular/pics/stamps.svg"));
        if (!stampFile.isEmpty()) {
            svgStampFile = std::make_unique<QSvgRenderer>(stampFile);
            if (!svgStampFile->isValid()) {
                svgStampFile.reset();
            }
        }
    }

    QSvgRenderer *r = svgStampFile.get();
    if (r && r->isValid() && r->elementExists(name)) {
        const QSize stampSize = r->boundsOnElement(name).size().toSize();
        const QSize pixmapSize = stampSize.scaled(size, size, keepAspectRatio ? Qt::KeepAspectRatioByExpanding : Qt::IgnoreAspectRatio);
        QPixmap pixmap(pixmapSize);
        pixmap.fill(Qt::transparent);
        QPainter p(&pixmap);
        r->render(&p, name);
        p.end();
        return pixmap;
    }

    // _name is a path (do this before loading as icon name to avoid some rare weirdness )
    QPixmap pixmap;
    pixmap.load(nameOrPath);
    if (!pixmap.isNull()) {
        pixmap = pixmap.scaled(size, size, keepAspectRatio ? Qt::KeepAspectRatioByExpanding : Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        return pixmap;
    }

    // _name is an icon name
    return QIcon::fromTheme(name).pixmap(size);
}
// END AnnotationUtils implementation

AnnotationProxy::AnnotationProxy()
{
}

AnnotationProxy::~AnnotationProxy()
{
}

// BEGIN Annotation implementation

class Annotation::Style::Private
{
public:
    Private()
        : m_opacity(1.0)
        , m_width(1.0)
        , m_style(Solid)
        , m_xCorners(0.0)
        , m_yCorners(0.0)
        , m_marks(3)
        , m_spaces(0)
        , m_effect(NoEffect)
        , m_effectIntensity(1.0)
    {
    }

    QColor m_color;
    double m_opacity;
    double m_width;
    LineStyle m_style;
    double m_xCorners;
    double m_yCorners;
    int m_marks;
    int m_spaces;
    LineEffect m_effect;
    double m_effectIntensity;
};

Annotation::Style::Style()
    : d(new Private)
{
}

Annotation::Style::~Style()
{
    delete d;
}

Annotation::Style::Style(const Style &other)
    : d(new Private)
{
    *d = *other.d;
}

Annotation::Style &Annotation::Style::operator=(const Style &other)
{
    if (this != &other) {
        *d = *other.d;
    }

    return *this;
}

void Annotation::Style::setColor(const QColor &color)
{
    d->m_color = color;
}

QColor Annotation::Style::color() const
{
    return d->m_color;
}

void Annotation::Style::setOpacity(double opacity)
{
    d->m_opacity = opacity;
}

double Annotation::Style::opacity() const
{
    return d->m_opacity;
}

void Annotation::Style::setWidth(double width)
{
    d->m_width = width;
}

double Annotation::Style::width() const
{
    return d->m_width;
}

void Annotation::Style::setLineStyle(LineStyle style)
{
    d->m_style = style;
}

Annotation::LineStyle Annotation::Style::lineStyle() const
{
    return d->m_style;
}

void Annotation::Style::setXCorners(double xCorners)
{
    d->m_xCorners = xCorners;
}

double Annotation::Style::xCorners() const
{
    return d->m_xCorners;
}

void Annotation::Style::setYCorners(double yCorners)
{
    d->m_yCorners = yCorners;
}

double Annotation::Style::yCorners() const
{
    return d->m_yCorners;
}

void Annotation::Style::setMarks(int marks)
{
    d->m_marks = marks;
}

int Annotation::Style::marks() const
{
    return d->m_marks;
}

void Annotation::Style::setSpaces(int spaces)
{
    d->m_spaces = spaces;
}

int Annotation::Style::spaces() const
{
    return d->m_spaces;
}

void Annotation::Style::setLineEffect(LineEffect effect)
{
    d->m_effect = effect;
}

Annotation::LineEffect Annotation::Style::lineEffect() const
{
    return d->m_effect;
}

void Annotation::Style::setEffectIntensity(double intensity)
{
    d->m_effectIntensity = intensity;
}

double Annotation::Style::effectIntensity() const
{
    return d->m_effectIntensity;
}

class Annotation::Window::Private
{
public:
    Private()
        : m_flags(-1)
        , m_width(0)
        , m_height(0)
    {
    }

    int m_flags;
    NormalizedPoint m_topLeft;
    int m_width;
    int m_height;
    QString m_title;
    QString m_summary;
};

Annotation::Window::Window()
    : d(new Private)
{
}

Annotation::Window::~Window()
{
    delete d;
}

Annotation::Window::Window(const Window &other)
    : d(new Private)
{
    *d = *other.d;
}

Annotation::Window &Annotation::Window::operator=(const Window &other)
{
    if (this != &other) {
        *d = *other.d;
    }

    return *this;
}

void Annotation::Window::setFlags(int flags)
{
    d->m_flags = flags;
}

int Annotation::Window::flags() const
{
    return d->m_flags;
}

void Annotation::Window::setTopLeft(const NormalizedPoint &point)
{
    d->m_topLeft = point;
}

NormalizedPoint Annotation::Window::topLeft() const
{
    return d->m_topLeft;
}

void Annotation::Window::setWidth(int width)
{
    d->m_width = width;
}

int Annotation::Window::width() const
{
    return d->m_width;
}

void Annotation::Window::setHeight(int height)
{
    d->m_height = height;
}

int Annotation::Window::height() const
{
    return d->m_height;
}

void Annotation::Window::setTitle(const QString &title)
{
    d->m_title = title;
}

QString Annotation::Window::title() const
{
    return d->m_title;
}

void Annotation::Window::setSummary(const QString &summary)
{
    d->m_summary = summary;
}

QString Annotation::Window::summary() const
{
    return d->m_summary;
}

class Annotation::Revision::Private
{
public:
    Private()
        : m_annotation(nullptr)
        , m_scope(Reply)
        , m_type(None)
    {
    }

    Annotation *m_annotation;
    RevisionScope m_scope;
    RevisionType m_type;
};

Annotation::Revision::Revision()
    : d(new Private)
{
}

Annotation::Revision::~Revision()
{
    delete d;
}

Annotation::Revision::Revision(const Revision &other)
    : d(new Private)
{
    *d = *other.d;
}

Annotation::Revision &Annotation::Revision::operator=(const Revision &other)
{
    if (this != &other) {
        *d = *other.d;
    }

    return *this;
}

void Annotation::Revision::setAnnotation(Annotation *annotation)
{
    d->m_annotation = annotation;
}

Annotation *Annotation::Revision::annotation() const
{
    return d->m_annotation;
}

void Annotation::Revision::setScope(RevisionScope scope)
{
    d->m_scope = scope;
}

Annotation::RevisionScope Annotation::Revision::scope() const
{
    return d->m_scope;
}

void Annotation::Revision::setType(RevisionType type)
{
    d->m_type = type;
}

Annotation::RevisionType Annotation::Revision::type() const
{
    return d->m_type;
}

AnnotationPrivate::AnnotationPrivate()
    : m_page(nullptr)
    , m_flags(0)
    , m_disposeFunc(nullptr)
{
}

AnnotationPrivate::~AnnotationPrivate()
{
    // delete all children revisions
    if (m_revisions.isEmpty()) {
        return;
    }

    for (const Annotation::Revision &revision : qAsConst(m_revisions)) {
        delete revision.annotation();
    }
}

AnnotationPrivate *AnnotationPrivate::get(Annotation *a)
{
    return a ? a->d_ptr : nullptr;
}

Annotation::Annotation(AnnotationPrivate &dd)
    : d_ptr(&dd)
{
}

Annotation::Annotation(AnnotationPrivate &dd, const QDomNode &description)
    : d_ptr(&dd)
{
    d_ptr->setAnnotationProperties(description);
}

Annotation::~Annotation()
{
    if (d_ptr->m_disposeFunc) {
        d_ptr->m_disposeFunc(this);
    }

    delete d_ptr;
}

void Annotation::setAuthor(const QString &author)
{
    Q_D(Annotation);
    d->m_author = author;
}

QString Annotation::author() const
{
    Q_D(const Annotation);
    return d->m_author;
}

void Annotation::setContents(const QString &contents)
{
    Q_D(Annotation);
    d->m_contents = contents;
}

QString Annotation::contents() const
{
    Q_D(const Annotation);
    return d->m_contents;
}

void Annotation::setUniqueName(const QString &name)
{
    Q_D(Annotation);
    d->m_uniqueName = name;
}

QString Annotation::uniqueName() const
{
    Q_D(const Annotation);
    return d->m_uniqueName;
}

void Annotation::setModificationDate(const QDateTime &date)
{
    Q_D(Annotation);
    d->m_modifyDate = date;
}

QDateTime Annotation::modificationDate() const
{
    Q_D(const Annotation);
    return d->m_modifyDate;
}

void Annotation::setCreationDate(const QDateTime &date)
{
    Q_D(Annotation);
    d->m_creationDate = date;
}

QDateTime Annotation::creationDate() const
{
    Q_D(const Annotation);
    return d->m_creationDate;
}

void Annotation::setFlags(int flags)
{
    Q_D(Annotation);
    d->m_flags = flags;
}

int Annotation::flags() const
{
    Q_D(const Annotation);
    return d->m_flags;
}

void Annotation::setBoundingRectangle(const NormalizedRect &rectangle)
{
    Q_D(Annotation);
    d->m_boundary = rectangle;
    d->resetTransformation();
    if (d->m_page) {
        d->transform(d->m_page->rotationMatrix());
    }
}

NormalizedRect Annotation::boundingRectangle() const
{
    Q_D(const Annotation);
    return d->m_boundary;
}

NormalizedRect Annotation::transformedBoundingRectangle() const
{
    Q_D(const Annotation);
    return d->m_transformedBoundary;
}

void Annotation::translate(const NormalizedPoint &coord)
{
    Q_D(Annotation);
    d->translate(coord);
    d->resetTransformation();
    if (d->m_page) {
        d->transform(d->m_page->rotationMatrix());
    }
}

void Annotation::adjust(const NormalizedPoint &deltaCoord1, const NormalizedPoint &deltaCoord2)
{
    Q_D(Annotation);
    d->adjust(deltaCoord1, deltaCoord2);
    d->resetTransformation();
    if (d->m_page) {
        d->transform(d->m_page->rotationMatrix());
    }
}

bool Annotation::openDialogAfterCreation() const
{
    Q_D(const Annotation);
    return d->openDialogAfterCreation();
}

Annotation::Style &Annotation::style()
{
    Q_D(Annotation);
    return d->m_style;
}

const Annotation::Style &Annotation::style() const
{
    Q_D(const Annotation);
    return d->m_style;
}

Annotation::Window &Annotation::window()
{
    Q_D(Annotation);
    return d->m_window;
}

const Annotation::Window &Annotation::window() const
{
    Q_D(const Annotation);
    return d->m_window;
}

QList<Annotation::Revision> &Annotation::revisions()
{
    Q_D(Annotation);
    return d->m_revisions;
}

const QList<Annotation::Revision> &Annotation::revisions() const
{
    Q_D(const Annotation);
    return d->m_revisions;
}

void Annotation::setNativeId(const QVariant &id)
{
    Q_D(Annotation);
    d->m_nativeId = id;
}

QVariant Annotation::nativeId() const
{
    Q_D(const Annotation);
    return d->m_nativeId;
}

void Annotation::setDisposeDataFunction(DisposeDataFunction func)
{
    Q_D(Annotation);
    d->m_disposeFunc = func;
}

bool Annotation::canBeMoved() const
{
    Q_D(const Annotation);

    // Don't move annotations if they cannot be modified
    if (!d->m_page || !d->m_page->m_doc->m_parent->canModifyPageAnnotation(this)) {
        return false;
    }

    // highlight "requires" to be "bounded" to text, and that's tricky for now
    if (subType() == AHighlight) {
        return false;
    }

    return true;
}

bool Annotation::canBeResized() const
{
    Q_D(const Annotation);

    // Don't resize annotations if they cannot be modified
    if (!d->m_page || !d->m_page->m_doc->m_parent->canModifyPageAnnotation(this)) {
        return false;
    }

    return d->canBeResized();
}

void Annotation::store(QDomNode &annNode, QDomDocument &document) const
{
    Q_D(const Annotation);
    // create [base] element of the annotation node
    QDomElement e = document.createElement(QStringLiteral("base"));
    annNode.appendChild(e);

    // store -contents- attributes
    if (!d->m_author.isEmpty()) {
        e.setAttribute(QStringLiteral("author"), d->m_author);
    }
    if (!d->m_contents.isEmpty()) {
        e.setAttribute(QStringLiteral("contents"), d->m_contents);
    }
    if (!d->m_uniqueName.isEmpty()) {
        e.setAttribute(QStringLiteral("uniqueName"), d->m_uniqueName);
    }
    if (d->m_modifyDate.isValid()) {
        e.setAttribute(QStringLiteral("modifyDate"), d->m_modifyDate.toString(Qt::ISODate));
    }
    if (d->m_creationDate.isValid()) {
        e.setAttribute(QStringLiteral("creationDate"), d->m_creationDate.toString(Qt::ISODate));
    }

    // store -other- attributes
    if (d->m_flags) { // Strip internal flags
        e.setAttribute(QStringLiteral("flags"), d->m_flags & ~(External | ExternallyDrawn | BeingMoved | BeingResized));
    }
    if (d->m_style.color().isValid()) {
        e.setAttribute(QStringLiteral("color"), d->m_style.color().name(QColor::HexArgb));
    }
    if (d->m_style.opacity() != 1.0) {
        e.setAttribute(QStringLiteral("opacity"), QString::number(d->m_style.opacity()));
    }

    // Sub-Node-1 - boundary
    QDomElement bE = document.createElement(QStringLiteral("boundary"));
    e.appendChild(bE);
    bE.setAttribute(QStringLiteral("l"), QString::number(d->m_boundary.left));
    bE.setAttribute(QStringLiteral("t"), QString::number(d->m_boundary.top));
    bE.setAttribute(QStringLiteral("r"), QString::number(d->m_boundary.right));
    bE.setAttribute(QStringLiteral("b"), QString::number(d->m_boundary.bottom));

    // Sub-Node-2 - penStyle
    if (d->m_style.width() != 1 || d->m_style.lineStyle() != Solid || d->m_style.xCorners() != 0 || d->m_style.yCorners() != 0.0 || d->m_style.marks() != 3 || d->m_style.spaces() != 0) {
        QDomElement psE = document.createElement(QStringLiteral("penStyle"));
        e.appendChild(psE);
        psE.setAttribute(QStringLiteral("width"), QString::number(d->m_style.width()));
        psE.setAttribute(QStringLiteral("style"), (int)d->m_style.lineStyle());
        psE.setAttribute(QStringLiteral("xcr"), QString::number(d->m_style.xCorners()));
        psE.setAttribute(QStringLiteral("ycr"), QString::number(d->m_style.yCorners()));
        psE.setAttribute(QStringLiteral("marks"), d->m_style.marks());
        psE.setAttribute(QStringLiteral("spaces"), d->m_style.spaces());
    }

    // Sub-Node-3 - penEffect
    if (d->m_style.lineEffect() != NoEffect || d->m_style.effectIntensity() != 1.0) {
        QDomElement peE = document.createElement(QStringLiteral("penEffect"));
        e.appendChild(peE);
        peE.setAttribute(QStringLiteral("effect"), (int)d->m_style.lineEffect());
        peE.setAttribute(QStringLiteral("intensity"), QString::number(d->m_style.effectIntensity()));
    }

    // Sub-Node-4 - window
    if (d->m_window.flags() != -1 || !d->m_window.title().isEmpty() || !d->m_window.summary().isEmpty()) {
        QDomElement wE = document.createElement(QStringLiteral("window"));
        e.appendChild(wE);
        wE.setAttribute(QStringLiteral("flags"), d->m_window.flags());
        wE.setAttribute(QStringLiteral("top"), QString::number(d->m_window.topLeft().x));
        wE.setAttribute(QStringLiteral("left"), QString::number(d->m_window.topLeft().y));
        wE.setAttribute(QStringLiteral("width"), d->m_window.width());
        wE.setAttribute(QStringLiteral("height"), d->m_window.height());
        wE.setAttribute(QStringLiteral("title"), d->m_window.title());
        wE.setAttribute(QStringLiteral("summary"), d->m_window.summary());
    }

    // create [revision] element of the annotation node (if any)
    if (d->m_revisions.isEmpty()) {
        return;
    }

    // add all revisions as children of revisions element
    for (const Revision &revision : qAsConst(d->m_revisions)) {
        // create revision element
        QDomElement r = document.createElement(QStringLiteral("revision"));
        annNode.appendChild(r);
        // set element attributes
        r.setAttribute(QStringLiteral("revScope"), (int)revision.scope());
        r.setAttribute(QStringLiteral("revType"), (int)revision.type());
        // use revision as the annotation element, so fill it up
        AnnotationUtils::storeAnnotation(revision.annotation(), r, document);
    }
}

QDomNode Annotation::getAnnotationPropertiesDomNode() const
{
    QDomDocument doc(QStringLiteral("documentInfo"));
    QDomElement node = doc.createElement(QStringLiteral("annotation"));

    store(node, doc);
    return node;
}

void Annotation::setAnnotationProperties(const QDomNode &node)
{
    // Save off internal properties that aren't contained in node
    Okular::PagePrivate *p = d_ptr->m_page;
    QVariant nativeID = d_ptr->m_nativeId;
    const int internalFlags = d_ptr->m_flags & (External | ExternallyDrawn | BeingMoved | BeingResized);
    Annotation::DisposeDataFunction disposeFunc = d_ptr->m_disposeFunc;

    // Replace AnnotationPrivate object with a fresh copy
    AnnotationPrivate *new_d_ptr = d_ptr->getNewAnnotationPrivate();
    delete (d_ptr);
    d_ptr = new_d_ptr;

    // Set the annotations properties from node
    d_ptr->setAnnotationProperties(node);

    // Restore internal properties
    d_ptr->m_page = p;
    d_ptr->m_nativeId = nativeID;
    d_ptr->m_flags = d_ptr->m_flags | internalFlags;
    d_ptr->m_disposeFunc = disposeFunc;

    // Transform annotation to current page rotation
    d_ptr->transform(d_ptr->m_page->rotationMatrix());
}

double AnnotationPrivate::distanceSqr(double x, double y, double xScale, double yScale) const
{
    return m_transformedBoundary.distanceSqr(x, y, xScale, yScale);
}

void AnnotationPrivate::annotationTransform(const QTransform &matrix)
{
    resetTransformation();
    transform(matrix);
}

void AnnotationPrivate::transform(const QTransform &matrix)
{
    m_transformedBoundary.transform(matrix);
}

void AnnotationPrivate::baseTransform(const QTransform &matrix)
{
    m_boundary.transform(matrix);
}

void AnnotationPrivate::resetTransformation()
{
    m_transformedBoundary = m_boundary;
}

void AnnotationPrivate::translate(const NormalizedPoint &coord)
{
    m_boundary.left = m_boundary.left + coord.x;
    m_boundary.right = m_boundary.right + coord.x;
    m_boundary.top = m_boundary.top + coord.y;
    m_boundary.bottom = m_boundary.bottom + coord.y;
}

void AnnotationPrivate::adjust(const NormalizedPoint &deltaCoord1, const NormalizedPoint &deltaCoord2)
{
    m_boundary.left = m_boundary.left + qBound(-m_boundary.left, deltaCoord1.x, m_boundary.right - m_boundary.left);
    m_boundary.top = m_boundary.top + qBound(-m_boundary.top, deltaCoord1.y, m_boundary.bottom - m_boundary.top);
    ;
    m_boundary.right = m_boundary.right + qBound(m_boundary.left - m_boundary.right, deltaCoord2.x, 1. - m_boundary.right);
    m_boundary.bottom = m_boundary.bottom + qBound(m_boundary.top - m_boundary.bottom, deltaCoord2.y, 1. - m_boundary.bottom);
}

bool AnnotationPrivate::openDialogAfterCreation() const
{
    return false;
}

void AnnotationPrivate::setAnnotationProperties(const QDomNode &node)
{
    // get the [base] element of the annotation node
    QDomElement e = AnnotationUtils::findChildElement(node, QStringLiteral("base"));
    if (e.isNull()) {
        return;
    }

    // parse -contents- attributes
    if (e.hasAttribute(QStringLiteral("author"))) {
        m_author = e.attribute(QStringLiteral("author"));
    }
    if (e.hasAttribute(QStringLiteral("contents"))) {
        m_contents = e.attribute(QStringLiteral("contents"));
    }
    if (e.hasAttribute(QStringLiteral("uniqueName"))) {
        m_uniqueName = e.attribute(QStringLiteral("uniqueName"));
    }
    if (e.hasAttribute(QStringLiteral("modifyDate"))) {
        m_modifyDate = QDateTime::fromString(e.attribute(QStringLiteral("modifyDate")), Qt::ISODate);
    }
    if (e.hasAttribute(QStringLiteral("creationDate"))) {
        m_creationDate = QDateTime::fromString(e.attribute(QStringLiteral("creationDate")), Qt::ISODate);
    }

    // parse -other- attributes
    if (e.hasAttribute(QStringLiteral("flags"))) {
        m_flags = e.attribute(QStringLiteral("flags")).toInt();
    }
    if (e.hasAttribute(QStringLiteral("color"))) {
        m_style.setColor(QColor(e.attribute(QStringLiteral("color"))));
    }
    if (e.hasAttribute(QStringLiteral("opacity"))) {
        m_style.setOpacity(e.attribute(QStringLiteral("opacity")).toDouble());
    }

    // parse -the-subnodes- (describing Style, Window, Revision(s) structures)
    // Note: all subnodes if present must be 'attributes complete'
    QDomNode eSubNode = e.firstChild();
    while (eSubNode.isElement()) {
        QDomElement ee = eSubNode.toElement();
        eSubNode = eSubNode.nextSibling();

        // parse boundary
        if (ee.tagName() == QLatin1String("boundary")) {
            m_boundary = NormalizedRect(ee.attribute(QStringLiteral("l")).toDouble(), ee.attribute(QStringLiteral("t")).toDouble(), ee.attribute(QStringLiteral("r")).toDouble(), ee.attribute(QStringLiteral("b")).toDouble());
        }
        // parse penStyle if not default
        else if (ee.tagName() == QLatin1String("penStyle")) {
            m_style.setWidth(ee.attribute(QStringLiteral("width")).toDouble());
            m_style.setLineStyle((Annotation::LineStyle)ee.attribute(QStringLiteral("style")).toInt());
            m_style.setXCorners(ee.attribute(QStringLiteral("xcr")).toDouble());
            m_style.setYCorners(ee.attribute(QStringLiteral("ycr")).toDouble());
            m_style.setMarks(ee.attribute(QStringLiteral("marks")).toInt());
            m_style.setSpaces(ee.attribute(QStringLiteral("spaces")).toInt());
        }
        // parse effectStyle if not default
        else if (ee.tagName() == QLatin1String("penEffect")) {
            m_style.setLineEffect((Annotation::LineEffect)ee.attribute(QStringLiteral("effect")).toInt());
            m_style.setEffectIntensity(ee.attribute(QStringLiteral("intensity")).toDouble());
        }
        // parse window if present
        else if (ee.tagName() == QLatin1String("window")) {
            m_window.setFlags(ee.attribute(QStringLiteral("flags")).toInt());
            m_window.setTopLeft(NormalizedPoint(ee.attribute(QStringLiteral("top")).toDouble(), ee.attribute(QStringLiteral("left")).toDouble()));
            m_window.setWidth(ee.attribute(QStringLiteral("width")).toInt());
            m_window.setHeight(ee.attribute(QStringLiteral("height")).toInt());
            m_window.setTitle(ee.attribute(QStringLiteral("title")));
            m_window.setSummary(ee.attribute(QStringLiteral("summary")));
        }
    }

    // get the [revisions] element of the annotation node
    QDomNode revNode = node.firstChild();
    for (; revNode.isElement(); revNode = revNode.nextSibling()) {
        QDomElement revElement = revNode.toElement();
        if (revElement.tagName() != QLatin1String("revision")) {
            continue;
        }

        // compile the Revision structure crating annotation
        Annotation::Revision revision;
        revision.setScope((Annotation::RevisionScope)revElement.attribute(QStringLiteral("revScope")).toInt());
        revision.setType((Annotation::RevisionType)revElement.attribute(QStringLiteral("revType")).toInt());
        revision.setAnnotation(AnnotationUtils::createAnnotation(revElement));

        // if annotation is valid, add revision to internal list
        if (revision.annotation()) {
            m_revisions.append(revision);
        }
    }

    m_transformedBoundary = m_boundary;
}

bool AnnotationPrivate::canBeResized() const
{
    return false;
}

// END Annotation implementation

/** TextAnnotation [Annotation] */

class Okular::TextAnnotationPrivate : public Okular::AnnotationPrivate
{
public:
    TextAnnotationPrivate()
        : AnnotationPrivate()
        , m_textType(TextAnnotation::Linked)
        , m_textIcon(QStringLiteral("Comment"))
        , m_inplaceAlign(0)
        , m_inplaceIntent(TextAnnotation::Unknown)
    {
    }

    void transform(const QTransform &matrix) override;
    void baseTransform(const QTransform &matrix) override;
    void resetTransformation() override;
    void translate(const NormalizedPoint &coord) override;
    bool openDialogAfterCreation() const override;
    void setAnnotationProperties(const QDomNode &node) override;
    bool canBeResized() const override;
    AnnotationPrivate *getNewAnnotationPrivate() override;

    TextAnnotation::TextType m_textType;
    QString m_textIcon;
    QFont m_textFont;
    QColor m_textColor;
    int m_inplaceAlign;
    NormalizedPoint m_inplaceCallout[3];
    NormalizedPoint m_transformedInplaceCallout[3];
    TextAnnotation::InplaceIntent m_inplaceIntent;
};

/*
  The default textIcon for text annotation is Note as the PDF Reference says
*/
TextAnnotation::TextAnnotation()
    : Annotation(*new TextAnnotationPrivate())
{
}

TextAnnotation::TextAnnotation(const QDomNode &description)
    : Annotation(*new TextAnnotationPrivate(), description)
{
}

TextAnnotation::~TextAnnotation()
{
}

void TextAnnotation::setTextType(TextType textType)
{
    Q_D(TextAnnotation);
    d->m_textType = textType;
}

TextAnnotation::TextType TextAnnotation::textType() const
{
    Q_D(const TextAnnotation);
    return d->m_textType;
}

void TextAnnotation::setTextIcon(const QString &icon)
{
    Q_D(TextAnnotation);
    d->m_textIcon = icon;
}

QString TextAnnotation::textIcon() const
{
    Q_D(const TextAnnotation);
    return d->m_textIcon;
}

void TextAnnotation::setTextFont(const QFont &font)
{
    Q_D(TextAnnotation);
    d->m_textFont = font;
}

QFont TextAnnotation::textFont() const
{
    Q_D(const TextAnnotation);
    return d->m_textFont;
}

void TextAnnotation::setTextColor(const QColor &color)
{
    Q_D(TextAnnotation);
    d->m_textColor = color;
}

QColor TextAnnotation::textColor() const
{
    Q_D(const TextAnnotation);
    return d->m_textColor;
}

void TextAnnotation::setInplaceAlignment(int alignment)
{
    Q_D(TextAnnotation);
    d->m_inplaceAlign = alignment;
}

int TextAnnotation::inplaceAlignment() const
{
    Q_D(const TextAnnotation);
    return d->m_inplaceAlign;
}

void TextAnnotation::setInplaceCallout(const NormalizedPoint &point, int index)
{
    if (index < 0 || index > 2) {
        return;
    }

    Q_D(TextAnnotation);
    d->m_inplaceCallout[index] = point;
}

NormalizedPoint TextAnnotation::inplaceCallout(int index) const
{
    if (index < 0 || index > 2) {
        return NormalizedPoint();
    }

    Q_D(const TextAnnotation);
    return d->m_inplaceCallout[index];
}

NormalizedPoint TextAnnotation::transformedInplaceCallout(int index) const
{
    if (index < 0 || index > 2) {
        return NormalizedPoint();
    }

    Q_D(const TextAnnotation);
    return d->m_transformedInplaceCallout[index];
}

void TextAnnotation::setInplaceIntent(InplaceIntent intent)
{
    Q_D(TextAnnotation);
    d->m_inplaceIntent = intent;
}

TextAnnotation::InplaceIntent TextAnnotation::inplaceIntent() const
{
    Q_D(const TextAnnotation);
    return d->m_inplaceIntent;
}

Annotation::SubType TextAnnotation::subType() const
{
    return AText;
}

void TextAnnotation::store(QDomNode &node, QDomDocument &document) const
{
    Q_D(const TextAnnotation);
    // recurse to parent objects storing properties
    Annotation::store(node, document);

    // create [text] element
    QDomElement textElement = document.createElement(QStringLiteral("text"));
    node.appendChild(textElement);

    // store the optional attributes
    if (d->m_textType != Linked) {
        textElement.setAttribute(QStringLiteral("type"), (int)d->m_textType);
    }
    if (!d->m_textIcon.isEmpty()) {
        textElement.setAttribute(QStringLiteral("icon"), d->m_textIcon);
    }
    if (d->m_textFont != QApplication::font()) {
        textElement.setAttribute(QStringLiteral("font"), d->m_textFont.toString());
    }
    if (d->m_textColor.isValid()) {
        textElement.setAttribute(QStringLiteral("fontColor"), d->m_textColor.name());
    }
    if (d->m_inplaceAlign) {
        textElement.setAttribute(QStringLiteral("align"), d->m_inplaceAlign);
    }
    if (d->m_inplaceIntent != Unknown) {
        textElement.setAttribute(QStringLiteral("intent"), (int)d->m_inplaceIntent);
    }

    // Sub-Node - callout
    if (d->m_inplaceCallout[0].x != 0.0) {
        QDomElement calloutElement = document.createElement(QStringLiteral("callout"));
        textElement.appendChild(calloutElement);
        calloutElement.setAttribute(QStringLiteral("ax"), QString::number(d->m_inplaceCallout[0].x));
        calloutElement.setAttribute(QStringLiteral("ay"), QString::number(d->m_inplaceCallout[0].y));
        calloutElement.setAttribute(QStringLiteral("bx"), QString::number(d->m_inplaceCallout[1].x));
        calloutElement.setAttribute(QStringLiteral("by"), QString::number(d->m_inplaceCallout[1].y));
        calloutElement.setAttribute(QStringLiteral("cx"), QString::number(d->m_inplaceCallout[2].x));
        calloutElement.setAttribute(QStringLiteral("cy"), QString::number(d->m_inplaceCallout[2].y));
    }
}

void TextAnnotationPrivate::transform(const QTransform &matrix)
{
    AnnotationPrivate::transform(matrix);

    for (NormalizedPoint &np : m_transformedInplaceCallout) {
        np.transform(matrix);
    }
}

void TextAnnotationPrivate::baseTransform(const QTransform &matrix)
{
    AnnotationPrivate::baseTransform(matrix);

    for (NormalizedPoint &np : m_inplaceCallout) {
        np.transform(matrix);
    }
}

void TextAnnotationPrivate::resetTransformation()
{
    AnnotationPrivate::resetTransformation();

    for (int i = 0; i < 3; ++i) {
        m_transformedInplaceCallout[i] = m_inplaceCallout[i];
    }
}

void TextAnnotationPrivate::translate(const NormalizedPoint &coord)
{
    AnnotationPrivate::translate(coord);

#define ADD_COORD(c1, c2)                                                                                                                                                                                                                      \
    {                                                                                                                                                                                                                                          \
        c1.x = c1.x + c2.x;                                                                                                                                                                                                                    \
        c1.y = c1.y + c2.y;                                                                                                                                                                                                                    \
    }
    ADD_COORD(m_inplaceCallout[0], coord)
    ADD_COORD(m_inplaceCallout[1], coord)
    ADD_COORD(m_inplaceCallout[2], coord)
#undef ADD_COORD
}

bool TextAnnotationPrivate::openDialogAfterCreation() const
{
    return (m_textType == Okular::TextAnnotation::Linked) || (m_inplaceIntent == TextAnnotation::InplaceIntent::Unknown);
}

void TextAnnotationPrivate::setAnnotationProperties(const QDomNode &node)
{
    Okular::AnnotationPrivate::setAnnotationProperties(node);

    // loop through the whole children looking for a 'text' element
    QDomNode subNode = node.firstChild();
    while (subNode.isElement()) {
        QDomElement e = subNode.toElement();
        subNode = subNode.nextSibling();
        if (e.tagName() != QLatin1String("text")) {
            continue;
        }

        // parse the attributes
        if (e.hasAttribute(QStringLiteral("type"))) {
            m_textType = (TextAnnotation::TextType)e.attribute(QStringLiteral("type")).toInt();
        }
        if (e.hasAttribute(QStringLiteral("icon"))) {
            m_textIcon = e.attribute(QStringLiteral("icon"));
        }
        if (e.hasAttribute(QStringLiteral("font"))) {
            m_textFont.fromString(e.attribute(QStringLiteral("font")));
        }
        if (e.hasAttribute(QStringLiteral("fontColor"))) {
            m_textColor = QColor(e.attribute(QStringLiteral("fontColor")));
        }
        if (e.hasAttribute(QStringLiteral("align"))) {
            m_inplaceAlign = e.attribute(QStringLiteral("align")).toInt();
        }
        if (e.hasAttribute(QStringLiteral("intent"))) {
            m_inplaceIntent = (TextAnnotation::InplaceIntent)e.attribute(QStringLiteral("intent")).toInt();
        }

        // parse the subnodes
        QDomNode eSubNode = e.firstChild();
        while (eSubNode.isElement()) {
            QDomElement ee = eSubNode.toElement();
            eSubNode = eSubNode.nextSibling();

            if (ee.tagName() == QLatin1String("escapedText")) {
                m_contents = ee.firstChild().toCDATASection().data();
            } else if (ee.tagName() == QLatin1String("callout")) {
                m_inplaceCallout[0].x = ee.attribute(QStringLiteral("ax")).toDouble();
                m_inplaceCallout[0].y = ee.attribute(QStringLiteral("ay")).toDouble();
                m_inplaceCallout[1].x = ee.attribute(QStringLiteral("bx")).toDouble();
                m_inplaceCallout[1].y = ee.attribute(QStringLiteral("by")).toDouble();
                m_inplaceCallout[2].x = ee.attribute(QStringLiteral("cx")).toDouble();
                m_inplaceCallout[2].y = ee.attribute(QStringLiteral("cy")).toDouble();
            }
        }

        // loading complete
        break;
    }

    for (int i = 0; i < 3; ++i) {
        m_transformedInplaceCallout[i] = m_inplaceCallout[i];
    }
}

bool TextAnnotationPrivate::canBeResized() const
{
    if (m_textType != TextAnnotation::Linked) {
        return true;
    }
    return false;
}

AnnotationPrivate *TextAnnotationPrivate::getNewAnnotationPrivate()
{
    return new TextAnnotationPrivate();
}

/** LineAnnotation [Annotation] */

class Okular::LineAnnotationPrivate : public Okular::AnnotationPrivate
{
public:
    LineAnnotationPrivate()
        : AnnotationPrivate()
        , m_lineStartStyle(LineAnnotation::None)
        , m_lineEndStyle(LineAnnotation::None)
        , m_lineClosed(false)
        , m_lineShowCaption(false)
        , m_lineLeadingFwdPt(0)
        , m_lineLeadingBackPt(0)
        , m_lineIntent(LineAnnotation::Unknown)
    {
    }

    void transform(const QTransform &matrix) override;
    void baseTransform(const QTransform &matrix) override;
    void resetTransformation() override;
    void translate(const NormalizedPoint &coord) override;
    double distanceSqr(double x, double y, double xScale, double yScale) const override;
    void setAnnotationProperties(const QDomNode &node) override;
    AnnotationPrivate *getNewAnnotationPrivate() override;

    QList<NormalizedPoint> m_linePoints;
    QList<NormalizedPoint> m_transformedLinePoints;
    LineAnnotation::TermStyle m_lineStartStyle;
    LineAnnotation::TermStyle m_lineEndStyle;
    bool m_lineClosed : 1;
    bool m_lineShowCaption : 1;
    QColor m_lineInnerColor;
    double m_lineLeadingFwdPt;
    double m_lineLeadingBackPt;
    LineAnnotation::LineIntent m_lineIntent;
};

LineAnnotation::LineAnnotation()
    : Annotation(*new LineAnnotationPrivate())
{
}

LineAnnotation::LineAnnotation(const QDomNode &description)
    : Annotation(*new LineAnnotationPrivate(), description)
{
}

LineAnnotation::~LineAnnotation()
{
}

void LineAnnotation::setLinePoints(const QList<NormalizedPoint> &points)
{
    Q_D(LineAnnotation);
    d->m_linePoints = points;
}

QList<NormalizedPoint> LineAnnotation::linePoints() const
{
    Q_D(const LineAnnotation);
    return d->m_linePoints;
}

QList<NormalizedPoint> LineAnnotation::transformedLinePoints() const
{
    Q_D(const LineAnnotation);
    return d->m_transformedLinePoints;
}

void LineAnnotation::setLineStartStyle(TermStyle style)
{
    Q_D(LineAnnotation);
    d->m_lineStartStyle = style;
}

LineAnnotation::TermStyle LineAnnotation::lineStartStyle() const
{
    Q_D(const LineAnnotation);
    return d->m_lineStartStyle;
}

void LineAnnotation::setLineEndStyle(TermStyle style)
{
    Q_D(LineAnnotation);
    d->m_lineEndStyle = style;
}

LineAnnotation::TermStyle LineAnnotation::lineEndStyle() const
{
    Q_D(const LineAnnotation);
    return d->m_lineEndStyle;
}

void LineAnnotation::setLineClosed(bool closed)
{
    Q_D(LineAnnotation);
    d->m_lineClosed = closed;
}

bool LineAnnotation::lineClosed() const
{
    Q_D(const LineAnnotation);
    return d->m_lineClosed;
}

void LineAnnotation::setLineInnerColor(const QColor &color)
{
    Q_D(LineAnnotation);
    d->m_lineInnerColor = color;
}

QColor LineAnnotation::lineInnerColor() const
{
    Q_D(const LineAnnotation);
    return d->m_lineInnerColor;
}

void LineAnnotation::setLineLeadingForwardPoint(double point)
{
    Q_D(LineAnnotation);
    d->m_lineLeadingFwdPt = point;
}

double LineAnnotation::lineLeadingForwardPoint() const
{
    Q_D(const LineAnnotation);
    return d->m_lineLeadingFwdPt;
}

void LineAnnotation::setLineLeadingBackwardPoint(double point)
{
    Q_D(LineAnnotation);
    d->m_lineLeadingBackPt = point;
}

double LineAnnotation::lineLeadingBackwardPoint() const
{
    Q_D(const LineAnnotation);
    return d->m_lineLeadingBackPt;
}

void LineAnnotation::setShowCaption(bool show)
{
    Q_D(LineAnnotation);
    d->m_lineShowCaption = show;
}

bool LineAnnotation::showCaption() const
{
    Q_D(const LineAnnotation);
    return d->m_lineShowCaption;
}

void LineAnnotation::setLineIntent(LineIntent intent)
{
    Q_D(LineAnnotation);
    d->m_lineIntent = intent;
}

LineAnnotation::LineIntent LineAnnotation::lineIntent() const
{
    Q_D(const LineAnnotation);
    return d->m_lineIntent;
}

Annotation::SubType LineAnnotation::subType() const
{
    return ALine;
}

void LineAnnotation::store(QDomNode &node, QDomDocument &document) const
{
    Q_D(const LineAnnotation);
    // recurse to parent objects storing properties
    Annotation::store(node, document);

    // create [line] element
    QDomElement lineElement = document.createElement(QStringLiteral("line"));
    node.appendChild(lineElement);

    // store the attributes
    if (d->m_lineStartStyle != None) {
        lineElement.setAttribute(QStringLiteral("startStyle"), (int)d->m_lineStartStyle);
    }
    if (d->m_lineEndStyle != None) {
        lineElement.setAttribute(QStringLiteral("endStyle"), (int)d->m_lineEndStyle);
    }
    if (d->m_lineClosed) {
        lineElement.setAttribute(QStringLiteral("closed"), d->m_lineClosed);
    }
    if (d->m_lineInnerColor.isValid()) {
        lineElement.setAttribute(QStringLiteral("innerColor"), d->m_lineInnerColor.name());
    }
    if (d->m_lineLeadingFwdPt != 0.0) {
        lineElement.setAttribute(QStringLiteral("leadFwd"), QString::number(d->m_lineLeadingFwdPt));
    }
    if (d->m_lineLeadingBackPt != 0.0) {
        lineElement.setAttribute(QStringLiteral("leadBack"), QString::number(d->m_lineLeadingBackPt));
    }
    if (d->m_lineShowCaption) {
        lineElement.setAttribute(QStringLiteral("showCaption"), d->m_lineShowCaption);
    }
    if (d->m_lineIntent != Unknown) {
        lineElement.setAttribute(QStringLiteral("intent"), d->m_lineIntent);
    }

    // append the list of points
    int points = d->m_linePoints.count();
    if (points > 1) {
        QList<NormalizedPoint>::const_iterator it = d->m_linePoints.begin(), end = d->m_linePoints.end();
        while (it != end) {
            const NormalizedPoint &p = *it;
            QDomElement pElement = document.createElement(QStringLiteral("point"));
            lineElement.appendChild(pElement);
            pElement.setAttribute(QStringLiteral("x"), QString::number(p.x));
            pElement.setAttribute(QStringLiteral("y"), QString::number(p.y));
            it++; // to avoid loop
        }
    }
}

void LineAnnotationPrivate::transform(const QTransform &matrix)
{
    AnnotationPrivate::transform(matrix);

    QMutableListIterator<NormalizedPoint> it(m_transformedLinePoints);
    while (it.hasNext()) {
        it.next().transform(matrix);
    }
}

void LineAnnotationPrivate::baseTransform(const QTransform &matrix)
{
    AnnotationPrivate::baseTransform(matrix);

    QMutableListIterator<NormalizedPoint> it(m_linePoints);
    while (it.hasNext()) {
        it.next().transform(matrix);
    }
}

void LineAnnotationPrivate::resetTransformation()
{
    AnnotationPrivate::resetTransformation();

    m_transformedLinePoints = m_linePoints;
}

void LineAnnotationPrivate::translate(const NormalizedPoint &coord)
{
    AnnotationPrivate::translate(coord);

    QMutableListIterator<NormalizedPoint> it(m_linePoints);
    while (it.hasNext()) {
        NormalizedPoint &p = it.next();
        p.x = p.x + coord.x;
        p.y = p.y + coord.y;
    }
}

void LineAnnotationPrivate::setAnnotationProperties(const QDomNode &node)
{
    Okular::AnnotationPrivate::setAnnotationProperties(node);

    // loop through the whole children looking for a 'line' element
    QDomNode subNode = node.firstChild();
    while (subNode.isElement()) {
        QDomElement e = subNode.toElement();
        subNode = subNode.nextSibling();
        if (e.tagName() != QLatin1String("line")) {
            continue;
        }

        // parse the attributes
        if (e.hasAttribute(QStringLiteral("startStyle"))) {
            m_lineStartStyle = (LineAnnotation::TermStyle)e.attribute(QStringLiteral("startStyle")).toInt();
        }
        if (e.hasAttribute(QStringLiteral("endStyle"))) {
            m_lineEndStyle = (LineAnnotation::TermStyle)e.attribute(QStringLiteral("endStyle")).toInt();
        }
        if (e.hasAttribute(QStringLiteral("closed"))) {
            m_lineClosed = e.attribute(QStringLiteral("closed")).toInt();
        }
        if (e.hasAttribute(QStringLiteral("innerColor"))) {
            m_lineInnerColor = QColor(e.attribute(QStringLiteral("innerColor")));
        }
        if (e.hasAttribute(QStringLiteral("leadFwd"))) {
            m_lineLeadingFwdPt = e.attribute(QStringLiteral("leadFwd")).toDouble();
        }
        if (e.hasAttribute(QStringLiteral("leadBack"))) {
            m_lineLeadingBackPt = e.attribute(QStringLiteral("leadBack")).toDouble();
        }
        if (e.hasAttribute(QStringLiteral("showCaption"))) {
            m_lineShowCaption = e.attribute(QStringLiteral("showCaption")).toInt();
        }
        if (e.hasAttribute(QStringLiteral("intent"))) {
            m_lineIntent = (LineAnnotation::LineIntent)e.attribute(QStringLiteral("intent")).toInt();
        }

        // parse all 'point' subnodes
        QDomNode pointNode = e.firstChild();
        while (pointNode.isElement()) {
            QDomElement pe = pointNode.toElement();
            pointNode = pointNode.nextSibling();

            if (pe.tagName() != QLatin1String("point")) {
                continue;
            }

            NormalizedPoint p;
            p.x = pe.attribute(QStringLiteral("x"), QStringLiteral("0.0")).toDouble();
            p.y = pe.attribute(QStringLiteral("y"), QStringLiteral("0.0")).toDouble();
            m_linePoints.append(p);
        }

        // loading complete
        break;
    }

    m_transformedLinePoints = m_linePoints;
}

AnnotationPrivate *LineAnnotationPrivate::getNewAnnotationPrivate()
{
    return new LineAnnotationPrivate();
}

double LineAnnotationPrivate::distanceSqr(double x, double y, double xScale, double yScale) const
{
    QList<NormalizedPoint> transformedLinePoints = m_transformedLinePoints;

    if (m_lineClosed) { // Close the path
        transformedLinePoints.append(transformedLinePoints.first());
    }

    if (m_lineInnerColor.isValid()) {
        QPolygonF polygon;
        for (const NormalizedPoint &p : qAsConst(transformedLinePoints)) {
            polygon.append(QPointF(p.x, p.y));
        }

        if (polygon.containsPoint(QPointF(x, y), Qt::WindingFill)) {
            return 0;
        }
    }

    return strokeDistance(::distanceSqr(x, y, xScale, yScale, transformedLinePoints), m_style.width() * xScale / (m_page->m_width * 2));
}

/** GeomAnnotation [Annotation] */

class Okular::GeomAnnotationPrivate : public Okular::AnnotationPrivate
{
public:
    GeomAnnotationPrivate()
        : AnnotationPrivate()
        , m_geomType(GeomAnnotation::InscribedSquare)
    {
    }
    void setAnnotationProperties(const QDomNode &node) override;
    bool canBeResized() const override;
    AnnotationPrivate *getNewAnnotationPrivate() override;
    double distanceSqr(double x, double y, double xScale, double yScale) const override;

    GeomAnnotation::GeomType m_geomType;
    QColor m_geomInnerColor;
};

GeomAnnotation::GeomAnnotation()
    : Annotation(*new GeomAnnotationPrivate())
{
}

GeomAnnotation::GeomAnnotation(const QDomNode &description)
    : Annotation(*new GeomAnnotationPrivate(), description)
{
}

GeomAnnotation::~GeomAnnotation()
{
}

void GeomAnnotation::setGeometricalType(GeomType type)
{
    Q_D(GeomAnnotation);
    d->m_geomType = type;
}

GeomAnnotation::GeomType GeomAnnotation::geometricalType() const
{
    Q_D(const GeomAnnotation);
    return d->m_geomType;
}

void GeomAnnotation::setGeometricalInnerColor(const QColor &color)
{
    Q_D(GeomAnnotation);
    d->m_geomInnerColor = color;
}

QColor GeomAnnotation::geometricalInnerColor() const
{
    Q_D(const GeomAnnotation);
    return d->m_geomInnerColor;
}

Annotation::SubType GeomAnnotation::subType() const
{
    return AGeom;
}

void GeomAnnotation::store(QDomNode &node, QDomDocument &document) const
{
    Q_D(const GeomAnnotation);
    // recurse to parent objects storing properties
    Annotation::store(node, document);

    // create [geom] element
    QDomElement geomElement = document.createElement(QStringLiteral("geom"));
    node.appendChild(geomElement);

    // append the optional attributes
    if (d->m_geomType != InscribedSquare) {
        geomElement.setAttribute(QStringLiteral("type"), (int)d->m_geomType);
    }
    if (d->m_geomInnerColor.isValid()) {
        geomElement.setAttribute(QStringLiteral("color"), d->m_geomInnerColor.name());
    }
}

void GeomAnnotationPrivate::setAnnotationProperties(const QDomNode &node)
{
    Okular::AnnotationPrivate::setAnnotationProperties(node);
    // loop through the whole children looking for a 'geom' element
    QDomNode subNode = node.firstChild();
    while (subNode.isElement()) {
        QDomElement e = subNode.toElement();
        subNode = subNode.nextSibling();
        if (e.tagName() != QLatin1String("geom")) {
            continue;
        }

        // parse the attributes
        if (e.hasAttribute(QStringLiteral("type"))) {
            m_geomType = (GeomAnnotation::GeomType)e.attribute(QStringLiteral("type")).toInt();
        }
        if (e.hasAttribute(QStringLiteral("color"))) {
            m_geomInnerColor = QColor(e.attribute(QStringLiteral("color")));
        }
        // compatibility
        if (e.hasAttribute(QStringLiteral("width"))) {
            m_style.setWidth(e.attribute(QStringLiteral("width")).toInt());
        }

        // loading complete
        break;
    }
}

bool GeomAnnotationPrivate::canBeResized() const
{
    return true;
}

AnnotationPrivate *GeomAnnotationPrivate::getNewAnnotationPrivate()
{
    return new GeomAnnotationPrivate();
}

double GeomAnnotationPrivate::distanceSqr(double x, double y, double xScale, double yScale) const
{
    double distance = 0;
    // the line thickness is applied unevenly (only on the "inside") - account for this
    bool withinShape = false;
    switch (m_geomType) {
    case GeomAnnotation::InscribedCircle: {
        // calculate the center point and focus lengths of the ellipse
        const double centerX = (m_transformedBoundary.left + m_transformedBoundary.right) / 2.0;
        const double centerY = (m_transformedBoundary.top + m_transformedBoundary.bottom) / 2.0;
        const double focusX = (m_transformedBoundary.right - centerX);
        const double focusY = (m_transformedBoundary.bottom - centerY);

        const double focusXSqr = pow(focusX, 2);
        const double focusYSqr = pow(focusY, 2);

        // to calculate the distance from the ellipse, we will first find the point "projection"
        // that lies on the ellipse and is closest to the point (x,y)
        // This point can obviously be written as "center + lambda(inputPoint - center)".
        // Because the point lies on the ellipse, we know that:
        //   1 = ((center.x - projection.x)/focusX)^2 + ((center.y - projection.y)/focusY)^2
        // After filling in projection.x = center.x + lambda * (inputPoint.x - center.x)
        // and its y-equivalent, we can solve for lambda:
        const double lambda = sqrt(focusXSqr * focusYSqr / (focusYSqr * pow(x - centerX, 2) + focusXSqr * pow(y - centerY, 2)));

        // if the ellipse is filled, we treat all points within as "on" it
        if (lambda > 1) {
            if (m_geomInnerColor.isValid()) {
                return 0;
            } else {
                withinShape = true;
            }
        }

        // otherwise we calculate the squared distance from the projected point on the ellipse
        NormalizedPoint projection(centerX, centerY);
        projection.x += lambda * (x - centerX);
        projection.y += lambda * (y - centerY);

        distance = projection.distanceSqr(x, y, xScale, yScale);
        break;
    }

    case GeomAnnotation::InscribedSquare:
        // if the square is filled, only check the bounding box
        if (m_geomInnerColor.isValid()) {
            return AnnotationPrivate::distanceSqr(x, y, xScale, yScale);
        }

        const QList<NormalizedPoint> edges = {NormalizedPoint(m_transformedBoundary.left, m_transformedBoundary.top),
                                              NormalizedPoint(m_transformedBoundary.right, m_transformedBoundary.top),
                                              NormalizedPoint(m_transformedBoundary.right, m_transformedBoundary.bottom),
                                              NormalizedPoint(m_transformedBoundary.left, m_transformedBoundary.bottom),
                                              NormalizedPoint(m_transformedBoundary.left, m_transformedBoundary.top)};
        distance = ::distanceSqr(x, y, xScale, yScale, edges);

        if (m_transformedBoundary.contains(x, y)) {
            withinShape = true;
        }

        break;
    }
    if (withinShape) {
        distance = strokeDistance(distance, m_style.width() * xScale / m_page->m_width);
    }

    return distance;
}

/** HighlightAnnotation [Annotation] */

class HighlightAnnotation::Quad::Private
{
public:
    Private()
    {
    }

    NormalizedPoint m_points[4];
    NormalizedPoint m_transformedPoints[4];
    bool m_capStart : 1;
    bool m_capEnd : 1;
    double m_feather;
};

HighlightAnnotation::Quad::Quad()
    : d(new Private)
{
}

HighlightAnnotation::Quad::~Quad()
{
    delete d;
}

HighlightAnnotation::Quad::Quad(const Quad &other)
    : d(new Private)
{
    *d = *other.d;
}

HighlightAnnotation::Quad &HighlightAnnotation::Quad::operator=(const Quad &other)
{
    if (this != &other) {
        *d = *other.d;
    }

    return *this;
}

void HighlightAnnotation::Quad::setPoint(const NormalizedPoint &point, int index)
{
    if (index < 0 || index > 3) {
        return;
    }

    d->m_points[index] = point;
}

NormalizedPoint HighlightAnnotation::Quad::point(int index) const
{
    if (index < 0 || index > 3) {
        return NormalizedPoint();
    }

    return d->m_points[index];
}

NormalizedPoint HighlightAnnotation::Quad::transformedPoint(int index) const
{
    if (index < 0 || index > 3) {
        return NormalizedPoint();
    }

    return d->m_transformedPoints[index];
}

void HighlightAnnotation::Quad::setCapStart(bool value)
{
    d->m_capStart = value;
}

bool HighlightAnnotation::Quad::capStart() const
{
    return d->m_capStart;
}

void HighlightAnnotation::Quad::setCapEnd(bool value)
{
    d->m_capEnd = value;
}

bool HighlightAnnotation::Quad::capEnd() const
{
    return d->m_capEnd;
}

void HighlightAnnotation::Quad::setFeather(double width)
{
    d->m_feather = width;
}

double HighlightAnnotation::Quad::feather() const
{
    return d->m_feather;
}

void HighlightAnnotation::Quad::transform(const QTransform &matrix)
{
    for (int i = 0; i < 4; ++i) {
        d->m_transformedPoints[i] = d->m_points[i];
        d->m_transformedPoints[i].transform(matrix);
    }
}

class Okular::HighlightAnnotationPrivate : public Okular::AnnotationPrivate
{
public:
    HighlightAnnotationPrivate()
        : AnnotationPrivate()
        , m_highlightType(HighlightAnnotation::Highlight)
    {
    }

    void transform(const QTransform &matrix) override;
    void baseTransform(const QTransform &matrix) override;
    double distanceSqr(double x, double y, double xScale, double yScale) const override;
    void setAnnotationProperties(const QDomNode &node) override;
    AnnotationPrivate *getNewAnnotationPrivate() override;

    HighlightAnnotation::HighlightType m_highlightType;
    QList<HighlightAnnotation::Quad> m_highlightQuads;
};

HighlightAnnotation::HighlightAnnotation()
    : Annotation(*new HighlightAnnotationPrivate())
{
}

HighlightAnnotation::HighlightAnnotation(const QDomNode &description)
    : Annotation(*new HighlightAnnotationPrivate(), description)
{
}

HighlightAnnotation::~HighlightAnnotation()
{
}

void HighlightAnnotation::setHighlightType(HighlightType type)
{
    Q_D(HighlightAnnotation);
    d->m_highlightType = type;
}

HighlightAnnotation::HighlightType HighlightAnnotation::highlightType() const
{
    Q_D(const HighlightAnnotation);
    return d->m_highlightType;
}

QList<HighlightAnnotation::Quad> &HighlightAnnotation::highlightQuads()
{
    Q_D(HighlightAnnotation);
    return d->m_highlightQuads;
}

const QList<HighlightAnnotation::Quad> &HighlightAnnotation::highlightQuads() const
{
    Q_D(const HighlightAnnotation);
    return d->m_highlightQuads;
}

void HighlightAnnotation::store(QDomNode &node, QDomDocument &document) const
{
    Q_D(const HighlightAnnotation);
    // recurse to parent objects storing properties
    Annotation::store(node, document);

    // create [hl] element
    QDomElement hlElement = document.createElement(QStringLiteral("hl"));
    node.appendChild(hlElement);

    // append the optional attributes
    if (d->m_highlightType != Highlight) {
        hlElement.setAttribute(QStringLiteral("type"), (int)d->m_highlightType);
    }
    if (d->m_highlightQuads.count() < 1) {
        return;
    }
    // append highlight quads, all children describe quads
    QList<Quad>::const_iterator it = d->m_highlightQuads.begin(), end = d->m_highlightQuads.end();
    for (; it != end; ++it) {
        QDomElement quadElement = document.createElement(QStringLiteral("quad"));
        hlElement.appendChild(quadElement);
        const Quad &q = *it;
        quadElement.setAttribute(QStringLiteral("ax"), QString::number(q.point(0).x));
        quadElement.setAttribute(QStringLiteral("ay"), QString::number(q.point(0).y));
        quadElement.setAttribute(QStringLiteral("bx"), QString::number(q.point(1).x));
        quadElement.setAttribute(QStringLiteral("by"), QString::number(q.point(1).y));
        quadElement.setAttribute(QStringLiteral("cx"), QString::number(q.point(2).x));
        quadElement.setAttribute(QStringLiteral("cy"), QString::number(q.point(2).y));
        quadElement.setAttribute(QStringLiteral("dx"), QString::number(q.point(3).x));
        quadElement.setAttribute(QStringLiteral("dy"), QString::number(q.point(3).y));
        if (q.capStart()) {
            quadElement.setAttribute(QStringLiteral("start"), 1);
        }
        if (q.capEnd()) {
            quadElement.setAttribute(QStringLiteral("end"), 1);
        }
        quadElement.setAttribute(QStringLiteral("feather"), QString::number(q.feather()));
    }
}

Annotation::SubType HighlightAnnotation::subType() const
{
    return AHighlight;
}

void HighlightAnnotationPrivate::transform(const QTransform &matrix)
{
    AnnotationPrivate::transform(matrix);

    QMutableListIterator<HighlightAnnotation::Quad> it(m_highlightQuads);
    while (it.hasNext()) {
        it.next().transform(matrix);
    }
}

void HighlightAnnotationPrivate::baseTransform(const QTransform &matrix)
{
    AnnotationPrivate::baseTransform(matrix);

    QMutableListIterator<HighlightAnnotation::Quad> it(m_highlightQuads);
    while (it.hasNext()) {
        it.next().transform(matrix);
    }
}

void HighlightAnnotationPrivate::setAnnotationProperties(const QDomNode &node)
{
    Okular::AnnotationPrivate::setAnnotationProperties(node);
    m_highlightQuads.clear();

    // loop through the whole children looking for a 'hl' element
    QDomNode subNode = node.firstChild();
    while (subNode.isElement()) {
        QDomElement e = subNode.toElement();
        subNode = subNode.nextSibling();
        if (e.tagName() != QLatin1String("hl")) {
            continue;
        }

        // parse the attributes
        if (e.hasAttribute(QStringLiteral("type"))) {
            m_highlightType = (HighlightAnnotation::HighlightType)e.attribute(QStringLiteral("type")).toInt();
        }

        // parse all 'quad' subnodes
        QDomNode quadNode = e.firstChild();
        for (; quadNode.isElement(); quadNode = quadNode.nextSibling()) {
            QDomElement qe = quadNode.toElement();
            if (qe.tagName() != QLatin1String("quad")) {
                continue;
            }

            HighlightAnnotation::Quad q;
            q.setPoint(NormalizedPoint(qe.attribute(QStringLiteral("ax"), QStringLiteral("0.0")).toDouble(), qe.attribute(QStringLiteral("ay"), QStringLiteral("0.0")).toDouble()), 0);
            q.setPoint(NormalizedPoint(qe.attribute(QStringLiteral("bx"), QStringLiteral("0.0")).toDouble(), qe.attribute(QStringLiteral("by"), QStringLiteral("0.0")).toDouble()), 1);
            q.setPoint(NormalizedPoint(qe.attribute(QStringLiteral("cx"), QStringLiteral("0.0")).toDouble(), qe.attribute(QStringLiteral("cy"), QStringLiteral("0.0")).toDouble()), 2);
            q.setPoint(NormalizedPoint(qe.attribute(QStringLiteral("dx"), QStringLiteral("0.0")).toDouble(), qe.attribute(QStringLiteral("dy"), QStringLiteral("0.0")).toDouble()), 3);
            q.setCapStart(qe.hasAttribute(QStringLiteral("start")));
            q.setCapEnd(qe.hasAttribute(QStringLiteral("end")));
            q.setFeather(qe.attribute(QStringLiteral("feather"), QStringLiteral("0.1")).toDouble());

            q.transform(QTransform());

            m_highlightQuads.append(q);
        }

        // loading complete
        break;
    }
}

AnnotationPrivate *HighlightAnnotationPrivate::getNewAnnotationPrivate()
{
    return new HighlightAnnotationPrivate();
}

double HighlightAnnotationPrivate::distanceSqr(double x, double y, double xScale, double yScale) const
{
    NormalizedPoint point(x, y);
    double outsideDistance = DBL_MAX;
    for (const HighlightAnnotation::Quad &quad : m_highlightQuads) {
        QList<NormalizedPoint> pathPoints;

        // first, we check if the point is within the area described by the 4 quads
        // this is the case, if the point is always on one side of each segments delimiting the polygon:
        pathPoints << quad.transformedPoint(0);
        int directionVote = 0;
        for (int i = 1; i < 5; ++i) {
            NormalizedPoint thisPoint = quad.transformedPoint(i % 4);
            directionVote += (isLeftOfVector(pathPoints.back(), thisPoint, point)) ? 1 : -1;
            pathPoints << thisPoint;
        }
        if (abs(directionVote) == 4) {
            return 0;
        }

        // if that's not the case, we treat the outline as path and simply determine
        // the distance from the path to the point
        const double thisOutsideDistance = ::distanceSqr(x, y, xScale, yScale, pathPoints);
        if (thisOutsideDistance < outsideDistance) {
            outsideDistance = thisOutsideDistance;
        }
    }

    return outsideDistance;
}

/** StampAnnotation [Annotation] */

class Okular::StampAnnotationPrivate : public Okular::AnnotationPrivate
{
public:
    StampAnnotationPrivate()
        : AnnotationPrivate()
        , m_stampIconName(QStringLiteral("Draft"))
    {
    }
    void setAnnotationProperties(const QDomNode &node) override;
    bool canBeResized() const override;
    AnnotationPrivate *getNewAnnotationPrivate() override;

    QString m_stampIconName;
};

StampAnnotation::StampAnnotation()
    : Annotation(*new StampAnnotationPrivate())
{
}

StampAnnotation::StampAnnotation(const QDomNode &description)
    : Annotation(*new StampAnnotationPrivate(), description)
{
}

StampAnnotation::~StampAnnotation()
{
}

void StampAnnotation::setStampIconName(const QString &name)
{
    Q_D(StampAnnotation);
    d->m_stampIconName = name;
}

QString StampAnnotation::stampIconName() const
{
    Q_D(const StampAnnotation);
    return d->m_stampIconName;
}

Annotation::SubType StampAnnotation::subType() const
{
    return AStamp;
}

void StampAnnotation::store(QDomNode &node, QDomDocument &document) const
{
    Q_D(const StampAnnotation);
    // recurse to parent objects storing properties
    Annotation::store(node, document);

    // create [stamp] element
    QDomElement stampElement = document.createElement(QStringLiteral("stamp"));
    node.appendChild(stampElement);

    // append the optional attributes
    if (d->m_stampIconName != QLatin1String("Draft")) {
        stampElement.setAttribute(QStringLiteral("icon"), d->m_stampIconName);
    }
}

void StampAnnotationPrivate::setAnnotationProperties(const QDomNode &node)
{
    Okular::AnnotationPrivate::setAnnotationProperties(node);

    // loop through the whole children looking for a 'stamp' element
    QDomNode subNode = node.firstChild();
    while (subNode.isElement()) {
        QDomElement e = subNode.toElement();
        subNode = subNode.nextSibling();
        if (e.tagName() != QLatin1String("stamp")) {
            continue;
        }

        // parse the attributes
        if (e.hasAttribute(QStringLiteral("icon"))) {
            m_stampIconName = e.attribute(QStringLiteral("icon"));
        }

        // loading complete
        break;
    }
}

bool StampAnnotationPrivate::canBeResized() const
{
    return true;
}

AnnotationPrivate *StampAnnotationPrivate::getNewAnnotationPrivate()
{
    return new StampAnnotationPrivate();
}

/** InkAnnotation [Annotation] */

class Okular::InkAnnotationPrivate : public Okular::AnnotationPrivate
{
public:
    InkAnnotationPrivate()
        : AnnotationPrivate()
    {
    }

    void transform(const QTransform &matrix) override;
    void baseTransform(const QTransform &matrix) override;
    void resetTransformation() override;
    double distanceSqr(double x, double y, double xScale, double yScale) const override;
    void translate(const NormalizedPoint &coord) override;
    void setAnnotationProperties(const QDomNode &node) override;
    AnnotationPrivate *getNewAnnotationPrivate() override;

    QList<QList<NormalizedPoint>> m_inkPaths;
    QList<QList<NormalizedPoint>> m_transformedInkPaths;
};

InkAnnotation::InkAnnotation()
    : Annotation(*new InkAnnotationPrivate())
{
}

InkAnnotation::InkAnnotation(const QDomNode &description)
    : Annotation(*new InkAnnotationPrivate(), description)
{
}

InkAnnotation::~InkAnnotation()
{
}

void InkAnnotation::setInkPaths(const QList<QList<NormalizedPoint>> &paths)
{
    Q_D(InkAnnotation);
    d->m_inkPaths = paths;
}

QList<QList<NormalizedPoint>> InkAnnotation::inkPaths() const
{
    Q_D(const InkAnnotation);
    return d->m_inkPaths;
}

QList<QList<NormalizedPoint>> InkAnnotation::transformedInkPaths() const
{
    Q_D(const InkAnnotation);
    return d->m_transformedInkPaths;
}

Annotation::SubType InkAnnotation::subType() const
{
    return AInk;
}

void InkAnnotation::store(QDomNode &node, QDomDocument &document) const
{
    Q_D(const InkAnnotation);
    // recurse to parent objects storing properties
    Annotation::store(node, document);

    // create [ink] element
    QDomElement inkElement = document.createElement(QStringLiteral("ink"));
    node.appendChild(inkElement);

    // append the optional attributes
    if (d->m_inkPaths.count() < 1) {
        return;
    }

    QList<QList<NormalizedPoint>>::const_iterator pIt = d->m_inkPaths.begin(), pEnd = d->m_inkPaths.end();
    for (; pIt != pEnd; ++pIt) {
        QDomElement pathElement = document.createElement(QStringLiteral("path"));
        inkElement.appendChild(pathElement);
        const QList<NormalizedPoint> &path = *pIt;
        for (const NormalizedPoint &point : path) {
            QDomElement pointElement = document.createElement(QStringLiteral("point"));
            pathElement.appendChild(pointElement);
            pointElement.setAttribute(QStringLiteral("x"), QString::number(point.x));
            pointElement.setAttribute(QStringLiteral("y"), QString::number(point.y));
        }
    }
}

double InkAnnotationPrivate::distanceSqr(double x, double y, double xScale, double yScale) const
{
    double distance = DBL_MAX;
    for (const QList<NormalizedPoint> &path : m_transformedInkPaths) {
        const double thisDistance = ::distanceSqr(x, y, xScale, yScale, path);
        if (thisDistance < distance) {
            distance = thisDistance;
        }
    }
    return strokeDistance(distance, m_style.width() * xScale / (m_page->m_width * 2));
}

void InkAnnotationPrivate::transform(const QTransform &matrix)
{
    AnnotationPrivate::transform(matrix);

    for (int i = 0; i < m_transformedInkPaths.count(); ++i) {
        QMutableListIterator<NormalizedPoint> it(m_transformedInkPaths[i]);
        while (it.hasNext()) {
            it.next().transform(matrix);
        }
    }
}

void InkAnnotationPrivate::baseTransform(const QTransform &matrix)
{
    AnnotationPrivate::baseTransform(matrix);

    for (int i = 0; i < m_inkPaths.count(); ++i) {
        QMutableListIterator<NormalizedPoint> it(m_inkPaths[i]);
        while (it.hasNext()) {
            it.next().transform(matrix);
        }
    }
}

void InkAnnotationPrivate::resetTransformation()
{
    AnnotationPrivate::resetTransformation();

    m_transformedInkPaths = m_inkPaths;
}

void InkAnnotationPrivate::translate(const NormalizedPoint &coord)
{
    AnnotationPrivate::translate(coord);

    for (int i = 0; i < m_inkPaths.count(); ++i) {
        QMutableListIterator<NormalizedPoint> it(m_inkPaths[i]);
        while (it.hasNext()) {
            NormalizedPoint &p = it.next();
            p.x = p.x + coord.x;
            p.y = p.y + coord.y;
        }
    }
}

void InkAnnotationPrivate::setAnnotationProperties(const QDomNode &node)
{
    Okular::AnnotationPrivate::setAnnotationProperties(node);
    m_inkPaths.clear();

    // loop through the whole children looking for a 'ink' element
    QDomNode subNode = node.firstChild();
    while (subNode.isElement()) {
        QDomElement e = subNode.toElement();
        subNode = subNode.nextSibling();
        if (e.tagName() != QLatin1String("ink")) {
            continue;
        }

        // parse the 'path' subnodes
        QDomNode pathNode = e.firstChild();
        while (pathNode.isElement()) {
            QDomElement pathElement = pathNode.toElement();
            pathNode = pathNode.nextSibling();

            if (pathElement.tagName() != QLatin1String("path")) {
                continue;
            }

            // build each path parsing 'point' subnodes
            QList<NormalizedPoint> path;
            QDomNode pointNode = pathElement.firstChild();
            while (pointNode.isElement()) {
                QDomElement pointElement = pointNode.toElement();
                pointNode = pointNode.nextSibling();

                if (pointElement.tagName() != QLatin1String("point")) {
                    continue;
                }

                NormalizedPoint p;
                p.x = pointElement.attribute(QStringLiteral("x"), QStringLiteral("0.0")).toDouble();
                p.y = pointElement.attribute(QStringLiteral("y"), QStringLiteral("0.0")).toDouble();
                path.append(p);
            }

            // add the path to the path list if it contains at least 2 nodes
            if (path.count() >= 2) {
                m_inkPaths.append(path);
            }
        }

        // loading complete
        break;
    }

    m_transformedInkPaths = m_inkPaths;
}

AnnotationPrivate *InkAnnotationPrivate::getNewAnnotationPrivate()
{
    return new InkAnnotationPrivate();
}

/** CaretAnnotation [Annotation] */

class Okular::CaretAnnotationPrivate : public Okular::AnnotationPrivate
{
public:
    CaretAnnotationPrivate()
        : AnnotationPrivate()
        , m_symbol(CaretAnnotation::None)
    {
    }

    void setAnnotationProperties(const QDomNode &node) override;
    AnnotationPrivate *getNewAnnotationPrivate() override;

    CaretAnnotation::CaretSymbol m_symbol;
};

static QString caretSymbolToString(CaretAnnotation::CaretSymbol symbol)
{
    switch (symbol) {
    case CaretAnnotation::None:
        return QStringLiteral("None");
    case CaretAnnotation::P:
        return QStringLiteral("P");
    }
    return QString();
}

static CaretAnnotation::CaretSymbol caretSymbolFromString(const QString &symbol)
{
    if (symbol == QLatin1String("None")) {
        return CaretAnnotation::None;
    } else if (symbol == QLatin1String("P")) {
        return CaretAnnotation::P;
    }
    return CaretAnnotation::None;
}

void CaretAnnotationPrivate::setAnnotationProperties(const QDomNode &node)
{
    Okular::AnnotationPrivate::setAnnotationProperties(node);

    // loop through the whole children looking for a 'caret' element
    QDomNode subNode = node.firstChild();
    while (subNode.isElement()) {
        QDomElement e = subNode.toElement();
        subNode = subNode.nextSibling();
        if (e.tagName() != QLatin1String("caret")) {
            continue;
        }

        // parse the attributes
        if (e.hasAttribute(QStringLiteral("symbol"))) {
            m_symbol = caretSymbolFromString(e.attribute(QStringLiteral("symbol")));
        }

        // loading complete
        break;
    }
}

AnnotationPrivate *CaretAnnotationPrivate::getNewAnnotationPrivate()
{
    return new CaretAnnotationPrivate();
}

CaretAnnotation::CaretAnnotation()
    : Annotation(*new CaretAnnotationPrivate())
{
}

CaretAnnotation::CaretAnnotation(const QDomNode &description)
    : Annotation(*new CaretAnnotationPrivate(), description)
{
}

CaretAnnotation::~CaretAnnotation()
{
}

void CaretAnnotation::setCaretSymbol(CaretAnnotation::CaretSymbol symbol)
{
    Q_D(CaretAnnotation);
    d->m_symbol = symbol;
}

CaretAnnotation::CaretSymbol CaretAnnotation::caretSymbol() const
{
    Q_D(const CaretAnnotation);
    return d->m_symbol;
}

Annotation::SubType CaretAnnotation::subType() const
{
    return ACaret;
}

void CaretAnnotation::store(QDomNode &node, QDomDocument &document) const
{
    Q_D(const CaretAnnotation);
    // recurse to parent objects storing properties
    Annotation::store(node, document);

    // create [caret] element
    QDomElement caretElement = document.createElement(QStringLiteral("caret"));
    node.appendChild(caretElement);

    // append the optional attributes
    if (d->m_symbol != None) {
        caretElement.setAttribute(QStringLiteral("symbol"), caretSymbolToString(d->m_symbol));
    }
}

/** FileAttachmentAnnotation [Annotation] */

class Okular::FileAttachmentAnnotationPrivate : public Okular::AnnotationPrivate
{
public:
    FileAttachmentAnnotationPrivate()
        : AnnotationPrivate()
        , icon(QStringLiteral("PushPin"))
        , embfile(nullptr)
    {
    }
    ~FileAttachmentAnnotationPrivate() override
    {
        delete embfile;
    }

    void setAnnotationProperties(const QDomNode &node) override;
    AnnotationPrivate *getNewAnnotationPrivate() override;

    // data fields
    QString icon;
    EmbeddedFile *embfile;
};

void FileAttachmentAnnotationPrivate::setAnnotationProperties(const QDomNode &node)
{
    Okular::AnnotationPrivate::setAnnotationProperties(node);

    // loop through the whole children looking for a 'fileattachment' element
    QDomNode subNode = node.firstChild();
    while (subNode.isElement()) {
        QDomElement e = subNode.toElement();
        subNode = subNode.nextSibling();
        if (e.tagName() != QLatin1String("fileattachment")) {
            continue;
        }

        // loading complete
        break;
    }
}

AnnotationPrivate *FileAttachmentAnnotationPrivate::getNewAnnotationPrivate()
{
    return new FileAttachmentAnnotationPrivate();
}

FileAttachmentAnnotation::FileAttachmentAnnotation()
    : Annotation(*new FileAttachmentAnnotationPrivate())
{
}

FileAttachmentAnnotation::FileAttachmentAnnotation(const QDomNode &description)
    : Annotation(*new FileAttachmentAnnotationPrivate(), description)
{
}

FileAttachmentAnnotation::~FileAttachmentAnnotation()
{
}

void FileAttachmentAnnotation::store(QDomNode &node, QDomDocument &document) const
{
    // recurse to parent objects storing properties
    Annotation::store(node, document);

    // create [fileattachment] element
    QDomElement fileAttachmentElement = document.createElement(QStringLiteral("fileattachment"));
    node.appendChild(fileAttachmentElement);
}

Annotation::SubType FileAttachmentAnnotation::subType() const
{
    return AFileAttachment;
}

QString FileAttachmentAnnotation::fileIconName() const
{
    Q_D(const FileAttachmentAnnotation);
    return d->icon;
}

void FileAttachmentAnnotation::setFileIconName(const QString &iconName)
{
    Q_D(FileAttachmentAnnotation);
    d->icon = iconName;
}

EmbeddedFile *FileAttachmentAnnotation::embeddedFile() const
{
    Q_D(const FileAttachmentAnnotation);
    return d->embfile;
}

void FileAttachmentAnnotation::setEmbeddedFile(EmbeddedFile *ef)
{
    Q_D(FileAttachmentAnnotation);
    d->embfile = ef;
}

/** SoundAnnotation [Annotation] */

class Okular::SoundAnnotationPrivate : public Okular::AnnotationPrivate
{
public:
    SoundAnnotationPrivate()
        : AnnotationPrivate()
        , icon(QStringLiteral("Speaker"))
        , sound(nullptr)
    {
    }
    ~SoundAnnotationPrivate() override
    {
        delete sound;
    }

    void setAnnotationProperties(const QDomNode &node) override;
    AnnotationPrivate *getNewAnnotationPrivate() override;

    // data fields
    QString icon;
    Sound *sound;
};

void SoundAnnotationPrivate::setAnnotationProperties(const QDomNode &node)
{
    Okular::AnnotationPrivate::setAnnotationProperties(node);

    // loop through the whole children looking for a 'sound' element
    QDomNode subNode = node.firstChild();
    while (subNode.isElement()) {
        QDomElement e = subNode.toElement();
        subNode = subNode.nextSibling();
        if (e.tagName() != QLatin1String("sound")) {
            continue;
        }

        // loading complete
        break;
    }
}

AnnotationPrivate *SoundAnnotationPrivate::getNewAnnotationPrivate()
{
    return new SoundAnnotationPrivate();
}

SoundAnnotation::SoundAnnotation()
    : Annotation(*new SoundAnnotationPrivate())
{
}

SoundAnnotation::SoundAnnotation(const QDomNode &description)
    : Annotation(*new SoundAnnotationPrivate(), description)
{
}

SoundAnnotation::~SoundAnnotation()
{
}

void SoundAnnotation::store(QDomNode &node, QDomDocument &document) const
{
    // recurse to parent objects storing properties
    Annotation::store(node, document);

    // create [sound] element
    QDomElement soundElement = document.createElement(QStringLiteral("sound"));
    node.appendChild(soundElement);
}

Annotation::SubType SoundAnnotation::subType() const
{
    return ASound;
}

QString SoundAnnotation::soundIconName() const
{
    Q_D(const SoundAnnotation);
    return d->icon;
}

void SoundAnnotation::setSoundIconName(const QString &iconName)
{
    Q_D(SoundAnnotation);
    d->icon = iconName;
}

Sound *SoundAnnotation::sound() const
{
    Q_D(const SoundAnnotation);
    return d->sound;
}

void SoundAnnotation::setSound(Sound *s)
{
    Q_D(SoundAnnotation);
    d->sound = s;
}

/** MovieAnnotation [Annotation] */

class Okular::MovieAnnotationPrivate : public Okular::AnnotationPrivate
{
public:
    MovieAnnotationPrivate()
        : AnnotationPrivate()
        , movie(nullptr)
    {
    }
    ~MovieAnnotationPrivate() override
    {
        delete movie;
    }

    void setAnnotationProperties(const QDomNode &node) override;
    AnnotationPrivate *getNewAnnotationPrivate() override;

    // data fields
    Movie *movie;
};

void MovieAnnotationPrivate::setAnnotationProperties(const QDomNode &node)
{
    Okular::AnnotationPrivate::setAnnotationProperties(node);

    // loop through the whole children looking for a 'movie' element
    QDomNode subNode = node.firstChild();
    while (subNode.isElement()) {
        QDomElement e = subNode.toElement();
        subNode = subNode.nextSibling();
        if (e.tagName() != QLatin1String("movie")) {
            continue;
        }

        // loading complete
        break;
    }
}

AnnotationPrivate *MovieAnnotationPrivate::getNewAnnotationPrivate()
{
    return new MovieAnnotationPrivate();
}

MovieAnnotation::MovieAnnotation()
    : Annotation(*new MovieAnnotationPrivate())
{
}

MovieAnnotation::MovieAnnotation(const QDomNode &description)
    : Annotation(*new MovieAnnotationPrivate(), description)
{
}

MovieAnnotation::~MovieAnnotation()
{
}

void MovieAnnotation::store(QDomNode &node, QDomDocument &document) const
{
    // recurse to parent objects storing properties
    Annotation::store(node, document);

    // create [movie] element
    QDomElement movieElement = document.createElement(QStringLiteral("movie"));
    node.appendChild(movieElement);
}

Annotation::SubType MovieAnnotation::subType() const
{
    return AMovie;
}

Movie *MovieAnnotation::movie() const
{
    Q_D(const MovieAnnotation);
    return d->movie;
}

void MovieAnnotation::setMovie(Movie *movie)
{
    Q_D(MovieAnnotation);
    d->movie = movie;
}

/** ScreenAnnotation [Annotation] */

class Okular::ScreenAnnotationPrivate : public Okular::AnnotationPrivate
{
public:
    ScreenAnnotationPrivate();
    ~ScreenAnnotationPrivate() override;

    void setAnnotationProperties(const QDomNode &node) override;
    AnnotationPrivate *getNewAnnotationPrivate() override;

    Okular::Action *m_action;
    QMap<Okular::Annotation::AdditionalActionType, Okular::Action *> m_additionalActions;
};

ScreenAnnotationPrivate::ScreenAnnotationPrivate()
    : m_action(nullptr)
{
}

ScreenAnnotationPrivate::~ScreenAnnotationPrivate()
{
    delete m_action;
    qDeleteAll(m_additionalActions);
}

void ScreenAnnotationPrivate::setAnnotationProperties(const QDomNode &node)
{
    Okular::AnnotationPrivate::setAnnotationProperties(node);

    // loop through the whole children looking for a 'screen' element
    QDomNode subNode = node.firstChild();
    while (subNode.isElement()) {
        QDomElement e = subNode.toElement();
        subNode = subNode.nextSibling();
        if (e.tagName() != QLatin1String("screen")) {
            continue;
        }

        // loading complete
        break;
    }
}

AnnotationPrivate *ScreenAnnotationPrivate::getNewAnnotationPrivate()
{
    return new ScreenAnnotationPrivate();
}

ScreenAnnotation::ScreenAnnotation()
    : Annotation(*new ScreenAnnotationPrivate())
{
}

ScreenAnnotation::ScreenAnnotation(const QDomNode &description)
    : Annotation(*new ScreenAnnotationPrivate(), description)
{
}

ScreenAnnotation::~ScreenAnnotation()
{
}

void ScreenAnnotation::store(QDomNode &node, QDomDocument &document) const
{
    // recurse to parent objects storing properties
    Annotation::store(node, document);

    // create [screen] element
    QDomElement movieElement = document.createElement(QStringLiteral("screen"));
    node.appendChild(movieElement);
}

Annotation::SubType ScreenAnnotation::subType() const
{
    return AScreen;
}

void ScreenAnnotation::setAdditionalAction(AdditionalActionType type, Action *action)
{
    Q_D(ScreenAnnotation);
    if (d->m_additionalActions.contains(type)) {
        delete d->m_additionalActions.value(type);
    }

    d->m_additionalActions.insert(type, action);
}

Action *ScreenAnnotation::additionalAction(AdditionalActionType type) const
{
    Q_D(const ScreenAnnotation);
    if (!d->m_additionalActions.contains(type)) {
        return nullptr;
    } else {
        return d->m_additionalActions.value(type);
    }
}

void ScreenAnnotation::setAction(Action *action)
{
    Q_D(ScreenAnnotation);

    delete d->m_action;
    d->m_action = action;
}

Action *ScreenAnnotation::action() const
{
    Q_D(const ScreenAnnotation);
    return d->m_action;
}

/** WidgetAnnotation [Annotation] */

class Okular::WidgetAnnotationPrivate : public Okular::AnnotationPrivate
{
public:
    ~WidgetAnnotationPrivate() override;
    void setAnnotationProperties(const QDomNode &node) override;
    AnnotationPrivate *getNewAnnotationPrivate() override;

    QMap<Okular::Annotation::AdditionalActionType, Okular::Action *> m_additionalActions;
};

WidgetAnnotationPrivate::~WidgetAnnotationPrivate()
{
    qDeleteAll(m_additionalActions);
}

void WidgetAnnotationPrivate::setAnnotationProperties(const QDomNode &node)
{
    Okular::AnnotationPrivate::setAnnotationProperties(node);

    // loop through the whole children looking for a 'widget' element
    QDomNode subNode = node.firstChild();
    while (subNode.isElement()) {
        QDomElement e = subNode.toElement();
        subNode = subNode.nextSibling();
        if (e.tagName() != QLatin1String("widget")) {
            continue;
        }

        // loading complete
        break;
    }
}

AnnotationPrivate *WidgetAnnotationPrivate::getNewAnnotationPrivate()
{
    return new WidgetAnnotationPrivate();
}

WidgetAnnotation::WidgetAnnotation()
    : Annotation(*new WidgetAnnotationPrivate())
{
}

WidgetAnnotation::WidgetAnnotation(const QDomNode &description)
    : Annotation(*new WidgetAnnotationPrivate, description)
{
}

WidgetAnnotation::~WidgetAnnotation()
{
}

void WidgetAnnotation::store(QDomNode &node, QDomDocument &document) const
{
    // recurse to parent objects storing properties
    Annotation::store(node, document);

    // create [widget] element
    QDomElement movieElement = document.createElement(QStringLiteral("widget"));
    node.appendChild(movieElement);
}

Annotation::SubType WidgetAnnotation::subType() const
{
    return AWidget;
}

void WidgetAnnotation::setAdditionalAction(AdditionalActionType type, Action *action)
{
    Q_D(WidgetAnnotation);
    if (d->m_additionalActions.contains(type)) {
        delete d->m_additionalActions.value(type);
    }

    d->m_additionalActions.insert(type, action);
}

Action *WidgetAnnotation::additionalAction(AdditionalActionType type) const
{
    Q_D(const WidgetAnnotation);
    if (!d->m_additionalActions.contains(type)) {
        return nullptr;
    } else {
        return d->m_additionalActions.value(type);
    }
}

/** RichMediaAnnotation [Annotation] */

class Okular::RichMediaAnnotationPrivate : public Okular::AnnotationPrivate
{
public:
    RichMediaAnnotationPrivate();
    ~RichMediaAnnotationPrivate() override;
    void setAnnotationProperties(const QDomNode &node) override;
    AnnotationPrivate *getNewAnnotationPrivate() override;

    // data fields
    Movie *movie;
    EmbeddedFile *embeddedFile;
};

RichMediaAnnotationPrivate::RichMediaAnnotationPrivate()
    : movie(nullptr)
    , embeddedFile(nullptr)
{
}

RichMediaAnnotationPrivate::~RichMediaAnnotationPrivate()
{
    delete movie;
    delete embeddedFile;
}

void RichMediaAnnotationPrivate::setAnnotationProperties(const QDomNode &node)
{
    Okular::AnnotationPrivate::setAnnotationProperties(node);

    // loop through the whole children looking for a 'richMedia' element
    QDomNode subNode = node.firstChild();
    while (subNode.isElement()) {
        QDomElement e = subNode.toElement();
        subNode = subNode.nextSibling();
        if (e.tagName() != QLatin1String("richMedia")) {
            continue;
        }

        // loading complete
        break;
    }
}

AnnotationPrivate *RichMediaAnnotationPrivate::getNewAnnotationPrivate()
{
    return new RichMediaAnnotationPrivate();
}

RichMediaAnnotation::RichMediaAnnotation()
    : Annotation(*new RichMediaAnnotationPrivate())
{
}

RichMediaAnnotation::RichMediaAnnotation(const QDomNode &description)
    : Annotation(*new RichMediaAnnotationPrivate, description)
{
}

RichMediaAnnotation::~RichMediaAnnotation()
{
}

void RichMediaAnnotation::store(QDomNode &node, QDomDocument &document) const
{
    // recurse to parent objects storing properties
    Annotation::store(node, document);

    // create [richMedia] element
    QDomElement movieElement = document.createElement(QStringLiteral("richMedia"));
    node.appendChild(movieElement);
}

Annotation::SubType RichMediaAnnotation::subType() const
{
    return ARichMedia;
}

void RichMediaAnnotation::setMovie(Movie *movie)
{
    Q_D(RichMediaAnnotation);

    delete d->movie;
    d->movie = movie;
}

Movie *RichMediaAnnotation::movie() const
{
    Q_D(const RichMediaAnnotation);

    return d->movie;
}

EmbeddedFile *RichMediaAnnotation::embeddedFile() const
{
    Q_D(const RichMediaAnnotation);

    return d->embeddedFile;
}

void RichMediaAnnotation::setEmbeddedFile(EmbeddedFile *embeddedFile)
{
    Q_D(RichMediaAnnotation);

    delete d->embeddedFile;
    d->embeddedFile = embeddedFile;
}

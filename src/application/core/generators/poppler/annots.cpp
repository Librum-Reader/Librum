/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>
    SPDX-FileCopyrightText: 2012 Guillermo A. Amaral B. <gamaral@kde.org>

    Work sponsored by the LiMux project of the city of Munich:
    SPDX-FileCopyrightText: 2017 Klarälvdalens Datakonsult AB a KDAB Group
   company <info@kdab.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "annots.h"

// qt/kde includes
#include <core/annotations.h>
#include <core/area.h>
#include <QFileInfo>
#include <QLoggingCategory>
#include <QVariant>
#include "debug_pdf.h"
#include "generator_pdf.h"
#include "popplerembeddedfile.h"

Q_DECLARE_METATYPE(Poppler::Annotation*)

extern Okular::Sound* createSoundFromPopplerSound(
    const Poppler::SoundObject* popplerSound);
extern Okular::Movie* createMovieFromPopplerMovie(
    const Poppler::MovieObject* popplerMovie);
extern Okular::Movie* createMovieFromPopplerScreen(
    const Poppler::LinkRendition* popplerScreen);
extern QPair<Okular::Movie*, Okular::EmbeddedFile*>
    createMovieFromPopplerRichMedia(
        const Poppler::RichMediaAnnotation* popplerRichMedia);

static void disposeAnnotation(const Okular::Annotation* ann)
{
    Poppler::Annotation* popplerAnn =
        qvariant_cast<Poppler::Annotation*>(ann->nativeId());
    delete popplerAnn;
}

static QPointF normPointToPointF(const Okular::NormalizedPoint& pt)
{
    return QPointF(pt.x, pt.y);
}

static QRectF normRectToRectF(const Okular::NormalizedRect& rect)
{
    return QRectF(QPointF(rect.left, rect.top),
                  QPointF(rect.right, rect.bottom));
}

// Poppler and Okular share the same flag values, but we don't want to export
// internal flags
static int maskExportedFlags(int flags)
{
    return flags &
           (Okular::Annotation::Hidden | Okular::Annotation::FixedSize |
            Okular::Annotation::FixedRotation | Okular::Annotation::DenyPrint |
            Okular::Annotation::DenyWrite | Okular::Annotation::DenyDelete |
            Okular::Annotation::ToggleHidingOnMouse);
}

// BEGIN PopplerAnnotationProxy implementation
PopplerAnnotationProxy::PopplerAnnotationProxy(
    Poppler::Document* doc, QMutex* userMutex,
    QHash<Okular::Annotation*, Poppler::Annotation*>* annotsOnOpenHash) :
    ppl_doc(doc),
    mutex(userMutex),
    annotationsOnOpenHash(annotsOnOpenHash)
{
}

PopplerAnnotationProxy::~PopplerAnnotationProxy()
{
}

bool PopplerAnnotationProxy::supports(Capability cap) const
{
    switch(cap)
    {
    case Addition:
    case Modification:
    case Removal:
        return true;
    default:
        return false;
    }
}

static Poppler::TextAnnotation::TextType okularToPoppler(
    Okular::TextAnnotation::TextType ott)
{
    switch(ott)
    {
    case Okular::TextAnnotation::Linked:
        return Poppler::TextAnnotation::Linked;
    case Okular::TextAnnotation::InPlace:
        return Poppler::TextAnnotation::InPlace;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << ott;
    }

    return Poppler::TextAnnotation::Linked;
}

static Poppler::Annotation::LineEffect okularToPoppler(
    Okular::Annotation::LineEffect ole)
{
    switch(ole)
    {
    case Okular::Annotation::NoEffect:
        return Poppler::Annotation::NoEffect;
    case Okular::Annotation::Cloudy:
        return Poppler::Annotation::Cloudy;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << ole;
    }

    return Poppler::Annotation::NoEffect;
}

static Poppler::Annotation::LineStyle okularToPoppler(
    Okular::Annotation::LineStyle ols)
{
    switch(ols)
    {
    case Okular::Annotation::Solid:
        return Poppler::Annotation::Solid;
    case Okular::Annotation::Dashed:
        return Poppler::Annotation::Dashed;
    case Okular::Annotation::Beveled:
        return Poppler::Annotation::Beveled;
    case Okular::Annotation::Inset:
        return Poppler::Annotation::Inset;
    case Okular::Annotation::Underline:
        return Poppler::Annotation::Underline;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << ols;
    }

    return Poppler::Annotation::Solid;
}

static Poppler::TextAnnotation::InplaceIntent okularToPoppler(
    Okular::TextAnnotation::InplaceIntent oii)
{
    switch(oii)
    {
    case Okular::TextAnnotation::Unknown:
        return Poppler::TextAnnotation::Unknown;
    case Okular::TextAnnotation::Callout:
        return Poppler::TextAnnotation::Callout;
    case Okular::TextAnnotation::TypeWriter:
        return Poppler::TextAnnotation::TypeWriter;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << oii;
    }

    return Poppler::TextAnnotation::Unknown;
}

static Poppler::LineAnnotation::TermStyle okularToPoppler(
    Okular::LineAnnotation::TermStyle ots)
{
    switch(ots)
    {
    case Okular::LineAnnotation::Square:
        return Poppler::LineAnnotation::Square;
    case Okular::LineAnnotation::Circle:
        return Poppler::LineAnnotation::Circle;
    case Okular::LineAnnotation::Diamond:
        return Poppler::LineAnnotation::Diamond;
    case Okular::LineAnnotation::OpenArrow:
        return Poppler::LineAnnotation::OpenArrow;
    case Okular::LineAnnotation::ClosedArrow:
        return Poppler::LineAnnotation::ClosedArrow;
    case Okular::LineAnnotation::None:
        return Poppler::LineAnnotation::None;
    case Okular::LineAnnotation::Butt:
        return Poppler::LineAnnotation::Butt;
    case Okular::LineAnnotation::ROpenArrow:
        return Poppler::LineAnnotation::ROpenArrow;
    case Okular::LineAnnotation::RClosedArrow:
        return Poppler::LineAnnotation::RClosedArrow;
    case Okular::LineAnnotation::Slash:
        return Poppler::LineAnnotation::Slash;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << ots;
    }

    return Poppler::LineAnnotation::None;
}

static Poppler::LineAnnotation::LineIntent okularToPoppler(
    Okular::LineAnnotation::LineIntent oli)
{
    switch(oli)
    {
    case Okular::LineAnnotation::Unknown:
        return Poppler::LineAnnotation::Unknown;
    case Okular::LineAnnotation::Arrow:
        return Poppler::LineAnnotation::Arrow;
    case Okular::LineAnnotation::Dimension:
        return Poppler::LineAnnotation::Dimension;
    case Okular::LineAnnotation::PolygonCloud:
        return Poppler::LineAnnotation::PolygonCloud;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << oli;
    }

    return Poppler::LineAnnotation::Unknown;
}

static Poppler::GeomAnnotation::GeomType okularToPoppler(
    Okular::GeomAnnotation::GeomType ogt)
{
    switch(ogt)
    {
    case Okular::GeomAnnotation::InscribedSquare:
        return Poppler::GeomAnnotation::InscribedSquare;
    case Okular::GeomAnnotation::InscribedCircle:
        return Poppler::GeomAnnotation::InscribedCircle;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << ogt;
    }

    return Poppler::GeomAnnotation::InscribedSquare;
}

static Poppler::HighlightAnnotation::HighlightType okularToPoppler(
    Okular::HighlightAnnotation::HighlightType oht)
{
    switch(oht)
    {
    case Okular::HighlightAnnotation::Highlight:
        return Poppler::HighlightAnnotation::Highlight;
    case Okular::HighlightAnnotation::Squiggly:
        return Poppler::HighlightAnnotation::Squiggly;
    case Okular::HighlightAnnotation::Underline:
        return Poppler::HighlightAnnotation::Underline;
    case Okular::HighlightAnnotation::StrikeOut:
        return Poppler::HighlightAnnotation::StrikeOut;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << oht;
    }

    return Poppler::HighlightAnnotation::Highlight;
}

static Poppler::CaretAnnotation::CaretSymbol okularToPoppler(
    Okular::CaretAnnotation::CaretSymbol ocs)
{
    switch(ocs)
    {
    case Okular::CaretAnnotation::None:
        return Poppler::CaretAnnotation::None;
    case Okular::CaretAnnotation::P:
        return Poppler::CaretAnnotation::P;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << ocs;
    }

    return Poppler::CaretAnnotation::None;
}

static Poppler::Annotation::Style okularToPoppler(
    const Okular::Annotation::Style& oStyle)
{
    Poppler::Annotation::Style pStyle;
    pStyle.setColor(oStyle.color());
    pStyle.setOpacity(oStyle.opacity());
    pStyle.setLineEffect(okularToPoppler(oStyle.lineEffect()));
    pStyle.setEffectIntensity(oStyle.effectIntensity());
    pStyle.setWidth(oStyle.width());
    pStyle.setLineStyle(okularToPoppler(oStyle.lineStyle()));
    pStyle.setXCorners(oStyle.xCorners());
    pStyle.setYCorners(oStyle.yCorners());

    return pStyle;
}

static Poppler::Annotation::Popup okularToPoppler(
    const Okular::Annotation::Window& oWindow)
{
    Poppler::Annotation::Popup pWindow;
    pWindow.setGeometry(QRectF(oWindow.topLeft().x, oWindow.topLeft().y,
                               oWindow.width(), oWindow.height()));
    // flags being ints is super fragile, should be enums on both ends, but
    // Poppler::Annotation::setPopup is a noop so it's not like it matters
    pWindow.setFlags(oWindow.flags());
    pWindow.setTitle(oWindow.title());
    pWindow.setSummary(oWindow.summary());

    return pWindow;
}

static void setSharedAnnotationPropertiesToPopplerAnnotation(
    const Okular::Annotation* okularAnnotation,
    Poppler::Annotation* popplerAnnotation)
{
    popplerAnnotation->setAuthor(okularAnnotation->author());
    popplerAnnotation->setContents(okularAnnotation->contents());
    popplerAnnotation->setUniqueName(okularAnnotation->uniqueName());

    // Note: flags and boundary must be set first in order to correctly handle
    // FixedRotation annotations.
    popplerAnnotation->setFlags(maskExportedFlags(okularAnnotation->flags()));
    popplerAnnotation->setBoundary(
        normRectToRectF(okularAnnotation->boundingRectangle()));

    popplerAnnotation->setStyle(okularToPoppler(okularAnnotation->style()));
    popplerAnnotation->setPopup(okularToPoppler(okularAnnotation->window()));

    popplerAnnotation->setCreationDate(okularAnnotation->creationDate());
    popplerAnnotation->setModificationDate(
        okularAnnotation->modificationDate());
}

#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(21, 10, 0)
static void setPopplerStampAnnotationCustomImage(
    const Poppler::Page* page, Poppler::StampAnnotation* pStampAnnotation,
    const Okular::StampAnnotation* oStampAnnotation)
{
    const QSize size = page->pageSize();
    const QRect rect = Okular::AnnotationUtils::annotationGeometry(
        oStampAnnotation, size.width(), size.height());

    QImage image =
        Okular::AnnotationUtils::loadStamp(oStampAnnotation->stampIconName(),
                                           qMax(rect.width(), rect.height()))
            .toImage();

    if(!image.isNull())
    {
        pStampAnnotation->setStampCustomImage(image);
    }
}
#endif

static void updatePopplerAnnotationFromOkularAnnotation(
    const Okular::TextAnnotation* oTextAnnotation,
    Poppler::TextAnnotation* pTextAnnotation)
{
    pTextAnnotation->setTextIcon(oTextAnnotation->textIcon());
    pTextAnnotation->setTextFont(oTextAnnotation->textFont());
    pTextAnnotation->setTextColor(oTextAnnotation->textColor());
    pTextAnnotation->setInplaceAlign(oTextAnnotation->inplaceAlignment());
    pTextAnnotation->setInplaceIntent(
        okularToPoppler(oTextAnnotation->inplaceIntent()));
    pTextAnnotation->setCalloutPoints(QVector<QPointF>());
}

static void updatePopplerAnnotationFromOkularAnnotation(
    const Okular::LineAnnotation* oLineAnnotation,
    Poppler::LineAnnotation* pLineAnnotation)
{
    QLinkedList<QPointF> points;
    const QList<Okular::NormalizedPoint> annotPoints =
        oLineAnnotation->linePoints();
    for(const Okular::NormalizedPoint& p : annotPoints)
    {
        points.append(normPointToPointF(p));
    }
    pLineAnnotation->setLinePoints(points);
    pLineAnnotation->setLineStartStyle(
        okularToPoppler(oLineAnnotation->lineStartStyle()));
    pLineAnnotation->setLineEndStyle(
        okularToPoppler(oLineAnnotation->lineEndStyle()));
    pLineAnnotation->setLineClosed(oLineAnnotation->lineClosed());
    pLineAnnotation->setLineInnerColor(oLineAnnotation->lineInnerColor());
    pLineAnnotation->setLineLeadingForwardPoint(
        oLineAnnotation->lineLeadingForwardPoint());
    pLineAnnotation->setLineLeadingBackPoint(
        oLineAnnotation->lineLeadingBackwardPoint());
    pLineAnnotation->setLineShowCaption(oLineAnnotation->showCaption());
    pLineAnnotation->setLineIntent(
        okularToPoppler(oLineAnnotation->lineIntent()));
}

static void updatePopplerAnnotationFromOkularAnnotation(
    const Okular::GeomAnnotation* oGeomAnnotation,
    Poppler::GeomAnnotation* pGeomAnnotation)
{
    pGeomAnnotation->setGeomType(
        okularToPoppler(oGeomAnnotation->geometricalType()));
    pGeomAnnotation->setGeomInnerColor(
        oGeomAnnotation->geometricalInnerColor());
}

static void updatePopplerAnnotationFromOkularAnnotation(
    const Okular::HighlightAnnotation* oHighlightAnnotation,
    Poppler::HighlightAnnotation* pHighlightAnnotation)
{
    pHighlightAnnotation->setHighlightType(
        okularToPoppler(oHighlightAnnotation->highlightType()));

    const QList<Okular::HighlightAnnotation::Quad>& oQuads =
        oHighlightAnnotation->highlightQuads();
    QList<Poppler::HighlightAnnotation::Quad> pQuads;
    for(const Okular::HighlightAnnotation::Quad& oQuad : oQuads)
    {
        Poppler::HighlightAnnotation::Quad pQuad;
        pQuad.points[0] = normPointToPointF(oQuad.point(3));
        pQuad.points[1] = normPointToPointF(oQuad.point(2));
        pQuad.points[2] = normPointToPointF(oQuad.point(1));
        pQuad.points[3] = normPointToPointF(oQuad.point(0));
        pQuad.capStart = oQuad.capStart();
        pQuad.capEnd = oQuad.capEnd();
        pQuad.feather = oQuad.feather();
        pQuads << pQuad;
    }
    pHighlightAnnotation->setHighlightQuads(pQuads);
}

#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(21, 10, 0)
static void updatePopplerAnnotationFromOkularAnnotation(
    const Okular::StampAnnotation* oStampAnnotation,
    Poppler::StampAnnotation* pStampAnnotation, const Poppler::Page* page)
{
    setPopplerStampAnnotationCustomImage(page, pStampAnnotation,
                                         oStampAnnotation);
}
#else
static void updatePopplerAnnotationFromOkularAnnotation(
    const Okular::StampAnnotation* oStampAnnotation,
    Poppler::StampAnnotation* pStampAnnotation)
{
    pStampAnnotation->setStampIconName(oStampAnnotation->stampIconName());
}
#endif

static void updatePopplerAnnotationFromOkularAnnotation(
    const Okular::InkAnnotation* oInkAnnotation,
    Poppler::InkAnnotation* pInkAnnotation)
{
    QList<QLinkedList<QPointF>> paths;
    const QList<QList<Okular::NormalizedPoint>> inkPathsList =
        oInkAnnotation->inkPaths();
    for(const QList<Okular::NormalizedPoint>& path : inkPathsList)
    {
        QLinkedList<QPointF> points;
        for(const Okular::NormalizedPoint& p : path)
        {
            points.append(normPointToPointF(p));
        }
        paths.append(points);
    }
    pInkAnnotation->setInkPaths(paths);
}

static void updatePopplerAnnotationFromOkularAnnotation(
    const Okular::CaretAnnotation* oCaretAnnotation,
    Poppler::CaretAnnotation* pCaretAnnotation)
{
    pCaretAnnotation->setCaretSymbol(
        okularToPoppler(oCaretAnnotation->caretSymbol()));
}

static Poppler::Annotation* createPopplerAnnotationFromOkularAnnotation(
    const Okular::TextAnnotation* oTextAnnotation)
{
    Poppler::TextAnnotation* pTextAnnotation = new Poppler::TextAnnotation(
        okularToPoppler(oTextAnnotation->textType()));

    setSharedAnnotationPropertiesToPopplerAnnotation(oTextAnnotation,
                                                     pTextAnnotation);
    updatePopplerAnnotationFromOkularAnnotation(oTextAnnotation,
                                                pTextAnnotation);

    return pTextAnnotation;
}

static Poppler::Annotation* createPopplerAnnotationFromOkularAnnotation(
    const Okular::LineAnnotation* oLineAnnotation)
{
    const auto points = oLineAnnotation->linePoints();
    Poppler::LineAnnotation* pLineAnnotation = new Poppler::LineAnnotation(
        points.size() == 2 ? Poppler::LineAnnotation::StraightLine
                           : Poppler::LineAnnotation::Polyline);

    setSharedAnnotationPropertiesToPopplerAnnotation(oLineAnnotation,
                                                     pLineAnnotation);
    updatePopplerAnnotationFromOkularAnnotation(oLineAnnotation,
                                                pLineAnnotation);

    return pLineAnnotation;
}

static Poppler::Annotation* createPopplerAnnotationFromOkularAnnotation(
    const Okular::GeomAnnotation* oGeomAnnotation)
{
    Poppler::GeomAnnotation* pGeomAnnotation = new Poppler::GeomAnnotation();

    setSharedAnnotationPropertiesToPopplerAnnotation(oGeomAnnotation,
                                                     pGeomAnnotation);
    updatePopplerAnnotationFromOkularAnnotation(oGeomAnnotation,
                                                pGeomAnnotation);

    return pGeomAnnotation;
}

static Poppler::Annotation* createPopplerAnnotationFromOkularAnnotation(
    const Okular::HighlightAnnotation* oHighlightAnnotation)
{
    Poppler::HighlightAnnotation* pHighlightAnnotation =
        new Poppler::HighlightAnnotation();

    setSharedAnnotationPropertiesToPopplerAnnotation(oHighlightAnnotation,
                                                     pHighlightAnnotation);
    updatePopplerAnnotationFromOkularAnnotation(oHighlightAnnotation,
                                                pHighlightAnnotation);

    return pHighlightAnnotation;
}

#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(21, 10, 0)
static Poppler::Annotation* createPopplerAnnotationFromOkularAnnotation(
    const Okular::StampAnnotation* oStampAnnotation, Poppler::Page* page)
{
    Poppler::StampAnnotation* pStampAnnotation = new Poppler::StampAnnotation();

    setSharedAnnotationPropertiesToPopplerAnnotation(oStampAnnotation,
                                                     pStampAnnotation);
    updatePopplerAnnotationFromOkularAnnotation(oStampAnnotation,
                                                pStampAnnotation, page);

    return pStampAnnotation;
}
#else
static Poppler::Annotation* createPopplerAnnotationFromOkularAnnotation(
    const Okular::StampAnnotation* oStampAnnotation)
{
    Poppler::StampAnnotation* pStampAnnotation = new Poppler::StampAnnotation();

    setSharedAnnotationPropertiesToPopplerAnnotation(oStampAnnotation,
                                                     pStampAnnotation);
    updatePopplerAnnotationFromOkularAnnotation(oStampAnnotation,
                                                pStampAnnotation);

    return pStampAnnotation;
}
#endif

static Poppler::Annotation* createPopplerAnnotationFromOkularAnnotation(
    const Okular::InkAnnotation* oInkAnnotation)
{
    Poppler::InkAnnotation* pInkAnnotation = new Poppler::InkAnnotation();

    setSharedAnnotationPropertiesToPopplerAnnotation(oInkAnnotation,
                                                     pInkAnnotation);
    updatePopplerAnnotationFromOkularAnnotation(oInkAnnotation, pInkAnnotation);

    return pInkAnnotation;
}

static Poppler::Annotation* createPopplerAnnotationFromOkularAnnotation(
    const Okular::CaretAnnotation* oCaretAnnotation)
{
    Poppler::CaretAnnotation* pCaretAnnotation = new Poppler::CaretAnnotation();

    setSharedAnnotationPropertiesToPopplerAnnotation(oCaretAnnotation,
                                                     pCaretAnnotation);
    updatePopplerAnnotationFromOkularAnnotation(oCaretAnnotation,
                                                pCaretAnnotation);

    return pCaretAnnotation;
}

void PopplerAnnotationProxy::notifyAddition(Okular::Annotation* okl_ann,
                                            int page)
{
    QMutexLocker ml(mutex);

    Poppler::Page* ppl_page = ppl_doc->page(page);

    // Create poppler annotation
    Poppler::Annotation* ppl_ann = nullptr;
    switch(okl_ann->subType())
    {
    case Okular::Annotation::AText:
        ppl_ann = createPopplerAnnotationFromOkularAnnotation(
            static_cast<Okular::TextAnnotation*>(okl_ann));
        break;
    case Okular::Annotation::ALine:
        ppl_ann = createPopplerAnnotationFromOkularAnnotation(
            static_cast<Okular::LineAnnotation*>(okl_ann));
        break;
    case Okular::Annotation::AGeom:
        ppl_ann = createPopplerAnnotationFromOkularAnnotation(
            static_cast<Okular::GeomAnnotation*>(okl_ann));
        break;
    case Okular::Annotation::AHighlight:
        ppl_ann = createPopplerAnnotationFromOkularAnnotation(
            static_cast<Okular::HighlightAnnotation*>(okl_ann));
        break;
    case Okular::Annotation::AStamp:
#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(21, 10, 0)
    {
        bool wasDenyWriteEnabled =
            okl_ann->flags() & Okular::Annotation::DenyWrite;

        if(wasDenyWriteEnabled)
        {
            okl_ann->setFlags(okl_ann->flags() &
                              ~Okular::Annotation::DenyWrite);
        }

        ppl_ann = createPopplerAnnotationFromOkularAnnotation(
            static_cast<Okular::StampAnnotation*>(okl_ann), ppl_page);
        if(deletedStampsAnnotationAppearance.find(
               static_cast<Okular::StampAnnotation*>(okl_ann)) !=
           deletedStampsAnnotationAppearance.end())
        {
            ppl_ann->setAnnotationAppearance(
                *deletedStampsAnnotationAppearance
                     [static_cast<Okular::StampAnnotation*>(okl_ann)]
                         .get());
            deletedStampsAnnotationAppearance.erase(
                static_cast<Okular::StampAnnotation*>(okl_ann));

            if(wasDenyWriteEnabled)
            {
                okl_ann->setFlags(okl_ann->flags() |
                                  Okular::Annotation::DenyWrite);
            }
        }
    }
#else
        ppl_ann = createPopplerAnnotationFromOkularAnnotation(
            static_cast<Okular::StampAnnotation*>(okl_ann));
#endif
    break;
    case Okular::Annotation::AInk:
        ppl_ann = createPopplerAnnotationFromOkularAnnotation(
            static_cast<Okular::InkAnnotation*>(okl_ann));
        break;
    case Okular::Annotation::ACaret:
        ppl_ann = createPopplerAnnotationFromOkularAnnotation(
            static_cast<Okular::CaretAnnotation*>(okl_ann));
        break;
    default:
        qWarning() << "Unsupported annotation type" << okl_ann->subType();
        return;
    }

#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(21, 10, 0)
    okl_ann->setFlags(okl_ann->flags() | Okular::Annotation::ExternallyDrawn);
#else
    // Poppler doesn't render StampAnnotations yet
    if(ppl_ann->subType() != Poppler::Annotation::AStamp)
    {
        okl_ann->setFlags(okl_ann->flags() |
                          Okular::Annotation::ExternallyDrawn);
    }
#endif

    // Bind poppler object to page
    ppl_page->addAnnotation(ppl_ann);
    delete ppl_page;

    // Set pointer to poppler annotation as native Id
    okl_ann->setNativeId(QVariant::fromValue(ppl_ann));
    okl_ann->setDisposeDataFunction(disposeAnnotation);

    qCDebug(OkularPdfDebug) << okl_ann->uniqueName();
}

void PopplerAnnotationProxy::notifyModification(
    const Okular::Annotation* okl_ann, int page, bool appearanceChanged)
{
    Q_UNUSED(page);
    Q_UNUSED(appearanceChanged);

    Poppler::Annotation* ppl_ann =
        qvariant_cast<Poppler::Annotation*>(okl_ann->nativeId());

    if(!ppl_ann)
    {  // Ignore non-native annotations
        return;
    }

    QMutexLocker ml(mutex);

    if(okl_ann->flags() &
       (Okular::Annotation::BeingMoved | Okular::Annotation::BeingResized))
    {
        // Okular ui already renders the annotation on its own
        ppl_ann->setFlags(Poppler::Annotation::Hidden);
        return;
    }

    // Set basic properties
    // Note: flags and boundary must be set first in order to correctly handle
    // FixedRotation annotations.
    ppl_ann->setFlags(maskExportedFlags(okl_ann->flags()));
    ppl_ann->setBoundary(normRectToRectF(okl_ann->boundingRectangle()));

    ppl_ann->setAuthor(okl_ann->author());
    ppl_ann->setContents(okl_ann->contents());

    ppl_ann->setStyle(okularToPoppler(okl_ann->style()));

    // Set type-specific properties (if any)
    switch(ppl_ann->subType())
    {
    case Poppler::Annotation::AText:
    {
        const Okular::TextAnnotation* okl_txtann =
            static_cast<const Okular::TextAnnotation*>(okl_ann);
        Poppler::TextAnnotation* ppl_txtann =
            static_cast<Poppler::TextAnnotation*>(ppl_ann);
        updatePopplerAnnotationFromOkularAnnotation(okl_txtann, ppl_txtann);
        break;
    }
    case Poppler::Annotation::ALine:
    {
        const Okular::LineAnnotation* okl_lineann =
            static_cast<const Okular::LineAnnotation*>(okl_ann);
        Poppler::LineAnnotation* ppl_lineann =
            static_cast<Poppler::LineAnnotation*>(ppl_ann);
        updatePopplerAnnotationFromOkularAnnotation(okl_lineann, ppl_lineann);
        break;
    }
    case Poppler::Annotation::AGeom:
    {
        const Okular::GeomAnnotation* okl_geomann =
            static_cast<const Okular::GeomAnnotation*>(okl_ann);
        Poppler::GeomAnnotation* ppl_geomann =
            static_cast<Poppler::GeomAnnotation*>(ppl_ann);
        updatePopplerAnnotationFromOkularAnnotation(okl_geomann, ppl_geomann);
        break;
    }
    case Poppler::Annotation::AHighlight:
    {
        const Okular::HighlightAnnotation* okl_hlann =
            static_cast<const Okular::HighlightAnnotation*>(okl_ann);
        Poppler::HighlightAnnotation* ppl_hlann =
            static_cast<Poppler::HighlightAnnotation*>(ppl_ann);
        updatePopplerAnnotationFromOkularAnnotation(okl_hlann, ppl_hlann);
        break;
    }
    case Poppler::Annotation::AStamp:
    {
        const Okular::StampAnnotation* okl_stampann =
            static_cast<const Okular::StampAnnotation*>(okl_ann);
        Poppler::StampAnnotation* ppl_stampann =
            static_cast<Poppler::StampAnnotation*>(ppl_ann);
#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(21, 10, 0)
        Poppler::Page* ppl_page = ppl_doc->page(page);
        updatePopplerAnnotationFromOkularAnnotation(okl_stampann, ppl_stampann,
                                                    ppl_page);
        delete ppl_page;
#else
        updatePopplerAnnotationFromOkularAnnotation(okl_stampann, ppl_stampann);
#endif
        break;
    }
    case Poppler::Annotation::AInk:
    {
        const Okular::InkAnnotation* okl_inkann =
            static_cast<const Okular::InkAnnotation*>(okl_ann);
        Poppler::InkAnnotation* ppl_inkann =
            static_cast<Poppler::InkAnnotation*>(ppl_ann);
        updatePopplerAnnotationFromOkularAnnotation(okl_inkann, ppl_inkann);
        break;
    }
    default:
        qCDebug(OkularPdfDebug) << "Type-specific property modification is not "
                                   "implemented for this annotation type";
        break;
    }

    qCDebug(OkularPdfDebug) << okl_ann->uniqueName();
}

void PopplerAnnotationProxy::notifyRemoval(Okular::Annotation* okl_ann,
                                           int page)
{
    Poppler::Annotation* ppl_ann =
        qvariant_cast<Poppler::Annotation*>(okl_ann->nativeId());

    if(!ppl_ann)
    {  // Ignore non-native annotations
        return;
    }

    QMutexLocker ml(mutex);

    Poppler::Page* ppl_page = ppl_doc->page(page);
    annotationsOnOpenHash->remove(okl_ann);
#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(21, 10, 0)
    if(okl_ann->subType() == Okular::Annotation::AStamp)
    {
        deletedStampsAnnotationAppearance[static_cast<Okular::StampAnnotation*>(
            okl_ann)] = ppl_ann->annotationAppearance();
    }
#endif
    ppl_page->removeAnnotation(ppl_ann);  // Also destroys ppl_ann
    delete ppl_page;

    okl_ann->setNativeId(QVariant::fromValue(
        0));  // So that we don't double-free in disposeAnnotation

    qCDebug(OkularPdfDebug) << okl_ann->uniqueName();
}

// END PopplerAnnotationProxy implementation

static Okular::Annotation::LineStyle popplerToOkular(
    Poppler::Annotation::LineStyle s)
{
    switch(s)
    {
    case Poppler::Annotation::Solid:
        return Okular::Annotation::Solid;
    case Poppler::Annotation::Dashed:
        return Okular::Annotation::Dashed;
    case Poppler::Annotation::Beveled:
        return Okular::Annotation::Beveled;
    case Poppler::Annotation::Inset:
        return Okular::Annotation::Inset;
    case Poppler::Annotation::Underline:
        return Okular::Annotation::Underline;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << s;
    }

    return Okular::Annotation::Solid;
}

static Okular::Annotation::LineEffect popplerToOkular(
    Poppler::Annotation::LineEffect e)
{
    switch(e)
    {
    case Poppler::Annotation::NoEffect:
        return Okular::Annotation::NoEffect;
    case Poppler::Annotation::Cloudy:
        return Okular::Annotation::Cloudy;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << e;
    }

    return Okular::Annotation::NoEffect;
}

static Okular::Annotation::RevisionScope popplerToOkular(
    Poppler::Annotation::RevScope s)
{
    switch(s)
    {
    case Poppler::Annotation::Root:
        Q_UNREACHABLE();
    case Poppler::Annotation::Reply:
        return Okular::Annotation::Reply;
    case Poppler::Annotation::Group:
        return Okular::Annotation::Group;
    case Poppler::Annotation::Delete:
        return Okular::Annotation::Delete;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << s;
    }

    return Okular::Annotation::Reply;
}

static Okular::Annotation::RevisionType popplerToOkular(
    Poppler::Annotation::RevType t)
{
    switch(t)
    {
    case Poppler::Annotation::None:
        return Okular::Annotation::None;
    case Poppler::Annotation::Marked:
        return Okular::Annotation::Marked;
    case Poppler::Annotation::Unmarked:
        return Okular::Annotation::Unmarked;
    case Poppler::Annotation::Accepted:
        return Okular::Annotation::Accepted;
    case Poppler::Annotation::Rejected:
        return Okular::Annotation::Rejected;
    case Poppler::Annotation::Cancelled:
        return Okular::Annotation::Cancelled;
    case Poppler::Annotation::Completed:
        return Okular::Annotation::Completed;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << t;
    }

    return Okular::Annotation::None;
}

static Okular::TextAnnotation::TextType popplerToOkular(
    Poppler::TextAnnotation::TextType ptt)
{
    switch(ptt)
    {
    case Poppler::TextAnnotation::Linked:
        return Okular::TextAnnotation::Linked;
    case Poppler::TextAnnotation::InPlace:
        return Okular::TextAnnotation::InPlace;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << ptt;
    }

    return Okular::TextAnnotation::Linked;
}

static Okular::TextAnnotation::InplaceIntent popplerToOkular(
    Poppler::TextAnnotation::InplaceIntent pii)
{
    switch(pii)
    {
    case Poppler::TextAnnotation::Unknown:
        return Okular::TextAnnotation::Unknown;
    case Poppler::TextAnnotation::Callout:
        return Okular::TextAnnotation::Callout;
    case Poppler::TextAnnotation::TypeWriter:
        return Okular::TextAnnotation::TypeWriter;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << pii;
    }

    return Okular::TextAnnotation::Unknown;
}

static Okular::LineAnnotation::TermStyle popplerToOkular(
    Poppler::LineAnnotation::TermStyle pts)
{
    switch(pts)
    {
    case Poppler::LineAnnotation::Square:
        return Okular::LineAnnotation::Square;
    case Poppler::LineAnnotation::Circle:
        return Okular::LineAnnotation::Circle;
    case Poppler::LineAnnotation::Diamond:
        return Okular::LineAnnotation::Diamond;
    case Poppler::LineAnnotation::OpenArrow:
        return Okular::LineAnnotation::OpenArrow;
    case Poppler::LineAnnotation::ClosedArrow:
        return Okular::LineAnnotation::ClosedArrow;
    case Poppler::LineAnnotation::None:
        return Okular::LineAnnotation::None;
    case Poppler::LineAnnotation::Butt:
        return Okular::LineAnnotation::Butt;
    case Poppler::LineAnnotation::ROpenArrow:
        return Okular::LineAnnotation::ROpenArrow;
    case Poppler::LineAnnotation::RClosedArrow:
        return Okular::LineAnnotation::RClosedArrow;
    case Poppler::LineAnnotation::Slash:
        return Okular::LineAnnotation::Slash;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << pts;
    }

    return Okular::LineAnnotation::None;
}

static Okular::LineAnnotation::LineIntent popplerToOkular(
    Poppler::LineAnnotation::LineIntent pli)
{
    switch(pli)
    {
    case Poppler::LineAnnotation::Unknown:
        return Okular::LineAnnotation::Unknown;
    case Poppler::LineAnnotation::Arrow:
        return Okular::LineAnnotation::Arrow;
    case Poppler::LineAnnotation::Dimension:
        return Okular::LineAnnotation::Dimension;
    case Poppler::LineAnnotation::PolygonCloud:
        return Okular::LineAnnotation::PolygonCloud;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << pli;
    }

    return Okular::LineAnnotation::Unknown;
}

static Okular::GeomAnnotation::GeomType popplerToOkular(
    Poppler::GeomAnnotation::GeomType pgt)
{
    switch(pgt)
    {
    case Poppler::GeomAnnotation::InscribedSquare:
        return Okular::GeomAnnotation::InscribedSquare;
    case Poppler::GeomAnnotation::InscribedCircle:
        return Okular::GeomAnnotation::InscribedCircle;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << pgt;
    }

    return Okular::GeomAnnotation::InscribedSquare;
}

static Okular::HighlightAnnotation::HighlightType popplerToOkular(
    Poppler::HighlightAnnotation::HighlightType pht)
{
    switch(pht)
    {
    case Poppler::HighlightAnnotation::Highlight:
        return Okular::HighlightAnnotation::Highlight;
    case Poppler::HighlightAnnotation::Squiggly:
        return Okular::HighlightAnnotation::Squiggly;
    case Poppler::HighlightAnnotation::Underline:
        return Okular::HighlightAnnotation::Underline;
    case Poppler::HighlightAnnotation::StrikeOut:
        return Okular::HighlightAnnotation::StrikeOut;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << pht;
    }

    return Okular::HighlightAnnotation::Highlight;
}

static Okular::CaretAnnotation::CaretSymbol popplerToOkular(
    Poppler::CaretAnnotation::CaretSymbol pcs)
{
    switch(pcs)
    {
    case Poppler::CaretAnnotation::None:
        return Okular::CaretAnnotation::None;
    case Poppler::CaretAnnotation::P:
        return Okular::CaretAnnotation::P;
    default:
        qWarning() << Q_FUNC_INFO << "unknown value" << pcs;
    }

    return Okular::CaretAnnotation::None;
}

static Okular::Annotation* createAnnotationFromPopplerAnnotation(
    Poppler::TextAnnotation* popplerAnnotation)
{
    Okular::TextAnnotation* oTextAnn = new Okular::TextAnnotation();

    oTextAnn->setTextType(popplerToOkular(popplerAnnotation->textType()));
    oTextAnn->setTextIcon(popplerAnnotation->textIcon());
    oTextAnn->setTextFont(popplerAnnotation->textFont());
    oTextAnn->setTextColor(popplerAnnotation->textColor());
    // this works because we use the same 0:left, 1:center, 2:right meaning both
    // in poppler and okular
    oTextAnn->setInplaceAlignment(popplerAnnotation->inplaceAlign());
    oTextAnn->setInplaceIntent(
        popplerToOkular(popplerAnnotation->inplaceIntent()));
    for(int i = 0; i < 3; ++i)
    {
        const QPointF p = popplerAnnotation->calloutPoint(i);
        oTextAnn->setInplaceCallout({ p.x(), p.y() }, i);
    }

    return oTextAnn;
}

static Okular::Annotation* createAnnotationFromPopplerAnnotation(
    const Poppler::LineAnnotation* popplerAnnotation)
{
    Okular::LineAnnotation* oLineAnn = new Okular::LineAnnotation();

    oLineAnn->setLineStartStyle(
        popplerToOkular(popplerAnnotation->lineStartStyle()));
    oLineAnn->setLineEndStyle(
        popplerToOkular(popplerAnnotation->lineEndStyle()));
    oLineAnn->setLineClosed(popplerAnnotation->isLineClosed());
    oLineAnn->setLineInnerColor(popplerAnnotation->lineInnerColor());
    oLineAnn->setLineLeadingForwardPoint(
        popplerAnnotation->lineLeadingForwardPoint());
    oLineAnn->setLineLeadingBackwardPoint(
        popplerAnnotation->lineLeadingBackPoint());
    oLineAnn->setShowCaption(popplerAnnotation->lineShowCaption());
    oLineAnn->setLineIntent(popplerToOkular(popplerAnnotation->lineIntent()));

    QList<Okular::NormalizedPoint> points;
    const QLinkedList<QPointF> popplerPoints = popplerAnnotation->linePoints();
    for(const QPointF& p : popplerPoints)
    {
        points << Okular::NormalizedPoint(p.x(), p.y());
    }
    oLineAnn->setLinePoints(points);

    return oLineAnn;
}

static Okular::Annotation* createAnnotationFromPopplerAnnotation(
    const Poppler::GeomAnnotation* popplerAnnotation)
{
    Okular::GeomAnnotation* oGeomAnn = new Okular::GeomAnnotation();

    oGeomAnn->setGeometricalType(
        popplerToOkular(popplerAnnotation->geomType()));
    oGeomAnn->setGeometricalInnerColor(popplerAnnotation->geomInnerColor());

    return oGeomAnn;
}

static Okular::Annotation* createAnnotationFromPopplerAnnotation(
    const Poppler::HighlightAnnotation* popplerAnnotation)
{
    Okular::HighlightAnnotation* oHighlightAnn =
        new Okular::HighlightAnnotation();

    oHighlightAnn->setHighlightType(
        popplerToOkular(popplerAnnotation->highlightType()));

    const QList<Poppler::HighlightAnnotation::Quad> popplerHq =
        popplerAnnotation->highlightQuads();
    QList<Okular::HighlightAnnotation::Quad>& okularHq =
        oHighlightAnn->highlightQuads();

    for(const Poppler::HighlightAnnotation::Quad& popplerQ : popplerHq)
    {
        Okular::HighlightAnnotation::Quad q;

        // Poppler stores highlight points in swapped order
        q.setPoint(Okular::NormalizedPoint(popplerQ.points[0].x(),
                                           popplerQ.points[0].y()),
                   3);
        q.setPoint(Okular::NormalizedPoint(popplerQ.points[1].x(),
                                           popplerQ.points[1].y()),
                   2);
        q.setPoint(Okular::NormalizedPoint(popplerQ.points[2].x(),
                                           popplerQ.points[2].y()),
                   1);
        q.setPoint(Okular::NormalizedPoint(popplerQ.points[3].x(),
                                           popplerQ.points[3].y()),
                   0);

        q.setCapStart(popplerQ.capStart);
        q.setCapEnd(popplerQ.capEnd);
        q.setFeather(popplerQ.feather);
        okularHq << q;
    }

    return oHighlightAnn;
}

static Okular::Annotation* createAnnotationFromPopplerAnnotation(
    const Poppler::InkAnnotation* popplerAnnotation)
{
    Okular::InkAnnotation* oInkAnn = new Okular::InkAnnotation();

    const QList<QLinkedList<QPointF>> popplerInkPaths =
        popplerAnnotation->inkPaths();
    QList<QList<Okular::NormalizedPoint>> okularInkPaths;
    for(const QLinkedList<QPointF>& popplerInkPath : popplerInkPaths)
    {
        QList<Okular::NormalizedPoint> okularInkPath;
        for(const QPointF& popplerPoint : popplerInkPath)
        {
            okularInkPath << Okular::NormalizedPoint(popplerPoint.x(),
                                                     popplerPoint.y());
        }
        okularInkPaths << okularInkPath;
    }

    oInkAnn->setInkPaths(okularInkPaths);

    return oInkAnn;
}

static Okular::Annotation* createAnnotationFromPopplerAnnotation(
    const Poppler::CaretAnnotation* popplerAnnotation)
{
    Okular::CaretAnnotation* oCaretAnn = new Okular::CaretAnnotation();

    oCaretAnn->setCaretSymbol(
        popplerToOkular(popplerAnnotation->caretSymbol()));

    return oCaretAnn;
}

static Okular::Annotation* createAnnotationFromPopplerAnnotation(
    const Poppler::StampAnnotation* popplerAnnotation)
{
    Okular::StampAnnotation* oStampAnn = new Okular::StampAnnotation();

    oStampAnn->setStampIconName(popplerAnnotation->stampIconName());

    return oStampAnn;
}

Okular::Annotation* createAnnotationFromPopplerAnnotation(
    Poppler::Annotation* popplerAnnotation, const Poppler::Page& popplerPage,
    bool* doDelete)
{
    Okular::Annotation* okularAnnotation = nullptr;
    *doDelete = true;
    bool tieToOkularAnn = false;
    bool externallyDrawn = false;
    switch(popplerAnnotation->subType())
    {
    case Poppler::Annotation::AFileAttachment:
    {
        Poppler::FileAttachmentAnnotation* attachann =
            static_cast<Poppler::FileAttachmentAnnotation*>(popplerAnnotation);
        Okular::FileAttachmentAnnotation* f =
            new Okular::FileAttachmentAnnotation();
        okularAnnotation = f;
        tieToOkularAnn = true;
        *doDelete = false;

        f->setFileIconName(attachann->fileIconName());
        f->setEmbeddedFile(new PDFEmbeddedFile(attachann->embeddedFile()));

        break;
    }
    case Poppler::Annotation::ASound:
    {
        Poppler::SoundAnnotation* soundann =
            static_cast<Poppler::SoundAnnotation*>(popplerAnnotation);
        Okular::SoundAnnotation* s = new Okular::SoundAnnotation();
        okularAnnotation = s;

        s->setSoundIconName(soundann->soundIconName());
        s->setSound(createSoundFromPopplerSound(soundann->sound()));

        break;
    }
    case Poppler::Annotation::AMovie:
    {
        Poppler::MovieAnnotation* movieann =
            static_cast<Poppler::MovieAnnotation*>(popplerAnnotation);
        Okular::MovieAnnotation* m = new Okular::MovieAnnotation();
        okularAnnotation = m;
        tieToOkularAnn = true;
        *doDelete = false;

        m->setMovie(createMovieFromPopplerMovie(movieann->movie()));

        break;
    }
    case Poppler::Annotation::AWidget:
    {
        okularAnnotation = new Okular::WidgetAnnotation();
        break;
    }
    case Poppler::Annotation::AScreen:
    {
        Okular::ScreenAnnotation* m = new Okular::ScreenAnnotation();
        okularAnnotation = m;
        tieToOkularAnn = true;
        *doDelete = false;
        break;
    }
    case Poppler::Annotation::ARichMedia:
    {
        Poppler::RichMediaAnnotation* richmediaann =
            static_cast<Poppler::RichMediaAnnotation*>(popplerAnnotation);
        const QPair<Okular::Movie*, Okular::EmbeddedFile*> result =
            createMovieFromPopplerRichMedia(richmediaann);

        if(result.first)
        {
            Okular::RichMediaAnnotation* r = new Okular::RichMediaAnnotation();
            tieToOkularAnn = true;
            *doDelete = false;
            okularAnnotation = r;

            r->setMovie(result.first);
            r->setEmbeddedFile(result.second);
        }

        break;
    }
    case Poppler::Annotation::AText:
    {
        externallyDrawn = true;
        tieToOkularAnn = true;
        *doDelete = false;
        okularAnnotation = createAnnotationFromPopplerAnnotation(
            static_cast<Poppler::TextAnnotation*>(popplerAnnotation));
        break;
    }
    case Poppler::Annotation::ALine:
    {
        externallyDrawn = true;
        tieToOkularAnn = true;
        *doDelete = false;
        okularAnnotation = createAnnotationFromPopplerAnnotation(
            static_cast<Poppler::LineAnnotation*>(popplerAnnotation));
        break;
    }
    case Poppler::Annotation::AGeom:
    {
        externallyDrawn = true;
        tieToOkularAnn = true;
        *doDelete = false;
        okularAnnotation = createAnnotationFromPopplerAnnotation(
            static_cast<Poppler::GeomAnnotation*>(popplerAnnotation));
        break;
    }
    case Poppler::Annotation::AHighlight:
    {
        externallyDrawn = true;
        tieToOkularAnn = true;
        *doDelete = false;
        okularAnnotation = createAnnotationFromPopplerAnnotation(
            static_cast<Poppler::HighlightAnnotation*>(popplerAnnotation));
        break;
    }
    case Poppler::Annotation::AInk:
    {
        externallyDrawn = true;
        tieToOkularAnn = true;
        *doDelete = false;
        okularAnnotation = createAnnotationFromPopplerAnnotation(
            static_cast<Poppler::InkAnnotation*>(popplerAnnotation));
        break;
    }
    case Poppler::Annotation::ACaret:
    {
        externallyDrawn = true;
        tieToOkularAnn = true;
        *doDelete = false;
        okularAnnotation = createAnnotationFromPopplerAnnotation(
            static_cast<Poppler::CaretAnnotation*>(popplerAnnotation));
        break;
    }
    case Poppler::Annotation::AStamp:
#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(21, 10, 0)
        externallyDrawn = true;
#endif
        tieToOkularAnn = true;
        *doDelete = false;
        okularAnnotation = createAnnotationFromPopplerAnnotation(
            static_cast<Poppler::StampAnnotation*>(popplerAnnotation));
        break;
    default:
    {
        break;
    }
    }
    if(okularAnnotation)
    {
        // the Contents field might have lines separated by \r
        QString contents = popplerAnnotation->contents();
        contents.replace(QLatin1Char('\r'), QLatin1Char('\n'));

        okularAnnotation->setAuthor(popplerAnnotation->author());
        okularAnnotation->setContents(contents);
        okularAnnotation->setUniqueName(popplerAnnotation->uniqueName());
        okularAnnotation->setModificationDate(
            popplerAnnotation->modificationDate());
        okularAnnotation->setCreationDate(popplerAnnotation->creationDate());
        okularAnnotation->setFlags(popplerAnnotation->flags() |
                                   Okular::Annotation::External);
        okularAnnotation->setBoundingRectangle(
            Okular::NormalizedRect::fromQRectF(popplerAnnotation->boundary()));

        if(externallyDrawn)
        {
            okularAnnotation->setFlags(okularAnnotation->flags() |
                                       Okular::Annotation::ExternallyDrawn);
        }
#if POPPLER_VERSION_MACRO >= QT_VERSION_CHECK(21, 10, 0)
        if(okularAnnotation->subType() == Okular::Annotation::SubType::AStamp)
        {
            Okular::StampAnnotation* oStampAnn =
                static_cast<Okular::StampAnnotation*>(okularAnnotation);
            Poppler::StampAnnotation* pStampAnn =
                static_cast<Poppler::StampAnnotation*>(popplerAnnotation);
            QFileInfo stampIconFile = oStampAnn->stampIconName();
            if(stampIconFile.exists() && stampIconFile.isFile())
            {
                setPopplerStampAnnotationCustomImage(&popplerPage, pStampAnn,
                                                     oStampAnn);
            }

            oStampAnn->setFlags(okularAnnotation->flags() |
                                Okular::Annotation::Flag::DenyWrite);
        }
#endif

        // Convert the poppler annotation style to Okular annotation style
        Okular::Annotation::Style& okularStyle = okularAnnotation->style();
        const Poppler::Annotation::Style popplerStyle =
            popplerAnnotation->style();
        okularStyle.setColor(popplerStyle.color());
        okularStyle.setOpacity(popplerStyle.opacity());
        okularStyle.setWidth(popplerStyle.width());
        okularStyle.setLineStyle(popplerToOkular(popplerStyle.lineStyle()));
        okularStyle.setXCorners(popplerStyle.xCorners());
        okularStyle.setYCorners(popplerStyle.yCorners());
        const QVector<double>& dashArray = popplerStyle.dashArray();
        if(dashArray.size() > 0)
        {
            okularStyle.setMarks(dashArray[0]);
        }
        if(dashArray.size() > 1)
        {
            okularStyle.setSpaces(dashArray[1]);
        }
        okularStyle.setLineEffect(popplerToOkular(popplerStyle.lineEffect()));
        okularStyle.setEffectIntensity(popplerStyle.effectIntensity());

        // Convert the poppler annotation popup to Okular annotation window
        Okular::Annotation::Window& okularWindow = okularAnnotation->window();
        const Poppler::Annotation::Popup popplerPopup =
            popplerAnnotation->popup();
        // This assumes that both "flags" int mean the same, but since we don't
        // use the flags in okular anywhere it's not really that important
        okularWindow.setFlags(popplerPopup.flags());
        const QRectF popplerGeometry = popplerPopup.geometry();
        const QSize popplerPageSize = popplerPage.pageSize();
        okularWindow.setTopLeft(Okular::NormalizedPoint(
            popplerGeometry.top(), popplerGeometry.left(),
            popplerPageSize.width(), popplerPageSize.height()));
        okularWindow.setWidth(popplerGeometry.width());
        okularWindow.setHeight(popplerGeometry.height());
        okularWindow.setTitle(popplerPopup.title());
        okularWindow.setSummary(popplerPopup.summary());

        // Convert the poppler revisions to Okular revisions
        QList<Okular::Annotation::Revision>& okularRevisions =
            okularAnnotation->revisions();
        const QList<Poppler::Annotation*> popplerRevisions =
            popplerAnnotation->revisions();
        for(Poppler::Annotation* popplerRevision : popplerRevisions)
        {
            bool deletePopplerRevision;
            Okular::Annotation::Revision okularRevision;
            okularRevision.setAnnotation(createAnnotationFromPopplerAnnotation(
                popplerRevision, popplerPage, &deletePopplerRevision));
            okularRevision.setScope(
                popplerToOkular(popplerRevision->revisionScope()));
            okularRevision.setType(
                popplerToOkular(popplerRevision->revisionType()));
            okularRevisions << okularRevision;

            if(deletePopplerRevision)
            {
                delete popplerRevision;
            }
        }

        if(tieToOkularAnn)
        {
            okularAnnotation->setNativeId(
                QVariant::fromValue(popplerAnnotation));
            okularAnnotation->setDisposeDataFunction(disposeAnnotation);
        }
    }
    return okularAnnotation;
}

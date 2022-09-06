/*
    SPDX-FileCopyrightText: 2006 Pino Toscano <toscano.pino@tiscali.it>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kdjvu.h"

#include <QByteArray>
#include <QDomDocument>
#include <QFile>
#include <QHash>
#include <QPainter>
#include <QQueue>
#include <QString>

#include <KLocalizedString>
#include <QDebug>

#include <libdjvu/ddjvuapi.h>
#include <libdjvu/miniexp.h>

#include <stdio.h>

QDebug &operator<<(QDebug &s, const ddjvu_rect_t r)
{
    s.nospace() << "[" << r.x << "," << r.y << " - " << r.w << "x" << r.h << "]";
    return s.space();
}

static void which_ddjvu_message(const ddjvu_message_t *msg)
{
#ifdef KDJVU_DEBUG
    qDebug() << "which_djvu_message(...):" << msg->m_any.tag;
    switch (msg->m_any.tag) {
    case DDJVU_ERROR:
        qDebug().nospace() << "ERROR: file " << msg->m_error.filename << ", line " << msg->m_error.lineno;
        qDebug().nospace() << "ERROR: function '" << msg->m_error.function << "'";
        qDebug().nospace() << "ERROR: '" << msg->m_error.message << "'";
        break;
    case DDJVU_INFO:
        qDebug().nospace() << "INFO: '" << msg->m_info.message << "'";
        break;
    case DDJVU_CHUNK:
        qDebug().nospace() << "CHUNK: '" << QByteArray(msg->m_chunk.chunkid) << "'";
        break;
    case DDJVU_PROGRESS:
        qDebug().nospace() << "PROGRESS: '" << msg->m_progress.percent << "'";
        break;
    default:;
    }
#else
    Q_UNUSED(msg);
#endif
}

/**
 * Explore the message queue until there are message left in it.
 */
static void handle_ddjvu_messages(ddjvu_context_t *ctx, int wait)
{
    const ddjvu_message_t *msg;
    if (wait) {
        ddjvu_message_wait(ctx);
    }
    while ((msg = ddjvu_message_peek(ctx))) {
        which_ddjvu_message(msg);
        ddjvu_message_pop(ctx);
    }
}

/**
 * Explore the message queue until the message \p mid is found.
 */
static void wait_for_ddjvu_message(ddjvu_context_t *ctx, ddjvu_message_tag_t mid)
{
    ddjvu_message_wait(ctx);
    const ddjvu_message_t *msg;
    while ((msg = ddjvu_message_peek(ctx)) && msg && (msg->m_any.tag != mid)) {
        which_ddjvu_message(msg);
        ddjvu_message_pop(ctx);
    }
}

/**
 * Convert a clockwise coefficient \p r for a rotation to a counter-clockwise
 * and vice versa.
 */
static int flipRotation(int r)
{
    return (4 - r) % 4;
}

static miniexp_t find_second_in_pair(miniexp_t theexp, const char *which)
{
    miniexp_t exp = theexp;
    while (exp) {
        miniexp_t cur = miniexp_car(exp);
        if (!miniexp_consp(cur) || !miniexp_symbolp(miniexp_car(cur))) {
            exp = miniexp_cdr(exp);
            continue;
        }

        const QString id = QString::fromUtf8(miniexp_to_name(miniexp_car(cur)));
        if (id == QLatin1String(which)) {
            return miniexp_cadr(cur);
        }
        exp = miniexp_cdr(exp);
    }
    return miniexp_nil;
}

static bool find_replace_or_add_second_in_pair(miniexp_t theexp, const char *which, miniexp_t replacement)
{
    miniexp_t exp = miniexp_cdddr(theexp);
    while (exp) {
        miniexp_t cur = miniexp_car(exp);
        if (!miniexp_consp(cur) || !miniexp_symbolp(miniexp_car(cur))) {
            exp = miniexp_cdr(exp);
            continue;
        }

        const QString id = QString::fromUtf8(miniexp_to_name(miniexp_car(cur)));
        if (id == QLatin1String(which)) {
            miniexp_t reversed = miniexp_reverse(cur);
            miniexp_rplaca(reversed, replacement);
            cur = miniexp_reverse(reversed);
            return true;
        }
        exp = miniexp_cdr(exp);
    }
    // TODO add the new replacement ad the end of the list
    return false;
}

// ImageCacheItem

class ImageCacheItem
{
public:
    ImageCacheItem(int p, int w, int h, const QImage &i)
        : page(p)
        , width(w)
        , height(h)
        , img(i)
    {
    }

    int page;
    int width;
    int height;
    QImage img;
};

// KdjVu::Page

KDjVu::Page::Page()
{
}

KDjVu::Page::~Page()
{
}

int KDjVu::Page::width() const
{
    return m_width;
}

int KDjVu::Page::height() const
{
    return m_height;
}

int KDjVu::Page::dpi() const
{
    return m_dpi;
}

int KDjVu::Page::orientation() const
{
    return m_orientation;
}

// KDjVu::Link

KDjVu::Link::~Link()
{
}

KDjVu::Link::LinkArea KDjVu::Link::areaType() const
{
    return m_area;
}

QPoint KDjVu::Link::point() const
{
    return m_point;
}

QSize KDjVu::Link::size() const
{
    return m_size;
}

QPolygon KDjVu::Link::polygon() const
{
    return m_poly;
}

// KDjVu::PageLink

KDjVu::PageLink::PageLink()
{
}

int KDjVu::PageLink::type() const
{
    return KDjVu::Link::PageLink;
}

QString KDjVu::PageLink::page() const
{
    return m_page;
}

// KDjVu::UrlLink

KDjVu::UrlLink::UrlLink()
{
}

int KDjVu::UrlLink::type() const
{
    return KDjVu::Link::UrlLink;
}

QString KDjVu::UrlLink::url() const
{
    return m_url;
}

// KDjVu::Annotation

KDjVu::Annotation::Annotation(miniexp_t anno)
    : m_anno(anno)
{
}

KDjVu::Annotation::~Annotation()
{
}

QPoint KDjVu::Annotation::point() const
{
    miniexp_t area = miniexp_nth(3, m_anno);
    int a = miniexp_to_int(miniexp_nth(1, area));
    int b = miniexp_to_int(miniexp_nth(2, area));
    return QPoint(a, b);
}

QString KDjVu::Annotation::comment() const
{
    return QString::fromUtf8(miniexp_to_str(miniexp_nth(2, m_anno)));
}

void KDjVu::Annotation::setComment(const QString &comment)
{
    miniexp_t exp = m_anno;
    exp = miniexp_cdr(exp);
    exp = miniexp_cdr(exp);
    miniexp_rplaca(exp, miniexp_string(comment.toUtf8().constData()));
}

QColor KDjVu::Annotation::color() const
{
    return QColor();
}

void KDjVu::Annotation::setColor(const QColor &)
{
}

// KDjVu::TextAnnotation

KDjVu::TextAnnotation::TextAnnotation(miniexp_t anno)
    : Annotation(anno)
    , m_inlineText(true)
{
    const int num = miniexp_length(m_anno);
    for (int j = 4; j < num; ++j) {
        miniexp_t curelem = miniexp_nth(j, m_anno);
        if (!miniexp_listp(curelem)) {
            continue;
        }

        QString id = QString::fromUtf8(miniexp_to_name(miniexp_nth(0, curelem)));
        if (id == QLatin1String("pushpin")) {
            m_inlineText = false;
        }
    }
}

QSize KDjVu::TextAnnotation::size() const
{
    miniexp_t area = miniexp_nth(3, m_anno);
    int c = miniexp_to_int(miniexp_nth(3, area));
    int d = miniexp_to_int(miniexp_nth(4, area));
    return QSize(c, d);
}

int KDjVu::TextAnnotation::type() const
{
    return KDjVu::Annotation::TextAnnotation;
}

QColor KDjVu::TextAnnotation::color() const
{
    miniexp_t col = find_second_in_pair(m_anno, "backclr");
    if (!miniexp_symbolp(col)) {
        return Qt::transparent;
    }

    return QColor(QString::fromUtf8(miniexp_to_name(col)));
}

void KDjVu::TextAnnotation::setColor(const QColor &color)
{
    const QByteArray col = color.name().toLatin1();
    find_replace_or_add_second_in_pair(m_anno, "backclr", miniexp_symbol(col.constData()));
}

bool KDjVu::TextAnnotation::inlineText() const
{
    return m_inlineText;
}

// KDjVu::LineAnnotation

KDjVu::LineAnnotation::LineAnnotation(miniexp_t anno)
    : Annotation(anno)
    , m_isArrow(false)
    , m_width(miniexp_nil)
{
    const int num = miniexp_length(m_anno);
    for (int j = 4; j < num; ++j) {
        miniexp_t curelem = miniexp_nth(j, m_anno);
        if (!miniexp_listp(curelem)) {
            continue;
        }

        QString id = QString::fromUtf8(miniexp_to_name(miniexp_nth(0, curelem)));
        if (id == QLatin1String("arrow")) {
            m_isArrow = true;
        } else if (id == QLatin1String("width")) {
            m_width = curelem;
        }
    }
}

int KDjVu::LineAnnotation::type() const
{
    return KDjVu::Annotation::LineAnnotation;
}

QColor KDjVu::LineAnnotation::color() const
{
    miniexp_t col = find_second_in_pair(m_anno, "lineclr");
    if (!miniexp_symbolp(col)) {
        return Qt::black;
    }

    return QColor(QString::fromUtf8(miniexp_to_name(col)));
}

void KDjVu::LineAnnotation::setColor(const QColor &color)
{
    const QByteArray col = color.name().toLatin1();
    find_replace_or_add_second_in_pair(m_anno, "lineclr", miniexp_symbol(col.constData()));
}

QPoint KDjVu::LineAnnotation::point2() const
{
    miniexp_t area = miniexp_nth(3, m_anno);
    int c = miniexp_to_int(miniexp_nth(3, area));
    int d = miniexp_to_int(miniexp_nth(4, area));
    return QPoint(c, d);
}

bool KDjVu::LineAnnotation::isArrow() const
{
    return m_isArrow;
}

int KDjVu::LineAnnotation::width() const
{
    if (m_width == miniexp_nil) {
        return 1;
    }

    return miniexp_to_int(miniexp_cadr(m_width));
}

void KDjVu::LineAnnotation::setWidth(int width)
{
    find_replace_or_add_second_in_pair(m_anno, "width", miniexp_number(width));
}

// KDjVu::TextEntity

KDjVu::TextEntity::TextEntity()
{
}

KDjVu::TextEntity::~TextEntity()
{
}

QString KDjVu::TextEntity::text() const
{
    return m_text;
}

QRect KDjVu::TextEntity::rect() const
{
    return m_rect;
}

class KDjVu::Private
{
public:
    Private()
        : m_djvu_cxt(nullptr)
        , m_djvu_document(nullptr)
        , m_format(nullptr)
        , m_docBookmarks(nullptr)
        , m_cacheEnabled(true)
    {
    }

    QImage generateImageTile(ddjvu_page_t *djvupage, int &res, int width, int row, int xdelta, int height, int col, int ydelta);

    void readBookmarks();
    void fillBookmarksRecurse(QDomDocument &maindoc, QDomNode &curnode, miniexp_t exp, int offset = -1);

    void readMetaData(int page);

    int pageWithName(const QString &name);

    ddjvu_context_t *m_djvu_cxt;
    ddjvu_document_t *m_djvu_document;
    ddjvu_format_t *m_format;

    QVector<KDjVu::Page *> m_pages;
    QVector<ddjvu_page_t *> m_pages_cache;

    QList<ImageCacheItem *> mImgCache;

    QHash<QString, QVariant> m_metaData;
    QDomDocument *m_docBookmarks;

    QHash<QString, int> m_pageNamesCache;

    bool m_cacheEnabled;

    static unsigned int s_formatmask[4];
};

unsigned int KDjVu::Private::s_formatmask[4] = {0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000};

QImage KDjVu::Private::generateImageTile(ddjvu_page_t *djvupage, int &res, int width, int row, int xdelta, int height, int col, int ydelta)
{
    ddjvu_rect_t renderrect;
    renderrect.x = row * xdelta;
    renderrect.y = col * ydelta;
    int realwidth = qMin(width - renderrect.x, xdelta);
    int realheight = qMin(height - renderrect.y, ydelta);
    renderrect.w = realwidth;
    renderrect.h = realheight;
#ifdef KDJVU_DEBUG
    qDebug() << "renderrect:" << renderrect;
#endif
    ddjvu_rect_t pagerect;
    pagerect.x = 0;
    pagerect.y = 0;
    pagerect.w = width;
    pagerect.h = height;
#ifdef KDJVU_DEBUG
    qDebug() << "pagerect:" << pagerect;
#endif
    handle_ddjvu_messages(m_djvu_cxt, false);
    QImage res_img(realwidth, realheight, QImage::Format_RGB32);
    // the following line workarounds a rare crash in djvulibre;
    // it should be fixed with >= 3.5.21
    ddjvu_page_get_width(djvupage);
    res = ddjvu_page_render(djvupage, DDJVU_RENDER_COLOR, &pagerect, &renderrect, m_format, res_img.bytesPerLine(), (char *)res_img.bits());
    if (!res) {
        res_img.fill(Qt::white);
    }
#ifdef KDJVU_DEBUG
    qDebug() << "rendering result:" << res;
#endif
    handle_ddjvu_messages(m_djvu_cxt, false);

    return res_img;
}

void KDjVu::Private::readBookmarks()
{
    if (!m_djvu_document) {
        return;
    }

    miniexp_t outline;
    while ((outline = ddjvu_document_get_outline(m_djvu_document)) == miniexp_dummy) {
        handle_ddjvu_messages(m_djvu_cxt, true);
    }

    if (miniexp_listp(outline) && (miniexp_length(outline) > 0) && miniexp_symbolp(miniexp_nth(0, outline)) && (QString::fromUtf8(miniexp_to_name(miniexp_nth(0, outline))) == QLatin1String("bookmarks"))) {
        m_docBookmarks = new QDomDocument(QStringLiteral("KDjVuBookmarks"));
        fillBookmarksRecurse(*m_docBookmarks, *m_docBookmarks, outline, 1);
        ddjvu_miniexp_release(m_djvu_document, outline);
    }
}

void KDjVu::Private::fillBookmarksRecurse(QDomDocument &maindoc, QDomNode &curnode, miniexp_t exp, int offset)
{
    if (!miniexp_listp(exp)) {
        return;
    }

    int l = miniexp_length(exp);
    for (int i = qMax(offset, 0); i < l; ++i) {
        miniexp_t cur = miniexp_nth(i, exp);

        if (miniexp_consp(cur) && (miniexp_length(cur) > 0) && miniexp_stringp(miniexp_nth(0, cur)) && miniexp_stringp(miniexp_nth(1, cur))) {
            QString title = QString::fromUtf8(miniexp_to_str(miniexp_nth(0, cur)));
            QString dest = QString::fromUtf8(miniexp_to_str(miniexp_nth(1, cur)));
            QDomElement el = maindoc.createElement(QStringLiteral("item"));
            el.setAttribute(QStringLiteral("title"), title);
            if (!dest.isEmpty()) {
                if (dest.at(0) == QLatin1Char('#')) {
                    dest.remove(0, 1);
                    bool isNumber = false;
                    dest.toInt(&isNumber);
                    if (isNumber) {
                        // it might be an actual page number, but could also be a page label
                        // so resolve the number, and get the real page number
                        int pageNo = pageWithName(dest);
                        if (pageNo != -1) {
                            el.setAttribute(QStringLiteral("PageNumber"), QString::number(pageNo + 1));
                        } else {
                            el.setAttribute(QStringLiteral("PageNumber"), dest);
                        }
                    } else {
                        el.setAttribute(QStringLiteral("PageName"), dest);
                    }
                } else {
                    el.setAttribute(QStringLiteral("URL"), dest);
                }
            }
            curnode.appendChild(el);
            if (!el.isNull() && (miniexp_length(cur) > 2)) {
                fillBookmarksRecurse(maindoc, el, cur, 2);
            }
        }
    }
}

void KDjVu::Private::readMetaData(int page)
{
    if (!m_djvu_document) {
        return;
    }

    miniexp_t annots;
    while ((annots = ddjvu_document_get_pageanno(m_djvu_document, page)) == miniexp_dummy) {
        handle_ddjvu_messages(m_djvu_cxt, true);
    }

    if (!miniexp_listp(annots) || miniexp_length(annots) == 0) {
        return;
    }

    miniexp_t exp = miniexp_nth(0, annots);
    int size = miniexp_length(exp);
    if (size <= 1 || qstrncmp(miniexp_to_name(miniexp_nth(0, exp)), "metadata", 8)) {
        return;
    }

    for (int i = 1; i < size; ++i) {
        miniexp_t cur = miniexp_nth(i, exp);
        if (miniexp_length(cur) != 2) {
            continue;
        }

        QString id = QString::fromUtf8(miniexp_to_name(miniexp_nth(0, cur)));
        QString value = QString::fromUtf8(miniexp_to_str(miniexp_nth(1, cur)));
        m_metaData[id.toLower()] = value;
    }
}

int KDjVu::Private::pageWithName(const QString &name)
{
    const int pageNo = m_pageNamesCache.value(name, -1);
    if (pageNo != -1) {
        return pageNo;
    }

    const QByteArray utfName = name.toUtf8();
    const int fileNum = ddjvu_document_get_filenum(m_djvu_document);
    ddjvu_fileinfo_t info;
    for (int i = 0; i < fileNum; ++i) {
        if (DDJVU_JOB_OK != ddjvu_document_get_fileinfo(m_djvu_document, i, &info)) {
            continue;
        }
        if (info.type != 'P') {
            continue;
        }
        if ((utfName == info.id) || (utfName == info.name) || (utfName == info.title)) {
            m_pageNamesCache.insert(name, info.pageno);
            return info.pageno;
        }
    }
    return -1;
}

KDjVu::KDjVu()
    : d(new Private)
{
    // creating the djvu context
    d->m_djvu_cxt = ddjvu_context_create("KDjVu");
    // creating the rendering format
#if DDJVUAPI_VERSION >= 18
    d->m_format = ddjvu_format_create(DDJVU_FORMAT_RGBMASK32, 4, Private::s_formatmask);
#else
    d->m_format = ddjvu_format_create(DDJVU_FORMAT_RGBMASK32, 3, Private::s_formatmask);
#endif
    ddjvu_format_set_row_order(d->m_format, 1);
    ddjvu_format_set_y_direction(d->m_format, 1);
}

KDjVu::~KDjVu()
{
    closeFile();

    ddjvu_format_release(d->m_format);
    ddjvu_context_release(d->m_djvu_cxt);

    delete d;
}

bool KDjVu::openFile(const QString &fileName)
{
    // first, close the old file
    if (d->m_djvu_document) {
        closeFile();
    }

    // load the document..., use UTF-8 variant to work on Windows, too, see bug 422500
    d->m_djvu_document = ddjvu_document_create_by_filename_utf8(d->m_djvu_cxt, fileName.toUtf8().constData(), true);
    if (!d->m_djvu_document) {
        return false;
    }
    // ...and wait for its loading
    wait_for_ddjvu_message(d->m_djvu_cxt, DDJVU_DOCINFO);
    if (ddjvu_document_decoding_error(d->m_djvu_document)) {
        ddjvu_document_release(d->m_djvu_document);
        d->m_djvu_document = nullptr;
        return false;
    }

    qDebug() << "# of pages:" << ddjvu_document_get_pagenum(d->m_djvu_document);
    int numofpages = ddjvu_document_get_pagenum(d->m_djvu_document);
    d->m_pages.clear();
    d->m_pages.resize(numofpages);
    d->m_pages_cache.clear();
    d->m_pages_cache.resize(numofpages);

    // get the document type
    QString doctype;
    switch (ddjvu_document_get_type(d->m_djvu_document)) {
    case DDJVU_DOCTYPE_UNKNOWN:
        doctype = i18nc("Type of DjVu document", "Unknown");
        break;
    case DDJVU_DOCTYPE_SINGLEPAGE:
        doctype = i18nc("Type of DjVu document", "Single Page");
        break;
    case DDJVU_DOCTYPE_BUNDLED:
        doctype = i18nc("Type of DjVu document", "Bundled");
        break;
    case DDJVU_DOCTYPE_INDIRECT:
        doctype = i18nc("Type of DjVu document", "Indirect");
        break;
    case DDJVU_DOCTYPE_OLD_BUNDLED:
        doctype = i18nc("Type of DjVu document", "Bundled (old)");
        break;
    case DDJVU_DOCTYPE_OLD_INDEXED:
        doctype = i18nc("Type of DjVu document", "Indexed (old)");
        break;
    }
    if (!doctype.isEmpty()) {
        d->m_metaData[QStringLiteral("documentType")] = doctype;
    }
    // get the number of components
    d->m_metaData[QStringLiteral("componentFile")] = ddjvu_document_get_filenum(d->m_djvu_document);

    // read the pages
    for (int i = 0; i < numofpages; ++i) {
        ddjvu_status_t sts;
        ddjvu_pageinfo_t info;
        while ((sts = ddjvu_document_get_pageinfo(d->m_djvu_document, i, &info)) < DDJVU_JOB_OK) {
            handle_ddjvu_messages(d->m_djvu_cxt, true);
        }
        if (sts >= DDJVU_JOB_FAILED) {
            qDebug().nospace() << "\t>>> page " << i << " failed: " << sts;
            return false;
        }

        KDjVu::Page *p = new KDjVu::Page();
        p->m_width = info.width;
        p->m_height = info.height;
        p->m_dpi = info.dpi;
#if DDJVUAPI_VERSION >= 18
        p->m_orientation = flipRotation(info.rotation);
#else
        p->m_orientation = 0;
#endif
        d->m_pages[i] = p;
    }

    // reading the metadata from the first page only should be enough
    if (numofpages > 0) {
        d->readMetaData(0);
    }

    return true;
}

void KDjVu::closeFile()
{
    // deleting the old TOC
    delete d->m_docBookmarks;
    d->m_docBookmarks = nullptr;
    // deleting the pages
    qDeleteAll(d->m_pages);
    d->m_pages.clear();
    // releasing the djvu pages
    QVector<ddjvu_page_t *>::Iterator it = d->m_pages_cache.begin(), itEnd = d->m_pages_cache.end();
    for (; it != itEnd; ++it) {
        ddjvu_page_release(*it);
    }
    d->m_pages_cache.clear();
    // clearing the image cache
    qDeleteAll(d->mImgCache);
    d->mImgCache.clear();
    // clearing the old metadata
    d->m_metaData.clear();
    // cleaning the page names mapping
    d->m_pageNamesCache.clear();
    // releasing the old document
    if (d->m_djvu_document) {
        ddjvu_document_release(d->m_djvu_document);
    }
    d->m_djvu_document = nullptr;
}

QVariant KDjVu::metaData(const QString &key) const
{
    QHash<QString, QVariant>::ConstIterator it = d->m_metaData.constFind(key);
    return it != d->m_metaData.constEnd() ? it.value() : QVariant();
}

const QDomDocument *KDjVu::documentBookmarks() const
{
    if (!d->m_docBookmarks) {
        d->readBookmarks();
    }
    return d->m_docBookmarks;
}

void KDjVu::linksAndAnnotationsForPage(int pageNum, QList<KDjVu::Link *> *links, QList<KDjVu::Annotation *> *annotations) const
{
    if ((pageNum < 0) || (pageNum >= d->m_pages.count()) || (!links && !annotations)) {
        return;
    }

    miniexp_t annots;
    while ((annots = ddjvu_document_get_pageanno(d->m_djvu_document, pageNum)) == miniexp_dummy) {
        handle_ddjvu_messages(d->m_djvu_cxt, true);
    }

    if (!miniexp_listp(annots)) {
        return;
    }

    if (links) {
        links->clear();
    }
    if (annotations) {
        annotations->clear();
    }

    int l = miniexp_length(annots);
    for (int i = 0; i < l; ++i) {
        miniexp_t cur = miniexp_nth(i, annots);
        int num = miniexp_length(cur);
        if ((num < 4) || !miniexp_symbolp(miniexp_nth(0, cur)) || (qstrncmp(miniexp_to_name(miniexp_nth(0, cur)), "maparea", 7) != 0)) {
            continue;
        }

        QString target;
        QString type;
        if (miniexp_symbolp(miniexp_nth(0, miniexp_nth(3, cur)))) {
            type = QString::fromUtf8(miniexp_to_name(miniexp_nth(0, miniexp_nth(3, cur))));
        }
        KDjVu::Link *link = nullptr;
        KDjVu::Annotation *ann = nullptr;
        miniexp_t urlexp = miniexp_nth(1, cur);
        if (links && (type == QLatin1String("rect") || type == QLatin1String("oval") || type == QLatin1String("poly"))) {
            if (miniexp_stringp(urlexp)) {
                target = QString::fromUtf8(miniexp_to_str(miniexp_nth(1, cur)));
            } else if (miniexp_listp(urlexp) && (miniexp_length(urlexp) == 3) && miniexp_symbolp(miniexp_nth(0, urlexp)) && (qstrncmp(miniexp_to_name(miniexp_nth(0, urlexp)), "url", 3) == 0)) {
                target = QString::fromUtf8(miniexp_to_str(miniexp_nth(1, urlexp)));
            }
            if (target.isEmpty() || ((target.length() > 0) && target.at(0) == QLatin1Char('#'))) {
                KDjVu::PageLink *plink = new KDjVu::PageLink();
                plink->m_page = target;
                link = plink;
            } else {
                KDjVu::UrlLink *ulink = new KDjVu::UrlLink();
                ulink->m_url = target;
                link = ulink;
            }
        } else if (annotations && (type == QLatin1String("text") || type == QLatin1String("line"))) {
            if (type == QLatin1String("text")) {
                KDjVu::TextAnnotation *textann = new KDjVu::TextAnnotation(cur);
                ann = textann;
            } else if (type == QLatin1String("line")) {
                KDjVu::LineAnnotation *lineann = new KDjVu::LineAnnotation(cur);
                ann = lineann;
            }
        }
        if (link /* safety check */ && links) {
            link->m_area = KDjVu::Link::UnknownArea;
            miniexp_t area = miniexp_nth(3, cur);
            int arealength = miniexp_length(area);
            if ((arealength == 5) && (type == QLatin1String("rect") || type == QLatin1String("oval"))) {
                link->m_point = QPoint(miniexp_to_int(miniexp_nth(1, area)), miniexp_to_int(miniexp_nth(2, area)));
                link->m_size = QSize(miniexp_to_int(miniexp_nth(3, area)), miniexp_to_int(miniexp_nth(4, area)));
                if (type == QLatin1String("rect")) {
                    link->m_area = KDjVu::Link::RectArea;
                } else {
                    link->m_area = KDjVu::Link::EllipseArea;
                }
            } else if ((arealength > 0) && (arealength % 2 == 1) && type == QLatin1String("poly")) {
                link->m_area = KDjVu::Link::PolygonArea;
                QPolygon poly;
                for (int j = 1; j < arealength; j += 2) {
                    poly << QPoint(miniexp_to_int(miniexp_nth(j, area)), miniexp_to_int(miniexp_nth(j + 1, area)));
                }
                link->m_poly = poly;
            }

            if (link->m_area != KDjVu::Link::UnknownArea) {
                links->append(link);
            }
        } else if (ann /* safety check */ && annotations) {
            annotations->append(ann);
        }
    }
}

const QVector<KDjVu::Page *> &KDjVu::pages() const
{
    return d->m_pages;
}

QImage KDjVu::image(int page, int width, int height, int rotation)
{
    if (d->m_cacheEnabled) {
        bool found = false;
        QList<ImageCacheItem *>::Iterator it = d->mImgCache.begin(), itEnd = d->mImgCache.end();
        for (; (it != itEnd) && !found; ++it) {
            ImageCacheItem *cur = *it;
            if ((cur->page == page) && (rotation % 2 == 0 ? cur->width == width && cur->height == height : cur->width == height && cur->height == width)) {
                found = true;
            }
        }
        if (found) {
            // taking the element and pushing to the top of the list
            --it;
            ImageCacheItem *cur2 = *it;
            d->mImgCache.erase(it);
            d->mImgCache.push_front(cur2);

            return cur2->img;
        }
    }

    if (!d->m_pages_cache.at(page)) {
        ddjvu_page_t *newpage = ddjvu_page_create_by_pageno(d->m_djvu_document, page);
        // wait for the new page to be loaded
        ddjvu_status_t sts;
        while ((sts = ddjvu_page_decoding_status(newpage)) < DDJVU_JOB_OK) {
            handle_ddjvu_messages(d->m_djvu_cxt, true);
        }
        d->m_pages_cache[page] = newpage;
    }
    ddjvu_page_t *djvupage = d->m_pages_cache[page];

    /*
        if ( ddjvu_page_get_rotation( djvupage ) != flipRotation( rotation ) )
        {
    // TODO: test documents with initial rotation != 0
    //        ddjvu_page_set_rotation( djvupage, m_pages.at( page )->orientation() );
            ddjvu_page_set_rotation( djvupage, (ddjvu_page_rotation_t)flipRotation( rotation ) );
        }
    */

    static const int xdelta = 1500;
    static const int ydelta = 1500;

    int xparts = width / xdelta + 1;
    int yparts = height / ydelta + 1;

    QImage newimg;

    int res = 10000;
    if ((xparts == 1) && (yparts == 1)) {
        // only one part -- render at once with no need to auxiliary image
        newimg = d->generateImageTile(djvupage, res, width, 0, xdelta, height, 0, ydelta);
    } else {
        // more than one part -- need to render piece-by-piece and to compose
        // the results
        newimg = QImage(width, height, QImage::Format_RGB32);
        QPainter p;
        p.begin(&newimg);
        int parts = xparts * yparts;
        for (int i = 0; i < parts; ++i) {
            const int row = i % xparts;
            const int col = i / xparts;
            int tmpres = 0;
            const QImage tempp = d->generateImageTile(djvupage, tmpres, width, row, xdelta, height, col, ydelta);
            p.drawImage(row * xdelta, col * ydelta, tempp);
            res = qMin(tmpres, res);
        }
        p.end();
    }

    if (res && d->m_cacheEnabled) {
        // delete all the cached pixmaps for the current page with a size that
        // differs no more than 35% of the new pixmap size
        int imgsize = newimg.width() * newimg.height();
        if (imgsize > 0) {
            for (int i = 0; i < d->mImgCache.count();) {
                ImageCacheItem *cur = d->mImgCache.at(i);
                if ((cur->page == page) && (abs(cur->img.width() * cur->img.height() - imgsize) < imgsize * 0.35)) {
                    d->mImgCache.removeAt(i);
                    delete cur;
                } else {
                    ++i;
                }
            }
        }

        // the image cache has too many elements, remove the last
        if (d->mImgCache.size() >= 10) {
            delete d->mImgCache.last();
            d->mImgCache.removeLast();
        }
        ImageCacheItem *ich = new ImageCacheItem(page, width, height, newimg);
        d->mImgCache.push_front(ich);
    }

    return newimg;
}

bool KDjVu::exportAsPostScript(const QString &fileName, const QList<int> &pageList) const
{
    if (!d->m_djvu_document || fileName.trimmed().isEmpty() || pageList.isEmpty()) {
        return false;
    }

    QFile f(fileName);
    f.open(QIODevice::ReadWrite);
    bool ret = exportAsPostScript(&f, pageList);
    if (ret) {
        f.close();
    }
    return ret;
}

bool KDjVu::exportAsPostScript(QFile *file, const QList<int> &pageList) const
{
    if (!d->m_djvu_document || !file || pageList.isEmpty()) {
        return false;
    }

    FILE *f = fdopen(file->handle(), "w+");
    if (!f) {
        qDebug() << "error while getting the FILE*";
        return false;
    }

    QString pl;
    for (const int p : pageList) {
        if (!pl.isEmpty()) {
            pl += QLatin1String(",");
        }
        pl += QString::number(p);
    }
    pl.prepend(QStringLiteral("-page="));

    // setting the options
    static const int optc = 1;
    const char **optv = (const char **)malloc(1 * sizeof(char *));
    QByteArray plb = pl.toLatin1();
    optv[0] = plb.constData();

    ddjvu_job_t *printjob = ddjvu_document_print(d->m_djvu_document, f, optc, optv);
    while (!ddjvu_job_done(printjob)) {
        handle_ddjvu_messages(d->m_djvu_cxt, true);
    }

    free(optv);

    return fclose(f) == 0;
}

QList<KDjVu::TextEntity> KDjVu::textEntities(int page, const QString &granularity) const
{
    if ((page < 0) || (page >= d->m_pages.count())) {
        return QList<KDjVu::TextEntity>();
    }

    miniexp_t r;
    while ((r = ddjvu_document_get_pagetext(d->m_djvu_document, page, nullptr)) == miniexp_dummy) {
        handle_ddjvu_messages(d->m_djvu_cxt, true);
    }

    if (r == miniexp_nil) {
        return QList<KDjVu::TextEntity>();
    }

    QList<KDjVu::TextEntity> ret;

    int height = d->m_pages.at(page)->height();

    QQueue<miniexp_t> queue;
    queue.enqueue(r);

    while (!queue.isEmpty()) {
        miniexp_t cur = queue.dequeue();

        if (miniexp_listp(cur) && (miniexp_length(cur) > 0) && miniexp_symbolp(miniexp_nth(0, cur))) {
            int size = miniexp_length(cur);
            QString sym = QString::fromUtf8(miniexp_to_name(miniexp_nth(0, cur)));
            if (sym == granularity) {
                if (size >= 6) {
                    int xmin = miniexp_to_int(miniexp_nth(1, cur));
                    int ymin = miniexp_to_int(miniexp_nth(2, cur));
                    int xmax = miniexp_to_int(miniexp_nth(3, cur));
                    int ymax = miniexp_to_int(miniexp_nth(4, cur));
                    QRect rect(xmin, height - ymax, xmax - xmin, ymax - ymin);
                    KDjVu::TextEntity entity;
                    entity.m_rect = rect;
                    entity.m_text = QString::fromUtf8(miniexp_to_str(miniexp_nth(5, cur)));
                    ret.append(entity);
                }
            } else {
                for (int i = 5; i < size; ++i) {
                    queue.enqueue(miniexp_nth(i, cur));
                }
            }
        }
    }

    return ret;
}

void KDjVu::setCacheEnabled(bool enable)
{
    if (enable == d->m_cacheEnabled) {
        return;
    }

    d->m_cacheEnabled = enable;
    if (!d->m_cacheEnabled) {
        qDeleteAll(d->mImgCache);
        d->mImgCache.clear();
    }
}

bool KDjVu::isCacheEnabled() const
{
    return d->m_cacheEnabled;
}

int KDjVu::pageNumber(const QString &name) const
{
    if (!d->m_djvu_document) {
        return -1;
    }

    return d->pageWithName(name);
}

/*
    SPDX-FileCopyrightText: 2006 Pino Toscano <toscano.pino@tiscali.it>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _KDJVU_
#define _KDJVU_

#include <QColor>
#include <QImage>
#include <QList>
#include <QPolygon>
#include <QRect>
#include <QVariant>
#include <QVector>

class QDomDocument;
class QFile;

#ifndef MINIEXP_H
typedef struct miniexp_s *miniexp_t;
#endif

/**
 * @brief Qt (KDE) encapsulation of the DjVuLibre
 */
class KDjVu
{
public:
    KDjVu();
    ~KDjVu();

    KDjVu(const KDjVu &) = delete;
    KDjVu &operator=(const KDjVu &) = delete;

    /**
     * A DjVu page.
     */
    class Page
    {
        friend class KDjVu;

    public:
        ~Page();

        int width() const;
        int height() const;
        int dpi() const;
        int orientation() const;

    private:
        Page();

        int m_width;
        int m_height;
        int m_dpi;
        int m_orientation;
    };

    /**
     * The base implementation for a DjVu link.
     */
    class Link
    {
        friend class KDjVu;

    public:
        Link() = default;
        virtual ~Link();

        Link(const Link &) = delete;
        Link &operator=(const Link &) = delete;

        enum LinkType { PageLink, UrlLink };
        enum LinkArea { UnknownArea, RectArea, EllipseArea, PolygonArea };
        virtual int type() const = 0;
        LinkArea areaType() const;
        QPoint point() const;
        QSize size() const;
        QPolygon polygon() const;

    private:
        LinkArea m_area;
        QPoint m_point;
        QSize m_size;
        QPolygon m_poly;
    };

    /**
     * A link to reach a page of a DjVu document.
     */
    class PageLink : public Link
    {
        friend class KDjVu;

    public:
        int type() const override;
        QString page() const;

    private:
        PageLink();
        QString m_page;
    };

    /**
     * A DjVu link to open an external Url.
     */
    class UrlLink : public Link
    {
        friend class KDjVu;

    public:
        int type() const override;
        QString url() const;

    private:
        UrlLink();
        QString m_url;
    };

    /**
     * The base implementation for a DjVu annotation.
     */
    class Annotation
    {
        friend class KDjVu;

    public:
        virtual ~Annotation();

        Annotation(const Annotation &) = delete;
        Annotation &operator=(const Annotation &) = delete;

        enum AnnotationType { TextAnnotation, LineAnnotation };
        virtual int type() const = 0;
        QPoint point() const;
        QString comment() const;
        void setComment(const QString &comment);
        virtual QColor color() const;
        virtual void setColor(const QColor &color);

    protected:
        explicit Annotation(miniexp_t anno);

        miniexp_t m_anno;
        QPoint m_point;
    };

    /**
     * A DjVu text annotation.
     */
    class TextAnnotation : public Annotation
    {
        friend class KDjVu;

    public:
        int type() const override;
        QColor color() const override;
        void setColor(const QColor &color) override;
        QSize size() const;
        bool inlineText() const;

    private:
        explicit TextAnnotation(miniexp_t anno);
        QSize m_size;
        bool m_inlineText;
    };

    /**
     * A DjVu line annotation.
     */
    class LineAnnotation : public Annotation
    {
        friend class KDjVu;

    public:
        int type() const override;
        QColor color() const override;
        void setColor(const QColor &color) override;
        QPoint point2() const;
        bool isArrow() const;
        int width() const;
        void setWidth(int width);

    private:
        explicit LineAnnotation(miniexp_t anno);
        QPoint m_point2;
        bool m_isArrow;
        miniexp_t m_width;
    };

    /**
     * A DjVu text entity.
     */
    class TextEntity
    {
        friend class KDjVu;

    public:
        ~TextEntity();

        QString text() const;
        QRect rect() const;

    private:
        TextEntity();

        QString m_text;
        QRect m_rect;
    };

    /**
     * Opens the file \p fileName, closing the old one if necessary.
     */
    bool openFile(const QString &fileName);
    /**
     * Close the file currently opened, if any.
     */
    void closeFile();

    /**
     * The pages of the current document, or an empty vector otherwise.
     * \note KDjVu handles the pages, so you don't need to delete them manually
     * \return a vector with the pages of the current document
     */
    const QVector<KDjVu::Page *> &pages() const;

    /**
     * Get the metadata for the specified \p key, or a null variant otherwise.
     */
    QVariant metaData(const QString &key) const;

    /**
     * Get an XML document with the bookmarks of the current document (if any).
     * The XML will look like this:
     * \verbatim
     * <!DOCTYPE KDjVuBookmarks>
     * <item title="Title 1" destination="dest1">
     *   <item title="Title 1.1" destination="dest1.1" />
     *   ...
     * </item>
     * <item title="Title 2" destination="dest2">
     * \endverbatim
     */
    const QDomDocument *documentBookmarks() const;

    /**
     * Reads the links and the annotations for the page \p pageNum
     *
     * For both \p links and \p annotations , you can pass either a valid pointer
     * (in case you want to extract that kind of information), or a null pointer
     * (if you don't want that information).
     */
    void linksAndAnnotationsForPage(int pageNum, QList<KDjVu::Link *> *links, QList<KDjVu::Annotation *> *annotations) const;

    /**
     * Check if the image for the specified \p page with the specified
     * \p width, \p height and \p rotation is already in cache, and returns
     * it. If not, a null image is returned.
     */
    QImage image(int page, int width, int height, int rotation);

    /**
     * Export the currently open document as PostScript file \p fileName.
     * \returns whether the exporting was successful
     */
    bool exportAsPostScript(const QString &fileName, const QList<int> &pageList) const;

    /**
     * Export the currently open document as PostScript file.
     * \returns whether the exporting was successful
     */
    bool exportAsPostScript(QFile *file, const QList<int> &pageList) const;

    /**
     * Return the list of the text entities for the specified \p page, that matches the
     * specified \p granularity.
     */
    QList<KDjVu::TextEntity> textEntities(int page, const QString &granularity) const;

    /**
     * Enable or disable the internal rendered pages cache.
     */
    void setCacheEnabled(bool enable);
    /**
     * \returns whether the internal rendered pages cache is enabled
     */
    bool isCacheEnabled() const;

    /**
     * Return the page number of the page whose title is \p name.
     */
    int pageNumber(const QString &name) const;

private:
    class Private;
    Private *const d;
};

#endif

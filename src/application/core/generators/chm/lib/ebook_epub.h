/*
    Kchmviewer - a CHM and EPUB file viewer with broad language support
    SPDX-FileCopyrightText: 2004-2014 George Yunaev gyunaev@ulduzsoft.com

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef EBOOK_EPUB_H
#define EBOOK_EPUB_H

#include <QFile>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QUrl>
#include "ebook.h"
#include "zip.h"

class QXmlDefaultHandler;

class EBook_EPUB : public EBook
{
public:
    EBook_EPUB();
    ~EBook_EPUB() override;

    /*!
     * \brief Attempts to load epub file.
     * \param archiveName filename.
     * \return EBook object on success, NULL on failure.
     *
     * Loads a epub file.
     * \ingroup init
     */
    bool load(const QString& archiveName) override;

    /*!
     * \brief Closes all the files, and frees the appropriate data.
     * \ingroup init
     */
    void close() override;

    /*!
     * \brief Gets the title name of the opened ebook.
     * \return The name of the opened document, or an empty string if no ebook
     * has been loaded. \ingroup information
     */
    QString title() const override;

    /*!
     * \brief Gets the default URL of the e-book which should be opened when the
     * book it first open
     *
     * \return The home page name, with a '/' added in front and relative to
     *         the root of the archive filesystem. If no book has been opened,
     * returns "/". \ingroup information
     */
    QUrl homeUrl() const override;

    /*!
     * \brief Checks whether the specific feature is present in this file.
     * \return true if it is available; false otherwise.
     * \ingroup information
     */
    bool hasFeature(Feature code) const override;

    /*!
     * \brief Parses and fills up the Table of Contents (TOC)
     * \param topics A pointer to the container which will store the parsed
     * results. Will be cleaned before parsing. \return true if the tree is
     * present and parsed successfully, false otherwise. The parser is built to
     * be error-prone, however it still can abort with qFatal() by really buggy
     * files; please report a bug if the file is opened ok under Windows.
     * \ingroup fileparsing
     */
    bool getTableOfContents(QList<EBookTocEntry>& toc) const override;

    /*!
     * \brief Parses the index table
     * \param indexes A pointer to the container which will store the parsed
     * results. Will be cleaned before parsing. \return true if the tree is
     * present and parsed successfully, false otherwise. The parser is built to
     * be error-prone, however it still can abort with qFatal() by really buggy
     * chm file; so far it never happened on indexes. \ingroup fileparsing
     */
    bool getIndex(QList<EBookIndexEntry>& index) const override;

    /*!
     * \brief Retrieves the content associated with the url from the current
     * ebook as QString. \param str A string where the retreived content should
     * be stored. \param url An URL in chm file to retreive content from. Must
     * be absolute. \return true if the content is successfully received; false
     * otherwise. Note content may be an empty string.
     *
     * This function retreives the file content (mostly for HTML pages) from the
     * ebook. Because the content in chm file might not be stored in Unicode, it
     * will be recoded according to current encoding. Do not use for binary
     * data.
     *
     * \sa setCurrentEncoding() currentEncoding() getFileContentAsBinary()
     * \ingroup dataretrieve
     */
    bool getFileContentAsString(QString& str, const QUrl& url) const override;

    /*!
     * \brief Retrieves the content from url in current chm file to QByteArray.
     * \param data A data array where the retreived content should be stored.
     * \param url An URL in chm file to retreive content from. Must be absolute.
     * \return true if the content is successfully received; false otherwise.
     *
     * This function retreives the file content from the chm archive opened by
     * load() function. The content is not encoded.
     *
     * \sa getFileContentAsString()
     * \ingroup dataretrieve
     */
    bool getFileContentAsBinary(QByteArray& data,
                                const QUrl& url) const override;

    /*!
     * \brief Obtains the list of all the files (URLs) in current ebook archive.
     * This is used in search and to dump the e-book content. \param files An
     * array to store list of URLs (file names) present in chm archive. \return
     * true if the enumeration succeed; false otherwise (I could hardly imagine
     * a reason).
     *
     * \ingroup dataretrieve
     */
    bool enumerateFiles(QList<QUrl>& files) override;

    /*!
     * \brief Gets the Title of the page referenced by url.
     * \param url An URL in ebook file to get title from. Must be absolute.
     * \return The title, or QString() if the URL cannot be found or not a HTML
     * page.
     *
     * \ingroup dataretrieve
     */
    QString getTopicByUrl(const QUrl& url) override;

    /*!
     * \brief Gets the current ebook encoding (set or autodetected) as qtcodec
     * \return The current encoding.
     *
     * \ingroup encoding
     */
    QString currentEncoding() const override;

    /*!
     * \brief Sets the ebook encoding to use for TOC and content
     * \param encoding An encoding to use.
     *
     * \ingroup encoding
     */
    bool setCurrentEncoding(const char* encoding) override;

    /*!
     * \brief Checks if this kind of URL is supported by the ebook format (i.e.
     * could be passed to ebook functions) \param url The url to check
     */
    bool isSupportedUrl(const QUrl& url) override;

    // Converts the string to the ebook-specific URL format
    QUrl pathToUrl(const QString& link) const override;

    // Extracts the path component from the URL
    QString urlToPath(const QUrl& link) const override;

private:
    // Parses the XML file using a specified parser
    bool parseXML(const QString& uri, QXmlDefaultHandler* parser);

    // Parses the book description file. Fills up the ebook info
    bool parseBookinfo();

    // Get file content from path
    bool getFileAsString(QString& str, const QString& path) const;
    bool getFileAsBinary(QByteArray& data, const QString& path) const;

    // ZIP archive fd and structs
    QFile m_epubFile;
    struct zip* m_zipFile;

    // Ebook info
    QString m_title;
    QString m_documentRoot;

    // List of files in the ebook
    QList<QUrl> m_ebookManifest;

    // Table of contents
    QList<EBookTocEntry> m_tocEntries;

    // Map of URL-Title
    QMap<QUrl, QString> m_urlTitleMap;
};

#endif  // EBOOK_EPUB_H

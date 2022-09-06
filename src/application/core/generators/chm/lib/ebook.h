/*
    Kchmviewer - a CHM and EPUB file viewer with broad language support
    SPDX-FileCopyrightText: 2004-2014 George Yunaev gyunaev@ulduzsoft.com

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef INCLUDE_EBOOK_H
#define INCLUDE_EBOOK_H

#include <QList>
#include <QString>
#include <QUrl>

//! Stores a single table of content entry
class EBookTocEntry
{
public:
    //! Content TOC icon indexes for CHM books (epub books contain no icons)
    enum Icon {
        IMAGE_NONE = -1,
        IMAGE_AUTO = -2,

        MAX_BUILTIN_ICONS = 42
    };

    //! Entry name
    QString name;

    //! Entry URL.
    QUrl url;

    //! Associated image number. Used for TOC only; indexes does not have the image.
    //! If IMAGE_NONE, no icon is associated. Otherwise use getBookIconPixmap() to get associated pixmap icon.
    Icon iconid;

    //! Indentation level for this entry.
    int indent;
};

//! Stores a single index entry
class EBookIndexEntry
{
public:
    //! Entry name
    QString name;

    //! Entry URLs. The index entry could have several URLs
    QList<QUrl> urls;

    //! Whether this is a 'see also' index type, and its value
    QString seealso;

    //! Indentation level for this entry.
    int indent;
};

//! Universal ebook files processor supporting both CHM and EPUB. Abstract.
class EBook
{
public:
    enum Feature {
        FEATURE_TOC,     // has table of contents
        FEATURE_INDEX,   // has index
        FEATURE_ENCODING // Could be encoded with different encodings
    };

    //! Default constructor and destructor.
    EBook();
    virtual ~EBook();

    EBook(const EBook &) = delete;
    EBook &operator=(const EBook &) = delete;

    /*!
     * \brief Attempts to load chm or epub file.
     * \param archiveName filename.
     * \return EBook object on success, NULL on failure.
     *
     * Loads a CHM or epub file. For CHM files it could internally load more than one file,
     * if files linked to this one are present locally (like MSDN).
     * \ingroup init
     */
    static EBook *loadFile(const QString &archiveName);

    /*!
     * \brief Closes all the files, and frees the appropriate data.
     * \ingroup init
     */
    virtual void close() = 0;

    /*!
     * \brief Gets the title name of the opened ebook.
     * \return The name of the opened document, or an empty string if no ebook has been loaded.
     * \ingroup information
     */
    virtual QString title() const = 0;

    /*!
     * \brief Gets the default URL of the e-book which should be opened when the book it first open
     *
     * \return The home page name, with a '/' added in front and relative to
     *         the root of the archive filesystem. If no book has been opened, returns "/".
     * \ingroup information
     */
    virtual QUrl homeUrl() const = 0;

    /*!
     * \brief Checks whether the specific feature is present in this file.
     * \return true if it is available; false otherwise.
     * \ingroup information
     */
    virtual bool hasFeature(Feature code) const = 0;

    /*!
     * \brief Parses and fills up the Table of Contents (TOC)
     * \param topics A pointer to the container which will store the parsed results.
     *               Will be cleaned before parsing.
     * \return true if the tree is present and parsed successfully, false otherwise.
     *         The parser is built to be error-prone, however it still can abort with qFatal()
     *         by really buggy files; please report a bug if the file is opened ok under Windows.
     * \ingroup fileparsing
     */
    virtual bool getTableOfContents(QList<EBookTocEntry> &toc) const = 0;

    /*!
     * \brief Parses the index table
     * \param indexes A pointer to the container which will store the parsed results.
     *               Will be cleaned before parsing.
     * \return true if the tree is present and parsed successfully, false otherwise.
     *         The parser is built to be error-prone, however it still can abort with qFatal()
     *         by really buggy chm file; so far it never happened on indexes.
     * \ingroup fileparsing
     */
    virtual bool getIndex(QList<EBookIndexEntry> &index) const = 0;

    /*!
     * \brief Retrieves the content associated with the url from the current ebook as QString.
     * \param str A string where the retreived content should be stored.
     * \param url An URL in chm file to retreive content from. Must be absolute.
     * \return true if the content is successfully received; false otherwise. Note content may be an empty string.
     *
     * This function retreives the file content (mostly for HTML pages) from the ebook. Because the content
     * in chm file might not be stored in Unicode, it will be recoded according to current encoding.
     * Do not use for binary data.
     *
     * \sa setCurrentEncoding() currentEncoding() getFileContentAsBinary()
     * \ingroup dataretrieve
     */
    virtual bool getFileContentAsString(QString &str, const QUrl &url) const = 0;

    /*!
     * \brief Retrieves the content from url in current chm file to QByteArray.
     * \param data A data array where the retreived content should be stored.
     * \param url An URL in chm file to retreive content from. Must be absolute.
     * \return true if the content is successfully received; false otherwise.
     *
     * This function retreives the file content from the chm archive opened by load()
     * function. The content is not encoded.
     *
     * \sa getFileContentAsString()
     * \ingroup dataretrieve
     */
    virtual bool getFileContentAsBinary(QByteArray &data, const QUrl &url) const = 0;

    /*!
     * \brief Obtains the list of all the files (URLs) in current ebook archive. This is used in search
     * and to dump the e-book content.
     * \param files An array to store list of URLs present in chm archive.
     * \return true if the enumeration succeed; false otherwise (I could hardly imagine a reason).
     *
     * \ingroup dataretrieve
     */
    virtual bool enumerateFiles(QList<QUrl> &files) = 0;

    /*!
     * \brief Gets the Title of the page referenced by url.
     * \param url An URL in ebook file to get title from. Must be absolute.
     * \return The title, or QString() if the URL cannot be found or not a HTML page.
     *
     * \ingroup dataretrieve
     */
    virtual QString getTopicByUrl(const QUrl &url) = 0;

    /*!
     * \brief Gets the current ebook encoding (set or autodetected) as qtcodec name. Must be implemented,
     * even if the book doesn't support change of encoding (then it should return a default encoding)
     * \return The current encoding.
     *
     * \ingroup encoding
     */
    virtual QString currentEncoding() const = 0;

    /*!
     * \brief Sets the ebook encoding to use for TOC and content
     * \param encoding An encoding to use.
     *
     * \ingroup encoding
     */
    virtual bool setCurrentEncoding(const char *encoding) = 0;

    /*!
     * \brief Checks if this kind of URL is supported by the ebook format (i.e. could be passed to ebook functions)
     * \param url The url to check
     */
    virtual bool isSupportedUrl(const QUrl &url) = 0;

    // Converts the string to the ebook-specific URL format
    virtual QUrl pathToUrl(const QString &link) const = 0;

    // Extracts the path component from the URL
    virtual QString urlToPath(const QUrl &link) const = 0;

protected:
    // Loads the file; returns true if loaded, false otherwise
    virtual bool load(const QString &archiveName) = 0;
};

#endif // INCLUDE_LIBCHMFILE_H

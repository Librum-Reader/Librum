/*
    Kchmviewer - a CHM and EPUB file viewer with broad language support
    SPDX-FileCopyrightText: 2004-2014 George Yunaev gyunaev@ulduzsoft.com

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef EBookSearch_H
#define EBookSearch_H

#include "helper_search_index.h"
#include <QDataStream>

class EBook;

class EBookSearch : public QObject
{
    Q_OBJECT

public:
    EBookSearch();
    ~EBookSearch() override;

    //! Loads the search index from the data stream \param stream.
    //! The index should be previously saved with generateIndex().
    bool loadIndex(QDataStream &stream);

    //! Generates the search index from the opened CHM file \param chmFile,
    //! and saves it to the data stream \param stream which should be writeable.
    //!
    //! To show the progress, this procedure emits a progressStep() signal periodically
    //! with the value showing current progress in percentage (i.e. from 0 to 100)
    //! After signal emission, the following event processing function will be called:
    //!         qApp->processEvents( QEventLoop::ExcludeUserInputEvents )
    //!    to make sure the dialogs (if any) are properly updated.
    //!
    //! If \param progressDls is not null, it will be used to display progress.
    //! Returns true if the index has been generated and saved, or false if internal
    //! error occurs, or (most likely) the cancelIndexGeneration() slot has been called.
    bool generateIndex(EBook *ebook, QDataStream &stream);

    //! Executes the search query. The \param query is a string like <i>"C++ language" class</i>,
    //! \param results is a pointer to QStringList, and \param limit limits the number of
    //! results in case the query is too generic (like \a "a" ).
    //! The \param ebookFile is used to get the current encoding information.
    //! The return value is false only if the index is not generated, or if a closing quote character
    //! is missing. Call hasIndex() to clarify. If search returns no results, the return value is
    //! true, but the \param results list will be empty.
    //!
    //! Note that the function does not clear \param results before adding search results, so if you are
    //! not merging search results, make sure it's empty.
    bool searchQuery(const QString &query, QList<QUrl> *results, EBook *ebookFile, unsigned int limit = 100);

    //! Returns true if a valid search index is present, and therefore search could be executed
    bool hasIndex() const;

Q_SIGNALS:
    void progressStep(int value, const QString &stepName);

public Q_SLOTS:
    void cancelIndexGeneration();

private Q_SLOTS:
    void updateProgress(int value, const QString &stepName);
    void processEvents();

private:
    QStringList m_keywordDocuments;
    QtAs::Index *m_Index;
};

#endif

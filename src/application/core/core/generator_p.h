/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    Work sponsored by the LiMux project of the city of Munich:
    SPDX-FileCopyrightText: 2017 Klarälvdalens Datakonsult AB a KDAB Group company <info@kdab.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_THREADEDGENERATOR_P_H
#define OKULAR_THREADEDGENERATOR_P_H

#include "area.h"

#include <QImage>
#include <QMutex>
#include <QSet>
#include <QThread>

class QEventLoop;

#include "generator.h"
#include "page.h"

namespace Okular
{
class DocumentObserver;
class DocumentPrivate;
class FontInfo;
class Generator;
class Page;
class PixmapGenerationThread;
class PixmapRequest;
class TextPage;
class TextPageGenerationThread;
class TilesManager;

class GeneratorPrivate
{
public:
    GeneratorPrivate();

    virtual ~GeneratorPrivate();

    Q_DECLARE_PUBLIC(Generator)
    Generator *q_ptr;

    PixmapGenerationThread *pixmapGenerationThread();
    TextPageGenerationThread *textPageGenerationThread();

    void pixmapGenerationFinished();
    void textpageGenerationFinished();

    QMutex *threadsLock();

    virtual QVariant metaData(const QString &key, const QVariant &option) const;
    virtual QImage image(PixmapRequest *);

    DocumentPrivate *m_document;
    // NOTE: the following should be a QSet< GeneratorFeature >,
    // but it is not to avoid #include'ing generator.h
    QSet<int> m_features;
    PixmapGenerationThread *mPixmapGenerationThread;
    TextPageGenerationThread *mTextPageGenerationThread;
    mutable QMutex m_mutex;
    QMutex m_threadsMutex;
    bool mPixmapReady : 1;
    bool mTextPageReady : 1;
    bool m_closing : 1;
    QEventLoop *m_closingLoop;
    QSizeF m_dpi;
};

class PixmapRequestPrivate
{
public:
    void swap();
    TilesManager *tilesManager() const;

    static PixmapRequestPrivate *get(const PixmapRequest *req);

    DocumentObserver *mObserver;
    int mPageNumber;
    int mWidth;
    int mHeight;
    int mPriority;
    int mFeatures;
    bool mForce : 1;
    bool mTile : 1;
    bool mPartialUpdatesWanted : 1;
    Page *mPage;
    NormalizedRect mNormalizedRect;
    QAtomicInt mShouldAbortRender;
    QImage mResultImage;
};

class TextRequestPrivate
{
public:
    static TextRequestPrivate *get(const TextRequest *req);

    Page *mPage;
    QAtomicInt mShouldAbortExtraction;
};

class PixmapGenerationThread : public QThread
{
    Q_OBJECT

public:
    explicit PixmapGenerationThread(Generator *generator);

    void startGeneration(PixmapRequest *request, bool calcBoundingBox);

    void endGeneration();

    PixmapRequest *request() const;

    QImage image() const;
    bool calcBoundingBox() const;
    NormalizedRect boundingBox() const;

protected:
    void run() override;

private:
    Generator *mGenerator;
    PixmapRequest *mRequest;
    NormalizedRect mBoundingBox;
    bool mCalcBoundingBox : 1;
};

class TextPageGenerationThread : public QThread
{
    Q_OBJECT

public:
    explicit TextPageGenerationThread(Generator *generator);

    void endGeneration();

    void setPage(Page *page);
    Page *page() const;

    TextPage *textPage() const;

    void abortExtraction();
    bool shouldAbortExtraction() const;

public Q_SLOTS:
    void startGeneration();

protected:
    void run() override;

private:
    Generator *mGenerator;
    TextPage *mTextPage;
    TextRequest mTextRequest;
};

class FontExtractionThread : public QThread
{
    Q_OBJECT

public:
    FontExtractionThread(Generator *generator, int pages);

    void startExtraction(bool async);
    void stopExtraction();

Q_SIGNALS:
    void gotFont(const Okular::FontInfo &);
    void progress(int page);

protected:
    void run() override;

private:
    Generator *mGenerator;
    int mNumOfPages;
    bool mGoOn;
};

}

Q_DECLARE_METATYPE(Okular::Page *)

#endif

/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generator_p.h"
#include <QDebug>
#include "fontinfo.h"
#include "utils.h"

using namespace Okular;

PixmapGenerationThread::PixmapGenerationThread(Generator* generator) :
    mGenerator(generator),
    mRequest(nullptr),
    mCalcBoundingBox(false)
{
}

void PixmapGenerationThread::startGeneration(PixmapRequest* request,
                                             bool calcBoundingBox)
{
    mRequest = request;
    mCalcBoundingBox = calcBoundingBox;

    start(QThread::InheritPriority);
}

void PixmapGenerationThread::endGeneration()
{
    mRequest = nullptr;
}

PixmapRequest* PixmapGenerationThread::request() const
{
    return mRequest;
}

QImage PixmapGenerationThread::image() const
{
    return mRequest ? PixmapRequestPrivate::get(mRequest)->mResultImage
                    : QImage();
}

bool PixmapGenerationThread::calcBoundingBox() const
{
    return mCalcBoundingBox;
}

NormalizedRect PixmapGenerationThread::boundingBox() const
{
    return mBoundingBox;
}

void PixmapGenerationThread::run()
{
    if(mRequest)
    {
        PixmapRequestPrivate::get(mRequest)->mResultImage =
            mGenerator->image(mRequest);

        if(mCalcBoundingBox)
        {
            mBoundingBox = Utils::imageBoundingBox(
                &PixmapRequestPrivate::get(mRequest)->mResultImage);
        }
    }
}

TextPageGenerationThread::TextPageGenerationThread(Generator* generator) :
    mGenerator(generator),
    mTextPage(nullptr)
{
    TextRequestPrivate* treqPriv = TextRequestPrivate::get(&mTextRequest);
    treqPriv->mPage = nullptr;
    treqPriv->mShouldAbortExtraction = 0;
}

void TextPageGenerationThread::startGeneration()
{
    if(page())
    {
        start(QThread::InheritPriority);
    }
}

void TextPageGenerationThread::endGeneration()
{
    TextRequestPrivate* treqPriv = TextRequestPrivate::get(&mTextRequest);
    treqPriv->mPage = nullptr;
    treqPriv->mShouldAbortExtraction = 0;
}

void TextPageGenerationThread::setPage(Page* page)
{
    TextRequestPrivate* treqPriv = TextRequestPrivate::get(&mTextRequest);
    treqPriv->mPage = page;
    treqPriv->mShouldAbortExtraction = 0;
}

Page* TextPageGenerationThread::page() const
{
    return mTextRequest.page();
}

TextPage* TextPageGenerationThread::textPage() const
{
    return mTextPage;
}

void TextPageGenerationThread::abortExtraction()
{
    // If extraction already finished no point in aborting
    if(!mTextPage)
    {
        TextRequestPrivate* treqPriv = TextRequestPrivate::get(&mTextRequest);
        treqPriv->mShouldAbortExtraction = 1;
    }
}

bool TextPageGenerationThread::shouldAbortExtraction() const
{
    return mTextRequest.shouldAbortExtraction();
}

void TextPageGenerationThread::run()
{
    mTextPage = nullptr;

    Q_ASSERT(page());

    mTextPage = mGenerator->textPage(&mTextRequest);

    if(mTextRequest.shouldAbortExtraction())
    {
        delete mTextPage;
        mTextPage = nullptr;
    }
}

FontExtractionThread::FontExtractionThread(Generator* generator, int pages) :
    mGenerator(generator),
    mNumOfPages(pages),
    mGoOn(true)
{
}

void FontExtractionThread::startExtraction(bool async)
{
    if(async)
    {
        connect(this, &FontExtractionThread::finished, this,
                &FontExtractionThread::deleteLater);
        start(QThread::InheritPriority);
    }
    else
    {
        run();
        deleteLater();
    }
}

void FontExtractionThread::stopExtraction()
{
    mGoOn = false;
}

void FontExtractionThread::run()
{
    for(int i = -1; i < mNumOfPages && mGoOn; ++i)
    {
        const FontInfo::List list = mGenerator->fontsForPage(i);
        for(const FontInfo& fi : list)
        {
            Q_EMIT gotFont(fi);
        }
        Q_EMIT progress(i);
    }
}

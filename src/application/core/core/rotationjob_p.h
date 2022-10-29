/*
    SPDX-FileCopyrightText: 2006 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_ROTATIONJOB_P_H_
#define _OKULAR_ROTATIONJOB_P_H_

#include <threadweaver/job.h>
#include <threadweaver/qobjectdecorator.h>
#include <QImage>
#include <QTransform>
#include "core/area.h"
#include "core/global.h"

namespace Okular
{
class DocumentObserver;
class PagePrivate;

class RotationJobInternal : public ThreadWeaver::Job
{
    friend class RotationJob;

public:
    QImage image() const;
    Rotation rotation() const;
    NormalizedRect rect() const;

    RotationJobInternal(const RotationJobInternal&) = delete;
    RotationJobInternal& operator=(const RotationJobInternal&) = delete;

protected:
    void run(ThreadWeaver::JobPointer self,
             ThreadWeaver::Thread* thread) override;

private:
    RotationJobInternal(const QImage& image, Rotation oldRotation,
                        Rotation newRotation);

    const QImage mImage;
    Rotation mOldRotation;
    Rotation mNewRotation;
    QImage mRotatedImage;
};

class RotationJob : public ThreadWeaver::QObjectDecorator
{
    Q_OBJECT

public:
    RotationJob(const QImage& image, Rotation oldRotation, Rotation newRotation,
                DocumentObserver* observer);

    void setPage(PagePrivate* pd);
    void setRect(const NormalizedRect& rect);
    void setIsPartialUpdate(bool partialUpdate);

    QImage image() const
    {
        return static_cast<const RotationJobInternal*>(job())->image();
    }

    Rotation rotation() const
    {
        return static_cast<const RotationJobInternal*>(job())->rotation();
    }

    DocumentObserver* observer() const;
    PagePrivate* page() const;
    NormalizedRect rect() const;
    bool isPartialUpdate() const;

    static QTransform rotationMatrix(Rotation from, Rotation to);

private:
    DocumentObserver* mObserver;
    PagePrivate* m_pd;
    NormalizedRect mRect;
    bool mIsPartialUpdate;
};

}  // namespace Okular

#endif

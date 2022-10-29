/*
    SPDX-FileCopyrightText: 2007 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_GSRENDERERTHREAD_H_
#define _OKULAR_GSRENDERERTHREAD_H_

#include <libspectre/spectre.h>
#include <QMutex>
#include <QQueue>
#include <QSemaphore>
#include <QString>
#include <QThread>

class QImage;
class GSGenerator;

namespace Okular
{
class PixmapRequest;
}

struct GSRendererThreadRequest
{
    explicit GSRendererThreadRequest(GSGenerator* _owner) :
        owner(_owner),
        request(nullptr),
        spectrePage(nullptr),
        textAAbits(1),
        graphicsAAbits(1),
        magnify(1.0),
        orientation(0),
        platformFonts(true)
    {
    }

    GSGenerator* owner;
    Okular::PixmapRequest* request;
    SpectrePage* spectrePage;
    int textAAbits;
    int graphicsAAbits;
    double magnify;
    int orientation;
    bool platformFonts;
};

Q_DECLARE_TYPEINFO(GSRendererThreadRequest, Q_MOVABLE_TYPE);

class GSRendererThread : public QThread
{
    Q_OBJECT

public:
    static GSRendererThread* getCreateRenderer();

    ~GSRendererThread() override;

    void addRequest(const GSRendererThreadRequest& req);

Q_SIGNALS:
    void imageDone(QImage* image, Okular::PixmapRequest* request);

private:
    GSRendererThread();

    QSemaphore m_semaphore;

    static GSRendererThread* theRenderer;

    void run() override;

    SpectreRenderContext* m_renderContext;
    QQueue<GSRendererThreadRequest> m_queue;
    QMutex m_queueMutex;
};

#endif

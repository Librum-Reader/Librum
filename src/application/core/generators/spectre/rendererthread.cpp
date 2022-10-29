/*
    SPDX-FileCopyrightText: 2007 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "rendererthread.h"
#include <QImage>
#include "core/generator.h"
#include "core/page.h"
#include "core/utils.h"
#include "spectre_debug.h"

GSRendererThread* GSRendererThread::theRenderer = nullptr;

GSRendererThread* GSRendererThread::getCreateRenderer()
{
    if(!theRenderer)
    {
        theRenderer = new GSRendererThread();
    }
    return theRenderer;
}

GSRendererThread::GSRendererThread()
{
    m_renderContext = spectre_render_context_new();
}

GSRendererThread::~GSRendererThread()
{
    spectre_render_context_free(m_renderContext);
}

void GSRendererThread::addRequest(const GSRendererThreadRequest& req)
{
    m_queueMutex.lock();
    m_queue.enqueue(req);
    m_queueMutex.unlock();
    m_semaphore.release();
}

void GSRendererThread::run()
{
    while(true)
    {
        m_semaphore.acquire();
        {
            m_queueMutex.lock();
            GSRendererThreadRequest req = m_queue.dequeue();
            m_queueMutex.unlock();

            spectre_render_context_set_scale(m_renderContext, req.magnify,
                                             req.magnify);
            spectre_render_context_set_use_platform_fonts(m_renderContext,
                                                          req.platformFonts);
            spectre_render_context_set_antialias_bits(
                m_renderContext, req.graphicsAAbits, req.textAAbits);
            // Do not use spectre_render_context_set_rotation makes some files
            // not render correctly, e.g. bug210499.ps so we basically do the
            // rendering without any rotation and then rotate to the orientation
            // as needed spectre_render_context_set_rotation(m_renderContext,
            // req.orientation);

            unsigned char* data = nullptr;
            int row_length = 0;
            int wantedWidth = req.request->width();
            int wantedHeight = req.request->height();

            if(req.orientation % 2)
            {
                qSwap(wantedWidth, wantedHeight);
            }

            spectre_page_render(req.spectrePage, m_renderContext, &data,
                                &row_length);

            // Qt needs the missing alpha of QImage::Format_RGB32 to be 0xff
            if(data && data[3] != 0xff)
            {
                for(int i = 3; i < row_length * wantedHeight; i += 4)
                {
                    data[i] = 0xff;
                }
            }

            QImage img;
            if(row_length == wantedWidth * 4)
            {
                img = QImage(data, wantedWidth, wantedHeight,
                             QImage::Format_RGB32);
            }
            else
            {
                // In case this ends up beign very slow we can try with some
                // memmove
                QImage aux(data, row_length / 4, wantedHeight,
                           QImage::Format_RGB32);
                img = QImage(aux.copy(0, 0, wantedWidth, wantedHeight));
            }

            switch(req.orientation)
            {
            case Okular::Rotation90:
            {
                QTransform m;
                m.rotate(90);
                img = img.transformed(m);
                break;
            }

            case Okular::Rotation180:
            {
                QTransform m;
                m.rotate(180);
                img = img.transformed(m);
                break;
            }
            case Okular::Rotation270:
            {
                QTransform m;
                m.rotate(270);
                img = img.transformed(m);
            }
            }

            QImage* image = new QImage(img.copy());
            free(data);

            if(image->width() != req.request->width() ||
               image->height() != req.request->height())
            {
                qCWarning(OkularSpectreDebug).nospace()
                    << "Generated image does not match wanted size: "
                    << "[" << image->width() << "x" << image->height()
                    << "] vs requested "
                    << "[" << req.request->width() << "x"
                    << req.request->height() << "]";
                QImage aux = image->scaled(wantedWidth, wantedHeight);
                delete image;
                image = new QImage(aux);
            }
            Q_EMIT imageDone(image, req.request);

            spectre_page_free(req.spectrePage);
        }
    }
}

/* kate: replace-tabs on; indent-width 4; */

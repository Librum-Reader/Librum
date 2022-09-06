/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pagecontroller_p.h"

// local includes
#include "page_p.h"
#include "rotationjob_p.h"

#include <threadweaver/queueing.h>

using namespace Okular;

PageController::PageController()
    : QObject()
{
}

PageController::~PageController()
{
}

void PageController::addRotationJob(RotationJob *job)
{
    connect(job, &RotationJob::done, this, &PageController::imageRotationDone);
    ThreadWeaver::enqueue(&m_weaver, job);
}

void PageController::imageRotationDone(const ThreadWeaver::JobPointer &j)
{
    RotationJob *job = static_cast<RotationJob *>(j.data());

    if (job->page()) {
        job->page()->imageRotationDone(job);

        Q_EMIT rotationFinished(job->page()->m_number, job->page()->m_page);
    }
}

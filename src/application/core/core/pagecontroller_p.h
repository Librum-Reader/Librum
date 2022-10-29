/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_PAGECONTROLLER_P_H_
#define _OKULAR_PAGECONTROLLER_P_H_

#include <threadweaver/queue.h>
#include <QObject>

namespace Okular
{
class Page;
class RotationJob;

/* There is one PageController per document. It receives notifications of
 * completed RotationJobs */
class PageController : public QObject
{
    Q_OBJECT

public:
    PageController();
    ~PageController() override;

    void addRotationJob(RotationJob* job);

Q_SIGNALS:
    void rotationFinished(int page, Okular::Page* okularPage);

private Q_SLOTS:
    void imageRotationDone(const ThreadWeaver::JobPointer& job);

private:
    ThreadWeaver::Queue m_weaver;
};

}  // namespace Okular

#endif

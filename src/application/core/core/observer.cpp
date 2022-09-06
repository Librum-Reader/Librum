/*
    SPDX-FileCopyrightText: 2005 Enrico Ros <eros.kde@email.it>
    SPDX-FileCopyrightText: 2005 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "observer.h"

using namespace Okular;

DocumentObserver::DocumentObserver()
{
}

DocumentObserver::~DocumentObserver()
{
}

void DocumentObserver::notifySetup(const QVector<Okular::Page *> &, int)
{
}

void DocumentObserver::notifyViewportChanged(bool)
{
}

void DocumentObserver::notifyPageChanged(int, int)
{
}

void DocumentObserver::notifyContentsCleared(int)
{
}

void DocumentObserver::notifyVisibleRectsChanged()
{
}

void DocumentObserver::notifyZoom(int)
{
}

bool DocumentObserver::canUnloadPixmap(int) const
{
    return true;
}

void DocumentObserver::notifyCurrentPageChanged(int, int)
{
}

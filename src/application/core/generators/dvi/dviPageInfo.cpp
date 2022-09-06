/*
    SPDX-FileCopyrightText: 2006 Luigi Toscano <luigi.toscano@tiscali.it>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "dviPageInfo.h"

dviPageInfo::dviPageInfo()
{
    sourceHyperLinkList.reserve(200);
}

dviPageInfo::~dviPageInfo()
{
}

void dviPageInfo::clear()
{
    sourceHyperLinkList.clear();
}

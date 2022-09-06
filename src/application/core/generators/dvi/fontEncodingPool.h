// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
// fontEncodingPool.h
//
// Part of KDVI - A DVI previewer for the KDE desktop environment
//
// SPDX-FileCopyrightText: 2003 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef _FONTENCODINGPOOL_H
#define _FONTENCODINGPOOL_H

#include "fontEncoding.h"

#include <QHash>

class QString;

class fontEncodingPool
{
public:
    fontEncodingPool();
    ~fontEncodingPool();

    fontEncodingPool(const fontEncodingPool &) = delete;
    fontEncodingPool &operator=(const fontEncodingPool &) = delete;

    fontEncoding *findByName(const QString &name);

private:
    QHash<QString, fontEncoding *> dictionary;
};

#endif

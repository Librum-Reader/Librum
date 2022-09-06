// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
// fontEncodingPool.cpp
//
// Part of KDVI - A DVI previewer for the KDE desktop environment
//
// SPDX-FileCopyrightText: 2003 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#include <config.h>

#ifdef HAVE_FREETYPE

#include "fontEncodingPool.h"

fontEncodingPool::fontEncodingPool()
{
}

fontEncodingPool::~fontEncodingPool()
{
    qDeleteAll(dictionary);
}

fontEncoding *fontEncodingPool::findByName(const QString &name)
{
    fontEncoding *ptr = dictionary.value(name);

    if (ptr == nullptr) {
        ptr = new fontEncoding(name);
        if (ptr->isValid()) {
            dictionary.insert(name, ptr);
        } else {
            delete ptr;
            ptr = nullptr;
        }
    }

    return ptr;
}

#endif // HAVE_FREETYPE

/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_SCRIPTER_H
#define OKULAR_SCRIPTER_H

#include "global.h"

class QString;

namespace Okular
{
class Document;
class DocumentPrivate;
class Event;
class ScripterPrivate;

class Scripter
{
    friend class Document;
    friend class DocumentPrivate;

public:
    ~Scripter();

    Scripter(const Scripter &) = delete;
    Scripter &operator=(const Scripter &) = delete;

    void execute(ScriptType type, const QString &script);

    void setEvent(Event *event);
    Event *event() const;

private:
    friend class ScripterPrivate;
    ScripterPrivate *d;

    explicit Scripter(DocumentPrivate *doc);
};

}

#endif

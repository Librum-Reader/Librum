/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_SCRIPT_EXECUTOR_KJS_P_H
#define OKULAR_SCRIPT_EXECUTOR_KJS_P_H

class QString;

namespace Okular
{
class DocumentPrivate;
class ExecutorKJSPrivate;
class Event;

class ExecutorKJS
{
public:
    explicit ExecutorKJS(DocumentPrivate* doc);
    ~ExecutorKJS();

    ExecutorKJS(const ExecutorKJS&) = delete;
    ExecutorKJS& operator=(const ExecutorKJS&) = delete;

    void execute(const QString& script, Event* event);

private:
    friend class ExecutorKJSPrivate;
    ExecutorKJSPrivate* d;
};

}  // namespace Okular

#endif

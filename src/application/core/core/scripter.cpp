/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "scripter.h"
#include <QDebug>
#include <QFile>
#include "debug_p.h"
#include "script/executor_kjs_p.h"

using namespace Okular;

class Okular::ScripterPrivate
{
public:
    explicit ScripterPrivate(DocumentPrivate* doc) :
        m_doc(doc)
#ifdef WITH_KJS
        ,
        m_kjs(nullptr)
#endif
        ,
        m_event(nullptr)
    {
    }

    DocumentPrivate* m_doc;
#ifdef WITH_KJS
    QScopedPointer<ExecutorKJS> m_kjs;
#endif
    Event* m_event;
};

Scripter::Scripter(DocumentPrivate* doc) :
    d(new ScripterPrivate(doc))
{
}

Scripter::~Scripter()
{
    delete d;
}

void Scripter::execute(ScriptType type, const QString& script)
{
    qCDebug(OkularCoreDebug) << "executing the script:";
#ifdef WITH_KJS
    static QString builtInScript;
    if(builtInScript.isNull())
    {
        QFile builtInResource(QStringLiteral(":/script/builtin.js"));
        if(!builtInResource.open(QIODevice::ReadOnly))
        {
            qCDebug(OkularCoreDebug) << "failed to load builtin script";
        }
        else
        {
            builtInScript = QString::fromUtf8(builtInResource.readAll());
            builtInResource.close();
        }
    }

    switch(type)
    {
    case JavaScript:
        if(!d->m_kjs)
        {
            d->m_kjs.reset(new ExecutorKJS(d->m_doc));
        }
        d->m_kjs->execute(builtInScript + script, d->m_event);
    }
#endif
}

void Scripter::setEvent(Event* event)
{
    d->m_event = event;
}

Event* Scripter::event() const
{
    return d->m_event;
}

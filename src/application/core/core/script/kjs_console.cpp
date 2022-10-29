/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>
    SPDX-FileCopyrightText: 2008 Harri Porten <porten@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <kjs/kjsarguments.h>
#include <kjs/kjsobject.h>
#include <kjs/kjsprototype.h>
#include <QDebug>
#include "../debug_p.h"
#include "kjs_console_p.h"

using namespace Okular;

static KJSPrototype* g_consoleProto;

#ifdef OKULAR_JS_CONSOLE

    #include <KDialog>
    #include <KStandardGuiItem>
    #include <QLayout>
    #include <QPlainTextEdit>

K_GLOBAL_STATIC(KDialog, g_jsConsoleWindow)
static QPlainTextEdit* g_jsConsoleLog = 0;

static void createConsoleWindow()
{
    if(g_jsConsoleWindow.exists())
        return;

    g_jsConsoleWindow->setButtons(KDialog::Close | KDialog::User1);
    g_jsConsoleWindow->setButtonGuiItem(KDialog::User1,
                                        KStandardGuiItem::clear());

    QVBoxLayout* mainLay = new QVBoxLayout(g_jsConsoleWindow->mainWidget());
    mainLay->setContentsMargins(0, 0, 0, 0);
    g_jsConsoleLog = new QPlainTextEdit(g_jsConsoleWindow->mainWidget());
    g_jsConsoleLog->setReadOnly(true);
    mainLay->addWidget(g_jsConsoleLog);

    QObject::connect(g_jsConsoleWindow, SIGNAL(closeClicked()),
                     g_jsConsoleWindow, SLOT(close()));
    QObject::connect(g_jsConsoleWindow, SIGNAL(user1Clicked()), g_jsConsoleLog,
                     SLOT(clear()));
}

static void showConsole()
{
    createConsoleWindow();
    g_jsConsoleWindow->show();
}

static void hideConsole()
{
    if(!g_jsConsoleWindow.exists())
        return;

    g_jsConsoleWindow->hide();
}

static void clearConsole()
{
    if(!g_jsConsoleWindow.exists())
        return;

    g_jsConsoleLog->clear();
}

static void outputToConsole(const QString& message)
{
    showConsole();
    g_jsConsoleLog->appendPlainText(message);
}

#else /* OKULAR_JS_CONSOLE */

static void showConsole()
{
}

static void hideConsole()
{
}

static void clearConsole()
{
}

static void outputToConsole(const QString& cMessage)
{
    qCDebug(OkularCoreDebug) << "CONSOLE:" << cMessage;
}

#endif /* OKULAR_JS_CONSOLE */

static KJSObject consoleClear(KJSContext*, void*, const KJSArguments&)
{
    clearConsole();
    return KJSUndefined();
}

static KJSObject consoleHide(KJSContext*, void*, const KJSArguments&)
{
    hideConsole();
    return KJSUndefined();
}

static KJSObject consolePrintln(KJSContext* ctx, void*,
                                const KJSArguments& arguments)
{
    QString cMessage = arguments.at(0).toString(ctx);
    outputToConsole(cMessage);

    return KJSUndefined();
}

static KJSObject consoleShow(KJSContext*, void*, const KJSArguments&)
{
    showConsole();
    return KJSUndefined();
}

void JSConsole::initType(KJSContext* ctx)
{
    static bool initialized = false;
    if(initialized)
    {
        return;
    }
    initialized = true;

    g_consoleProto = new KJSPrototype();

    g_consoleProto->defineFunction(ctx, QStringLiteral("clear"), consoleClear);
    g_consoleProto->defineFunction(ctx, QStringLiteral("hide"), consoleHide);
    g_consoleProto->defineFunction(ctx, QStringLiteral("println"),
                                   consolePrintln);
    g_consoleProto->defineFunction(ctx, QStringLiteral("hide"), consoleShow);
}

KJSObject JSConsole::object(KJSContext* ctx)
{
    return g_consoleProto->constructObject(ctx, nullptr);
}

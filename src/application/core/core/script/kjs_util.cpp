/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>
    SPDX-FileCopyrightText: 2008 Harri Porten <porten@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <kjs/kjsarguments.h>
#include <kjs/kjsobject.h>
#include <kjs/kjsprototype.h>
#include <QDateTime>
#include <QDebug>
#include <QLocale>
#include <QRegularExpression>
#include <QUrl>
#include <cmath>
#include "kjs_util_p.h"

using namespace Okular;

static KJSPrototype* g_utilProto;

static KJSObject crackURL(KJSContext* context, void*,
                          const KJSArguments& arguments)
{
    if(arguments.count() < 1)
    {
        return context->throwException(QStringLiteral("Missing URL argument"));
    }
    QString cURL = arguments.at(0).toString(context);
    QUrl url(QUrl::fromLocalFile(cURL));
    if(!url.isValid())
    {
        return context->throwException(QStringLiteral("Invalid URL"));
    }
    if(url.scheme() != QLatin1String("file") ||
       url.scheme() != QLatin1String("http") ||
       url.scheme() != QLatin1String("https"))
    {
        return context->throwException(QStringLiteral("Protocol not valid: '") +
                                       url.scheme() + QLatin1Char('\''));
    }

    KJSObject obj;
    obj.setProperty(context, QStringLiteral("cScheme"), url.scheme());
    if(!url.userName().isEmpty())
    {
        obj.setProperty(context, QStringLiteral("cUser"), url.userName());
    }
    if(!url.password().isEmpty())
    {
        obj.setProperty(context, QStringLiteral("cPassword"), url.password());
    }
    obj.setProperty(context, QStringLiteral("cHost"), url.host());
    obj.setProperty(context, QStringLiteral("nPort"), url.port(80));
    // TODO cPath       (Optional) The path portion of the URL.
    // TODO cParameters (Optional) The parameter string portion of the URL.
    if(url.hasFragment())
    {
        obj.setProperty(context, QStringLiteral("cFragments"),
                        url.fragment(QUrl::FullyDecoded));
    }

    return obj;
}

static KJSObject printd(KJSContext* context, void*,
                        const KJSArguments& arguments)
{
    if(arguments.count() < 2)
    {
        return context->throwException(QStringLiteral("Invalid arguments"));
    }

    KJSObject oFormat = arguments.at(0);
    QString format;
    QLocale defaultLocale;

    if(oFormat.isNumber())
    {
        int formatType = oFormat.toInt32(context);
        switch(formatType)
        {
        case 0:
            format = QStringLiteral("D:yyyyMMddHHmmss");
            break;
        case 1:
            format = QStringLiteral("yyyy.MM.dd HH:mm:ss");
            break;
        case 2:
            format = defaultLocale.dateTimeFormat(QLocale::ShortFormat);
            if(!format.contains(QStringLiteral("ss")))
            {
                format.insert(format.indexOf(QStringLiteral("mm")) + 2,
                              QStringLiteral(":ss"));
            }
            break;
        }
    }
    else
    {
        format = arguments.at(0).toString(context).replace(QLatin1String("tt"),
                                                           QLatin1String("ap"));
        format.replace(QLatin1Char('t'), QLatin1Char('a'));
        for(QChar& formatChar : format)
        {
            if(formatChar == QLatin1Char('M'))
            {
                formatChar = QLatin1Char('m');
            }
            else if(formatChar == QLatin1Char('m'))
            {
                formatChar = QLatin1Char('M');
            }
        }
    }

    QLocale locale(QStringLiteral("en_US"));
    const QStringList str = arguments.at(1).toString(context).split(
        QRegularExpression(QStringLiteral("\\W")));

    if(str.count() < 7)
    {
        qWarning() << "Unexpected printd oDate argument"
                   << arguments.at(1).toString(context);
        return context->throwException(QStringLiteral("Invalid arguments"));
    }

    QString myStr = QStringLiteral("%1/%2/%3 %4:%5:%6")
                        .arg(str[1], str[2], str[3], str[4], str[5], str[6]);
    QDateTime date =
        locale.toDateTime(myStr, QStringLiteral("MMM/d/yyyy H:m:s"));

    return KJSString(defaultLocale.toString(date, format));
}

/** Converts a Number to a String using l10n
 *
 * String numberToString( Number number, String format = 'g', int precision = 6,
 *                        String LocaleName = system )
 */
static KJSObject numberToString(KJSContext* context, void*,
                                const KJSArguments& arguments)
{
    if(arguments.count() < 1)
    {
        return context->throwException(QStringLiteral("Invalid arguments"));
    }

    const double number = arguments.at(0).toNumber(context);
    if(std::isnan(number))
    {
        return KJSString("NaN");
    }

    QChar format = QLatin1Char('g');
    if(arguments.count() >= 2)
    {
        const QString fmt = arguments.at(1).toString(context);
        if(!fmt.isEmpty())
        {
            format = fmt[0];
        }
    }

    int precision = 6;
    if(arguments.count() >= 3)
    {
        precision = arguments.at(2).toInt32(context);
    }

    QLocale locale;
    if(arguments.count() == 4)
    {
        locale = QLocale(arguments.at(3).toString(context));
    }

    return KJSString(locale.toString(number, format.toLatin1(), precision));
}

/** Converts a String to a Number trying with the current locale first and
 * if that fails trying with the reverse locale for the decimal separator
 *
 * Number stringToNumber( String number ) */
static KJSObject stringToNumber(KJSContext* context, void*,
                                const KJSArguments& arguments)
{
    if(arguments.count() < 1)
    {
        return context->throwException(QStringLiteral("Invalid arguments"));
    }

    const QString number = arguments.at(0).toString(context);
    if(number.isEmpty())
    {
        return KJSNumber(0);
    }

    const QLocale locale;
    bool ok;
    double converted = locale.toDouble(number, &ok);

    if(!ok)
    {
        const QLocale locale2(locale.decimalPoint() == QLatin1Char('.')
                                  ? QStringLiteral("de")
                                  : QStringLiteral("en"));
        converted = locale2.toDouble(number, &ok);
        if(!ok)
        {
            return KJSNumber(std::nan(""));
        }
    }

    return KJSNumber(converted);
}

void JSUtil::initType(KJSContext* ctx)
{
    static bool initialized = false;
    if(initialized)
    {
        return;
    }
    initialized = true;

    g_utilProto = new KJSPrototype();
    g_utilProto->defineFunction(ctx, QStringLiteral("crackURL"), crackURL);
    g_utilProto->defineFunction(ctx, QStringLiteral("printd"), printd);
    g_utilProto->defineFunction(ctx, QStringLiteral("stringToNumber"),
                                stringToNumber);
    g_utilProto->defineFunction(ctx, QStringLiteral("numberToString"),
                                numberToString);
}

KJSObject JSUtil::object(KJSContext* ctx)
{
    return g_utilProto->constructObject(ctx, nullptr);
}

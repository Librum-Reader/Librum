/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "unrarflavours.h"
#include <QRegularExpression>

ProcessArgs::ProcessArgs()
{
}

ProcessArgs::ProcessArgs(const QStringList& args, bool lsar) :
    appArgs { args },
    useLsar { lsar }
{
}

UnrarFlavour::UnrarFlavour()
{
}

UnrarFlavour::~UnrarFlavour()
{
}

void UnrarFlavour::setFileName(const QString& fileName)
{
    mFileName = fileName;
}

QString UnrarFlavour::fileName() const
{
    return mFileName;
}

NonFreeUnrarFlavour::NonFreeUnrarFlavour() :
    UnrarFlavour()
{
}

QStringList NonFreeUnrarFlavour::processListing(const QStringList& data)
{
    // unrar-nonfree just lists the files
    return data;
}

QString NonFreeUnrarFlavour::name() const
{
    return QStringLiteral("unrar-nonfree");
}

ProcessArgs NonFreeUnrarFlavour::processListArgs(const QString& fileName) const
{
    return ProcessArgs(QStringList() << QStringLiteral("lb") << fileName,
                       false);
}

ProcessArgs NonFreeUnrarFlavour::processOpenArchiveArgs(
    const QString& fileName, const QString& path) const
{
    return ProcessArgs(QStringList() << QStringLiteral("e") << fileName
                                     << path + QLatin1Char('/'),
                       false);
}

FreeUnrarFlavour::FreeUnrarFlavour() :
    UnrarFlavour()
{
}

QStringList FreeUnrarFlavour::processListing(const QStringList& data)
{
    QRegularExpression re(QStringLiteral("^ ([^/]+/([^\\s]+))$"));

    QStringList newdata;
    for(const QString& line : data)
    {
        QRegularExpressionMatch match = re.match(line);
        if(match.hasMatch())
        {
            newdata.append(match.captured(1));
        }
    }
    return newdata;
}

QString FreeUnrarFlavour::name() const
{
    return QStringLiteral("unrar-free");
}

ProcessArgs FreeUnrarFlavour::processListArgs(const QString&) const
{
    return ProcessArgs();
}

ProcessArgs FreeUnrarFlavour::processOpenArchiveArgs(const QString&,
                                                     const QString&) const
{
    return ProcessArgs();
}

UnarFlavour::UnarFlavour() :
    UnrarFlavour()
{
}

QStringList UnarFlavour::processListing(const QStringList& data)
{
    QStringList newdata = data;

    newdata.removeFirst();

    return newdata;
}

QString UnarFlavour::name() const
{
    return QStringLiteral("unar");
}

ProcessArgs UnarFlavour::processListArgs(const QString& fileName) const
{
    return ProcessArgs(QStringList() << fileName, true);
}

ProcessArgs UnarFlavour::processOpenArchiveArgs(const QString& fileName,
                                                const QString& path) const
{
    return ProcessArgs(QStringList() << fileName << QStringLiteral("-o")
                                     << path + QLatin1Char('/'),
                       false);
}

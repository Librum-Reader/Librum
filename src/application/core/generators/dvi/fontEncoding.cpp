// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0;
// -*- fontEncoding.cpp
//
// Part of KDVI - A DVI previewer for the KDE desktop environment
//
// SPDX-FileCopyrightText: 2003 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#include <config.h>

#ifdef HAVE_FREETYPE

    #include <QFile>
    #include <QLoggingCategory>
    #include <QProcess>
    #include <QStandardPaths>
    #include <QTextStream>
    #include "debug_dvi.h"
    #include "fontEncoding.h"

//#define DEBUG_FONTENC

fontEncoding::fontEncoding(const QString& encName)
{
    #ifdef DEBUG_FONTENC
    qCDebug(OkularDviDebug) << "fontEncoding( " << encName << " )";
    #endif

    _isValid = false;
    // Use kpsewhich to find the encoding file.
    QProcess kpsewhich;
    kpsewhich.setProcessChannelMode(QProcess::MergedChannels);

    // Make sure kpsewhich is in PATH and not just in the CWD
    static const QString fullPath =
        QStandardPaths::findExecutable(QStringLiteral("kpsewhich"));
    if(fullPath.isEmpty())
    {
        qCCritical(OkularDviDebug)
            << "fontEncoding::fontEncoding(...): kpsewhich is not in path.";
        return;
    }

    kpsewhich.start(fullPath, QStringList() << encName,
                    QIODevice::ReadOnly | QIODevice::Text);

    if(!kpsewhich.waitForStarted())
    {
        qCCritical(OkularDviDebug) << "fontEncoding::fontEncoding(...): "
                                      "kpsewhich could not be started.";
        return;
    }

    // We wait here while the external program runs concurrently.
    kpsewhich.waitForFinished(-1);

    const QString encFileName =
        QString::fromLocal8Bit(kpsewhich.readAll()).trimmed();
    if(encFileName.isEmpty())
    {
        qCCritical(OkularDviDebug)
            << QStringLiteral("fontEncoding::fontEncoding(...): The file '%1' "
                              "could not be found by kpsewhich.")
                   .arg(encName);
        return;
    }

    #ifdef DEBUG_FONTENC
    qCDebug(OkularDviDebug) << "FileName of the encoding: " << encFileName;
    #endif

    QFile file(encFileName);
    if(file.open(QIODevice::ReadOnly))
    {
        // Read the file (excluding comments) into the QString variable
        // 'fileContent'
        QTextStream stream(&file);
        QString fileContent;
        while(!stream.atEnd())
        {
            fileContent += stream.readLine().section(
                QLatin1Char('%'), 0,
                0);  // line of text excluding '\n' until first '%'-sign
        }
        file.close();

        fileContent = fileContent.trimmed();

        // Find the name of the encoding
        encodingFullName =
            fileContent.section(QLatin1Char('['), 0, 0).simplified().mid(1);
    #ifdef DEBUG_FONTENC
        qCDebug(OkularDviDebug) << "encodingFullName: " << encodingFullName;
    #endif

        fileContent = fileContent.section(QLatin1Char('['), 1, 1)
                          .section(QLatin1Char(']'), 0, 0)
                          .simplified();
        const QStringList glyphNameList =
            fileContent.split(QLatin1Char('/'), QString::SkipEmptyParts);

        int i = 0;
        for(QStringList::ConstIterator it = glyphNameList.constBegin();
            (it != glyphNameList.constEnd()) && (i < 256); ++it)
        {
            glyphNameVector[i] = (*it).simplified();
    #ifdef DEBUG_FONTENC
            qCDebug(OkularDviDebug) << i << ": " << glyphNameVector[i];
    #endif
            i++;
        }
        for(; i < 256; i++)
        {
            glyphNameVector[i] = QStringLiteral(".notdef");
        }
    }
    else
    {
        qCCritical(OkularDviDebug)
            << QStringLiteral("fontEncoding::fontEncoding(...): The file '%1' "
                              "could not be opened.")
                   .arg(encFileName);
        return;
    }

    _isValid = true;
}

#endif  // HAVE_FREETYPE

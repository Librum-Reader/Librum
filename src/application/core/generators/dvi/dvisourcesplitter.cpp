// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
//
// C++ Implementation: dvisourcesplitter
//
// Author: Jeroen Wijnhout <Jeroen.Wijnhout@kdemail.net>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//

#include <config.h>

#include "debug_dvi.h"
#include "dvisourcesplitter.h"

#include <QDir>

//#define DEBUG_SOURCESPLITTER

DVI_SourceFileSplitter::DVI_SourceFileSplitter(const QString &srclink, const QString &dviFile)
{
    QString filepart = srclink, linepart;
    // if sourcefilename starts with a number
    // then there could be a mix up, i.e. src:123file.tex
    // line 123 and file.tex or line 12 and 3file.tex?
    bool possibleNumberMixUp = false;

#ifdef DEBUG_SOURCESPLITTER
    qCDebug(OkularDviDebug) << "DVI_SourceSplitter: srclink " << srclink;
#endif

    // remove src: if necessary
    if (filepart.left(4) == QLatin1String("src:")) {
        filepart = srclink.mid(4);
    }

    // split first
    quint32 max = filepart.length(), i = 0;
    for (i = 0; i < max; ++i) {
        if (!filepart[i].isDigit()) {
            break;
        }
    }
    linepart = filepart.left(i);
    filepart = filepart.mid(i);

    // check for number mix up
    if (filepart[0] != QLatin1Char(' ') && (linepart.length() != 1)) {
        possibleNumberMixUp = true;
    }

    // remove a spaces
    filepart = filepart.trimmed();
    linepart = linepart.trimmed();

#ifdef DEBUG_SOURCESPLITTER
    qCDebug(OkularDviDebug) << "DVI_SourceSplitter: filepart " << filepart << " linepart " << linepart;
#endif

    // test if the file exists
    m_fileInfo.setFile(QFileInfo(dviFile).absoluteDir(), filepart);
    bool fiExists = m_fileInfo.exists();

    // if it doesn't exist, but adding ".tex"
    if (!fiExists && QFileInfo::exists(m_fileInfo.absoluteFilePath() + QStringLiteral(".tex"))) {
        m_fileInfo.setFile(m_fileInfo.absoluteFilePath() + QStringLiteral(".tex"));
    }

    // if that doesn't help either, perhaps the file started with a
    // number: move the numbers from the sourceline to the filename
    // one by one (also try to add .tex repeatedly)
    if (possibleNumberMixUp && !fiExists) {
        QFileInfo tempInfo(m_fileInfo);
        QString tempFileName = tempInfo.fileName();
        quint32 index, maxindex = linepart.length();
        bool found = false;
        for (index = 1; index < maxindex; ++index) {
            tempInfo.setFile(linepart.right(index) + tempFileName);
#ifdef DEBUG_SOURCESPLITTER
            qCDebug(OkularDviDebug) << "DVI_SourceSplitter: trying " << tempInfo.fileName();
#endif
            if (tempInfo.exists()) {
                found = true;
                break;
            }
            tempInfo.setFile(linepart.right(index) + tempFileName + QStringLiteral(".tex"));
#ifdef DEBUG_SOURCESPLITTER
            qCDebug(OkularDviDebug) << "DVI_SourceSplitter: trying " << tempInfo.fileName();
#endif
            if (tempInfo.exists()) {
                found = true;
                break;
            }
        }

        if (found) {
            m_fileInfo = tempInfo;
            linepart = linepart.left(maxindex - index);
        }
    }

    bool ok;
    m_line = linepart.toInt(&ok);
    if (!ok) {
        m_line = 0;
    }

#ifdef DEBUG_SOURCESPLITTER
    qCDebug(OkularDviDebug) << "DVI_SourceSplitter: result: file " << m_fileInfo.absoluteFilePath() << " line " << m_line;
#endif
}

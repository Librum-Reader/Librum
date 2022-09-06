// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0; -*-
//
// C++ Interface: dvisourcesplitter
//
// Author: Jeroen Wijnhout <Jeroen.Wijnhout@kdemail.net>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//

#ifndef DVI_SOURCEFILESPLITTER_H
#define DVI_SOURCEFILESPLITTER_H

#include <QFileInfo>

class QString;

class DVI_SourceFileSplitter
{
public:
    DVI_SourceFileSplitter(const QString &srclink, const QString &dviFile);

    QString fileName() const
    {
        return m_fileInfo.fileName();
    }
    QString filePath() const
    {
        return m_fileInfo.absoluteFilePath();
    }
    bool fileExists() const
    {
        return m_fileInfo.exists();
    }
    quint32 line() const
    {
        return m_line;
    }

private:
    QFileInfo m_fileInfo;
    quint32 m_line;
};
#endif

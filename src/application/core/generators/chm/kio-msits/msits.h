/*
    SPDX-FileCopyrightText: 2005 Georgy Yunaev <tim@krasnogorsk.ru>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MSITS_H
#define MSITS_H

#include <QUrl>
#include <kio/slavebase.h>

#include <QByteArray>
#include <QString>

#include "chm_lib.h"

class ProtocolMSITS : public KIO::SlaveBase
{
public:
    ProtocolMSITS(const QByteArray &, const QByteArray &);
    ~ProtocolMSITS() override;

    ProtocolMSITS(const ProtocolMSITS &) = delete;
    ProtocolMSITS &operator=(const ProtocolMSITS &) = delete;

    void get(const QUrl &) override;
    void listDir(const QUrl &url) override;
    void stat(const QUrl &url) override;

private:
    // This function does next thing:
    // - parses the URL to get a file name and URL inside the file;
    // - loads a new CHM file, if needed;
    // - returns the parsed URL inside the file;
    bool parseLoadAndLookup(const QUrl &, QString &abspath);

    // Resolve an object inside a CHM file
    inline bool ResolveObject(const QString &fileName, chmUnitInfo *ui)
    {
        return m_chmFile != nullptr && ::chm_resolve_object(m_chmFile, fileName.toUtf8().constData(), ui) == CHM_RESOLVE_SUCCESS;
    }

    // Retrieve an object from the CHM file
    inline size_t RetrieveObject(chmUnitInfo *ui, unsigned char *buffer, LONGUINT64 fileOffset, LONGINT64 bufferSize)
    {
        return ::chm_retrieve_object(m_chmFile, ui, buffer, fileOffset, bufferSize);
    }

    // An opened file name, if presend
    QString m_openedFile;

    // a CHM structure file pointer (from chmlib)
    chmFile *m_chmFile;
};

#endif /* MSITS_H */

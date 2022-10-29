/*
    SPDX-FileCopyrightText: 2013 Azat Khuzhin <a3at.mail@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "document.h"
#include <KEncodingProber>
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include "debug_txt.h"

using namespace Txt;

Document::Document(const QString& fileName)
{
#ifdef TXT_DEBUG
    qCDebug(OkularTxtDebug) << "Opening file" << fileName;
#endif

    QFile plainFile(fileName);
    if(!plainFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCDebug(OkularTxtDebug) << "Can't open file" << plainFile.fileName();
        return;
    }

    const QByteArray buffer = plainFile.readAll();
    setPlainText(toUnicode(buffer));
}

Document::~Document()
{
}

QString Document::toUnicode(const QByteArray& array)
{
    QByteArray encoding;
    KEncodingProber prober(KEncodingProber::Universal);
    int charsFeeded = 0;
    int chunkSize = 3000;  // ~= number of symbols in page.

    // Try to detect encoding.
    while(encoding.isEmpty() && charsFeeded < array.size())
    {
        prober.feed(array.mid(charsFeeded, chunkSize));
        charsFeeded += chunkSize;

        // No more data to feed - take what we have
        if(array.size() <= chunkSize)
        {
            encoding = prober.encoding();
        }

        if(prober.confidence() >= 0.5)
        {
            encoding = prober.encoding();
            break;
        }
    }

    if(encoding.isEmpty())
    {
        return QString();
    }

    qCDebug(OkularTxtDebug) << "Detected" << prober.encoding() << "encoding"
                            << "based on" << charsFeeded << "chars";
    return QTextCodec::codecForName(encoding)->toUnicode(array);
}

Q_LOGGING_CATEGORY(OkularTxtDebug, "org.kde.okular.generators.txt",
                   QtWarningMsg)

#pragma once
#include <QCryptographicHash>
#include <QDebug>
#include <QFile>
#include <QString>

namespace application::utility
{

QString generateFileHash(const QString& filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
        qWarning() << QString("Could not open book at path: %1").arg(filePath);

    QCryptographicHash hashGen(QCryptographicHash::Sha1);
    if(hashGen.addData(&file))
        return hashGen.result().toHex();

    qWarning() << "Error reading file for hash calculation:"
               << file.errorString();
    return "";
};

}  // namespace application::utility
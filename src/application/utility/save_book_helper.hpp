#pragma once

namespace application::utility
{

inline void saveDownloadedBookMediaChunkToFile(const QByteArray& data,
                                               bool isLastChunk,
                                               const QString& fileName,
                                               const QString& destination)
{
    // Use static QMap so all the downloading books could be saved concurrently
    static QMap<QString, QSharedPointer<QFile>> filesMap;
    if(isLastChunk)
    {
        filesMap.remove(fileName);
        return;
    }

    if(!filesMap.contains(fileName))
    {
        auto newFile = QSharedPointer<QFile>(new QFile(destination));
        filesMap.insert(fileName, newFile);
    }

    QSharedPointer<QFile> file;
    file = filesMap[fileName];
    if(!file->isOpen())
    {
        if(!file->open(QIODevice::Append))
        {
            qWarning() << "Could not open new book file: " << destination;
            return;
        }
    }

    file->write(data);
}

}  // namespace application::utility

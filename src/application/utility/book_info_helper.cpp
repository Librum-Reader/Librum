#include "book_info_helper.hpp"
#include <memory>
#include <QMimeDatabase>
#include "settings.hpp"

#include <QDebug>

namespace application::utility
{

QString BookInfoHelper::parseBookTitleFromFilePath(const QString& filePath)
{
    auto lastIndexOfSlash = filePath.lastIndexOf("/");
    auto lastIndexOfDot = filePath.lastIndexOf(".");
    
    if(lastIndexOfDot == -1)
        return filePath.mid(lastIndexOfSlash + 1);
    
    auto result = filePath.mid(lastIndexOfSlash + 1, 
                               lastIndexOfDot - lastIndexOfSlash - 1);
    return result;
}

void BookInfoHelper::getBookCover(const QString& filePath)
{
    Okular::Settings::instance(QStringLiteral("okularproviderrc"));
    m_currentDocument.reset(std::make_unique<Okular::Document>(nullptr).get());
    
    QMimeDatabase db;
    
    QString prefix = "file://";
    QString systemRelativePath = filePath.mid(prefix.size());
    auto mimeType = db.mimeTypeForUrl(systemRelativePath);
//    const Okular::Document::OpenResult res = m_currentDocument->openDocument(systemRelativePath, filePath, mimeType, "");
    
//    qDebug() << "Open result: " << res;
}

} // namespace application::utility
#pragma once
#include <QString>
#include <QObject>


namespace application
{

class IBookInfoHelper : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IBookInfoHelper() noexcept = default;
    
    virtual QString parseBookTitleFromFilePath(const QString& filePath) = 0;
    virtual void getBookCover(const QString& filePath) = 0;
    
signals:
    void bookCoverReady();
};

} // namespace application
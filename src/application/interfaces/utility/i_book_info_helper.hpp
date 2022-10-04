#pragma once
#include <QString>
#include <QObject>
#include <QPixmap>


namespace application
{

class IBookInfoHelper : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IBookInfoHelper() noexcept = default;
    
    virtual bool setupDocument(const QString& filePath) = 0;
    virtual QString getTitle() const = 0;
    virtual QString getAuthor() const = 0;
    virtual QString getCreator() const = 0;
    virtual int getPageCount() const = 0;
    virtual QString getCreationDate() const = 0;
    virtual QString getFormat() const = 0;
    virtual QString getDocumentSize() const = 0;
    virtual QString getPagesSize() const = 0;
    virtual void getCover() const = 0;
    
signals:
    void bookCoverGenerated(const QPixmap* pixmal);
    void gettingBookCoverFailed();
};

} // namespace application
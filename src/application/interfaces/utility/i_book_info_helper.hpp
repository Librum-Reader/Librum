#pragma once
#include <optional>
#include <QString>
#include <QObject>
#include <QPixmap>
#include "book_meta_data.hpp"


namespace application
{

class IBookInfoHelper : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IBookInfoHelper() noexcept = default;
    
    virtual std::optional<domain::models::BookMetaData> getBookMetaData(
            const QString& filePath) = 0;

    virtual void getCover() const = 0;
    
signals:
    void bookCoverGenerated(const QPixmap* pixmal);
    void gettingBookCoverFailed();
};

} // namespace application
#pragma once
#include <QObject>
#include <QString>


namespace adapters
{

class IBookStorageAccess : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IBookStorageAccess() noexcept = default;
    virtual void storeBook() = 0;
    
signals:
    void bookStoringFinished();
};

} // namespace adapters
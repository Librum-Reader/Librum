#pragma once
#include <QObject>
#include <QString>


namespace application
{

class IBookStorageGateway : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IBookStorageGateway() noexcept = default;
    virtual void storeBook() = 0;
    
signals:
    void storingBookFinished();
};

} // namespace application#pragma once
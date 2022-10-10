#pragma once
#include <QObject>
#include <QString>


namespace application
{

class IUserStorageGateway : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IUserStorageGateway() noexcept = default;
};

} // namespace application
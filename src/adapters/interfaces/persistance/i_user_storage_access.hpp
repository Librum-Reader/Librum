#pragma once
#include <QObject>
#include <QString>


namespace adapters
{

class IUserStorageAccess : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IUserStorageAccess() noexcept = default;
};

} // namespace adapters
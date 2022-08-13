#pragma once
#include <QObject>
#include <QString>


namespace adapters
{

class IBookController : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IBookController() noexcept = default;
    
    Q_INVOKABLE virtual void uploadBook(QString path) = 0;
};

} // namespace adapters
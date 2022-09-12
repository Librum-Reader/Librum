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
    Q_INVOKABLE virtual void openBook(QString path) = 0;
    Q_INVOKABLE virtual void setCurrentBookPath(QString path) = 0;
    Q_INVOKABLE virtual QString currentBookPath() = 0;
};

} // namespace adapters
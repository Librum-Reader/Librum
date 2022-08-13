#pragma once
#include <QObject>


namespace application
{

class IBookService : public QObject
{
    Q_OBJECT
    
public:
    virtual void uploadBook(QString path) = 0;
};

} // namespace application
#pragma once
#include <QObject>
#include <QString>


namespace domain::models
{

class Book
{
public:
    Book(QString title);
    
    QString title();
    void setTitle(QString newTitle);
    
    
private:
    QString m_title;
};

} // namespace domain::models
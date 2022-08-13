#pragma once
#include <QObject>
#include <QString>
#include "book.hpp"


namespace application
{

class IPdfToRumConverter : public QObject
{
    Q_OBJECT
    
public:
    virtual void convertToRum(QString path) = 0;
    
signals:
    void conversionFinished(domain::rum_content::Book);
};

} // namespace application
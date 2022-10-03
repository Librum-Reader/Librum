#pragma once
#include <QObject>
#include <QString>


namespace domain::models
{

class Tag
{
public:
    Tag(QString name);
    
    bool operator== (const Tag& other) const;
    
    const QString& getName() const;
    void setName(QString newName);
    
private:
    QString m_name;
};

} // namespace domain::models
#pragma once

#include <QObject>

class SidebarState : public QObject
{
    Q_OBJECT
    
public:
    explicit SidebarState(QObject *parent = nullptr) { Q_UNUSED(parent) }
    
    enum Tab
    {
        FreeBooks,
        Home,
        Statistics,
        Tools,
        AddOns,
        Settings
    };
    Q_ENUM(Tab)
    
signals:
    void currentStateChanged();
    
private:
};
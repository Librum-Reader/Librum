#pragma once

#include <QObject>

class SidebarState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(State currentState READ currentState WRITE setCurrentState NOTIFY currentStateChanged)
    
public:
    explicit SidebarState(QObject *parent = nullptr) { Q_UNUSED(parent) }
    
    enum State
    {
        Opened,
        Closed
    };
    Q_ENUM(State)
    
    State currentState() const;
    void setCurrentState(State newCurrentState);
    
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
    State m_currentState;
};




inline SidebarState::State SidebarState::currentState() const
{
    return m_currentState;
}

inline void SidebarState::setCurrentState(SidebarState::State newCurrentState)
{
    if (m_currentState == newCurrentState)
        return;
    
    m_currentState = newCurrentState;
    emit currentStateChanged();
}

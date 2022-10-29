#pragma once
#include <QObject>
#include <QQmlEngine>

namespace cpp_elements
{

class SidebarState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(State currentState READ currentState WRITE setCurrentState NOTIFY
                   currentStateChanged)
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit SidebarState(QObject* parent = nullptr);

    enum State
    {
        Closed,
        Opened
    };

    Q_ENUM(State);

    State currentState() const;
    void setCurrentState(State newState);

    virtual ~SidebarState();

signals:
    void currentStateChanged();

private:
    State m_currentState;
};

}  // namespace cpp_elements
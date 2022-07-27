#include "sidebar_state.hpp"


namespace cpp_elements
{

SidebarState::SidebarState(QObject *parent)
    : QObject{parent}
{
}

SidebarState::State SidebarState::currentState() const
{    
    return m_currentState;
}

void SidebarState::setCurrentState(SidebarState::State newState)
{
    if (m_currentState == newState)
        return;
    
    m_currentState = newState;
    emit currentStateChanged();
}

SidebarState::~SidebarState()
{
    
}

} // cpp_elements
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.elements 1.0


Item 
{
    id: root
    required property var sidebar
    property int closedSidebarWidth
    property int openedSidebarWidth
    property alias closeAnimation: closeAnim
    property alias openAnimation: openAnim
    
    
    ParallelAnimation
    {
        id: openAnim
        
        PropertyAnimation
        {
            target: sidebar
            property: "implicitWidth"
            from: root.closedSidebarWidth
            to: root.openedSidebarWidth
            duration: internal.slowDuration
            easing.type: internal.easingType
        }
        
        NumberAnimation
        {
            target: profileBox
            property: "arrowRotation"
            to: 180
            duration: internal.slowDuration
            easing.type: internal.easingType
        }
        
        PropertyAnimation
        {
            target: logoLabel
            property: "visible"
            to: true
            duration: internal.slowDuration
            easing.type: internal.easingType
        }
        
        PropertyAnimation
        {
            targets: [freeBooksItem, homeItem, statisticsItem, addOnsItem, settingsItem]
            property: "labelVisibility"
            to: true
            duration: internal.slowDuration
            easing.type: internal.easingType
        }
        
        PropertyAnimation
        {
            targets: [freeBooksItem, homeItem, statisticsItem, addOnsItem, settingsItem]
            property: "textOpacity"
            from: 0
            to: 1
            duration: internal.slowDuration * 2.5
            easing.type: internal.easingType
        }
        
        onFinished: SidebarState.currentState = SidebarState.Opened
    }
    
    ParallelAnimation
    {
        id: closeAnim
        
        PropertyAnimation
        {
            target: sidebar
            property: "implicitWidth"
            to: root.closedSidebarWidth
            duration: internal.slowDuration
            easing.type: internal.easingType
        }
        
        NumberAnimation
        {
            target: profileBox
            property: "arrowRotation"
            to: 0
            duration: internal.slowDuration
            easing.type: internal.easingType
        }
        
        PropertyAnimation
        {
            target: logoLabel
            property: "visible"
            to: false
            duration: internal.fastDuration
            easing.type: internal.easingType
        }
        
        PropertyAnimation
        {
            targets: [freeBooksItem, homeItem, statisticsItem, addOnsItem, settingsItem]
            property: "labelVisibility"
            to: false
            duration: internal.fastDuration
            easing.type: internal.easingType
        }
        
        onFinished: SidebarState.currentState = SidebarState.Closed
    }
    
    QtObject
    {
        id: internal
        property int slowDuration: 250
        property int fastDuration: 100
        property int easingType: Easing.InOutQuad
    }
}
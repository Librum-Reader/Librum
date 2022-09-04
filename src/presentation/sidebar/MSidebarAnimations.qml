import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.elements 1.0


Item 
{
    property int slowDuration  : 250
    property int fastDuration : 100
    property int defaultEasingType : Easing.InOutQuad
    
    property alias closeAnimation : closeAnim
    property alias openAnimation  : openAnim
    
    
    ParallelAnimation
    {
        id: openAnim
        
        PropertyAnimation
        {
            target: root
            property: "implicitWidth"
            from: closedWidth
            to: openedWidth
            duration: slowDuration
            easing.type: defaultEasingType
        }
        
        NumberAnimation
        {
            target: profileBox
            property: "arrowRotation"
            to: 180
            duration: slowDuration
            easing.type: defaultEasingType
        }
        
        PropertyAnimation
        {
            target: logoLabel
            property: "visible"
            to: true
            duration: slowDuration
            easing.type: defaultEasingType
        }
        
        PropertyAnimation
        {
            targets: [freeBooksItem, homeItem, statisticsItem, addOnButton, settingsItem]
            property: "labelVisibility"
            to: true
            duration: slowDuration
            easing.type: defaultEasingType
        }
        
        PropertyAnimation
        {
            targets: [freeBooksItem, homeItem, statisticsItem, addOnButton, settingsItem]
            property: "textOpacity"
            from: 0
            to: 1
            duration: slowDuration * 2.5
            easing.type: defaultEasingType
        }
        
        onFinished: SidebarState.currentState = SidebarState.Opened
    }
    
    ParallelAnimation
    {
        id: closeAnim
        
        PropertyAnimation
        {
            target: root
            property: "implicitWidth"
            to: root.closedWidth
            duration: slowDuration
            easing.type: defaultEasingType
        }
        
        NumberAnimation
        {
            target: profileBox
            property: "arrowRotation"
            to: 0
            duration: slowDuration
            easing.type: defaultEasingType
        }
        
        PropertyAnimation
        {
            target: logoLabel
            property: "visible"
            to: false
            duration: fastDuration
            easing.type: defaultEasingType
        }
        
        PropertyAnimation
        {
            targets: [freeBooksItem, homeItem, statisticsItem, addOnButton, settingsItem]
            property: "labelVisibility"
            to: false
            duration: fastDuration
            easing.type: defaultEasingType
        }
        
        onFinished: SidebarState.currentState = SidebarState.Closed
    }
}
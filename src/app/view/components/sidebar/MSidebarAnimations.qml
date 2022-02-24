import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


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
        
        PropertyAnimation
        {
            target: rightArrowImage
            property: "rotation"
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
            targets: [freeBooksButton, homeButton, statisticsButton, toolsButton, addOnButton, settingsButton]
            property: "labelVisibility"
            to: true
            duration: slowDuration
            easing.type: defaultEasingType
        }
        
        PropertyAnimation
        {
            targets: [freeBooksButton, homeButton, statisticsButton, toolsButton, addOnButton, settingsButton]
            property: "textOpacity"
            from: 0
            to: 1
            duration: slowDuration * 2.5
            easing.type: defaultEasingType
        }
    }
    
    ParallelAnimation
    {
        id: closeAnim
        
        PropertyAnimation
        {
            target: root
            property: "implicitWidth"
            from: openedWidth
            to: root.closedWidth
            duration: slowDuration
            easing.type: defaultEasingType
        }
        
        PropertyAnimation
        {
            target: rightArrowImage
            property: "rotation"
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
            targets: [freeBooksButton, homeButton, statisticsButton, toolsButton, addOnButton, settingsButton]
            property: "labelVisibility"
            to: false
            duration: fastDuration
            easing.type: defaultEasingType
        }
    }
}
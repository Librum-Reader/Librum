import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Item 
{
    property int defaultDuration : 250
    property int defaultEasingType : Easing.InOutQuad
    
    property alias closeAnimation : closeAnim
    property alias openAnimation  : openAnim
    
    
    ParallelAnimation
    {
        id: openAnim
        
        PropertyAnimation
        {
            target: root
            property: "width"
            to: root.openedWidth
            duration: defaultDuration
            easing.type: defaultEasingType
        }
        
        PropertyAnimation
        {
            target: rightArrowImage
            property: "rotation"
            to: 180
            duration: defaultDuration
            easing.type: defaultEasingType
        }
        
        PropertyAnimation
        {
            target: logoLabel
            property: "visible"
            to: true
            duration: defaultDuration
            easing.type: defaultEasingType
        }
        
        PropertyAnimation
        {
            targets: [downloadButton, homeButton, statisticsButton, toolsButton, addOnButton, settingsButton]
            property: "labelVisibility"
            to: true
            duration: defaultDuration
            easing.type: defaultEasingType
        }
        
        PropertyAnimation
        {
            targets: [downloadButton, homeButton, statisticsButton, toolsButton, addOnButton, settingsButton]
            property: "textOpacity"
            from: 0
            to: 1
            duration: defaultDuration * 2.5
            easing.type: defaultEasingType
        }
    }
    
    ParallelAnimation
    {
        id: closeAnim
        
        PropertyAnimation
        {
            target: root
            property: "width"
            to: root.closedWidth
            duration: defaultDuration
            easing.type: defaultEasingType
        }
        
        PropertyAnimation
        {
            target: rightArrowImage
            property: "rotation"
            to: 0
            duration: defaultDuration
            easing.type: defaultEasingType
        }
        
        PropertyAnimation
        {
            target: logoLabel
            property: "visible"
            to: false
            duration: 50
            easing.type: defaultEasingType
        }
        
        PropertyAnimation
        {
            targets: [downloadButton, homeButton, statisticsButton, toolsButton, addOnButton, settingsButton]
            property: "labelVisibility"
            to: false
            duration: defaultDuration / 2
            easing.type: defaultEasingType
        }
    }
}
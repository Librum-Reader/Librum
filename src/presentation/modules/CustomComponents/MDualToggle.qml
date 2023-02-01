import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Librum.style 1.0

/**
 A rectangular component which switches between two states which are represented
 as texts via @leftText and @rightText
 */
Item
{
    id: root
    property string leftText: "Left"
    property string rightText: "Right"
    property bool leftSelected: false
    property bool rightSelected: true   // default
    signal toggled(string newSelected)
    
    implicitHeight: 38
    implicitWidth: 178
    
    
    Pane
    {
        id: container
        anchors.fill: parent
        padding: 0
        background: Rectangle
        {
            color: Style.colorBackground
            border.color: Style.colorContainerBorder
            radius: 4
        }
        
        
        RowLayout
        {
            id: layout
            anchors.fill: parent
            spacing: 0
            
            
            Label
            {
                id: leftLabel
                Layout.fillHeight: true
                Layout.preferredWidth: (root.width - separator.width) / 2
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: root.leftText
                color: root.leftSelected ? Style.colorBasePurple : Style.colorLightText
                font.pointSize: 12
                font.weight: root.leftSelected ? Font.Bold : Font.DemiBold
                background: Rectangle
                {
                    id: leftLabelBackground
                    anchors.fill: parent
                    anchors.margins: 1
                    color: Style.colorSidebarMark
                    opacity: internal.leftRectOpacity
                    radius: 4
                }
                
                /**
                 Due to @leftLabel having rounder corners it doesn't fill up the
                 space towards the middle (infront of the @separator).
                 This Rectangle just fills the remaining space.
                 */
                Rectangle
                {
                    id: leftBackgroundFiller
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: 1
                    anchors.bottomMargin: 1
                    width: 3
                    opacity: leftLabelBackground.opacity
                    color: leftLabelBackground.color
                }
                
                MouseArea
                {
                    anchors.fill: parent
                    
                    onClicked: selectLeftAnimation.start()
                }
            }
            
            Rectangle
            {
                id: separator
                Layout.fillHeight: true
                Layout.preferredWidth: 2
                color: Style.colorLightPurple
            }
            
            Label
            {
                id: rightLabel
                Layout.fillHeight: true
                Layout.preferredWidth: (root.width - separator.width) / 2
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: root.rightText
                color: root.rightSelected ? Style.colorBasePurple : Style.colorLightText
                font.pointSize: 12
                font.weight: root.rightSelected ? Font.Bold : Font.DemiBold
                background: Rectangle
                {
                    id: rightLabelBackground
                    anchors.fill: parent
                    anchors.margins: 1
                    opacity: internal.rightRectOpacity
                    color: Style.colorSidebarMark
                    radius: 4
                }
                
                /**
                 Due to @rightLabel having rounder corners it doesn't fill up the
                 space towards the middle (after the @separator).
                 This Rectangle just fills the remaining space.
                 */
                Rectangle
                {
                    id: rightBackgroundFiller
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: 1
                    anchors.bottomMargin: 1
                    width: 3
                    opacity: rightLabelBackground.opacity
                    color: rightLabelBackground.color
                }
                
                MouseArea
                {
                    anchors.fill: parent
                    
                    onClicked: selectRightAnimation.start()
                }
            }
        }
    }
    
    
    SequentialAnimation
    {
        id: selectRightAnimation
        
        NumberAnimation
        {
            target: internal
            property: "leftRectOpacity"
            duration: 75
            to: 0
        }
        
        NumberAnimation
        {
            target: internal
            property: "rightRectOpacity"
            duration: 75
            to: 1
        }
        
        onFinished:
        {
            root.leftSelected = false;
            root.rightSelected = true;
            root.toggled(root.rightText);
        }
    }
    
    SequentialAnimation
    {
        id: selectLeftAnimation
        
        NumberAnimation
        {
            target: internal
            property: "rightRectOpacity"
            duration: 75
            to: 0
        }
        
        NumberAnimation
        {
            target: internal
            property: "leftRectOpacity"
            duration: 75
            to: 1
        }
        
        onFinished:
        {
            root.leftSelected = true;
            root.rightSelected = false;
            root.toggled(root.leftText);
        }
    }
    
    QtObject
    {
        id: internal
        property double leftRectOpacity
        property double rightRectOpacity
        
        Component.onCompleted:
        {
            // Hard-set it once during object creation, after that it
            // is managed by fluent animations
            internal.leftRectOpacity = root.leftSelected ? 1 : 0
            internal.rightRectOpacity = root.rightSelected ? 1 : 0
        }
    }
    
    
    function selectLeft()
    {
        selectLeftAnimation.start();
    }
    
    function selectRight()
    {
        selectRightAnimation.start();
    }
}
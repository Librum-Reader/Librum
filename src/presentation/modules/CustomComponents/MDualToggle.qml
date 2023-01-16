import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Librum.style 1.0

Item
{
    id: root
    property string leftProperty: "Left"
    property string rightProperty: "Right"
    property bool leftSelected: false
    property bool rightSelected: true
    signal selectedChanged(string newSelected)
    
    implicitHeight: 38
    implicitWidth: 178
    
    
    Pane
    {
        id: container
        property double leftRectOpacity
        property double rightRectOpacity
        
        Component.onCompleted:
        {
            leftRectOpacity = root.leftSelected ? 1 : 0
            rightRectOpacity = root.rightSelected ? 1 : 0
        }
        
        
        anchors.fill: parent
        padding: 0
        background: Rectangle
        {
            color: Style.colorBackground
            border.color: Style.colorLightBorder
            radius: 4
        }
        
        
        RowLayout
        {
            id: mainLayout
            anchors.fill: parent
            spacing: 0
            
            
            Label
            {
                id: leftText
                Layout.fillHeight: true
                Layout.preferredWidth: (root.width - separator.width) / 2
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: leftProperty
                color: root.leftSelected ? Style.colorBasePurple : Style.colorLightText3
                font.pointSize: 12
                font.weight: root.leftSelected ? Font.Bold : Font.DemiBold
                background: Rectangle
                {
                    anchors.fill: parent
                    anchors.margins: 1
                    color: Style.colorSidebarMark
                    opacity: container.leftRectOpacity
                    radius: 4
                }
                
                Rectangle
                {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: 1
                    anchors.bottomMargin: 1
                    width: 3
                    opacity: container.leftRectOpacity
                    color: Style.colorSidebarMark
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
                color: Style.colorMediumPurple
            }
            
            Label
            {
                id: rightText
                Layout.fillHeight: true
                Layout.preferredWidth: (root.width - separator.width) / 2
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: rightProperty
                color: root.rightSelected ? Style.colorBasePurple : Style.colorLightText3
                font.pointSize: 12
                font.weight: root.rightSelected ? Font.Bold : Font.DemiBold
                background: Rectangle
                {
                    anchors.fill: parent
                    anchors.margins: 1
                    opacity: container.rightRectOpacity
                    color: Style.colorSidebarMark
                    radius: 4
                }
                
                Rectangle
                {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: 1
                    anchors.bottomMargin: 1
                    width: 3
                    opacity: container.rightRectOpacity
                    color: Style.colorSidebarMark
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
            target: container
            property: "leftRectOpacity"
            duration: 75
            to: 0
        }
        
        NumberAnimation
        {
            target: container
            property: "rightRectOpacity"
            duration: 75
            to: 1
        }
        
        onFinished:
        {
            root.leftSelected = false;
            root.rightSelected = true;
            root.selectedChanged(root.rightProperty);
        }
    }
    
    SequentialAnimation
    {
        id: selectLeftAnimation
        
        NumberAnimation
        {
            target: container
            property: "rightRectOpacity"
            duration: 75
            to: 0
        }
        
        NumberAnimation
        {
            target: container
            property: "leftRectOpacity"
            duration: 75
            to: 1
        }
        
        onFinished:
        {
            root.leftSelected = true;
            root.rightSelected = false;
            root.selectedChanged(root.leftProperty);
        }
    }
}
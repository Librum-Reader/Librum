import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

FocusScope
{
    id: root
    implicitWidth: 38
    implicitHeight: 36
    
    property int defaultWidth : 0

    property int expensionWidth : 445
    property bool opened : false
    
    Rectangle
    {
        id: searchBar
        width: root.width
        height: root.height
        color: properties.colorBackground
        border.width: 1
        border.color: properties.colorLightBorder
        radius: 5
        
        RowLayout
        {
            id: layout
            anchors.fill: parent
            
            TextField
            {
                id: inputField
                visible: false
                z: 1
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter
                leftPadding: 12
                font.pointSize: 12
                placeholderText: "Test"
                placeholderTextColor: "gray"
                background: Rectangle
                {
                    anchors.fill: parent
                    radius: 4
                    color: "transparent"
                }                
            }
            
            Item
            {
                id: searchBarDefaultBox
                Layout.preferredHeight: parent.height
                Layout.preferredWidth: root.defaultWidth
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                
                Image
                {
                    id: searchBarIcon
                    anchors.centerIn: parent
                    sourceSize.height: 16
                    source: "/resources/images/search-icon.svg"
                    fillMode: Image.PreserveAspectFit
                    antialiasing: false
                }
                
                MouseArea
                {
                    anchors.fill: parent
                    Layout.alignment: Qt.AlignRight
                    
                    onClicked:
                    {
                        if(root.opened)
                        {
                            closeAnimation.start();
                            root.opened = false;
                            inputField.clear();
                        }
                        else
                        {
                            openAnimation.start();
                            root.opened = true;
                        }
                    }
                }
            }
        }
    }
    
    ParallelAnimation
    {
        id: openAnimation
        
        PropertyAnimation
        {
            target: root
            property: "width"
            to: root.expensionWidth
            duration: 300
            easing.type: Easing.InOutQuad
        }
        
        PropertyAnimation
        {
            target: root
            property: "x"
            to: root.x - root.expensionWidth + root.defaultWidth
            duration: 300
            easing.type: Easing.InOutQuad
        }
        
        PropertyAnimation
        {
            target: inputField
            property: "visible"
            to: true
            duration: 250
            easing.type: Easing.InOutQuad
        }
    }
    
    ParallelAnimation
    {
        id: closeAnimation
        
        PropertyAnimation
        {
            target: root
            property: "width"
            to: root.defaultWidth
            duration: 200
            easing.type: Easing.InOutQuad
        }
        
        PropertyAnimation
        {
            target: root
            property: "x"
            to: root.x + root.expensionWidth - root.defaultWidth
            duration: 200
            easing.type: Easing.InOutQuad
        }
        
        PropertyAnimation
        {
            target: inputField
            property: "visible"
            to: false
            duration: 50
            easing.type: Easing.InOutQuad
        }
    }
    
    Component.onCompleted: 
    {
        root.defaultWidth = width;
    }
}
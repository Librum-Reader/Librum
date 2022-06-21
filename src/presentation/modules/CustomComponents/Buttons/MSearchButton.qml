import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.icons


Item
{
    id: root
    property int defaultWidth: 0
    property int expansionWidth: 445
    property int openAnimationDuration: 300
    property int closeAnimationDuration: 200
    
    property int imageSize: 16
    property bool opened: false
    signal triggered(string query)
    
    implicitWidth: 38
    implicitHeight: 36
    
    
    Pane
    {
        id: container
        anchors.fill: parent
        padding: 0
        background: Rectangle
        {
            color: Style.colorBackground
            border.width: 1
            border.color: Style.colorLightBorder
            radius: 5
        }
        
        
        RowLayout
        {
            id: layout
            anchors.fill: parent
            
            
            TextField
            {
                id: inputField
                Layout.fillWidth: true
                z: 1
                visible: false
                leftPadding: 12
                color: Style.colorBaseText
                font.pointSize: 12
                placeholderText: "Search for entries"
                placeholderTextColor: Style.colorLightText2
                background: Rectangle
                {
                    anchors.fill: parent
                    radius: 4
                    color: "transparent"
                }
                
                onVisibleChanged: if(visible) forceActiveFocus();
                
                Keys.onPressed:
                    (event) =>
                    {
                        if(event.key === Qt.Key_Return)
                        {
                            triggered(inputField.text);
                        }
                        else if(event.key === Qt.Key_Escape)
                        {
                            if(root.opened)
                            {
                                root.close();
                            }
                        }
                    }
            }
            
            Item
            {
                id: searchBarDefaultBox
                Layout.preferredWidth: root.defaultWidth
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight
                
                Image
                {
                    id: searchBarIcon
                    anchors.centerIn: parent
                    source: Icons.searchBlack
                    fillMode: Image.PreserveAspectFit
                    sourceSize.height: root.imageSize
                }
                
                MouseArea
                {
                    anchors.fill: parent
                    
                    onClicked:
                    {
                        if(root.opened)
                            root.close();
                        else
                            root.open();
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
            property: "implicitWidth"
            to: root.expansionWidth
            duration: root.openAnimationDuration
            easing.type: Easing.InOutQuad
        }
        
        PropertyAnimation
        {
            target: inputField
            property: "visible"
            to: true
            duration: root.openAnimationDuration - 50
            easing.type: Easing.InOutQuad
        }
        
        PropertyAnimation
        {
            target: inputField
            property: "opacity"
            to: 1
            duration: 0
        }
        
        onFinished:
        {
            root.opened = true;
        }
    }
    
    
    ParallelAnimation
    {
        id: closeAnimation
        
        PropertyAnimation
        {
            target: root
            property: "implicitWidth"
            to: root.defaultWidth
            duration: root.closeAnimationDuration
            easing.type: Easing.InOutQuad
        }
        
        PropertyAnimation
        {
            target: inputField
            property: "opacity"
            to: 0
            duration: root.closeAnimationDuration / 3
            easing.type: Easing.InOutQuad
        }
        
        PropertyAnimation
        {
            target: inputField
            property: "visible"
            to: false
            duration: root.closeAnimationDuration / 2
            easing.type: Easing.InOutQuad
        }
        
        onFinished:
        {
            root.opened = false;
            inputField.clear();
        }
    }
    
    
    function open()
    {
        openAnimation.start();
    }
    
    function close()
    {
        closeAnimation.start();
    }
    
    
    Component.onCompleted:
    {
        root.defaultWidth = width;
    }
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }    
}

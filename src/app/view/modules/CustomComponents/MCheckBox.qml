import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope
{
    id: root
    property color borderColor : properties.colorDarkBorder
    property int borderRadius : 4
    property int borderWidth : 1
    property color backgroundColor : "transparent"
    property string imagePath : "/resources/images/check.svg"
    property bool imageDefaultVisibility : false
    property bool activated : false
    
    implicitWidth: 22
    implicitHeight: 22
    
    signal clicked()
    
    
    Rectangle
    {
        anchors.fill: parent
        antialiasing: true
        radius: root.borderRadius
        border.width: root.borderWidth
        border.color: root.borderColor
        color: root.backgroundColor
        
        Image
        {
            id: image
            anchors.centerIn: parent
            visible: imageDefaultVisibility
            sourceSize.width: parent.width - 8
            source: root.imagePath
            fillMode: Image.PreserveAspectFit
        }
        
        MouseArea
        {
            id: mouseArea
            anchors.fill: parent
            onClicked:
            {
                root.clicked();
                image.visible = !image.visible;
            }
        }
    }
}

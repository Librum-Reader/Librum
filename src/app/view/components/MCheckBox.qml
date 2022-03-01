import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope
{
    id: root
    implicitWidth: 22
    implicitHeight: 22
    
    property color borderColor : properties.colorDarkBorder
    property int borderRadius : 4
    property int borderWidth : 1
    property color backgroundColor : "transparent"
    property string imageSource : "/resources/images/check.svg"
    property bool imageDefaultVisibility : false
    property bool activated : false
    
    signal clicked()
    
    Rectangle
    {
        width: root.width
        height: root.height
        antialiasing: true
        radius: root.borderRadius
        border.width: root.borderWidth
        border.color: root.borderColor
        color: root.backgroundColor
        
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
        
        Image
        {
            id: image
            anchors.centerIn: parent
            visible: imageDefaultVisibility
            sourceSize.width: parent.width - 8
            source: root.imageSource
            fillMode: Image.PreserveAspectFit
        }
    }
}

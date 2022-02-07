import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope
{
    id: root
    width: checkboxWidth
    height: checkboxHeight
    
    property int checkboxHeight : 20
    property int checkboxWidth : 20
    property color borderColor : "black"
    property int borderRadius : 0
    property int borderWidth : 1
    property color backgroundColor : "transparent"
    property string imageSource : ""
    property bool imageDefaultVisibility : false
    property bool activated : false
    
    signal clicked()
    
    Rectangle
    {
        height: root.checkboxHeight
        width: root.checkboxWidth
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
            width: parent.width - 2
            source: root.imageSource
            fillMode: Image.PreserveAspectFit
        }
    }
}

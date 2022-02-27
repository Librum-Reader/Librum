import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope
{
    id: root
    
    implicitWidth: 100
    implicitHeight: 30
    property int buttonRadius : 4
    property color backgroundColor : "red"
    
    property color fontColor : "black"
    property string textContent : "Text here"
    property bool fontBold : false
    property int fontSize : 11

    signal clicked()
    
    
    Rectangle
    {
        id: prvt
        height: root.height
        width:  root.width
        radius: root.buttonRadius
        color: root.backgroundColor
        opacity: (mouseArea.pressed ? 0.9 : 1)
        
        Label
        {
            id: loginButtonText
            anchors.centerIn: parent
            text: root.textContent
            font.bold: root.fontBold
            font.pointSize: root.fontSize
            color: root.fontColor
        }
        
        MouseArea
        {
            id: mouseArea
            anchors.fill: parent
            onClicked: root.clicked()
        }
    }
}
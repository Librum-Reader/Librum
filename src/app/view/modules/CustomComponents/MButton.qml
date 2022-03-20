import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope
{
    id: root
    property int buttonRadius : 4
    property color borderColor: properties.colorDarkBorder
    property int borderWidth: 1
    property color backgroundColor : "red"
    property color fontColor : "black"
    property string textContent : "Text here"
    property bool fontBold : false
    property double fontSize : 11
    property string imagePath : ""
    property int imageSpacing : 8
    property int imageSize : 15
    
    implicitWidth: 100
    implicitHeight: 30
    
    signal clicked()
    
    
    Rectangle
    {
        id: container
        width:  parent.width
        height: parent.height
        color: root.backgroundColor
        border.width: root.borderWidth
        border.color: root.borderColor
        radius: root.buttonRadius
        opacity: (mouseArea.pressed ? 0.9 : 1)
        
        RowLayout
        {
            id: layout
            anchors.centerIn: parent
            spacing: root.imageSpacing
            
            Image
            {
                id: image
                visible: root.imagePath.length > 0
                source: root.imagePath
                sourceSize.width: root.imageSize
                fillMode: Image.PreserveAspectFit
            }
            
            Label
            {
                id: loginButtonText
                text: root.textContent
                font.bold: root.fontBold
                font.pointSize: root.fontSize
                color: root.fontColor
            }
        }
        
        MouseArea
        {
            id: mouseArea
            anchors.fill: parent
            onClicked: root.clicked()
        }
    }
}
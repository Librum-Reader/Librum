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
    property double fontSize : 11
    property string imageSource : ""
    property int spacing : 8
    property int imageSize : 15
    
    signal clicked()
    
    
    Rectangle
    {
        id: prvt
        height: root.height
        width:  root.width
        radius: root.buttonRadius
        color: root.backgroundColor
        opacity: (mouseArea.pressed ? 0.9 : 1)
        
        RowLayout
        {
            id: mainLayout
            width: image.width + loginButtonText.width + root.spacing
            anchors.centerIn: parent
            spacing: root.spacing
            
            Image
            {
                id: image
                visible: root.imageSource != ""
                source: root.imageSource
                fillMode: Image.PreserveAspectFit
                sourceSize.width: root.imageSize
                antialiasing: false
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
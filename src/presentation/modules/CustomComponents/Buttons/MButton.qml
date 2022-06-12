import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Item
{
    id: root
    property int buttonRadius : 4
    property color borderColor: "red"
    property int borderWidth: 1
    property color backgroundColor : "white"
    property string text
    property color fontColor : "black"
    property bool fontBold : false
    property double fontSize : 11
    property string imagePath
    property int imageSpacing : 8
    property int imageSize : 15
    property double opacityOnPressed: 0.9
    
    implicitWidth: 100
    implicitHeight: 30
    
    signal clicked()
    
    
    Rectangle
    {
        id: container
        anchors.fill: parent
        color: root.backgroundColor
        border.width: root.borderWidth
        border.color: root.borderColor
        radius: root.buttonRadius
        opacity: (mouseArea.pressed || root.activeFocus ? root.opacityOnPressed : 1)
        antialiasing: true
        
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
                visible: text.length > 0
                text: root.text
                font.bold: root.fontBold
                font.pointSize: root.fontSize
                font.family: properties.defaultFontFamily
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
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }    
}
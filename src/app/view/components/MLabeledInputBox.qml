import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope
{
    id: root
    property string placeholderContent : "placeholder"
    property int    inputFontSize : 11
    property color  placeholderColor : "black"
    property string headerText : "Header here"
    property color  borderColor : properties.colorLightBorder
    property int    borderWidth : 2
    property int    textPadding : 15
    property string imagePath : ""
    property string toggledImagePath : ""
    property bool   addImageToRight : false
    property bool   isError : false
    
    implicitWidth: 100
    implicitHeight: label.implicitHeight + inputBox.height
    
    
    Item
    {
        width: parent.width
        height: parent.height
        
        ColumnLayout
        {
            id: layout
            spacing: 0
            width: parent.width
            
            Label
            {
                id: label
                width: parent.width
                text: root.headerText
                font.family: properties.defaultFontFamily
                font.pointSize: 10.5
                font.weight: Font.Medium
                color: properties.colorBaseTitle
            }
            
            Rectangle
            {
                id: inputBox
                width: parent.width
                height: 40
                Layout.topMargin: 2
                border.width: root.borderWidth
                border.color: (root.isError ? properties.colorError : root.borderColor)
                radius: 5
                
                Row
                {
                    id: inBoxLayout
                    width: parent.width
                    height: parent.height
                    spacing: 0
                    
                    TextField
                    {
                        id: inputField
                        width: (addImageToRight ? parent.width - 30 : parent.width)
                        focus: true
                        selectByMouse: true
                        color: properties.colorBaseText
                        font.pointSize: root.inputFontSize
                        padding: root.textPadding
                        anchors.verticalCenter: parent.verticalCenter
                        placeholderText: root.placeholderContent
                        placeholderTextColor: root.placeholderColor
                        echoMode: (!root.addImageToRight || imageArea.pressed ? TextInput.Normal : TextInput.Password)
                        background: Rectangle   
                        {
                            anchors.fill: parent
                            radius: 4
                            color: "transparent"
                        }
                        
                        onTextEdited: root.isError = false
                    }
                    
                    
                    
                    Image
                    {
                        id: image
                        width:  20
                        height: 18
                        anchors.verticalCenter: parent.verticalCenter
                        visible: root.addImageToRight
                        source: (imageArea.pressed ? root.toggledImagePath : root.imagePath)
                        
                        MouseArea
                        {
                            id: imageArea
                            anchors.fill: parent
                        }
                    }
                }
            }
        }
    }
}
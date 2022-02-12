import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope
{
    id: root
    implicitWidth: 100
    implicitHeight: emailLabel.implicitHeight + inputBox.height
    
    
    property string placeholderContent : "placeholder"
    property int    inputFontSize : 11
    property color  placeholderColor : "black"
    property string headerText : "Header here"
    property color  borderColor : properties.colorLightBorder
    property int    borderWidth : 2
    property int    textPadding : 15
    property string image : ""
    property string toggleImage : ""
    property bool   addImageToRight : false
    property bool   isError : false
    
    Item
    {
        width: parent.width
        height: parent.height
        
        ColumnLayout
        {
            spacing: 0
            width: parent.width
            
            Label
            {
                id: emailLabel
                text: root.headerText
                textFormat: Text.RichText
                width: parent.width
                font.family: properties.defaultFontFamily
                font.pointSize: 10
            }
            
            Rectangle
            {
                id: inputBox
                height: 40
                width: parent.width
                Layout.topMargin: 2
                border.width: root.borderWidth
                border.color: (root.isError ? properties.colorError : root.borderColor)
                radius: 5
                
                Row
                {
                    width: parent.width
                    height: parent.height
                    spacing: 0
                    
                    TextField
                    {
                        id: inputField
                        width: (addImageToRight ? parent.width - 30 : parent.width)
                        focus: true
                        selectByMouse: true
                        background: Rectangle
                        {
                            anchors.fill: parent
                            radius: 4
                            color: "transparent"
                        }
                        
                        font.pointSize: root.inputFontSize
                        padding: root.textPadding
                        anchors.verticalCenter: parent.verticalCenter
                        placeholderText: root.placeholderContent
                        placeholderTextColor: root.placeholderColor
                        echoMode: (!root.addImageToRight || imageAtRight.pressed ? TextInput.Normal : TextInput.Password)
                        
                        onTextEdited:
                        {
                            root.isError = false
                        }
                    }
                    
                    Button
                    {
                        id: imageAtRight
                        width:  20
                        height: 18
                        visible: root.addImageToRight
                        anchors.verticalCenter: parent.verticalCenter
                        background: Rectangle
                        {
                            anchors.fill: parent
                            color: "transparent"
                        }
                        
                        Image
                        {
                            id: image
                            width: parent.width
                            height: parent.height
                            source: (imageAtRight.pressed ? root.toggleImage : root.image)
                        }
                    }
                }
            }
        }
    }
}
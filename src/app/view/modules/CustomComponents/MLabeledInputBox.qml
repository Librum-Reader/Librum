import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope
{
    id: root
    property alias text : inputField.text
    property int boxHeight : 40
    property int boxWidth : 100
    property string placeholderContent : "placeholder"
    property int inputFontSize : 12
    property color placeholderColor : "black"
    property string headerText : "Header here"
    property double headerFontSize : 10.5
    property int headerToBoxSpacing : 2
    property color borderColor : properties.colorLightBorder
    property int borderWidth : 2
    property int textPadding : 15
    property string imagePath : ""
    property string toggledImagePath : ""
    property bool addImageToRight : false
    property bool isError : false
    property bool autoFocus : false
    
    implicitWidth: boxWidth
    implicitHeight: label.implicitHeight + inputBox.height
    
    
    ColumnLayout
    {
        id: layout
        width: parent.width
        spacing: root.headerToBoxSpacing
        
        Label
        {
            id: label
            width: parent.width
            text: root.headerText
            font.family: properties.defaultFontFamily
            font.pointSize: root.headerFontSize
            font.weight: Font.Medium
            color: properties.colorBaseTitle
        }
        
        Rectangle
        {
            id: inputBox
            width: parent.width
            height: root.boxHeight
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
                    selectByMouse: true
                    color: properties.colorBaseText
                    font.pointSize: root.inputFontSize
                    font.family: properties.defaultFontFamily
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
                    
                    Component.onCompleted:
                    {
                        if(root.autoFocus)
                            forceActiveFocus();
                    }
                }
                
                Image
                {
                    id: image
                    width: 20
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
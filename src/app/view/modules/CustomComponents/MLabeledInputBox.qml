import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope
{
    id: root
    property alias text : inputField.text
    property int boxHeight : 40
    property string placeholderContent : ""
    property color placeholderColor : "black"
    property int inputFontSize : 12
    property color inputFontColor : properties.colorBaseText
    property string headerText : "Header here"
    property double headerFontSize : 10.5
    property color headerFontColor : properties.colorBaseTitle
    property int headerToBoxSpacing : 2
    property color borderColor : properties.colorLightBorder
    property int borderWidth : 2
    property int borderRadius : 5
    property int textPadding : 15
    property string imagePath : ""
    property string toggledImagePath : ""
    property bool addImageToRight : false
    property bool isError : false
    property bool autoFocus : false
    
    implicitWidth: 100
    implicitHeight: header.implicitHeight + inputBox.height
    
    
    ColumnLayout
    {
        id: layout
        width: parent.width
        spacing: root.headerToBoxSpacing
        
        Label
        {
            id: header
            width: parent.width
            text: root.headerText
            font.family: properties.defaultFontFamily
            font.pointSize: root.headerFontSize
            font.weight: Font.Medium
            color: root.headerFontColor
        }
        
        Rectangle
        {
            id: inputBox
            width: parent.width
            height: root.boxHeight
            border.width: root.borderWidth
            border.color: (root.isError ? properties.colorError : root.borderColor)
            radius: root.borderRadius
            
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
                    color: root.inputFontColor
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
                        radius: root.borderRadius
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
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope
{
    id: root
    Layout.preferredWidth: 100
    Layout.preferredHeight: emailLabel.implicitHeight + inputBox.height
    
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
    
    
    signal error(string message)
    
    function setError()
    {
        inputBox.border.color = properties.colorError
    }
    
    function removeError()
    {
        inputBox.border.color = root.borderColor
    }
    
    
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
            border.color: root.borderColor
            radius: 5
            
            Row
            {
                width: parent.width
                height: parent.height
                spacing: 0
                
                TextField
                {
                    id: inputField
                    width: parent.width
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
                    echoMode: (root.addImageToRight ? TextInput.Password : TextInput.Normal)
                    
                    onTextEdited:
                    {
                        removeError();
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
                        border.width: 0
                        color: "transparent"
                    }
                    
                    Image
                    {
                        id: image
                        width: parent.width
                        height: parent.height
                        source: root.image
                    }
                    
                    onPressedChanged:
                    {
                        if(pressed)
                        {
                            image.source = root.toggleImage
                            inputField.echoMode = TextInput.Normal
                        }
                        else
                        {
                            image.source = root.image
                            inputField.echoMode = TextInput.Password
                        }
                    }
                    
                    Component.onCompleted:
                    {
                        if(root.addImageToRight)
                            inputField.width = Qt.binding(function() { return inputBox.width - 30; })
                    }
                }
            }
        }
    }
}
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope
{
    id: root
    width: parent.width
    height: emailLabel.implicitHeight + inputBox.height
    
    property string placeholderContent : "placeholder"
    property int    inputFontSize : 11
    property color  placeholderColor : "#8E8EA9"
    property string headerText : "Header here"
    property color  borderColor : "#DCDCE4"
    property string fontFamily : "Droid Sans Fallback"
    property int    textPadding : 15
    property string image : ""
    property string toggleImage : ""
    property bool   addImageToRight : false
    
    
    ColumnLayout
    {
        spacing: 0
        width: parent.width
        
        Label
        {
            id: emailLabel
            text: root.headerText
            width: parent.width
            font.family: root.fontFamily
            font.pointSize: 10
        }
        
        Rectangle
        {
            id: inputBox
            height: 40
            width: parent.width
            Layout.topMargin: 2
            border.width: 2
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
                }
                
                Button
                {
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
                        id: showImage
                        width: parent.width
                        height: parent.height
                        source: root.image
                    }
                    
                    onPressedChanged:
                    {
                        if(pressed)
                        {
                            showImage.source = root.toggleImage
                            inputField.echoMode = TextInput.Normal
                        }
                        else
                        {
                            showImage.source = root.image
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
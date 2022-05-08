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
    property double inputFontSize : 12
    property color inputFontColor : properties.colorBaseText
    property string headerText : "Header here"
    property int headerFontWeight : Font.Medium
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
    implicitHeight: layout.height
    
    
    ColumnLayout
    {
        id: layout
        width: parent.width
        height: header.implicitHeight + inputBox.height
        spacing: root.headerToBoxSpacing
        
        Label
        {
            id: header
            Layout.fillWidth: true
            text: root.headerText
            font.family: properties.defaultFontFamily
            font.pointSize: root.headerFontSize
            font.weight: root.headerFontWeight
            color: root.headerFontColor
        }
        
        Pane
        {
            id: inputBox
            Layout.fillWidth: true
            Layout.preferredHeight: root.boxHeight
            padding: 0
            background: Rectangle
            {
                border.width: root.borderWidth
                border.color: (root.isError ? properties.colorError : root.borderColor)
                radius: root.borderRadius
            }
            
            
            RowLayout
            {
                id: inBoxLayout
                anchors.fill: parent
                spacing: 0
                
                TextField
                {
                    id: inputField
                    Layout.fillWidth: true
                    leftPadding: root.textPadding
                    rightPadding: root.textPadding
                    selectByMouse: true
                    color: root.inputFontColor
                    font.pointSize: root.inputFontSize
                    font.family: properties.defaultFontFamily
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
                    Layout.preferredWidth: 20
                    Layout.preferredHeight: 18
                    Layout.rightMargin: 10
                    Layout.alignment: Qt.AlignVCenter
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
    
    function clearText()
    {
        inputField.clear();
    }
}
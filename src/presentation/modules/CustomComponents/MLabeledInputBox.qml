import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0


Item
{
    id: root
    property alias text : inputField.text
    property int boxHeight : 40
    property string placeholderContent : ""
    property color placeholderColor : "black"
    property double inputFontSize : 12
    property color inputFontColor : Style.colorBaseText
    property bool readOnly: false
    property int inputFontWeight: Font.Normal
    property string headerText : "Header here"
    property int headerFontWeight : Font.Medium
    property double headerFontSize : 10.5
    property color headerFontColor : Style.colorBaseTitle
    property int headerToBoxSpacing : 2
    property color borderColor : Style.colorLightBorder
    property int borderWidth : 2
    property int borderRadius : 5
    property int textPadding : 15
    property string imagePath : ""
    property string toggledImagePath : ""
    property bool textHidden: toggledImagePath.length > 0 ? true : false
    signal edited
    
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
            font.family: Style.defaultFontFamily
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
                border.color: root.borderColor
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
                    readOnly: root.readOnly
                    color: root.inputFontColor
                    font.pointSize: root.inputFontSize
                    font.family: Style.defaultFontFamily
                    font.weight: root.inputFontWeight
                    placeholderText: root.placeholderContent
                    placeholderTextColor: root.placeholderColor
                    echoMode: root.textHidden ? TextInput.Password : TextInput.Normal
                    background: Rectangle   
                    {
                        anchors.fill: parent
                        radius: root.borderRadius
                        color: "transparent"
                    }
                    
                    onTextEdited: root.edited()
                    
                    Component.onCompleted:
                    {
                        if((inputBox.height/inputField.implicitHeight) / 2 > 0.1)
                        {
                            inputField.Layout.topMargin = inputField.Layout.topMargin - 1;
                        }
                    }
                }
                
                Image
                {
                    id: image
                    
                    Layout.preferredWidth: 20
                    Layout.preferredHeight: 18
                    Layout.rightMargin: 10
                    Layout.alignment: Qt.AlignVCenter
                    visible: root.imagePath.length > 0
                    source: root.textHidden ? root.imagePath : root.toggledImagePath
                    
                    MouseArea
                    {
                        id: imageArea
                        anchors.fill: parent
                        
                        onClicked: root.textHidden = !root.textHidden
                    }
                }
            }
        }
    }
    
    
    function giveFocus()
    {
        inputField.forceActiveFocus();
    }
    
    function clearText()
    {
        inputField.clear();
    }
}
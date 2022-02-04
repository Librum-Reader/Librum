import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Page
{
    id: root
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.loginWindowBackground
    }
    
    property int baseHeight : 800
    property int baseWidth : 542
    
    ColumnLayout
    {
        anchors.centerIn: parent
        width: root.baseWidth
        
        Rectangle
        {
            id: containerRect
            height: root.baseHeight
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            radius: 4
            color: properties.loginContentBackground
            
            ColumnLayout
            {
                id: inRectLayout
                anchors
                {
                    top:   parent.top
                    left:  parent.left
                    right: parent.right
                }
                
                anchors.margins: 15
                anchors.topMargin: 48
                
                Rectangle
                {
                    id: logo
                    height: 72
                    width: 72
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                    color: properties.colorBasePurple
                    radius: 8
                    antialiasing: true
                    
                    Rectangle
                    {
                        anchors.centerIn: parent
                        width: parent.width / 2
                        height: parent.height / 2
                        radius: width / 4
                        color: properties.loginContentBackground
                    }
                }
                
                Label
                {
                    id: welcomeText
                    Layout.topMargin: 24
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    text: "Welcome!"
                    color: properties.colorBaseText
                    font.bold: true
                    font.pointSize: 22
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    id: loginText
                    Layout.topMargin: 8
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    Layout.preferredWidth: 500
                    wrapMode: "WordWrap"
                    text: "Your credentials are only used to authenticate yourself. " + 
                          "Everything will be stored in a secure database."
                    horizontalAlignment: Text.AlignHCenter
                    
                    color: properties.colorLightText2
                    font.pointSize: 12
                    font.family: properties.defaultFontFamily
                }
                
                RowLayout
                {
                    spacing: 28
                    Layout.topMargin: 40
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    Layout.preferredWidth: 400
                    
                    MLabeledInputBox
                    {
                        id: firstNameInput
                        Layout.preferredWidth: 186
                        Layout.preferredHeight: 50
                        Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                        headerText: "First name"
                        placeholderContent: "John"
                        placeholderColor: properties.colorLightText
                    }
                    
                    MLabeledInputBox
                    {
                        id: lastNameInput
                        Layout.preferredWidth: 186
                        Layout.preferredHeight: 50
                        Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                        headerText: "Last name"
                        placeholderContent: "Doe"
                        placeholderColor: properties.colorLightText
                    }
                }
            }    
        }
        
        Label
        {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 14
            text: "Already have an account? Login"
            font.pointSize: 10
            color: properties.colorBasePurple
            
            MouseArea
            {
                anchors.fill: parent
                onClicked: loadPage("LoginPage");
            }
        }
    }
}

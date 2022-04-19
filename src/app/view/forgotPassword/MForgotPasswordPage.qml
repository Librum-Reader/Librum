import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents

Page
{
    id: root
    
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.loginWindowBackground
    }
    
    
    Rectangle
    {
        id: backgroundRect
        width: 542
        height: 550
        anchors.centerIn: parent
        color: properties.loginContentBackground
        radius: 6
        
        ColumnLayout
        {
            id: inRectLayout
            property int inRectMargin: 71
            
            width: parent.width
            
            Image
            {
                Layout.topMargin: 50
                Layout.alignment: Qt.AlignHCenter
                source: properties.imageEyeCrossedSwift
                sourceSize.width: 160
                fillMode: Image.PreserveAspectFit
            }
            
            Label
            {
                id: forgotPasswordText
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 28
                text: "Forgot Password"
                color: properties.colorBaseText
                font.bold: true
                font.pointSize: 20
                font.family: properties.defaultFontFamily
            }
            
            Label
            {
                id: resetText
                Layout.preferredWidth: 450
                Layout.topMargin: 8
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Qt.AlignHCenter
                text: "Enter your email and we'll send you a link to reset your password"
                wrapMode: Text.WordWrap
                color: properties.colorLightText2
                lineHeight: 1.1
                font.weight: Font.Medium
                font.pointSize: 13
                font.family: properties.defaultFontFamily
            }
            
            ColumnLayout
            {
                id: inputColumn
                Layout.fillWidth: true
                Layout.leftMargin: inRectLayout.inRectMargin
                Layout.rightMargin: inRectLayout.inRectMargin
                Layout.topMargin: 12
                Layout.alignment: Qt.AlignHCenter
                spacing: 0
                
                MLabeledInputBox
                {
                    id: emailInput
                    Layout.fillWidth: true
                    placeholderContent: "kaidoe@gmail.com"
                    placeholderColor: properties.colorLightText
                    headerText: ""
                    autoFocus: true
                }
                
                Label
                {
                    id: errorText
                    Layout.topMargin: 12
                    visible: false
                    text: "We cannot find your email"
                    color: properties.colorError
                    font.pointSize: 11.75
                    font.family: properties.defaultFontFamily
                }
                
                MButton
                {
                    id: sendEmailButton
                    Layout.fillWidth: true
                    Layout.preferredHeight: 42
                    Layout.topMargin: (errorText.visible ? 45 : 60)
                    Layout.alignment: Qt.AlignHCenter
                    borderWidth: 0
                    backgroundColor: properties.colorBasePurple
                    textContent: "Send Email"
                    fontSize: 12.25
                    fontColor: properties.colorBrightText
                    fontBold: true
                    
                    onClicked: 
                    {
                        loadPage("LoginPage");
                    }
                }
                
                RowLayout
                {
                    id: backTextLayout
                    Layout.topMargin: 24
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 6
                    
                    
                    Image
                    {
                        id: backIcon
                        source: properties.iconArrowRightHallow
                        sourceSize.width: 24
                        fillMode: Image.PreserveAspectFit
                        rotation: 180
                    }
                    
                    Label
                    {
                        id: backText
                        text: "Back to Login"
                        color: properties.colorLightText2
                        font.weight: Font.Medium
                        font.pointSize: 11.5
                        font.family: properties.defaultFontFamily
                    }
                }
            }
        }
    }
}
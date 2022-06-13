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
    
    
    Pane
    {
        id: backgroundRect
        width: 542
        height: 550
        anchors.centerIn: parent
        topPadding: 50
        verticalPadding: 0
        horizontalPadding: 0
        background: Rectangle
        {
            color: properties.loginContentBackground
            radius: 6
        }
        
        
        ColumnLayout
        {
            id: inRectLayout
            property int inRectMargin: 71
            
            width: parent.width
            
            Image
            {
                Layout.alignment: Qt.AlignHCenter
                source: properties.illustrationEyeCrossedSwift
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
                font.pointSize: 19
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
                font.pointSize: 12.5
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
                }
                
                Label
                {
                    id: errorText
                    Layout.topMargin: 10
                    visible: false
                    text: "We couldn't find your email"
                    color: properties.colorError
                    font.pointSize: 11.75
                    font.family: properties.defaultFontFamily
                }
                
                Label
                {
                    id: successText
                    property string email : ""
                    
                    Layout.topMargin: 10
                    visible: false
                    text: "Email sent to " + email
                    color: "green"
                    font.pointSize: 11.75
                    font.family: properties.defaultFontFamily
                }
                
                MButton
                {
                    id: sendEmailButton
                    Layout.fillWidth: true
                    Layout.preferredHeight: 42
                    Layout.topMargin: (errorText.visible || successText.visible ? 35 : 65)
                    Layout.alignment: Qt.AlignHCenter
                    borderWidth: 0
                    backgroundColor: properties.colorBasePurple
                    text: "Send Email"
                    fontSize: 12.25
                    fontColor: properties.colorBrightText
                    fontWeight: Font.Bold
                    
                    onClicked:
                    {
                        successText.email = emailInput.text;
                        successText.visible = true;
                        emailInput.clearText();
                    }
                }
                
                
                Rectangle
                {
                    id: backNavigation
                    Layout.preferredWidth: backNavigationLayout.width + 10
                    Layout.preferredHeight: backNavigationLayout.height + 5
                    Layout.topMargin: 18
                    Layout.alignment: Qt.AlignHCenter
                    
                    
                    RowLayout
                    {
                        id: backNavigationLayout
                        anchors.centerIn: parent
                        spacing: 6
                        
                        
                        Image
                        {
                            id: backIcon
                            source: properties.iconArrowtopBlackRight
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
                    
                    MouseArea
                    {
                        width: parent.width
                        height: parent.height
                        
                        onClicked: loadPage("LoginPage");
                    }
                }
            }
        }
    }
}
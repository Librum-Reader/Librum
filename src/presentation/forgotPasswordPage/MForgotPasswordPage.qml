import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons

MFlickWrapper
{
    id: root
    contentHeight: (Window.height < layout.implicitHeight ? layout.implicitHeight + page.bottomPadding : Window.height)
    
    
    Page
    {
        id: page
        anchors.fill: parent
        bottomPadding: 50
        background: Rectangle
        {
            anchors.fill: parent
            color: Style.loginWindowBackground
        }
        
        
        Shortcut
        {
            sequence: "Ctrl+Return"
            onActivated: sendEmailButton.buttonTriggeredAction()
        }
        
        Shortcut
        {
            sequence: "Ctrl+Backspace"
            onActivated: backButton.buttonTriggeredAction()
        }
        
        
        ColumnLayout
        {
            id: layout
            anchors.centerIn: parent
            spacing: -92
            
            
            Image
            {
                id: lockIllustration
                z: 2
                Layout.alignment: Qt.AlignHCenter
                source: Icons.lockProtected
                sourceSize.width: 250
                fillMode: Image.PreserveAspectFit
            }
            
            Pane
            {
                id: backgroundRect
                Layout.preferredWidth: 542
                topPadding: 86
                bottomPadding: 28
                horizontalPadding: 0
                background: Rectangle
                {
                    color: Style.loginContentBackground
                    radius: 6
                }
                
                
                ColumnLayout
                {
                    id: inRectLayout
                    property int inRectMargin: 71
                    
                    width: parent.width
                    
                    
                    Label
                    {
                        id: forgotPasswordText
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 32
                        text: "Forgot Password"
                        color: Style.colorBaseText
                        font.bold: true
                        font.pointSize: 19
                        font.family: Style.defaultFontFamily
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
                        color: Style.colorLightText2
                        lineHeight: 1.1
                        font.weight: Font.Medium
                        font.pointSize: 12.5
                        font.family: Style.defaultFontFamily
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
                            placeholderColor: Style.colorLightText
                            headerText: ""
                        }
                        
                        Label
                        {
                            id: errorText
                            Layout.topMargin: 10
                            visible: false
                            text: "We couldn't find your email"
                            color: Style.colorError
                            font.pointSize: 11.75
                            font.family: Style.defaultFontFamily
                        }
                        
                        Label
                        {
                            id: successText
                            property string email : ""
                            
                            Layout.topMargin: 10
                            visible: false
                            text: "Email sent to: " + email
                            color: "green"
                            font.pointSize: 11.75
                            font.family: Style.defaultFontFamily
                        }
                        
                        MButton
                        {
                            id: sendEmailButton
                            Layout.fillWidth: true
                            Layout.preferredHeight: 42
                            Layout.topMargin: (errorText.visible || successText.visible ? 32 : 56)
                            Layout.alignment: Qt.AlignHCenter
                            borderWidth: 0
                            backgroundColor: Style.colorBasePurple
                            text: "Send Email"
                            fontSize: 12.25
                            fontColor: Style.colorBrightText
                            fontWeight: Font.Bold
                            
                            onClicked: buttonTriggeredAction()
                            
                            
                            function buttonTriggeredAction()
                            {
                                successText.email = emailInput.text;
                                successText.visible = true;
                                emailInput.clearText();
                            }
                        }
                        
                        
                        MButton
                        {
                            id: backButton
                            Layout.preferredWidth: 145
                            Layout.preferredHeight: 42
                            Layout.alignment: Qt.AlignHCenter
                            Layout.topMargin: 18
                            centerContentHorizontally: false
                            borderWidth: 0
                            backgroundColor: "transparent"
                            opacityOnPressed: 0.7
                            text: "Back to Login"
                            fontSize: 12.20
                            fontWeight: Font.Medium
                            fontColor: Style.colorLightText2
                            imagePath: Icons.arrowtopBlackRight
                            imageSize: 28
                            imageRotation: 180
                            imageSpacing: 4
                            
                            onClicked: buttonTriggeredAction()
                            
                            function buttonTriggeredAction()
                            {
                                loadPage(loginPage)
                            }
                        }
                    }
                }
            }
        }
    }
}
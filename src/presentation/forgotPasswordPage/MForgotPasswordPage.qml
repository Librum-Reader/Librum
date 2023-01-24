import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0

MFlickWrapper
{
    id: root
    contentHeight: internal.calculatePageContentHeight()
    
    
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
            onActivated: internal.sendPasswordResetEmail()
        }
        
        Shortcut
        {
            sequence: "Ctrl+Backspace"
            onActivated: internal.backToLoginPage()
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
                id: background
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
                    id: backgroundLayout
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
                    }
                    
                    Label
                    {
                        id: resetPasswordText
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
                    }
                    
                    ColumnLayout
                    {
                        id: inputColumn
                        Layout.fillWidth: true
                        Layout.leftMargin: internal.inWindowPadding
                        Layout.rightMargin: internal.inWindowPadding
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
                        }
                        
                        Label
                        {
                            id: successText
                            Layout.topMargin: 10
                            visible: false
                            text: "Email sent to: " + "placeholder@librum.com"
                            color: Style.colorMediumGreen
                            font.pointSize: 11.75
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
                            
                            onClicked: internal.sendPasswordResetEmail()
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
                            
                            onClicked: internal.backToLoginPage()
                        }
                    }
                }
            }
        }
    }
    
    QtObject
    {
        id: internal
        property int inWindowPadding: 71
        
        function sendPasswordResetEmail()
        {
            successText.email = emailInput.text;
            successText.visible = true;
            emailInput.clearText();
        }
        
        function backToLoginPage()
        {
            loadPage(loginPage);
        }
        
        function calculatePageContentHeight()
        {
            return Window.height < layout.implicitHeight ? 
                        layout.implicitHeight + page.bottomPadding : Window.height
        }
    }
}
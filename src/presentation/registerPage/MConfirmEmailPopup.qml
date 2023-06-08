import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0


Popup
{
    id: root
    signal decisionMade
    
    implicitWidth: 646
    implicitHeight: layout.height
    padding: 0
    closePolicy: Popup.NoAutoClose
    background: Rectangle { color: "transparent"; radius: 4 }
    modal: true
    Overlay.modal: Rectangle { color: Style.colorPopupDim; opacity: 1 }
    
    
    MFlickWrapper
    {
        id: flickWrapper
        anchors.fill: parent
        contentHeight: layout.height
        
        ColumnLayout
        {
            id: layout
            width: parent.width
            spacing: -92
            
            
            Image
            {
                id: attentionIllustration
                z: 2
                Layout.alignment: Qt.AlignHCenter
                Layout.rightMargin: 10
                source: Icons.attentionPurple
                sourceSize.width: 250
                fillMode: Image.PreserveAspectFit
            }
            
            Pane
            {
                id: backgroundRect
                Layout.fillWidth: true
                topPadding: 86
                horizontalPadding: 62
                bottomPadding: 62
                background: Rectangle { color: Style.colorPopupBackground; radius: 6 }
                
                
                ColumnLayout
                {
                    id: inRectLayout
                    width: parent.width
                    spacing: 22
                    
                    
                    Label
                    {
                        id: whoopsText
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 18
                        text: "Confirm Your Email"
                        color: Style.colorTitle
                        font.weight: Font.Medium
                        font.pointSize: 42
                    }
                    
                    Label
                    {
                        id: explenation
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                        text: "You're are almost ready to go!\nConfirm your email by clicking the link we sent you."
                        horizontalAlignment: Qt.AlignHCenter
                        color: Style.colorLightText
                        font.weight: Font.Medium
                        font.pointSize: 15
                    }
                    
                    RowLayout
                    {
                        id: buttonRow
                        Layout.preferredWidth: parent.width
                        Layout.preferredHeight: doneButton.height
                        Layout.topMargin: 24
                        spacing: 42
                        
                        
                        MButton
                        {
                            id: doneButton
                            Layout.preferredWidth: 120
                            Layout.preferredHeight: 40
                            Layout.alignment: Qt.AlignBottom | Qt.AlignRight
                            borderWidth: activeFocus ? 0 : 1
                            backgroundColor: activeFocus ? Style.colorBasePurple : "transparent"
                            opacityOnPressed: 0.7
                            text: "Done!"
                            fontSize: 12.75
                            fontWeight: Font.Bold
                            textColor: activeFocus ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                            
                            onClicked: internal.done()
                            
                            KeyNavigation.tab: resendButton
                            KeyNavigation.right: resendButton
                            Keys.onReturnPressed: internal.done()
                        }
                        
                        MButton
                        {
                            id: resendButton
                            Layout.preferredWidth: 120
                            Layout.preferredHeight: 40
                            Layout.alignment: Qt.AlignBottom | Qt.AlignLeft
                            borderWidth: focus ? 0 : 1
                            backgroundColor: focus ? Style.colorBasePurple : "transparent"
                            opacityOnPressed: 0.7
                            text: "Resend"
                            fontSize: 12.75
                            fontWeight: Font.Bold
                            textColor: focus ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                            
                            onClicked: internal.resend()
                            
                            KeyNavigation.tab: doneButton
                            KeyNavigation.left: doneButton
                            Keys.onReturnPressed: internal.resend()
                        }
                    }
                }
            }
        }
    }
    
    QtObject
    {
        id: internal
        
        function done()
        {
            decisionMade();
            root.close();
        }
        
        function resend()
        {
            console.log("Resend");
        }
    }
    
    function giveFocus()
    {
        doneButton.forceActiveFocus();
    }
}
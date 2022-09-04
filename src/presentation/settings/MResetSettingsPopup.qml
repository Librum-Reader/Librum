import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0


Popup
{
    id: root
    property var resetSettings
    signal decisionMade
    signal resetChoosed
    signal keepChoosed
    
    implicitWidth: 646
    implicitHeight: layout.height
    padding: 0
    background: Rectangle
    {
        color: "transparent"
        radius: 4
    }
    modal: true
    Overlay.modal: Rectangle
    {
        color: "#aa32324D"
        opacity: 1
    }
    
    onOpened: keepButton.forceActiveFocus()
    
    
    MFlickWrapper
    {
        width: parent.width
        height: parent.height
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
                horizontalPadding: 82
                bottomPadding: 66
                background: Rectangle
                {
                    color: Style.colorBackground
                    radius: 6
                }
                
                
                ColumnLayout
                {
                    id: inRectLayout
                    width: parent.width
                    spacing: 22
                    
                    
                    Label
                    {
                        id: whoops
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 18
                        text: "Reset settings?"
                        color: Style.colorBaseTitle
                        font.weight: Font.Medium
                        font.pointSize: 42
                        font.family: Style.defaultFontFamily
                    }
                    
                    Label
                    {
                        id: explenation
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                        text: "Resetting your settings is a permanent action, there will be no way to restore them!"
                        horizontalAlignment: Qt.AlignHCenter
                        color: Style.colorLightText3
                        font.weight: Font.Medium
                        font.pointSize: 15
                        font.family: Style.defaultFontFamily
                    }
                    
                    RowLayout
                    {
                        id: buttonRow
                        Layout.preferredWidth: parent.width
                        Layout.preferredHeight: keepButton.height
                        Layout.topMargin: 36
                        spacing: 42
                        
                        MButton
                        {
                            id: keepButton
                            Layout.preferredWidth: 180
                            Layout.preferredHeight: 40
                            Layout.alignment: Qt.AlignBottom | Qt.AlignRight
                            borderWidth: activeFocus ? 0 : 1
                            borderColor: Style.colorLightBorder2
                            backgroundColor: activeFocus ? Style.colorBasePurple : "transparent"
                            opacityOnPressed: 0.7
                            text: "No, Keep"
                            fontSize: 12.75
                            fontWeight: Font.Bold
                            fontColor: activeFocus ? Style.colorBackground : Style.colorBaseTitle
                            
                            onClicked: buttonAction()
                            
                            KeyNavigation.tab: resetButton
                            KeyNavigation.right: resetButton
                            Keys.onReturnPressed: buttonAction()
                            
                            function buttonAction()
                            {
                                root.keepChoosed();
                                root.decisionMade();
                            }
                        }
                        
                        MButton
                        {
                            id: resetButton
                            Layout.preferredWidth: 180
                            Layout.preferredHeight: 40
                            Layout.alignment: Qt.AlignBottom | Qt.AlignLeft
                            borderWidth: activeFocus ? 0 : 1
                            borderColor: Style.colorLightBorder2
                            backgroundColor: activeFocus ? Style.colorBaseRed : "transparent"
                            opacityOnPressed: 0.7
                            text: "Yes, Reset"
                            fontSize: 12.75
                            fontWeight: Font.Bold
                            fontColor: activeFocus ? Style.colorBackground : Style.colorBaseTitle
                            
                            onClicked: buttonAction()
                            
                            KeyNavigation.tab: keepButton
                            KeyNavigation.left: keepButton
                            Keys.onReturnPressed: buttonAction()
                            
                            function buttonAction()
                            {
                                root.resetChoosed();
                                root.decisionMade();
                            }
                        }
                    }
                }
            }
        }
    }
    
    function giveFocus()
    {
        acceptButton.forceActiveFocus();
    }
}
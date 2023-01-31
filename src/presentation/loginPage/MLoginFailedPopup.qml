import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0


Popup
{
    id: root
    implicitWidth: 612
    implicitHeight: layout.height
    padding: 0
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
    background: Rectangle { color: "transparent"; radius: 4 }
    modal: true
    Overlay.modal: Rectangle { color: "#aa32324D"; opacity: 1 }
    
    
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
                id: container
                Layout.fillWidth: true
                topPadding: 86
                horizontalPadding: 62
                bottomPadding: 52
                background: Rectangle { color: Style.colorBackground; radius: 6 }
                
                
                ColumnLayout
                {
                    id: inContainerLayout
                    width: parent.width
                    spacing: 0
                    
                    
                    Label
                    {
                        id: header
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 18
                        text: "We're Sorry"
                        color: Style.colorBaseTitle
                        font.weight: Font.Medium
                        font.pointSize: 42
                    }
                    
                    Label
                    {
                        id: explenationText
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        Layout.topMargin: 22
                        wrapMode: Text.WordWrap
                        text: "Logging you in failed, please try again later."
                        horizontalAlignment: Qt.AlignHCenter
                        color: Style.colorLightText
                        font.weight: Font.Medium
                        font.pointSize: 15
                    }
                    
                    RowLayout
                    {
                        id: buttonRow
                        Layout.preferredWidth: parent.width
                        Layout.preferredHeight: okButton.height
                        Layout.topMargin: 62
                        spacing: 42
                        
                        MButton
                        {
                            id: okButton
                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 40
                            Layout.alignment: Qt.AlignBottom | Qt.AlignRight
                            borderWidth: activeFocus ? 0 : 1
                            borderColor: Style.colorLightBorder2
                            backgroundColor: activeFocus ? Style.colorBasePurple : "transparent"
                            opacityOnPressed: 0.7
                            text: "Ok"
                            fontSize: 12.75
                            fontWeight: Font.Bold
                            fontColor: activeFocus ? Style.colorBackground : Style.colorBaseTitle
                            
                            onClicked: root.close();
                            
                            Keys.onPressed:
                                (event) =>
                                {
                                    if(event.key === Qt.Key_Right || event.key === Qt.Key_Tab)
                                    {
                                        declineButton.forceActiveFocus();
                                    }
                                    else if(event.key === Qt.Key_Return)
                                    {
                                        root.close();
                                    }
                                }
                        }
                        
                        MButton
                        {
                            id: reportButton
                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 40
                            Layout.alignment: Qt.AlignBottom | Qt.AlignLeft
                            borderWidth: focus ? 0 : 1
                            borderColor: Style.colorLightBorder2
                            backgroundColor: focus ? Style.colorBasePurple : "transparent"
                            opacityOnPressed: 0.7
                            text: "Report"
                            fontSize: 12.75
                            fontWeight: Font.Bold
                            fontColor: focus ? Style.colorBackground : Style.colorBaseTitle
                            
                            onClicked: internal.report()
                            
                            Keys.onPressed:
                                (event) =>
                                {
                                    if(event.key === Qt.Key_Left || event.key === Qt.Key_Tab)
                                    {
                                        acceptButton.forceActiveFocus();
                                    }
                                    else if(event.key === Qt.Key_Return)
                                    {
                                        internal.report();
                                    }
                                }
                        }
                    }
                }
            }
        }
    }
    
    QtObject
    {
        id: internal
        
        
        function report()
        {
            root.close();
        }
    }
    
    
    function giveFocus()
    {
        okButton.forceActiveFocus();
    }
}
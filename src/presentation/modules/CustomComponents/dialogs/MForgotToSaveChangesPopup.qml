import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons


Popup
{
    id: root
    implicitWidth: 680
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
    
    ColumnLayout
    {
        id: mainLayout
        width: parent.width
        spacing: -92
        
        
        Image
        {
            id: attentionIllustration
            z: 2
            Layout.alignment: Qt.AlignHCenter
            source: Icons.attention
            sourceSize.width: 250
            fillMode: Image.PreserveAspectFit
        }

        Pane
        {
            id: backgroundRect
            Layout.fillWidth: true
            topPadding: 86
            horizontalPadding: 72
            bottomPadding: 48
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
                    text: "Whoops"
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
                    text: "It looks like you forgot to save your changes, are you sure that you dont want to save them?"
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
                    Layout.preferredHeight: acceptButton.height
                    Layout.topMargin: 72
                    spacing: 0
                    
                    MButton
                    {
                        id: acceptButton
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 40
                        Layout.alignment: Qt.AlignBottom | Qt.AlignLeft
                        borderWidth: activeFocus ? 0 : 1
                        borderColor: Style.colorLightBorder2
                        backgroundColor: activeFocus ? Style.colorBasePurple : "transparent"
                        text: "Save"
                        fontSize: 12.75
                        fontWeight: Font.Bold
                        fontColor: activeFocus ? Style.colorBackground : Style.colorBaseTitle
                        
                        onClicked: root.close()
                                                
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
                        id: declineButton
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 40
                        Layout.alignment: Qt.AlignBottom | Qt.AlignRight
                        borderWidth: focus ? 0 : 1
                        borderColor: Style.colorLightBorder2
                        backgroundColor: focus ? Style.colorBasePurple : "transparent"
                        text: "Don't save"
                        fontSize: 12.75
                        fontWeight: Font.Bold
                        fontColor: focus ? Style.colorBackground : Style.colorBaseTitle
                        
                        onClicked: root.close()
                        
                        Keys.onPressed:
                            (event) =>
                            {
                                if(event.key === Qt.Key_Left || event.key === Qt.Key_Tab)
                                {
                                    acceptButton.forceActiveFocus();
                                }
                                else if(event.key === Qt.Key_Return)
                                {
                                    root.close();
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
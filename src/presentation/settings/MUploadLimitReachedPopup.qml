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
    signal resetChoosed
    signal keepChoosed
    
    implicitWidth: 666
    implicitHeight: layout.height
    padding: 0
    modal: true
    background: Rectangle { color: "transparent"; radius: 4 }
    Overlay.modal: Rectangle { color: Style.colorPopupDim; opacity: 1 }
    
    onOpened: okButton.forceActiveFocus()
    
    
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
                horizontalPadding: 58
                bottomPadding: 66
                background: Rectangle { color: Style.colorPopupBackground; radius: 6 }
                
                
                ColumnLayout
                {
                    id: contentLayout
                    width: parent.width
                    spacing: 22
                    
                    
                    Label
                    {
                        id: whoopsText
                        Layout.topMargin: 18
                        Layout.alignment: Qt.AlignHCenter
                        text: "Upload Limit Reached"
                        color: Style.colorTitle
                        font.weight: Font.Medium
                        font.pointSize: 42
                    }
                    
                    Label
                    {
                        id: permanentActionText
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                        text: "You have reached your 2GB upload limit!\nDelete unused books to free up some space"
                        horizontalAlignment: Qt.AlignHCenter
                        color: Style.colorLightText
                        font.weight: Font.Medium
                        font.pointSize: 15
                    }
                    
                    MButton
                    {
                        id: okButton
                        Layout.preferredWidth: 180
                        Layout.preferredHeight: 40
                        Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
                        Layout.topMargin: 36
                        borderWidth: 1
                        backgroundColor: Style.colorBasePurple
                        opacityOnPressed: 0.7
                        text: "Ok"
                        fontSize: 12.75
                        fontWeight: Font.Bold
                        textColor: Style.colorFocusedButtonText
                        
                        onClicked: root.close()
                        Keys.onReturnPressed: root.close()
                    }
                }
            }
        }
    }
    
    function giveFocus()
    {
        okButton.forceActiveFocus();
    }
}
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
    signal closeChoosed
    signal goToLibraryChoosed
    
    implicitWidth: 666
    implicitHeight: layout.height
    padding: 0
    modal: true
    background: Rectangle { color: "transparent"; radius: 4 }
    Overlay.modal: Rectangle { color: Style.colorPopupDim; opacity: 1 }
    
    onOpened: closeButton.forceActiveFocus()
    
    
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
                    
                    RowLayout
                    {
                        id: buttonRow
                        Layout.preferredWidth: parent.width
                        Layout.preferredHeight: closeButton.height
                        Layout.topMargin: 36
                        spacing: 42
                        
                        MButton
                        {
                            id: closeButton
                            Layout.preferredWidth: 180
                            Layout.preferredHeight: 40
                            Layout.alignment: Qt.AlignBottom | Qt.AlignRight
                            borderWidth: activeFocus ? 0 : 1
                            backgroundColor: activeFocus ? Style.colorBasePurple : "transparent"
                            opacityOnPressed: 0.7
                            text: "Close"
                            fontSize: 12.75
                            fontWeight: Font.Bold
                            textColor: activeFocus ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                            
                            onClicked: internal.close()
                            
                            KeyNavigation.tab: goToLibraryButton
                            KeyNavigation.right: goToLibraryButton
                            Keys.onReturnPressed: internal.close()
                        }
                        
                        MButton
                        {
                            id: goToLibraryButton
                            Layout.preferredWidth: 180
                            Layout.preferredHeight: 40
                            Layout.alignment: Qt.AlignBottom
                            borderWidth: activeFocus ? 0 : 1
                            backgroundColor: activeFocus ? Style.colorBasePurple : "transparent"
                            opacityOnPressed: 0.7
                            text: "Go To Library"
                            fontSize: 12.75
                            fontWeight: Font.Bold
                            textColor: activeFocus ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                            
                            onClicked: internal.goToLibrary()
                            
                            KeyNavigation.tab: closeButton
                            KeyNavigation.left: closeButton
                            Keys.onReturnPressed: internal.goToLibrary()
                        }
                    }
                }
            }
        }
    }
    
    QtObject
    {
        id: internal
        
        function close()
        {
            root.closeChoosed();
            root.decisionMade();
        }
        
        function goToLibrary()
        {
            root.goToLibraryChoosed();
            root.decisionMade();
        }
    }
    
    function giveFocus()
    {
        closeButton.forceActiveFocus();
    }
}
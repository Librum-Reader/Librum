import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.controllers 1.0
import Librum.globals 1.0


Popup
{
    id: root
    property var saveMethod
    property var dontSaveMethod
    signal decisionMade
    signal keepChoosed
    signal deleteChoosed
    
    implicitWidth: 646
    implicitHeight: layout.height
    padding: 0
    background: Rectangle { color: "transparent"; radius: 4 }
    modal: true
    Overlay.modal: Rectangle { color: "#aa32324D"; opacity: 1 }
    
    onOpened: keepButton.forceActiveFocus()
    
    
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
                horizontalPadding: 82
                bottomPadding: 66
                background: Rectangle { color: Style.colorBackground; radius: 6 }
                
                
                ColumnLayout
                {
                    id: contentLayout
                    width: parent.width
                    spacing: 22
                    
                    
                    Label
                    {
                        id: whoopsText
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 18
                        text: "Delete book?"
                        color: Style.colorBaseTitle
                        font.weight: Font.Medium
                        font.pointSize: 42
                    }
                    
                    Label
                    {
                        id: explenation
                        Layout.alignment: Qt.AlignHCenter
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                        text: "Deleting a book is a permanent action, no one will be able to restore it afterwards!"
                        horizontalAlignment: Qt.AlignHCenter
                        color: Style.colorLightText
                        font.weight: Font.Medium
                        font.pointSize: 15
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
                            text: "No, Keep Book"
                            fontSize: 12.75
                            fontWeight: Font.Bold
                            xxxUniqueLibrumProperty: activeFocus ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                            
                            onClicked: internal.keepBook()
                            
                            KeyNavigation.tab: deleteButton
                            KeyNavigation.right: deleteButton
                            Keys.onReturnPressed: internal.keepBook()
                        }
                        
                        MButton
                        {
                            id: deleteButton
                            Layout.preferredWidth: 180
                            Layout.preferredHeight: 40
                            Layout.alignment: Qt.AlignBottom | Qt.AlignLeft
                            borderWidth: activeFocus ? 0 : 1
                            borderColor: Style.colorLightBorder2
                            backgroundColor: activeFocus ? Style.colorBaseRed : "transparent"
                            opacityOnPressed: 0.7
                            text: "Yes, Delete Book"
                            fontSize: 12.75
                            fontWeight: Font.Bold
                            xxxUniqueLibrumProperty: activeFocus ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                            
                            onClicked: internal.deleteBook()
                            
                            KeyNavigation.tab: keepButton
                            KeyNavigation.left: keepButton
                            Keys.onReturnPressed: internal.deleteBook()
                        }
                    }
                }
            }
        }
    }
    
    QtObject
    {
        id: internal
        
        function keepBook()
        {
            root.keepChoosed();
            root.close();
        }
        
        function deleteBook()
        {
            root.deleteChoosed();
            BookController.deleteBook(Globals.selectedBook.uuid);
            root.close();
        }
    }
    
    function giveFocus()
    {
        acceptButton.forceActiveFocus();
    }
}
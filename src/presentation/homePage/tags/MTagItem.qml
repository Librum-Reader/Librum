import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.globals 1.0


Item
{
    id: root
    required property int index
    property bool selected: false
    readonly property alias renameable: container.renameable
    signal removeTag(int index)
    signal renamedTag(int index, string text)
    signal startedRenaming(string prevText)
    
    implicitHeight: 38
    implicitWidth: 400
    
    
    Pane
    {
        id: container
        property bool renameable: false
        
        anchors.fill: parent
        padding: 0
        background: Rectangle
        {
            color: root.selected ? Style.colorSidebarMark :
                                   mouseArea.containsMouse || ListView.currentIndex === index
                                   ? Style.colorLightHighlight : Style.colorButtonBackground
            radius: 4
        }
        
        
        RowLayout
        {
            id: layout
            anchors.fill: parent
            spacing: 0
            
            
            TextField
            {
                id: content
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.leftMargin: 12
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                leftPadding: 0
                text: Globals.bookTags[root.index].name
                font.weight: root.selected ? Font.Medium : Font.Normal
                font.pointSize: 12
                color: Style.colorText
                readOnly: true
                background: Rectangle
                {
                    border.width: 0
                    color: "transparent"
                }
                
                onEditingFinished:
                {
                    root.stopRenaming();
                }
                
                onTextChanged: 
                {
                    // Prevent content being scrolled to the right by default
                    if(!content.activeFocus)
                        content.cursorPosition = 0;
                }
            }
            
            MButton
            {
                id: editButton
                Layout.preferredHeight: 28
                Layout.preferredWidth: 30
                Layout.rightMargin: 2
                Layout.alignment: Qt.AlignVCenter
                backgroundColor: "transparent"
                opacityOnPressed: 0.7
                borderColor: currentlyPressed ? Style.colorButtonBorder : "transparent"
                radius: 4
                imagePath: currentlyPressed ? Icons.editPurple : Icons.editLightGray
                imageSize: 17
                
                onClicked: root.startRenaming();
            }
            
            MButton
            {
                id: closeButton
                Layout.preferredHeight: 26
                Layout.preferredWidth: 26
                Layout.rightMargin: 16
                Layout.alignment: Qt.AlignVCenter
                backgroundColor: "transparent"
                opacityOnPressed: 0.7
                borderColor: currentlyPressed ? Style.colorButtonBorder : "transparent"
                radius: 4
                imagePath: currentlyPressed ? Icons.closePurple : Icons.closeGray
                imageSize: 12
                
                onClicked: root.removeTag(root.index)
            }
        }
    }
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
        
        onPressed: (mouse) => mouse.accepted = false
    }
    
    function startRenaming()
    {
        content.readOnly = false;
        content.selectAll();
        content.forceActiveFocus();
        container.renameable = true;
        
        root.startedRenaming(content.text);
    }
    
    function stopRenaming(saveText = true)
    {
        content.readOnly = true;
        content.select(0,0);
        content.deselect();
        container.renameable = false;
        
        root.renamedTag(root.index, content.text);
        
        // Make sure its bound to the model text after editing
        content.text = Qt.binding(function() { return Globals.bookTags[root.index].name });
    }
}
import QtQuick
import QtQuick.Controls
import CustomComponents
import QtQuick.Layouts
import Librum.style


Item
{
    id: root
    property ListView containingListview
    property int index
    property bool selected: false
    property double fontSize: 10.75
    property color fontColor: Style.colorLightText
    property int checkBoxSize: 18
    property int checkBoxImageSize: 9
    property int padding: 8
    property bool checkBoxStyle: true
    readonly property alias renameable: container.renameable
    signal clicked(var mouse, int index)
    signal rightClicked(var mouse, int index)
    signal hovered(int index)
    signal renamed(int index, string text)
    
    implicitWidth: 137
    implicitHeight: 36
    
    
    Pane
    {
        id: container
        property bool renameable: false
        
        anchors.fill: parent
        verticalPadding: 0
        horizontalPadding: root.padding
        background: Rectangle
        {
            color: root.selected ? Style.colorLightHighlight : 
                                   mouseArea.containsMouse || ListView.currentIndex === index
                                   ? Style.colorHoverHighlight : "transparent"
            radius: 4
            antialiasing: true
        }
        
        RowLayout
        {
            id: layout
            anchors.fill: parent
            spacing: 9
            
            
            MCheckBox
            {
                id: checkBox
                visible: root.checkBoxStyle
                Layout.preferredWidth: root.checkBoxSize
                Layout.preferredHeight: root.checkBoxSize
                checked: root.selected
                imageSize: root.checkBoxImageSize
            }
            
            TextField
            {
                id: content
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                padding: 0
                leftPadding: 0
                bottomPadding: 1
                readOnly: true
                text: root.getContent()
                color: root.checkBoxStyle == false && root.selected ? Style.colorBasePurple : root.fontColor
                font.pointSize: root.fontSize
                font.weight: root.selected ? Font.DemiBold : Font.Medium
                background: Rectangle { border.width: 0; color: "transparent" }
                
                onAccepted: stopRenaming()
                onTextChanged: 
                {
                    // Move cursor to the start of the text
                    if(!content.activeFocus)
                        content.cursorPosition = 0;
                }
            }
        }
    }
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        
        onClicked: (mouse) =>
                   {
                       if(mouse.button === Qt.LeftButton)
                       {
                           root.clicked(mouse, model.index);
                       }
                       else if(mouse.button === Qt.RightButton)
                       {
                           root.rightClicked(mouse, model.index);
                       }
                   }
    }
    
    
    function getRole()
    {
        return model.role;
    }
    
    function getContent()
    {
        return model.text;
    }
    
    function startRenaming()
    {
        content.readOnly = false;
        content.forceActiveFocus();
        content.selectAll();
        
        container.renameable = true;
    }
    
    function stopRenaming(saveText = true)
    {
        content.readOnly = true;
        content.select(0,0);
        root.forceActiveFocus();  // Remove focus from text input
        
        container.renameable = false;
        
        if(saveText)
            root.renamed(model.index, content.text);
        
        // Make sure that the binding is kept
        content.text = Qt.binding(function() { return root.getContent() });
    }
}
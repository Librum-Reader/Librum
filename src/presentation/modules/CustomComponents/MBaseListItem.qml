import QtQuick 2.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import QtQuick.Layouts 1.15
import Librum.style 1.0


Item
{
    id: root
    property ListView containingListview
    property int index
    property bool selected: false
    property double fontSize: 10.75
    property color fontColor: Style.colorLightText3
    property int checkBoxSize: 18
    property int checkBoxImageSize: 9
    property int padding: 8
    property bool checkBoxStyle: true
    property bool renameable: false
    signal clicked(var mouse, int index)
    signal rightClicked(var mouse, int index)
    signal hovered(int index)
    signal renamed(int index, string text)
    
    function getContent()
    {
        return model.text;
    }
    
    onRenameableChanged:
    {
        if(renameable)
        {
            content.readOnly = false;
            content.forceActiveFocus();
            content.selectAll()
        }
        else
        {
            content.readOnly = true;
            root.forceActiveFocus();
        }
    }
    
    implicitWidth: 137
    implicitHeight: 36
    
    
    Pane
    {
        id: container
        anchors.fill: parent
        verticalPadding: 0
        horizontalPadding: root.padding
        background: Rectangle
        {
            color: root.selected ? Style.colorSidebarMark : 
                                   mouseArea.containsMouse || ListView.currentIndex === index
                                   ? Style.colorLightGray : Style.colorBackground
            radius: 4
            antialiasing: true
        }
        
        RowLayout
        {
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
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: true
                padding: 0
                readOnly: true
                leftPadding: 0
                bottomPadding: 1
                color: root.checkBoxStyle == false && root.selected ? Style.colorBasePurple : root.fontColor
                text: root.getContent()
                font.pointSize: root.fontSize
                font.weight: root.selected ? Font.DemiBold : Font.Medium
                background: Rectangle
                {
                    border.width: 0
                    color: "transparent"
                }
                
                onAccepted:
                {
                    root.renamed(model.index, content.text);
                    root.renameable = false;
                    
                    // Make sure its bound to the model text after editing
                    content.text = Qt.binding(function() { return root.getContent() });
                }
            }
        }
    }
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        
        onClicked:
            (mouse) =>
            {
                if(mouse.button === Qt.LeftButton)
                    root.clicked(mouse, model.index);
                else if(mouse.button === Qt.RightButton)
                    root.rightClicked(mouse, model.index);
            }
    }
    
    function getRole()
    {
        return model.role;
    }
}
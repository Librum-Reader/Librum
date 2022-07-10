import QtQuick
import QtQuick.Controls
import QtQuick.Layouts 1.0
import CustomComponents
import Librum.style


Item
{
    id: root
    required property string content
    required property int index
    property ListView container
    property bool selected: false
    property int radius
    
    property int fontSize
    property color fontColor
    property int fontWeight
    property string fontFamily
    signal itemSelected(int index)
    signal itemDeselected(int index)
    
    implicitWidth: container.width
    implicitHeight: 30
    
    Keys.onReturnPressed:
    {
        if(!selected)
        {
            root.selected = true;
            root.itemSelected(root.index);
        }
        
        root.selected = false;
        root.itemDeselected(root.index);
    }
    
    
    Rectangle
    {
        anchors.fill: parent
        radius: root.radius
        color: root.selected ? Style.colorSidebarMark : mouseArea.containsMouse ? Style.colorLightGray : "transparent"
        
        RowLayout
        {
            anchors.fill: parent
            anchors.leftMargin: 8
            spacing: 9
            
            MCheckBox
            {
                id: checkBox
                Layout.preferredWidth: 18
                Layout.preferredHeight: 18
                checked: root.selected
                imageSize: 8
            }
            
            
            Label
            {
                id: label
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                color: root.fontColor
                text: root.content
                font.pointSize: root.fontSize
                font.family: root.fontFamily
                font.weight: root.selected ? Font.Medium : root.fontWeight
                elide: Text.ElideRight
            }
        }
    }
    
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        
        onClicked:
        {
            if(!selected)
            {
                container.currentIndex = index;
                root.selected = true;
                root.itemSelected(root.index);
                return;
            }
            
            root.selected = false;
            root.itemDeselected(root.index);
        }
    }
}
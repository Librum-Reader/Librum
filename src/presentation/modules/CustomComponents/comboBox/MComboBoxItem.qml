import QtQuick
import QtQuick.Controls
import Librum.style


Item
{
    id: root
    required property string content
    property ListView container
    property bool selected: container.currentItem === this
    required property int index
    property int radius
    
    property int fontSize
    property color fontColor
    property int fontWeight
    property string fontFamily
    signal close
    
    implicitWidth: container.width
    implicitHeight: 28
    
    Keys.onReturnPressed: root.close()
    
    
    Rectangle
    {
        anchors.fill: parent
        radius: root.radius
        color: root.selected ? Style.colorSidebarMark : mouseArea.containsMouse ? Style.colorLightGray : "transparent"
        
        Label
        {
            id: label
            anchors.fill: parent
            anchors.leftMargin: 6
            verticalAlignment: Text.AlignVCenter
            color: root.selected ? Style.colorBasePurple : root.fontColor
            text: root.content
            font.pointSize: root.fontSize
            font.family: root.fontFamily
            font.weight: root.selected ? Font.Medium : root.fontWeight
            elide: Text.ElideRight
        }
    }
    
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        
        onClicked:
        {
            container.currentIndex = index;
            root.close();
        }
    }
}
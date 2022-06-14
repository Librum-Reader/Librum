import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style


Item
{
    id: root
    property int pageNumber : 1
    property bool selected : false
    property int selectedFontSize : 12
    property int normalFontSize : 11
    signal clicked
    
    implicitWidth: 36
    implicitHeight: 36
    
    
    Rectangle
    {
        id: contentRect
        anchors.fill: parent
        color: (root.selected ? Style.colorBackground : "transparent")
        border.color: (root.selected ? Style.colorLightBorder : "transparent")
        radius: 5
        
        
        Label
        {
            anchors.centerIn: parent
            text: root.pageNumber.toString()
            font.pointSize: (root.selected ? root.selectedFontSize : root.normalFontSize)
            font.family: Style.defaultFontFamily
            font.bold: root.selected
            color: (root.selected ? Style.colorBasePurple : Style.colorBaseText)
        }
    }
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        
        onClicked: root.clicked()
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}
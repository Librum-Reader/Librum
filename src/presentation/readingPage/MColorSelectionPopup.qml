import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import Librum.icons

Popup
{
    id: root
    property string highlight: ""
    signal colorSelected(color color)
    
    width: selectionOptionsLayout.width
    height: 32
    padding: 0
    background: Rectangle
    {
        color: Style.colorControlBackground
        radius: 4
    }
    
    Image
    {
        id: triangleDecoration
        x: parent.width / 2 - implicitWidth / 2
        y: parent.y + parent.height - 1
        source: Icons.popupDroplet
        rotation: 180
    }
    
    RowLayout
    {
        id: selectionOptionsLayout
        height: parent.height
        spacing: 0
        
        component ColorItem: Rectangle
        {
            id: colorItem
            property color probeColor
            
            Layout.fillHeight: true
            Layout.preferredWidth: 36
            color: "transparent"
            
            Rectangle
            {
                width: parent.width / 2.8
                height: width
                anchors.centerIn: parent
                radius: width
                color: colorItem.probeColor
            }
            
            MouseArea
            {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                
                onClicked: root.colorSelected(colorItem.probeColor)
            }
        }
        
        component Separator: Rectangle
        {
            Layout.fillHeight: true
            Layout.preferredWidth: 2
            color: Style.colorSeparator
        }
        
        ColorItem
        {
            probeColor: "#F9D36B"
        }
        
        Separator {}
        
        ColorItem
        {
            probeColor: "#7CC767"
        }
        
        Separator {}
        
        ColorItem
        {
            probeColor: "#69AFF2"
        }
        
        Separator {}
        
        ColorItem
        {
            probeColor: "#F95C87"
        }
        
        Separator {}
        
        ColorItem
        {
            probeColor: "#C786D7"
        }
    }
}
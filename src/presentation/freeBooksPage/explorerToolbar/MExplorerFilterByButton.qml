import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import Librum.icons


Item
{
    id: root
    property bool opened: false
    signal clicked()
    
    implicitWidth: 100
    implicitHeight: 36
    
    
    Pane
    {
        id: container
        anchors.fill: parent
        padding: 0
        background: Rectangle
        {
            color: Style.colorControlBackground
            border.width: 1
            border.color: Style.colorContainerBorder
            radius: 5
        }
        
        
        RowLayout
        {
            anchors.centerIn: parent
            spacing: 5
            
            Image
            {
                id: filterByArrowIcon
                sourceSize.height: 14
                source: Icons.filter
                fillMode: Image.PreserveAspectFit
            }
            
            Label
            {
                id: filterByLabel
                Layout.topMargin: -1
                color: Style.colorText
                text: "Filters"
                font.pointSize: 12
                font.weight: Font.Bold
            }
        }
    }
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked: root.clicked()
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }    
}

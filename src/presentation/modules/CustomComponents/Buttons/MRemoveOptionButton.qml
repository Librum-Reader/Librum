import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import Librum.icons


Item
{
    id: root
    property string text
    signal clicked()
    
    implicitWidth: container.width
    implicitHeight: 36
    
    
    Pane
    {
        id: container
        height: parent.height
        padding: 12
        background: Rectangle
        {
            color: Style.colorLightPurple
            border.width: 1
            border.color: Style.colorMediumPurple
            radius: 5
        }
        
        
        RowLayout
        {
            anchors.centerIn: parent
            spacing: 6
            
            Label
            {
                id: filterByLabel
                color: Style.colorBasePurple
                text: root.text
                font.pointSize: 12
                font.family: Style.defaultFontFamily
                font.weight: Font.Bold
            }
            
            Image
            {
                id: filterByArrowIcon
                sourceSize.height: 11
                source: Icons.closePurple
                fillMode: Image.PreserveAspectFit
            }
            
        }
    }
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked: root.clicked()
    }
}

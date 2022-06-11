import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


Item
{
    id: root
    property string text : "Reset Something"
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
            color: properties.colorLightPurple
            border.width: 1
            border.color: properties.colorMediumPurple
            radius: 5
        }
        
        
        RowLayout
        {
            anchors.centerIn: parent
            spacing: 6
            
            Label
            {
                id: filterByLabel
                color: properties.colorBasePurple
                text: root.text
                font.pointSize: 12
                font.family: properties.defaultFontFamily
                font.weight: Font.Bold
            }
            
            Image
            {
                id: filterByArrowIcon
                sourceSize.height: 11
                source: properties.iconClosePurple
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

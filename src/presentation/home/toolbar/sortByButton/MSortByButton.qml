import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style


Item
{
    id: root
    signal clicked()
    
    implicitWidth: 100
    implicitHeight: 36
    
    
    ColumnLayout
    {
        anchors.fill: parent
        spacing: 4
        
        
        Pane
        {
            id: container
            Layout.fillWidth: true
            Layout.fillHeight: true
            padding: 0
            background: Rectangle
            {
                color: Style.colorBackground
                border.width: 1
                border.color: Style.colorLightBorder
                radius: 5
            }
            
            
            RowLayout
            {
                anchors.centerIn: parent
                spacing: 6
                
                Label
                {
                    id: sortByLabel
                    color: Style.colorBaseText
                    text: "Sort by"
                    font.pointSize: 12
                    font.family: Style.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Image
                {
                    id: sortByArrowIcon
                    sourceSize.height: 6
                    source: properties.iconDropDownGray
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked:
        {
            root.clicked();
        }
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}
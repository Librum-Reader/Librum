import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


FocusScope
{
    id: root
    property bool opened : false
    signal sortQueryEmitted()
    
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
                color: properties.colorBackground
                border.width: 1
                border.color: properties.colorLightBorder
                radius: 5
            }
            
            
            RowLayout
            {
                anchors.centerIn: parent
                spacing: 6
                
                Label
                {
                    id: sortByLabel
                    color: properties.colorBaseText
                    text: "Sort by"
                    font.pointSize: 12
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                
                    MouseArea
                    {
                        anchors.fill: parent
                        
                        onClicked:
                        {
                            root.opened = !root.opened
                            console.log("Clicked"); 
                        }
                    }
                }
                
                Image
                {
                    id: sortByArrowIcon
                    sourceSize.height: 6
                    source: properties.iconArrowDownFilled
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }
}
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import "readingToolbar"

Page
{
    id: root
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.pagesBackground
    }
    
    
    ColumnLayout
    {
        id: mainLayout
        width: parent.width
        
        
        MReadingToolBar
        {
            Layout.fillWidth: true
        }
    }
}
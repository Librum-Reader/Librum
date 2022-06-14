import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import "readingToolbar"


Page
{
    id: root
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.pagesBackground
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
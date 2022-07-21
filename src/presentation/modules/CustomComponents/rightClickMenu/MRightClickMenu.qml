import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Qt.labs.platform
import Librum.style
import Librum.icons


Popup
{
    id: root
    property ObjectModel objectModel
    
    implicitWidth: 170
    focus: true
    horizontalPadding: 0
    verticalPadding: 6
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    background: Rectangle
    {
        color: Style.colorBackground
        border.color: Style.colorLightBorder
        radius: 3
    }
    
    
    ColumnLayout
    {
        width: parent.width
        spacing: 0
        
        
        ListView
        {
            id: listView
            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            
            model: root.objectModel
        }
    }
}
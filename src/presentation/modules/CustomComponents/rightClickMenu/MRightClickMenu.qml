import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Qt.labs.platform
import QtQml.Models
import Librum.style
import Librum.icons
import "PopupPositionLogic.js" as Logic


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
        color: Style.colorContainerBackground
        border.color: Style.colorContainerBorder
        radius: 3
    }
    
    
    ColumnLayout
    {
        id: layout
        width: parent.width
        spacing: 0
        
        
        ListView
        {
            id: listView
            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            interactive: false
            
            model: root.objectModel
        }
    }
    
    // Calculate the popup's spawn position
    function setSpawnPosition(currentMousePosition, absoluteMousePosition, rootObject)
    {
        let offset = 2;
        root.x = Logic.getBookOptionsPopupXCoord(currentMousePosition.x,
                                                 absoluteMousePosition.x,
                                                 rootObject) + offset;
        root.y = Logic.getBookOptionsPopupYCoord(currentMousePosition.y,
                                                 absoluteMousePosition.y,
                                                 rootObject) + offset;
    }
}
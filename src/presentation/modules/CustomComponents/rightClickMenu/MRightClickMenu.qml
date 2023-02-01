import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CustomComponents 1.0
import Qt.labs.platform 1.0
import QtQml.Models 2.15
import Librum.style 1.0
import Librum.icons 1.0
import "PopupPositionLogic.js" as Logic


Popup
{
    id: root
    property ObjectModel objectModel
    property var mouseArea
    
    implicitWidth: 170
    focus: true
    horizontalPadding: 0
    verticalPadding: 6
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    background: Rectangle
    {
        color: Style.colorBackground
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
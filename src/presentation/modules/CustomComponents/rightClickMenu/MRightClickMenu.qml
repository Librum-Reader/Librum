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
    property var mouseArea
    
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
            interactive: false
            
            model: root.objectModel
        }
    }
    
    
    function setSpawnPosition(currentMousePosition, absoluteMousePosition, rootObject)
    {
        let offset = 2;
        root.x = root.getBookOptionsPopupXCoord(currentMousePosition.x, absoluteMousePosition.x, rootObject) + offset;
        root.y = root.getBookOptionsPopupYCoord(currentMousePosition.y, absoluteMousePosition.y, rootObject) + offset;
    }
    
    
    function getBookOptionsPopupXCoord(currentXPosition, absoluteXPosition, rootObject)
    {
        if(spaceToRootWidth(absoluteXPosition, rootObject) <= 0)
            return currentXPosition + spaceToRootWidth(absoluteXPosition, rootObject);
        
        return currentXPosition;
    }
    
    function spaceToRootWidth(xCoord, rootObject)
    {
        return rootObject.width - (xCoord + implicitWidth);
    }
    
    
    function getBookOptionsPopupYCoord(currentYPosition, absoluteYPosition, rootObject)
    {
        if(spaceToRootHeight(absoluteYPosition, rootObject) <= 0)
            return currentYPosition + spaceToRootHeight(absoluteYPosition, rootObject);
        
        return currentYPosition;
    }
    
    function spaceToRootHeight(yCoord, rootObject)
    {
        return rootObject.height - (yCoord + implicitHeight);
    }
}
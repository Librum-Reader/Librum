import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.icons


Popup
{
    id: root
    implicitWidth: 170
    focus: true
    horizontalPadding: 0
    verticalPadding: 8
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    background: Rectangle
    {
        color: Style.colorBackground
        border.color: Style.colorLightBorder
        radius: 4
    }
    
    
    ColumnLayout
    {
        width: parent.width
        spacing: 4
        
        
        MBookOptionsItem
        {
            Layout.fillWidth: true
            imagePath: Icons.book_open
            text: "Read book"
        }
        
        MBookOptionsItem
        {
            Layout.fillWidth: true
            imagePath: Icons.book
            imageSize: 14
            text: "Book details"
        }
        
        MBookOptionsItem
        {
            Layout.fillWidth: true
            imagePath: Icons.add_file
            imageSize: 15
            text: "Save to disk"
        }
        
        Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: Style.colorLightBorder }
        
        MBookOptionsItem
        {
            Layout.fillWidth: true
            imagePath: Icons.tag_gray
            imageSize: 17
            text: "Add tag"
        }
        
        MBookOptionsItem
        {
            Layout.fillWidth: true
            imagePath: Icons.check_circle
            text: "Mark as read"
        }
        
        MBookOptionsItem
        {
            Layout.fillWidth: true
            imagePath: Icons.trash_gray
            imageSize: 17
            text: "Delete book"
        }
    }
}
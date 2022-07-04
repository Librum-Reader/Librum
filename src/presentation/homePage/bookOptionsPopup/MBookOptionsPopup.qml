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
        
        
        MBookOptionsItem
        {
            Layout.fillWidth: true
            imagePath: Icons.book_open
            imageSize: 17
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
            Layout.bottomMargin: 4
            imagePath: Icons.add_file
            imageSize: 14
            text: "Save to disk"
        }
        
        Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: Style.colorLightBorder }
        
        MBookOptionsItem
        {
            Layout.fillWidth: true
            Layout.topMargin: 4
            imagePath: Icons.tag_gray
            imageSize: 16
            text: "Add tag"
        }
        
        MBookOptionsItem
        {
            Layout.fillWidth: true
            imagePath: Icons.check_circle
            imageSize: 17
            text: "Mark as read"
        }
        
        MBookOptionsItem
        {
            Layout.fillWidth: true
            imagePath: Icons.trash_gray
            imageSize: 16
            text: "Delete book"
        }
    }
}
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
            
            onClicked:
            {
                root.close();
                loadPage(readingPage);
            }
        }
        
        MBookOptionsItem
        {
            Layout.fillWidth: true
            imagePath: Icons.book
            imageSize: 14
            text: "Book details"
            
            onClicked:
            {
                bookDetailsPopup.open();
                root.close();
            }
        }
        
        MBookOptionsItem
        {
            Layout.fillWidth: true
            Layout.bottomMargin: 4
            imagePath: Icons.add_file
            imageSize: 14
            text: "Save to files"
            
            onClicked:
            {
                downloadFileDialog.open();
                root.close();
            }
        }
        
        Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: Style.colorLightBorder }
        
        MBookOptionsItem
        {
            Layout.fillWidth: true
            Layout.topMargin: 4
            imagePath: Icons.tag_gray
            imageSize: 16
            text: "Add tag"
            
            onClicked:
            {
                root.close();
            }
        }
        
        MBookOptionsItem
        {
            Layout.fillWidth: true
            imagePath: Icons.check_circle
            imageSize: 17
            text: "Mark as read"
            
            onClicked:
            {
                root.close();
            }
        }
        
        MBookOptionsItem
        {
            Layout.fillWidth: true
            imagePath: Icons.trash_gray
            imageSize: 16
            text: "Delete book"
            
            onClicked:
            {
                acceptDeletionPopup.open();
                root.close();
            }
        }
    }
    
    
    FileDialog
    {
        id: downloadFileDialog
        acceptLabel: "Save"
        fileMode: FileDialog.SaveFile
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
    }
}
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0


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
            text: "Manage tags"
            
            onClicked:
            {
                manageTagsPopup.open();
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
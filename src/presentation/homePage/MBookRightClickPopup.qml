import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CustomComponents 1.0
import QtQml.Models 2.15
import Librum.globals 1.0
import Librum.icons 1.0
import Librum.style 1.0

MRightClickMenu
{
    id: root
    signal downloadClicked()
    signal readBookClicked()
    signal bookDetailsClicked()
    signal saveToFilesClicked()
    signal manageTagsClicked()
    signal markAsReadClicked()
    signal uninstallClicked()
    signal deleteClicked()
    
    implicitHeight: internal.bookDownloaded ? 245 : 181 
    
    onOpened: internal.bookDownloaded = Globals.selectedBook.downloaded
    
    
    objectModel: ObjectModel
    {
        MRightClickMenuItem
        {
            width: root.width
            height: internal.bookDownloaded ? 0 : implicitHeight
            visible: !internal.bookDownloaded
            imagePath: Icons.bookPopupDownload
            imageSize: 18
            text: "Download"
            
            onClicked: root.downloadClicked()
        }
        
        MRightClickMenuItem
        {
            width: root.width
            height: internal.bookDownloaded ? implicitHeight : 0
            visible: internal.bookDownloaded
            imagePath: Icons.bookPopupRead
            imageSize: 17
            text: "Read book"
            
            onClicked: root.readBookClicked()
        }
        
        MRightClickMenuItem
        {
            width: root.width
            imagePath: Icons.bookPopupDetails
            imageSize: 14
            text: "Book details"
            
            onClicked: root.bookDetailsClicked()
        }
        
        MRightClickMenuItem
        {
            width: root.width
            height: internal.bookDownloaded ? implicitHeight : 0
            visible: internal.bookDownloaded
            Layout.bottomMargin: 4
            imagePath: Icons.bookPopupSave
            imageSize: 14
            text: "Save to files"
            
            onClicked: root.saveToFilesClicked()
        }
        
        Item { id: rectMargin1; height: 4; width: root.width }
        
        Rectangle { width: root.width; height: 1; color: Style.colorDarkSeparator }
        
        Item { id: rectMargin2; height: 4; width: root.width }
        
        MRightClickMenuItem
        {
            width: root.width
            Layout.topMargin: 4
            imagePath: Icons.bookPopupTags
            imageSize: 16
            text: "Manage tags"
            
            onClicked: root.manageTagsClicked()
        }
        
        MRightClickMenuItem
        {
            width: root.width
            imagePath: Icons.bookPopupMarkAsRead
            imageSize: 17
            text: "Mark as read"
            
            onClicked: root.markAsReadClicked()
        }
        
        MRightClickMenuItem
        {
            width: root.width
            height: internal.bookDownloaded ? implicitHeight : 0
            visible: internal.bookDownloaded
            imagePath: Icons.bookPopupUninstall
            imageSize: 12
            text: "Uninstall"
            
            onClicked: root.uninstallClicked()
        }
        
        MRightClickMenuItem
        {
            width: root.width
            imagePath: Icons.bookPopupDelete
            imageSize: 16
            text: "Delete book"
            
            onClicked: root.deleteClicked()
        }
    }
    
    QtObject
    {
        id: internal
        property bool bookDownloaded: true
    }
}
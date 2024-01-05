import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import QtQml.Models
import Librum.controllers
import Librum.globals
import Librum.icons
import Librum.style

MRightClickMenu {
    id: root
    signal downloadClicked
    signal readBookClicked
    signal bookDetailsClicked
    signal saveToFilesClicked
    signal manageTagsClicked
    signal addToFolderClicked
    signal removeClicked

    implicitHeight: internal.bookDownloaded ? 213 : 181 // 32px per item

    onOpened: internal.bookDownloaded = Globals.selectedBook.downloaded

    objectModel: ObjectModel {
        MRightClickMenuItem {
            width: root.width
            height: internal.bookDownloaded ? 0 : implicitHeight
            visible: !internal.bookDownloaded
            imagePath: Icons.bookPopupDownload
            imageSize: 18
            text: qsTr("Download")

            onClicked: root.downloadClicked()
        }

        MRightClickMenuItem {
            width: root.width
            height: internal.bookDownloaded ? implicitHeight : 0
            visible: internal.bookDownloaded
            imagePath: Icons.bookPopupRead
            imageSize: 17
            text: qsTr("Read book")

            onClicked: root.readBookClicked()
        }

        MRightClickMenuItem {
            width: root.width
            imagePath: Icons.bookPopupDetails
            imageSize: 14
            text: qsTr("Book details")

            onClicked: root.bookDetailsClicked()
        }

        MRightClickMenuItem {
            width: root.width
            height: internal.bookDownloaded ? implicitHeight : 0
            visible: internal.bookDownloaded
            Layout.bottomMargin: 4
            imagePath: Icons.bookPopupSave
            imageSize: 14
            text: qsTr("Save to files")

            onClicked: root.saveToFilesClicked()
        }

        Item {
            id: rectMargin1
            height: 4
            width: root.width
        }

        Rectangle {
            width: root.width
            height: 1
            color: Style.colorDarkSeparator
        }

        Item {
            id: rectMargin2
            height: 4
            width: root.width
        }

        MRightClickMenuItem {
            width: root.width
            Layout.topMargin: 4
            imagePath: Icons.bookPopupTags
            imageSize: 16
            text: qsTr("Manage tags")

            onClicked: root.manageTagsClicked()
        }

        MRightClickMenuItem {
            width: root.width
            imagePath: Icons.folder
            imageSize: 17
            //: If this is too long, use "To Folder" instead
            text: qsTr("Add to Folder")

            onClicked: root.addToFolderClicked()
        }

        MRightClickMenuItem {
            width: root.width
            imagePath: Icons.bookPopupDelete
            imageSize: 16
            text: qsTr("Remove book")

            onClicked: root.removeClicked()
        }
    }

    QtObject {
        id: internal
        property bool bookDownloaded: true
    }
}

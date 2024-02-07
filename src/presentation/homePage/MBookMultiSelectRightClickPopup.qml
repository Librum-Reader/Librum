import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import QtQml.Models
import Librum.globals
import Librum.icons
import Librum.style

MRightClickMenu {
    id: root
    signal markAsReadClicked
    signal removeClicked
    signal addToFolderClicked

    implicitHeight: 108 // 32px per item

    objectModel: ObjectModel {

        MRightClickMenuItem {
            width: root.width
            imagePath: Icons.bookPopupMarkAsRead
            imageSize: 17
            //: If this is too long in any language, use "Read" (past form) instead
            text: qsTr("Mark as read")

            onClicked: root.markAsReadClicked()
        }

        MRightClickMenuItem {
            width: root.width
            imagePath: Icons.bookPopupDelete
            imageSize: 16
            text: qsTr("Remove books")

            onClicked: root.removeClicked()
        }

        MRightClickMenuItem {
            width: root.width
            imagePath: Icons.folder
            imageSize: 17
            //: If this is too long, use "To Folder" instead
            text: qsTr("Move to Folder")

            onClicked: root.addToFolderClicked()
        }
    }
}

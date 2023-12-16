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
    signal deleteClicked
    signal uninstallClicked

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
            imagePath: Icons.bookPopupUninstall
            imageSize: 13
            text: qsTr("Uninstall books")

            onClicked: root.uninstallClicked()
        }

        MRightClickMenuItem {
            width: root.width
            imagePath: Icons.bookPopupDelete
            imageSize: 16
            text: qsTr("Delete books")

            onClicked: root.deleteClicked()
        }
    }
}

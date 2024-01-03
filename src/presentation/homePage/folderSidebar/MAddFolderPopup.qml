import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.fonts
import Librum.style

Popup {
    id: root
    implicitWidth: 560
    implicitHeight: 420
    padding: 8
    focus: true
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
    background: Rectangle {
        color: Style.colorPopupBackground
        border.width: 1
        border.color: Style.colorContainerBorder
        radius: 4
        antialiasing: true
    }
}

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import Librum.icons
import Librum.fonts

Item {
    id: root
    property bool opened: false
    signal filterSelected(string authors, string format, string date, bool onlyBooks, bool onlyFiles, bool read, bool unread)

    implicitWidth: container.width
    implicitHeight: 36

    Pane {
        id: container
        property int hPadding: 12
        property int minWidth: 104

        height: parent.height
        width: (layout.width + hPadding * 2) < minWidth ? minWidth : layout.width + hPadding * 2
        padding: 0
        background: Rectangle {
            color: Style.colorControlBackground
            border.width: 1
            border.color: Style.colorContainerBorder
            radius: 5
        }

        RowLayout {
            id: layout
            anchors.centerIn: parent
            spacing: 5

            Image {
                id: filterByArrowIcon
                sourceSize.height: 14
                source: Icons.filter
                fillMode: Image.PreserveAspectFit
            }

            Label {
                id: filterByLabel
                color: Style.colorText
                text: qsTr("Filters")
                font.pointSize: Fonts.size12
                font.weight: Font.Bold
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: selectionPopup.opened ? selectionPopup.close(
                                               ) : selectionPopup.open()
    }

    MFilterByPopup {
        id: selectionPopup
        y: root.height + 6
        closePolicy: Popup.CloseOnReleaseOutsideParent | Popup.CloseOnEscape

        onFilterQuerySent: {
            close()
            root.filterSelected(authors, format, date, onlyBooks, onlyFiles,
                                read, unread)
        }
    }

    function resetFilter() {
        selectionPopup.resetFilter()
    }

    function giveFocus() {
        root.forceActiveFocus()
    }
}

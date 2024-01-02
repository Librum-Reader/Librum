import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import Librum.icons
import Librum.fonts

Item {
    id: root
    signal itemSelected(int role)

    implicitWidth: layout.width
    implicitHeight: 36

    ColumnLayout {
        id: layout
        height: parent.height
        width: container.width
        spacing: 4

        Pane {
            id: container
            property int hPadding: 12
            property int minWidth: 104

            Layout.preferredWidth: (inButtonLayout.implicitWidth + hPadding * 2)
                                   < minWidth ? minWidth : (inButtonLayout.implicitWidth
                                                            + hPadding * 2)
            Layout.fillHeight: true
            padding: 0
            background: Rectangle {
                color: Style.colorControlBackground
                border.width: 1
                border.color: Style.colorContainerBorder
                radius: 5
            }

            RowLayout {
                id: inButtonLayout
                anchors.centerIn: parent
                spacing: 8

                Label {
                    id: sortByLabel
                    color: Style.colorText
                    text: qsTr("Sort")
                    font.pointSize: Fonts.size12
                    font.weight: Font.Bold
                }

                Image {
                    id: sortByArrowIcon
                    sourceSize.height: 6
                    source: Icons.dropdownLight
                    fillMode: Image.PreserveAspectFit
                    rotation: 180

                    NumberAnimation {
                        id: closeAnim
                        target: sortByArrowIcon
                        property: "rotation"
                        to: 180
                        duration: 175
                        easing.type: Easing.InOutQuad
                    }

                    NumberAnimation {
                        id: openAnim
                        target: sortByArrowIcon
                        property: "rotation"
                        to: 0
                        duration: 175
                        easing.type: Easing.InOutQuad
                    }
                }
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        cursorShape: Qt.PointingHandCursor

        onClicked: sortByPopup.opened ? sortByPopup.close() : sortByPopup.open()
    }

    MSortByPopup {
        id: sortByPopup
        y: root.height + 6
        closePolicy: Popup.CloseOnReleaseOutsideParent | Popup.CloseOnEscape

        onOpened: openAnim.start()
        onClosed: closeAnim.start()
        onItemSelected: role => root.itemSelected(role)
    }

    function giveFocus() {
        root.forceActiveFocus()
    }
}

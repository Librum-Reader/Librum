import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import Librum.style
import Librum.fonts
import Librum.controllers
import CustomComponents

Page {
    id: root
    horizontalPadding: 18
    background: Rectangle {
        color: Style.colorPageBackground
    }

    ColumnLayout {
        anchors.fill: parent

        Pane {
            id: bookGridContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 100
            padding: 0
            background: Rectangle {
                color: "transparent"
            }

            GridView {
                id: bookGrid
                width: cellWidth * 2
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                cellWidth: 120 + 14
                cellHeight: 190 + 14
                rightMargin: -14
                layoutDirection: Qt.LeftToRight
                LayoutMirroring.enabled: false
                LayoutMirroring.childrenInherit: true
                interactive: true
                boundsBehavior: Flickable.StopAtBounds
                flickDeceleration: 1650
                maximumFlickVelocity: 2500
                clip: true
                model: LibraryController.libraryModel
                delegate: MBook {}
            }

            ScrollBar {
                id: verticalScrollbar
                width: pressed ? 14 : 12
                hoverEnabled: true
                active: true
                policy: ScrollBar.AlwaysOff
                visible: bookGrid.contentHeight > bookGrid.height
                orientation: Qt.Vertical
                size: bookGrid.height / bookGrid.contentHeight
                minimumSize: 0.04
                position: (bookGrid.contentY - bookGrid.originY) / bookGrid.contentHeight
                onPositionChanged: if (pressed)
                                       bookGrid.contentY = position
                                               * bookGrid.contentHeight + bookGrid.originY
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.bottomMargin: 16
                anchors.bottom: parent.bottom
                horizontalPadding: 4

                contentItem: Rectangle {
                    color: Style.colorScrollBarHandle
                    opacity: verticalScrollbar.pressed ? 0.8 : 1
                    radius: 4
                }

                background: Rectangle {
                    implicitWidth: 26
                    implicitHeight: 200
                    color: "transparent"
                }
            }
        }
    }
}

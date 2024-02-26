import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import CustomComponents
import Librum.elements
import Librum.style
import Librum.icons
import Librum.controllers
import Librum.fonts

Item {
    id: root
    property alias freeBooksItem: freeBooksItem
    property alias homeItem: homeItem
    property alias statisticsItem: statisticsItem
    property alias addOnsItem: addOnsItem
    property alias settingsItem: settingsItem
    property MSidebarItem currentItem: internal.defaultItem

    implicitWidth: 72
    implicitHeight: Window.height


    /*
      Shortcuts to switch between pages
      */
    Shortcut {
        sequence: "Ctrl+1"
        onActivated: loadPage(freeBooksPage, root.freeBooksItem)
    }
    Shortcut {
        sequence: "Ctrl+2"
        onActivated: loadPage(homePage, root.homeItem)
    }
    Shortcut {
        sequence: "Ctrl+3"
        onActivated: loadPage(statisticsPage, root.statisticsItem)
    }
    Shortcut {
        sequence: "Ctrl+4"
        onActivated: loadPage(addOnsPage, root.addOnsItem)
    }
    Shortcut {
        sequence: "Ctrl+5"
        onActivated: loadPage(settings, root.settingsItem)
    }

    Pane {
        id: container
        anchors.fill: parent
        padding: 0
        background: RowLayout {
            spacing: 0

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Style.colorContainerBackground
            }

            Rectangle {
                Layout.preferredWidth: 1
                Layout.fillHeight: true
                color: Style.colorContainerBorder
            }
        }

        MFlickWrapper {
            id: flickWrapper
            anchors.fill: parent
            contentHeight: layout.implicitHeight

            ColumnLayout {
                id: layout
                width: parent.width
                height: container.height
                spacing: 0

                RowLayout {
                    id: logo
                    Layout.topMargin: 14
                    Layout.leftMargin: 14
                    spacing: 20

                    MLogo {
                        id: logoIcon
                        Layout.preferredWidth: 44
                        Layout.preferredHeight: 44
                    }

                    Label {
                        id: logoLabel
                        visible: false
                        text: AppInfoController.applicationName
                        font.bold: true
                        font.pointSize: Fonts.size16
                        color: Style.colorTitle
                    }
                }

                Rectangle {
                    id: topSeparator
                    Layout.fillWidth: true
                    Layout.preferredHeight: 2
                    Layout.topMargin: 15
                    color: Style.colorSeparator
                }

                MSidebarItem {
                    id: freeBooksItem
                    Layout.topMargin: 16
                    Layout.leftMargin: 10
                    imageWidth: 31
                    image: Icons.sidebarDownload
                    text: qsTr("Free books")

                    onClicked: loadPage(freeBooksPage, root.freeBooksItem)
                }

                Rectangle {
                    id: smallSeperator1
                    Layout.preferredWidth: parent.width * 0.61
                    Layout.preferredHeight: 2
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 16
                    color: Style.colorSeparator
                }

                MSidebarItem {
                    id: homeItem
                    Layout.topMargin: 15
                    Layout.leftMargin: 10
                    imageWidth: 30
                    preserveImageFit: false
                    imageHeight: 28
                    image: Icons.sidebarHome
                    text: qsTr("Home")

                    onClicked: loadPage(homePage, root.homeItem)
                }

                MSidebarItem {
                    id: statisticsItem
                    Layout.topMargin: 13
                    Layout.leftMargin: 10
                    imageWidth: 28
                    image: Icons.sidebarPieChart
                    text: qsTr("Statistics")

                    onClicked: loadPage(statisticsPage, root.statisticsItem)
                }

                Rectangle {
                    id: smallSeperator2
                    Layout.preferredWidth: parent.width * 0.61
                    Layout.preferredHeight: 2
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 15
                    color: Style.colorSeparator
                }

                MSidebarItem {
                    id: addOnsItem
                    Layout.topMargin: 13
                    Layout.leftMargin: 10
                    imageWidth: 30
                    image: Icons.sidebarAddOns
                    text: qsTr("Add-ons")

                    onClicked: loadPage(addOnsPage, root.addOnsItem)
                }

                MSidebarItem {
                    id: settingsItem
                    Layout.topMargin: 13
                    Layout.leftMargin: 10
                    imageWidth: 36
                    image: Icons.sidebarSettings
                    text: qsTr("Settings")

                    onClicked: loadPage(settings, root.settingsItem)
                }

                Item {
                    id: heightFiller
                    Layout.fillHeight: true
                    Layout.minimumHeight: 30
                }

                Rectangle {
                    id: bottomSeparator
                    Layout.fillWidth: true
                    Layout.preferredHeight: 2
                    color: Style.colorSeparator
                }


                /*
                  Box which contains the profile picture
                  */
                MProfileBox {
                    id: profileBox
                    sidebarWidth: 72
                    Layout.alignment: Qt.AlignBottom
                    Layout.topMargin: 2

                    onClicked: {
                        if (profilePopup.opened)
                            profilePopup.close()
                        else
                            profilePopup.open()
                    }
                }


                /*
                  Popup which is spawned after clicking the profile picture
                  */
                MProfilePopup {
                    id: profilePopup
                    x: {
                        if (baseRoot.rightAlign)
                            return -106
                        else
                            return 12
                    }
                    y: profileBox.y - implicitHeight + 6
                }
            }
        }
    }

    QtObject {
        id: internal
        property MSidebarItem defaultItem: homeItem
    }

    function changeSelectedItem(newItem) {
        root.currentItem.selected = false
        root.currentItem = newItem
        root.currentItem.selected = true
    }

    function resetSidebar() {
        changeSelectedItem(internal.defaultItem)
    }

    function giveFocus() {
        root.forceActiveFocus()
    }
}

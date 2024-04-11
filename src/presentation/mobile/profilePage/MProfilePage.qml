import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.fonts
import Librum.icons
import Librum.controllers
import CustomComponents

Page {
    id: root
    horizontalPadding: 16
    topPadding: 52
    background: Rectangle {
        color: Style.colorPageBackground
    }

    ColumnLayout {
        anchors.fill: parent

        Label {
            id: pageTitle
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Profile")
            color: Style.colorTitle
            font.weight: Font.Bold
            font.pointSize: Fonts.size19dot5
        }

        MFlickWrapper {
            Layout.topMargin: 8
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentHeight: contentLayout.implicitHeight
            clip: true

            ColumnLayout {
                id: contentLayout
                width: parent.width

                Pane {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 72
                    Layout.topMargin: 12
                    background: Rectangle {
                        color: Style.colorContainerBackground
                        radius: 10
                    }

                    MProfileCard {
                        width: parent.width
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                Label {
                    id: accountTitle
                    Layout.topMargin: 4
                    Layout.leftMargin: 10
                    text: qsTr("Account")
                    color: Style.colorTitle
                    font.weight: Font.DemiBold
                    font.pointSize: Fonts.size16
                }

                Pane {
                    Layout.fillWidth: true
                    Layout.preferredHeight: accountLayout.implicitHeight + 2 * verticalPadding
                    Layout.topMargin: 3
                    verticalPadding: 10
                    horizontalPadding: 6
                    background: Rectangle {
                        color: Style.colorContainerBackground
                        radius: 10
                    }

                    ColumnLayout {
                        id: accountLayout
                        width: parent.width
                        spacing: 0

                        MProfileItem {
                            Layout.fillWidth: true
                            text: qsTr("My Account")
                            icon: Icons.mobileProfile
                            iconWidth: 18

                            onClicked: loadPage(myAccountPage)
                        }

                        MProfileItem {
                            Layout.fillWidth: true
                            text: qsTr("Storage")
                            icon: Icons.mobileServer

                            onClicked: ;
                        }
                    }
                }

                Label {
                    id: settingsTitle
                    Layout.topMargin: 4
                    Layout.leftMargin: 10
                    text: qsTr("Settings")
                    color: Style.colorTitle
                    font.weight: Font.DemiBold
                    font.pointSize: Fonts.size16
                }

                Pane {
                    Layout.fillWidth: true
                    Layout.preferredHeight: settingsLayout.implicitHeight + 2 * verticalPadding
                    Layout.topMargin: 3
                    verticalPadding: 10
                    horizontalPadding: 6
                    background: Rectangle {
                        color: Style.colorContainerBackground
                        radius: 10
                    }

                    ColumnLayout {
                        id: settingsLayout
                        width: parent.width
                        spacing: 0

                        MProfileItem {
                            Layout.fillWidth: true
                            text: qsTr("About")
                            icon: Icons.mobileDiamond

                            onClicked: ;
                        }

                        MProfileItem {
                            Layout.fillWidth: true
                            text: qsTr("Appearance")
                            icon: Icons.mobileBrush

                            onClicked: SettingsController.setSetting(
                                           SettingKeys.Theme, "Light",
                                           SettingGroups.Appearance)
                        }

                        MProfileItem {
                            Layout.fillWidth: true
                            text: qsTr("Behavior")
                            icon: Icons.mobileGear

                            onClicked: ;
                        }
                    }
                }

                Label {
                    id: settingsTitle2
                    Layout.topMargin: 4
                    Layout.leftMargin: 10
                    text: qsTr("Actions")
                    color: Style.colorTitle
                    font.weight: Font.DemiBold
                    font.pointSize: Fonts.size16
                }

                Pane {
                    Layout.fillWidth: true
                    Layout.preferredHeight: settingsLayout2.implicitHeight + 2 * verticalPadding
                    Layout.topMargin: 3
                    Layout.bottomMargin: 14
                    verticalPadding: 10
                    horizontalPadding: 6
                    background: Rectangle {
                        color: Style.colorContainerBackground
                        radius: 10
                    }

                    ColumnLayout {
                        id: settingsLayout2
                        width: parent.width
                        spacing: 0

                        MProfileItem {
                            Layout.fillWidth: true
                            text: LibraryController.isSyncing ? qsTr("Refreshing...") : qsTr(
                                                                    "Refresh")
                            icon: Icons.mobileRefresh

                            onClicked: internal.reloadApplication()
                        }

                        MProfileItem {
                            Layout.fillWidth: true
                            text: qsTr("Logout")
                            icon: Icons.mobileSignOut

                            onClicked: {
                                AuthController.logoutUser()
                                loadPage(loginPage)
                            }
                        }
                    }
                }
            }
        }
    }

    QtObject {
        id: internal

        function reloadApplication() {
            if (spamStopper.available) {
                LibraryController.syncWithServer()
                UserController.syncWithServer()
                FolderController.syncWithServer()
                spamStopper.available = false
                spamStopper.start()
            }
        }
    }

    Timer {
        id: spamStopper
        property bool available: true

        interval: 1000
        onTriggered: spamStopper.available = true
    }
}

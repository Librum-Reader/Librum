import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.fonts
import Librum.controllers
import Librum.models

MFlickWrapper {
    id: root
    contentHeight: page.implicitHeight

    Page {
        id: page
        width: parent.width
        horizontalPadding: 48
        bottomPadding: 22
        background: Rectangle {
            anchors.fill: parent
            color: Style.colorPageBackground
        }

        Component.onCompleted: {
            UserController.syncWithServer()
        }

        ColumnLayout {
            id: layout
            width: parent.width
            spacing: 0

            RowLayout {
                id: titleRow
                Layout.fillWidth: true
                spacing: 0

                MTitle {
                    id: pageTitle
                    Layout.topMargin: 64
                    titleText: qsTr("Storage")
                    descriptionText: qsTr("Your storage")
                    titleSize: Fonts.size25
                    descriptionSize: Fonts.size13dot25
                }

                Item {
                    Layout.fillWidth: true
                }

                MButton {
                    id: upgradeButton
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignBottom
                    horizontalMargins: 12
                    borderWidth: 0
                    backgroundColor: Style.colorBasePurple
                    text: qsTr("Upgrade")
                    fontSize: Fonts.size12
                    fontWeight: Font.Bold
                    textColor: Style.colorFocusedButtonText
                    imagePath: Icons.heartHallow
                    imageSize: 18

                    onClicked: Qt.openUrlExternally(
                                   AppInfoController.website + "/pricing")
                }
            }

            Pane {
                id: container
                Layout.fillWidth: true
                Layout.topMargin: 32
                padding: 28
                background: Rectangle {
                    color: Style.colorContainerBackground
                    border.color: Style.colorContainerBorder
                    radius: 4
                    antialiasing: true
                }

                ColumnLayout {
                    width: parent.width
                    spacing: 26

                    RowLayout {
                        width: parent.width
                        spacing: 26

                        Pane {
                            id: tierPane
                            Layout.fillWidth: true
                            Layout.preferredWidth: 470
                            Layout.maximumWidth: 470
                            Layout.minimumWidth: 235
                            Layout.preferredHeight: 325
                            horizontalPadding: 34
                            background: Rectangle {
                                anchors.fill: parent
                                color: Style.colorPageBackground
                                radius: 4
                            }

                            ColumnLayout {
                                width: parent.width

                                Label {
                                    id: tierTitle
                                    Layout.alignment: Qt.AlignLeft
                                    Layout.topMargin: 6
                                    //: Keep capitalized ("TIER" as in subscription)
                                    text: qsTr("YOUR TIER")
                                    color: Style.colorPageSubtitle
                                    font.weight: Font.Bold
                                    font.pointSize: Fonts.size10dot5
                                }

                                Label {
                                    id: tierName
                                    Layout.alignment: Qt.AlignHCenter
                                    Layout.topMargin: 46
                                    text: UserController.role.toUpperCase()
                                    color: Style.colorSubtitle
                                    font.weight: Font.Medium
                                    font.pointSize: Fonts.size22
                                }

                                Label {
                                    id: storageAmount
                                    Layout.alignment: Qt.AlignHCenter
                                    Layout.topMargin: -8
                                    text: internal.bytesToGB(
                                              UserController.bookStorageLimit) + "GB"
                                    color: Style.colorMatteTitle
                                    font.weight: Font.Bold
                                    font.pointSize: Fonts.size46
                                }

                                RowLayout {
                                    id: upgradeButtonRow
                                    Layout.fillWidth: true
                                    Layout.topMargin: 36
                                    spacing: 12

                                    MButton {
                                        id: inlineUpgradeButton
                                        Layout.preferredWidth: 110
                                        Layout.preferredHeight: 38
                                        borderWidth: 0
                                        backgroundColor: Style.colorBasePurple
                                        text: qsTr("Upgrade")
                                        fontSize: Fonts.size12
                                        fontWeight: Font.Bold
                                        textColor: Style.colorFocusedButtonText

                                        onClicked: Qt.openUrlExternally(
                                                       AppInfoController.website + "/pricing")
                                    }

                                    MButton {
                                        id: whyOfferingTiersButton
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 38
                                        borderWidth: 1
                                        borderColor: Style.colorCheckboxBorder
                                        opacityOnPressed: 0.75
                                        backgroundColor: "transparent"
                                        text: qsTr("See why we offer multiple tiers")
                                        fontSize: Fonts.size12
                                        fontWeight: Font.Medium
                                        textColor: Style.colorText

                                        onClicked: Qt.openUrlExternally(
                                                       AppInfoController.website + "/whyTiers")
                                    }
                                }
                            }
                        }

                        Pane {
                            id: usedStoragePaneBold
                            Layout.fillWidth: true
                            Layout.minimumWidth: 340
                            Layout.preferredHeight: 325
                            horizontalPadding: 34
                            background: Rectangle {
                                anchors.fill: parent
                                color: Style.colorPageBackground
                                radius: 4
                            }

                            ColumnLayout {
                                width: parent.width
                                spacing: 0

                                Label {
                                    id: usedStorageTitle
                                    Layout.alignment: Qt.AlignLeft
                                    Layout.topMargin: 6
                                    //: Keep capitalized
                                    text: qsTr("USED STORAGE")
                                    color: Style.colorPageSubtitle
                                    font.weight: Font.Bold
                                    font.pointSize: Fonts.size10dot5
                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    Layout.topMargin: 85
                                    spacing: 0

                                    Item {
                                        id: usedStorageBox
                                        Layout.preferredWidth: parent.width / 2
                                        height: usedStorageText.height
                                                + usedStorageExplenationText.height

                                        Label {
                                            id: usedStorageText
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            text: internal.bytesToGB(
                                                      UserController.usedBookStorage,
                                                      2) + "GB"
                                            color: Style.colorBasePurple
                                            font.weight: Font.Bold
                                            font.pointSize: Fonts.size42
                                        }

                                        Label {
                                            id: usedStorageExplenationText
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.top: usedStorageText.bottom
                                            anchors.topMargin: 2
                                            text: qsTr("Used Storage")
                                            color: Style.colorLightText
                                            font.pointSize: Fonts.size11
                                        }
                                    }

                                    Item {
                                        id: availableStorageBox
                                        Layout.fillWidth: true
                                        height: remainingStorageText.height
                                                + remainingStorageExplenationText.height

                                        Label {
                                            id: remainingStorageText
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            text: internal.bytesToGB(
                                                      UserController.bookStorageLimit) + "GB"
                                            color: Style.colorLightText
                                            font.weight: Font.Bold
                                            font.pointSize: Fonts.size42
                                        }

                                        Label {
                                            id: remainingStorageExplenationText
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            anchors.top: remainingStorageText.bottom
                                            anchors.topMargin: 2
                                            text: qsTr("Remaining Storage")
                                            color: Style.colorLightText
                                            font.pointSize: Fonts.size11
                                        }
                                    }
                                }

                                Rectangle {
                                    id: progressBar
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 34
                                    Layout.topMargin: 32
                                    color: Style.colorLightPurple
                                    radius: 4

                                    Rectangle {
                                        id: progressBarFilling
                                        property int progress: parent.width * (UserController.usedBookStorage / UserController.bookStorageLimit)

                                        width: progress <= parent.width ? progress : parent.width
                                        height: parent.height
                                        color: Style.colorBasePurple
                                        radius: parent.radius
                                    }
                                }
                            }
                        }
                    }

                    RowLayout {
                        width: parent.width
                        spacing: 28

                        Pane {
                            id: yourBooksPane
                            Layout.fillWidth: true
                            Layout.preferredWidth: 470
                            Layout.maximumWidth: 470
                            Layout.minimumWidth: 235
                            Layout.preferredHeight: 325
                            horizontalPadding: 34
                            background: Rectangle {
                                anchors.fill: parent
                                color: Style.colorPageBackground
                                radius: 4
                            }

                            ColumnLayout {
                                width: parent.width
                                spacing: 0

                                Label {
                                    id: yourBooksTitle
                                    Layout.alignment: Qt.AlignLeft
                                    Layout.topMargin: 6
                                    //: Keep capitalized
                                    text: qsTr("YOUR BOOKS")
                                    color: Style.colorPageSubtitle
                                    font.weight: Font.Bold
                                    font.pointSize: Fonts.size10dot5
                                }

                                Label {
                                    id: bookCount
                                    Layout.alignment: Qt.AlignHCenter
                                    Layout.topMargin: 78
                                    text: LibraryController.bookCount
                                    color: Style.colorMatteTitle
                                    font.weight: Font.Bold
                                    font.pointSize: Fonts.size46
                                }

                                Label {
                                    id: bookCountDescription
                                    Layout.alignment: Qt.AlignHCenter
                                    text: qsTr("Books in your Library")
                                    color: Style.colorLightText
                                    font.pointSize: Fonts.size12
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    QtObject {
        id: internal

        // Convert bytes to GB and format them correctly, rules:
        // - Convert bytes to GB
        // - If bytes == 0, return "0"
        // - If result ends with a 0, e.g. "2.40" remove it, so "2.4"
        // - Else Round it up to "precision" amount
        function bytesToGB(bytes, precision = 1) {
            if (bytes === 0) {
                return "0"
            }

            const gibibytes = bytes / (1024 * 1024 * 1024)
            const rounded = gibibytes.toFixed(2)
            const formatted = rounded.replace(/\.?0+$/, "")

            return formatted
        }
    }
}

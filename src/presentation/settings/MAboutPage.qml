import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.elements
import Librum.controllers
import Librum.style
import Librum.icons
import Librum.fonts
import Librum.globalSettings

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

        ColumnLayout {
            id: layout
            width: parent.width
            spacing: 26

            MTitle {
                id: pageTitle
                Layout.topMargin: 64
                titleText: qsTr("About")
                descriptionText: qsTr("About this application")
                titleSize: Fonts.size25
                descriptionSize: Fonts.size13dot25
            }

            Pane {
                id: details
                Layout.fillWidth: true
                Layout.topMargin: 6
                topPadding: 24
                horizontalPadding: internal.pagePadding
                bottomPadding: 21
                background: Rectangle {
                    color: Style.colorContainerBackground
                    border.color: Style.colorContainerBorder
                    radius: 4
                    antialiasing: true
                }

                ColumnLayout {
                    id: inDetailsLayout
                    anchors.fill: parent
                    spacing: 0

                    Label {
                        id: detailsTitle
                        text: qsTr("Details")
                        color: Style.colorText
                        font.pointSize: Fonts.size16dot5
                        font.weight: Font.DemiBold
                    }

                    Label {
                        Layout.topMargin: 15
                        //: Keep it capitalized
                        text: qsTr("CURRENT VERSION")
                        color: Style.colorLightText
                        font.pointSize: Fonts.size10dot25
                        font.weight: Font.Bold
                    }

                    Label {
                        Layout.topMargin: 1
                        text: AppInfoController.currentVersion
                        color: Style.colorText
                        font.pointSize: Fonts.size13dot5
                    }

                    Label {
                        Layout.topMargin: 12
                        //: Keep it capitalized
                        text: qsTr("QT VERSION")
                        color: Style.colorLightText
                        font.pointSize: Fonts.size10dot25
                        font.weight: Font.Bold
                    }

                    Label {
                        Layout.topMargin: 1
                        text: AppInfoController.currentQtVersion
                        color: Style.colorText
                        font.pointSize: Fonts.size13dot5
                    }

                    MButton {
                        Layout.topMargin: 21
                        Layout.preferredWidth: 140
                        Layout.preferredHeight: 40
                        borderWidth: 0
                        backgroundColor: Style.colorBasePurple
                        text: qsTr("Feedback?")
                        fontSize: Fonts.size12
                        textColor: Style.colorFocusedButtonText
                        imageSpacing: 6
                        opacityOnPressed: 0.8

                        onClicked: {
                            Qt.openUrlExternally(AppInfoController.feedbackLink)

                            // Ask the user again in half a year after they've taken the survey
                            var date = new Date()
                            date.setDate((new Date()).getDate() + 183)
                            GlobalSettings.lastFeedbackQuery = date.toString()
                        }
                    }
                }
            }

            Pane {
                id: creator
                Layout.fillWidth: true
                topPadding: 24
                horizontalPadding: internal.pagePadding
                bottomPadding: 30
                background: Rectangle {
                    color: Style.colorContainerBackground
                    border.color: Style.colorContainerBorder
                    radius: 4
                    antialiasing: true
                }

                ColumnLayout {
                    id: inCreatorLayout
                    anchors.fill: parent
                    spacing: 0

                    Label {
                        id: creatorTitle
                        text: qsTr("Creator")
                        color: Style.colorText
                        font.pointSize: Fonts.size16dot5
                        font.weight: Font.DemiBold
                    }

                    Label {
                        Layout.topMargin: 18
                        //: Keep it capitalized
                        text: qsTr("COMPANY NAME")
                        color: Style.colorLightText
                        font.pointSize: Fonts.size10dot25
                        font.weight: Font.Bold
                    }

                    Label {
                        Layout.topMargin: 1
                        text: AppInfoController.companyName
                        color: Style.colorBasePurple
                        font.pointSize: Fonts.size13dot5
                    }

                    Label {
                        Layout.topMargin: 12
                        //: Keep it capitalized
                        text: qsTr("WEBSITE")
                        color: Style.colorLightText
                        font.pointSize: Fonts.size10dot25
                        font.weight: Font.Bold
                    }

                    Label {
                        text: AppInfoController.website
                        color: Style.colorBasePurple
                        font.pointSize: Fonts.size13dot5
                        opacity: websiteLinkArea.pressed ? 0.8 : 1

                        MouseArea {
                            id: websiteLinkArea
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor

                            onClicked: Qt.openUrlExternally(
                                           AppInfoController.website)
                        }
                    }

                    Label {
                        Layout.topMargin: 12
                        //: Keep it capitalized
                        text: qsTr("CONTACT")
                        color: Style.colorLightText
                        font.pointSize: Fonts.size10dot25
                        font.weight: Font.Bold
                    }

                    Label {
                        Layout.topMargin: 1
                        text: AppInfoController.companyEmail
                        color: Style.colorBasePurple
                        opacity: emailLinkArea.pressed ? 0.8 : 1
                        font.pointSize: Fonts.size13dot5

                        MouseArea {
                            id: emailLinkArea
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor

                            onClicked: Qt.openUrlExternally(
                                           "mailto:" + AppInfoController.companyEmail)
                        }
                    }

                    Label {
                        Layout.topMargin: 12
                        //: Keep it capitalized
                        text: qsTr("GITHUB")
                        color: Style.colorLightText
                        font.pointSize: Fonts.size10dot25
                        font.weight: Font.Bold
                    }

                    Label {
                        Layout.topMargin: 1
                        text: AppInfoController.githubLink
                        color: Style.colorBasePurple
                        opacity: githubLinkArea.pressed ? 0.8 : 1
                        font.pointSize: Fonts.size13dot5

                        MouseArea {
                            id: githubLinkArea
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor

                            onClicked: Qt.openUrlExternally(
                                           AppInfoController.githubLink)
                        }
                    }
                }
            }

            Pane {
                id: thisApp
                Layout.fillWidth: true
                Layout.minimumHeight: inThisAppLayout.height + 15
                topPadding: 24
                horizontalPadding: internal.pagePadding
                bottomPadding: 56
                background: Rectangle {
                    color: Style.colorContainerBackground
                    border.color: Style.colorContainerBorder
                    radius: 4
                    antialiasing: true
                }

                ColumnLayout {
                    id: inThisAppLayout
                    anchors.fill: parent
                    spacing: 0

                    Label {
                        id: thisAppTitle
                        text: qsTr("This App")
                        color: Style.colorText
                        font.pointSize: Fonts.size16dot5
                        font.weight: Font.DemiBold
                    }

                    RowLayout {
                        id: heartRow
                        Layout.fillWidth: true
                        Layout.topMargin: 40
                        spacing: 22

                        Image {
                            id: heartImage
                            source: Icons.heart
                            fillMode: Image.PreserveAspectFit
                            sourceSize.width: 80
                        }

                        Label {
                            id: thisAppText
                            Layout.alignment: baseRoot.rightAlign ? Qt.AlignRight : Qt.AlignLeft
                            text: qsTr("Librum is here for everyone who just wants to enjoy a good book.\n" + "We hope you have a great time using it! Feel free to leave us a rating and some feedback.")
                            wrapMode: Text.WordWrap
                            color: Style.colorText
                            font.pointSize: Fonts.size14
                            lineHeight: 1.10
                        }
                    }
                }
            }
        }
    }

    QtObject {
        id: internal
        property int pagePadding: 40
    }
}

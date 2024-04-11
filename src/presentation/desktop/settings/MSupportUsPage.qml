import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.fonts
import Librum.elements
import Librum.controllers

MFlickWrapper {
    id: root
    contentHeight: page.implicitHeight

    Page {
        id: page
        topPadding: 64
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
            spacing: 0

            MTitle {
                id: pageTitle
                titleText: qsTr("Support us")
                descriptionText: qsTr("Thanks for considering")
                titleSize: Fonts.size25
                descriptionSize: Fonts.size13dot25
            }

            Pane {
                id: container
                Layout.fillWidth: true
                Layout.topMargin: 32
                topPadding: 24
                horizontalPadding: 40
                bottomPadding: 38
                background: Rectangle {
                    color: Style.colorContainerBackground
                    border.color: Style.colorContainerBorder
                    radius: 4
                    antialiasing: true
                }

                ColumnLayout {
                    id: contentLayout
                    width: parent.width
                    spacing: 0

                    Label {
                        id: contentTitle
                        text: "Us"
                        color: Style.colorText
                        font.pointSize: Fonts.size20
                        font.weight: Font.DemiBold
                    }

                    Label {
                        Layout.fillWidth: true
                        Layout.topMargin: 15
                        text: qsTr("We are a small team of opensource developers creating apps for the community. We love\n" + "working on fun projects, supporting our community and trying to make the world a better place.")
                        wrapMode: Text.WordWrap
                        color: Style.colorLightText
                        font.pointSize: Fonts.size14
                    }

                    Label {
                        id: supportOnPatreonText
                        Layout.fillWidth: true
                        Layout.topMargin: 36
                        text: qsTr("If you feel like supporting us and our projects, feel free to support us:")
                        wrapMode: Text.WordWrap
                        color: Style.colorLightText
                        font.pointSize: Fonts.size14
                    }

                    MButton {
                        id: patreonButton
                        Layout.preferredHeight: 38
                        Layout.topMargin: 10
                        horizontalMargins: 12
                        borderWidth: 0
                        backgroundColor: Style.colorBasePurple
                        imagePath: Icons.heartHallow
                        imageSize: 18
                        imageSpacing: 10
                        text: qsTr("Support us")
                        fontWeight: Font.DemiBold
                        fontSize: Fonts.size12
                        textColor: Style.colorFocusedButtonText
                        radius: 4

                        onClicked: Qt.openUrlExternally(
                                       "https://librumreader.com/contribute/donate")
                    }

                    Label {
                        id: otherSupportText
                        Layout.fillWidth: true
                        Layout.topMargin: 40
                        text: qsTr("You can support us in many other ways as well, if you are a developer or a designer, feel free to") + " <a href=\""
                              + "https://librumreader.com/contribute\" style=\"color: "
                              + Style.colorBasePurple + "; text-decoration: none;\"> " + qsTr(
                                  "contribute to Librum") + "</a>.<br>" + qsTr(
                                  "If you are not, you can still help us by spreading the word about Librum.")
                        onLinkActivated: link => Qt.openUrlExternally(link)
                        textFormat: Text.RichText
                        wrapMode: Text.WordWrap
                        color: Style.colorLightText
                        font.pointSize: Fonts.size14

                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            acceptedButtons: Qt.NoButton // Don't eat the mouse clicks
                            cursorShape: otherSupportText.hoveredLink
                                         != "" ? Qt.PointingHandCursor : Qt.ArrowCursor
                        }
                    }

                    // RowLayout needed to put icon next to text
                    RowLayout {
                        id: heartColumn
                        Layout.topMargin: 48
                        spacing: 0

                        Image {
                            id: heartImage
                            Layout.alignment: Qt.AlignBottom
                            source: Icons.heart
                            sourceSize.width: 80
                            fillMode: Image.PreserveAspectFit
                        }

                        Label {
                            id: leaveText
                            Layout.fillWidth: true
                            Layout.leftMargin: 16
                            text: qsTr("Thank you for your support. We hope you enjoy Librum!")
                            wrapMode: Text.WordWrap
                            color: Style.colorLightText
                            font.pointSize: Fonts.size14
                        }
                    }
                }
            }
        }
    }
}

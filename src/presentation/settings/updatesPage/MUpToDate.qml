import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.elements
import Librum.controllers
import Librum.fonts

Item {
    id: root
    height: content.height

    Pane {
        id: content
        anchors.left: parent.left
        anchors.right: parent.right
        horizontalPadding: 40
        topPadding: 32
        bottomPadding: 42
        background: Rectangle {
            color: Style.colorContainerBackground
            border.color: Style.colorContainerBorder
            radius: 4
            antialiasing: true
        }

        ColumnLayout {
            id: layout
            width: parent.width
            spacing: 0

            Label {
                id: newUpdateTitle
                Layout.fillWidth: true
                text: qsTr("You are up to date!")
                wrapMode: Text.WordWrap
                color: Style.colorText
                font.pointSize: Fonts.size23
                font.weight: Font.Bold
            }

            Label {
                Layout.fillWidth: true
                Layout.topMargin: 7
                text: qsTr("Make sure to check for updates regularly, so you dont miss out on any great features.")
                wrapMode: Text.WordWrap
                color: Style.colorLightText
                font.pointSize: Fonts.size15
            }

            Label {
                Layout.fillWidth: true
                Layout.topMargin: 24
                text: qsTr("Your current version is:")
                wrapMode: Text.WordWrap
                color: Style.colorLightText
                font.pointSize: Fonts.size14
            }

            Label {
                Layout.fillWidth: true
                text: AppInfoController.currentVersion
                horizontalAlignment: Text.AlignLeft
                wrapMode: Text.WordWrap
                color: Style.colorBasePurple
                font.pointSize: Fonts.size14
                font.bold: true
            }

            Image {
                id: checkIllustration
                z: -1
                Layout.topMargin: 8
                Layout.leftMargin: -20
                source: Icons.checkBubbleIllustration
                sourceSize.width: 320
                fillMode: Image.PreserveAspectFit
            }

            Label {
                Layout.fillWidth: true
                Layout.topMargin: 32
                text: qsTr("See our latest changes at:")
                wrapMode: Text.WordWrap
                color: Style.colorLightText
                font.pointSize: Fonts.size13dot5
            }

            Label {
                Layout.preferredWidth: implicitWidth
                Layout.minimumWidth: implicitWidth
                text: AppInfoController.newsWebsite
                wrapMode: Text.WordWrap
                font.underline: true
                color: Style.colorBasePurple
                font.pointSize: Fonts.size13dot5
                opacity: newsWebsiteLinkArea.pressed ? 0.8 : 1

                MouseArea {
                    id: newsWebsiteLinkArea
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor

                    onClicked: Qt.openUrlExternally(
                                   AppInfoController.newsWebsite)
                }
            }
        }
    }
}

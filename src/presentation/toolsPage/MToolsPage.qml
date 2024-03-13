import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.fonts
import CustomComponents

Page {
    id: root
    background: Rectangle {
        color: Style.colorPageBackground
    }

    ColumnLayout {
        id: layout
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: 64
        anchors.rightMargin: 70
        spacing: 0

        MTitle {
            id: pageTitle
            Layout.topMargin: 44
            titleText: qsTr("PDF Tools")
            descriptionText: qsTr("Powerful tools to modify your PDF files")
        }

        Pane {
            id: contentBlock
            Layout.fillWidth: true
            Layout.topMargin: 45
            verticalPadding: 24
            horizontalPadding: 40
            background: Rectangle {
                color: Style.colorContainerBackground
                border.color: Style.colorContainerBorder
                radius: 4
                antialiasing: true
            }

            Flow {
                id: grid
                width: parent.width
                spacing: 16

                component ToolItem: Pane {
                    id: itemRoot
                    property string text: ""
                    property string description: ""
                    signal clicked

                    implicitWidth: 380
                    implicitHeight: 220
                    opacity: area.pressed ? 0.75 : 1
                    background: Rectangle {
                        color: Style.colorToolBackground
                        radius: 6
                    }

                    ColumnLayout {
                        spacing: 5
                        anchors.centerIn: parent

                        Label {
                            id: title
                            Layout.alignment: Qt.AlignHCenter
                            text: itemRoot.text
                            color: Style.colorLightTitle
                            font.bold: true
                            font.pointSize: Fonts.size20
                        }

                        Label {
                            id: description
                            Layout.alignment: Qt.AlignHCenter
                            text: itemRoot.description
                            color: Style.colorLightText
                            font.pointSize: Fonts.size13
                        }
                    }

                    MouseArea {
                        id: area
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor

                        onClicked: itemRoot.clicked()
                    }
                }

                ToolItem {
                    text: qsTr("Merge")
                    description: qsTr("Merge multiple books into one")
                    onClicked: mergePopup.open()
                }

                ToolItem {
                    text: qsTr("Extract Page(s)")
                    description: qsTr("Extract selected pages from a book")
                    onClicked: extractPagesPopup.open()
                }

                ToolItem {
                    text: qsTr("Convert")
                    description: qsTr("Convert books to a different format")
                    onClicked: convertPopup.open()
                }
            }
        }
    }

    MMergePopup {
        id: mergePopup
        width: parent.width * 0.6
        height: parent.height * 0.65
        y: (parent.height - height) / 2
        x: (parent.width - width) / 2
    }

    MExtractPagesPopup {
        id: extractPagesPopup
        width: parent.width * 0.6
        height: parent.height * 0.65
        y: (parent.height - height) / 2
        x: (parent.width - width) / 2
    }

    MConvertPopup {
        id: convertPopup
        width: parent.width * 0.6
        height: parent.height * 0.65
        y: (parent.height - height) / 2
        x: (parent.width - width) / 2
    }
}

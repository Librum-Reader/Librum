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
        anchors.fill: parent
        anchors.leftMargin: 64
        anchors.rightMargin: 70
        spacing: 0

        MTitle {
            id: pageTitle
            Layout.topMargin: 20
            titleText: qsTr("Tools")
            descriptionText: qsTr("Powerful tools to modify your books")
        }

        GridLayout {
            id: grid
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 30

            columns: 2

            component ToolItem: Pane {
                id: itemRoot
                property string text: ""
                signal clicked

                Layout.preferredWidth: 400
                Layout.preferredHeight: 200
                background: Rectangle {
                    color: Style.colorControlBackground
                    radius: 6
                }

                Label {
                    anchors.centerIn: parent
                    text: itemRoot.text
                    color: Style.colorText
                    font.bold: true
                    font.pointSize: Fonts.size23
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: itemRoot.clicked()
                }
            }

            ToolItem {
                text: qsTr("Merge PDFs")
                onClicked: mergePdfPopup.open()
            }
        }
    }

    MMergePdfToolPopup {
        id: mergePdfPopup
        width: parent.width * 0.8
        height: parent.height * 0.8
        y: (parent.height - height) / 2
        x: (parent.width - width) / 2
    }
}

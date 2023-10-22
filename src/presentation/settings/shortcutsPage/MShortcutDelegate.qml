import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.fonts

Item {
    id: root
    required property int index
    required property string shortcut
    required property string value

    // The gap width between ACTION and SETTING. When the page gets smaller the gap decreases,
    // this needs to be signaled to the header because it also needs to reize its gap aswell
    signal gapWidthChanged(int spacing)
    signal editClicked(int index)
    signal deleteClicked(string shortcut)

    height: 52
    width: listView.width

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: topBorder
            Layout.fillWidth: true
            Layout.preferredHeight: 2
            color: Style.colorLightSeparator
        }

        RowLayout {
            id: contentRow
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            Label {
                id: shortcutText
                Layout.leftMargin: 12
                Layout.preferredWidth: 150
                Layout.alignment: Qt.AlignVCenter
                text: root.shortcut
                color: Style.colorText
                font.pointSize: Fonts.bigSize
                font.weight: Font.DemiBold
            }

            Item {
                id: widthFiller
                Layout.fillWidth: true
                Layout.maximumWidth: 247

                onWidthChanged: root.gapWidthChanged(width)
            }

            Label {
                id: shortcutValue
                Layout.preferredWidth: 172
                Layout.alignment: Qt.AlignVCenter
                text: root.value.length > 0 ? root.value : "None"
                color: Style.colorText
                opacity: valueLabelArea.pressed ? 0.6 : 1
                font.pointSize: Fonts.bigSize
                font.weight: Font.DemiBold

                MouseArea {
                    id: valueLabelArea
                    anchors.fill: parent

                    onClicked: root.editClicked(root.index)
                }
            }

            Item {
                Layout.fillWidth: true
            }

            MButton {
                id: editButton
                Layout.preferredHeight: 37
                Layout.preferredWidth: 37
                backgroundColor: "transparent"
                opacityOnPressed: 0.8
                borderColor: "transparent"
                radius: 6
                borderColorOnPressed: Style.colorButtonBorder
                imagePath: Icons.edit
                imageSize: 23

                onClicked: root.editClicked(root.index)
            }

            MButton {
                id: deleteButton
                Layout.preferredHeight: 37
                Layout.preferredWidth: 37
                Layout.leftMargin: 13
                backgroundColor: "transparent"
                opacityOnPressed: 0.8
                borderColor: "transparent"
                radius: 6
                borderColorOnPressed: Style.colorButtonBorder
                imagePath: Icons.trash
                imageSize: 21

                onClicked: root.deleteClicked(root.shortcut)
            }
        }
    }
}

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.controllers
import Librum.style
import Librum.icons
import Librum.fonts

Item {
    id: root
    signal clicked

    Image {
        id: backgroundImage
        source: Icons.emptyHomeBackground
        sourceSize.width: parent.width
        sourceSize.height: parent.height

        ColumnLayout {
            id: layout
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -90

            Image {
                id: fileSwiftImage
                sourceSize.width: 250
                sourceSize.height: 135
                source: Icons.fileSwift
            }

            Label {
                id: addBooksQuestion
                Layout.preferredWidth: 250
                Layout.alignment: Qt.AlignHCenter
                text: "Quite empty here, what about adding your first book?"
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: Fonts.smallTitleSize
                font.weight: Font.DemiBold
                color: Style.colorLightText
                wrapMode: Text.WordWrap
            }

            MButton {
                id: addBooksButton
                Layout.preferredWidth: 134
                Layout.preferredHeight: 42
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 20
                backgroundColor: Style.colorLightHighlight
                opacityOnPressed: 0.75
                borderColor: Style.colorContainerBorder
                text: "Add book"
                textColor: Style.colorBasePurple
                fontWeight: Font.Bold
                fontSize: 13.5
                imagePath: Icons.addFirstBookPlus
                imageSize: 16

                onClicked: root.clicked()
            }
        }
    }
}

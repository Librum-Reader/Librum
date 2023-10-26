import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.icons
import CustomComponents
import Librum.fonts

Item {
    id: root
    signal clearFilters

    implicitWidth: layout.implicitWidth
    implicitHeight: layout.implicitHeight

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 20

        Label {
            id: text
            text: "No book satisfies the filter conditions"
            color: Style.colorTitle
            font.pointSize: Fonts.largeTitleSize
            font.weight: Font.Medium
        }

        MButton {
            id: removeFiltersButton
            Layout.preferredWidth: 170
            Layout.preferredHeight: 38
            Layout.alignment: Qt.AlignHCenter
            backgroundColor: Style.colorLightHighlight
            opacityOnPressed: 0.75
            borderColor: Style.colorLightPurple
            text: "Remove Filters"
            textColor: Style.colorBasePurple
            fontWeight: Font.Bold
            fontSize: Fonts.largeSize
            imagePath: Icons.cancelPurple
            imageSize: 11
            imageToRight: true

            onClicked: root.clearFilters()
        }
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.fonts

Item {
    id: root
    property string titleText: qsTr("Title")
    property string descriptionText: qsTr("Description")
    property double titleSize: Fonts.size28
    property double descriptionSize: Fonts.size14

    implicitWidth: layout.implicitWidth
    implicitHeight: layout.implicitHeight

    ColumnLayout {
        id: layout
        spacing: 0

        Label {
            id: title
            text: root.titleText
            color: Style.colorTitle
            font.weight: Font.Bold
            font.pointSize: root.titleSize
        }

        Label {
            id: description
            text: root.descriptionText
            color: Style.colorPageSubtitle
            font.pointSize: root.descriptionSize
        }
    }
}

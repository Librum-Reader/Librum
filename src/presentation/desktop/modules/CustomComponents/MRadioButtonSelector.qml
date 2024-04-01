import QtQuick
import CustomComponents
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style


/**
 A radio button selector for string items
 */
Item {
    id: root
    property var options: []
    property var displayOptions: [] // To display the values differently, e.g. due to translations
    property string currentSelected: options[0]
    signal newCurrentSelected

    implicitWidth: 120
    implicitHeight: layout.height

    ColumnLayout {
        id: layout
        width: parent.width
        spacing: 10

        Repeater {
            model: options
            delegate: MRadioButton {
                required property int index

                selected: root.currentSelected === root.options[index]
                text: root.options[index]
                displayText: root.displayOptions[index]

                onClicked: root.changeSelected(index)
            }
        }
    }

    function changeSelected(index) {
        root.currentSelected = root.options[index]
        root.newCurrentSelected()
    }
}

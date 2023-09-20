import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents

Item {
    id: root

    property alias searchingBarText: searchButton.text

    function openSearchingBar() {
        searchButton.open()
    }

    signal checkBoxClicked
    signal searchForAuthorsAndTitleTriggered(string authorsAndTitle)

    implicitWidth: 1714
    implicitHeight: 36

    RowLayout {
        id: layout
        anchors.fill: parent
        spacing: 12

        MWrappedCheckBox {
            id: checkBox

            onChecked: checkBoxClicked()
        }

        Item {
            id: widthFiller
            Layout.fillWidth: true
        }

        MSearchButton {
            id: searchButton
            expansionWidth: (widthFiller.width <= 445 ? widthFiller.width : 445)
            onTextEditingFinished: authorsAndTitle => {
                                       root.searchForAuthorsAndTitleTriggered(
                                           authorsAndTitle)
                                   }
        }
    }
}

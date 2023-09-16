import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents

Item {
    id: root
    signal checkBoxClicked
    signal searchForAuthorsAndTitleTriggered(string authorsAndTitle)
    signal setFiltersTriggered(string yearFrom, string yearTo, string language)

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

        MExplorerFilterByButton {
            id: filterByButton
            onClicked: filterByPopup.opened ? filterByPopup.close(
                                                  ) : filterByPopup.open()

            MExplorerFilterByPopup {
                id: filterByPopup
                y: filterByButton.y + filterByButton.height + internal.filterPopupTopSpacing
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                onFilterQuerySent: (yearFrom, yearTo, language) => {
                                       filterByPopup.close()
                                       resetFiltersButton.visible = true
                                       root.setFiltersTriggered(yearFrom,
                                                                yearTo,
                                                                language)
                                   }
            }
        }

        MRemoveOptionButton {
            id: resetFiltersButton
            visible: false
            text: "Remove Filters"

            onClicked: {
                // Reset filters
                visible = false
            }
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

    QtObject {
        id: internal
        property int filterPopupTopSpacing: 6
    }
}

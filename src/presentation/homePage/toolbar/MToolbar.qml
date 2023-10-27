import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.controllers
import Librum.models
import Librum.icons
import "filterByButton"
import "sortByButton"
import "tagSelector"

Item {
    id: root
    signal searchRequested(string query)
    signal checkBoxClicked

    implicitWidth: 1714
    implicitHeight: 36

    onWidthChanged: if (searchButton.opened)
                        searchButton.close()

    RowLayout {
        id: layout
        anchors.fill: parent
        spacing: 12

        MWrappedCheckBox {
            id: selectBooksCheckBox

            onChecked: checkBoxClicked()
        }

        MSortByButton {
            id: sortByButton

            onItemSelected: role => LibraryController.libraryModel.sortRole = role
        }

        MFilterByButton {
            id: filterByButton

            onFilterSelected: (authors, format, date, onlyBooks, onlyFiles, read, unread) => {
                                  LibraryController.libraryModel.setFilterRequest(
                                      authors, format, date, onlyBooks,
                                      onlyFiles, read, unread)

                                  resetFiltersButton.visible = true
                              }
        }

        MTagSelectorButton {
            id: tagSelectorButton

            onTagsSelected: resetTagsButton.visible = true
            onTagsRemoved: root.resetTags()
        }


        /*
          Button which appears when a filter is applied.
          When clicked, it clears the filters and disappears.
          */
        MRemoveOptionButton {
            id: resetFiltersButton
            visible: false
            text: "Remove Filters"

            onClicked: root.resetFilters()
        }


        /*
          Button which appears when tag filters are applied.
          When clicked, it clears the tag filters and disappears.
          */
        MRemoveOptionButton {
            id: resetTagsButton
            visible: false
            text: "Remove Tags"

            onClicked: root.resetTags()
        }

        Item {
            width: 36
            height: 36
            clip: true
            Layout.leftMargin: 2

            AnimatedImage {
                id: loadingAnimation
                anchors.centerIn: parent
                sourceSize.width: 60
                sourceSize.height: 60
                visible: AppInfoController.online && LibraryController.isSyncing
                playing: AppInfoController.online && LibraryController.isSyncing
                source: Icons.loadingAnimation
                fillMode: Image.PreserveAspectFit

                MouseArea {
                    id: loadingAnimationArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onContainsMouseChanged: containsMouse ? librarySyncingToolTip.open(
                                                                ) : librarySyncingToolTip.close()
                }
            }

            Image {
                id: isOfflineIndicator
                anchors.centerIn: parent
                visible: !AppInfoController.online
                sourceSize.width: 20
                fillMode: Image.PreserveAspectFit
                source: Icons.cloudOff

                MouseArea {
                    id: isOfflineIndicatorArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onContainsMouseChanged: containsMouse ? offlineToolTip.open(
                                                                ) : offlineToolTip.close()
                }
            }
        }

        Item {
            id: widthFiller
            Layout.fillWidth: true
        }

        MSearchButton {
            id: searchButton
            onTriggered: query => searchRequested(query)
            expansionWidth: (widthFiller.width <= 445 ? widthFiller.width : 445)

            onOpenedChanged: if (!opened)
                                 searchRequested("")
        }
    }

    MToolTip {
        id: offlineToolTip
        focusedItem: isOfflineIndicator
        content: "You are offline"
    }

    MToolTip {
        id: librarySyncingToolTip
        focusedItem: loadingAnimation
        xOffset: 16
        content: "Your library is being synchronized with the cloud"
    }

    function resetFilters() {
        LibraryController.libraryModel.setFilterRequest("", "", "", false,
                                                        false, false, false)
        filterByButton.resetFilter()
        resetFiltersButton.visible = false
    }

    function resetTags() {
        LibraryController.libraryModel.clearFilterTags()
        tagSelectorButton.clearSelections()
        resetTagsButton.visible = false
    }
}

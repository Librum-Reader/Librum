import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import CustomComponents
import Librum.elements
import Librum.style
import Librum.icons
import Librum.fonts
import Librum.controllers
import Librum.globals
import Librum.models
import "toolbar"
import "manageTagsPopup"

Page {
    id: root
    horizontalPadding: 64
    rightPadding: 70
    bottomPadding: 20
    background: Rectangle {
        anchors.fill: parent
        color: Style.colorPageBackground
    }

    Shortcut {
        sequence: SettingsController.shortcuts.AddBook
        onActivated: importFilesDialog.open()
    }

    Connections {
        target: LibraryController

        function onStorageLimitExceeded() {
            uploadLimitReachedPopup.open()
        }
    }

    DropArea {
        id: dropArea
        anchors.fill: parent

        onDropped: drop => internal.addBooks(drop.urls)

        ColumnLayout {
            id: layout
            anchors.fill: parent
            spacing: 0

            Rectangle {
                id: updateBanner
                Layout.fillWidth: true
                Layout.preferredHeight: 38
                Layout.leftMargin: -root.horizontalPadding
                Layout.rightMargin: -root.rightPadding
                visible: baseRoot.notifyAboutUpdates
                         && AppInfoController.newestVersion !== ""
                         && AppInfoController.currentVersion !== AppInfoController.newestVersion

                Rectangle {
                    anchors.fill: parent
                    color: Style.colorBannerBackground
                    opacity: 0.8
                }

                Label {
                    id: updateBannerText
                    anchors.centerIn: parent
                    text: qsTr('A new version is available!')
                          + ' <a href="update" style="color: #FFFFFF; text-decoration: underline;">'
                          + qsTr('Update Now') + '</a>'
                    onLinkActivated: baseRoot.loadSettingsUpdatesPage()
                    textFormat: Text.RichText
                    color: Style.colorBannerText
                    font.bold: true
                    font.pointSize: Fonts.size12

                    // Switch to the proper cursor when hovering above the link
                    MouseArea {
                        id: mouseArea
                        acceptedButtons: Qt.NoButton // Don't eat the mouse clicks
                        anchors.fill: parent
                        cursorShape: updateBannerText.hoveredLink
                                     != "" ? Qt.PointingHandCursor : Qt.ArrowCursor
                    }
                }

                MButton {
                    id: closeButton
                    width: 32
                    height: 32
                    anchors.right: parent.right
                    anchors.rightMargin: 6
                    anchors.verticalCenter: parent.verticalCenter
                    backgroundColor: "transparent"
                    opacityOnPressed: 0.7
                    borderColor: "transparent"
                    radius: 6
                    borderColorOnPressed: Style.colorButtonBorder
                    imagePath: Icons.closePopupWhite
                    imageSize: 12

                    onClicked: {
                        baseRoot.notifyAboutUpdates = false
                        updateBanner.visible = false
                    }
                }
            }

            RowLayout {
                id: headerRow
                Layout.fillWidth: true
                spacing: 0

                MTitle {
                    id: pageTitle
                    Layout.topMargin: updateBanner.visible ? 24 : 44
                    //: As in 'Home Page', might be closer to 'Start' in other languages
                    titleText: qsTr("Home")
                    descriptionText: qsTr("You have %1 books").arg(
                                         LibraryController.bookCount)
                }

                Item {
                    Layout.fillWidth: true
                }

                MButton {
                    id: addBooksButton
                    Layout.preferredHeight: 40
                    Layout.topMargin: 22
                    Layout.alignment: Qt.AlignBottom
                    horizontalMargins: 12
                    borderWidth: 0
                    backgroundColor: Style.colorBasePurple
                    text: qsTr("Add books")
                    textColor: Style.colorFocusedButtonText
                    fontWeight: Font.Bold
                    fontSize: Fonts.size13
                    imagePath: Icons.addWhite

                    onClicked: importFilesDialog.open()
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.maximumHeight: 45
                Layout.minimumHeight: 8
            }

            MToolbar {
                id: toolbar
                visible: !internal.libraryIsEmpty
                Layout.fillWidth: true
                z: 2

                onSearchRequested: query => LibraryController.libraryModel.sortString = query

                onCheckBoxActivated: activated => Globals.bookSelectionModeEnabled = activated
            }

            Pane {
                id: bookGridContainer
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: 100
                Layout.topMargin: 30
                visible: !internal.libraryIsEmpty && bookGrid.count != 0
                padding: 0
                background: Rectangle {
                    color: "transparent"
                }

                GridView {
                    id: bookGrid
                    anchors.fill: parent
                    cellWidth: internal.bookWidth + internal.horizontalBookSpacing
                    cellHeight: internal.bookHeight + internal.verticalBookSpacing
                    rightMargin: -internal.horizontalBookSpacing
                    interactive: true
                    boundsBehavior: Flickable.StopAtBounds
                    flickDeceleration: 15000
                    maximumFlickVelocity: 3500
                    clip: true
                    model: LibraryController.libraryModel
                    delegate: MBook {
                        id: bookDelegate

                        onLeftButtonClicked: {
                            if (model.downloaded) {
                                Globals.selectedBook = LibraryController.getBook(
                                            model.uuid)
                                internal.openBook()
                            } // Don't start downloading if downloading is already in progress.
                            else if (!bookDelegate.downloading) {
                                bookDelegate.downloading = true
                                LibraryController.downloadBookMedia(model.uuid)
                            }
                        }


                        /*
                      When right-clicking a book, open the bookOptions popup.
                      */
                        onRightButtonClicked: (index, mouse) => {
                                                  // Calculate where to spawn the bookOptions popup and set its position
                                                  let currentMousePosition = mapToItem(
                                                      bookGridContainer,
                                                      mouse.x, mouse.y)
                                                  let absoluteMousePosition = mapToItem(
                                                      root, mouse.x, mouse.y)

                                                  if (Globals.bookSelectionModeEnabled) {
                                                      bookMultiSelectOptionsPopup.setSpawnPosition(
                                                          currentMousePosition,
                                                          absoluteMousePosition,
                                                          root)
                                                  } else {
                                                      bookOptionsPopup.setSpawnPosition(
                                                          currentMousePosition,
                                                          absoluteMousePosition,
                                                          root)
                                                  }

                                                  // Open the bookOptions
                                                  internal.openBookOptionsPopup(
                                                      model)
                                              }


                        /*
                      When clicking more options, open the bookOptions popup
                      */
                        onMoreOptionClicked: (index, mouse) => {
                                                 // Calculate where to spawn the bookOptions popup and set its position
                                                 let currentMousePosition = mapToItem(
                                                     bookGridContainer,
                                                     mouse.x, mouse.y)

                                                 bookOptionsPopup.x = currentMousePosition.x
                                                 - bookOptionsPopup.implicitWidth / 2
                                                 bookOptionsPopup.y = currentMousePosition.y
                                                 - bookOptionsPopup.implicitHeight - 6

                                                 // Open the bookOptions
                                                 internal.openBookOptionsPopup(
                                                     model)
                                             }
                    }


                    /*
                  The options menu when e.g. right-clicking a book
                  */
                    MBookRightClickPopup {
                        id: bookOptionsPopup

                        onDownloadClicked: {
                            close()
                        }

                        onReadBookClicked: {
                            internal.openBook()
                        }

                        onBookDetailsClicked: {
                            bookDetailsPopup.open()
                            close()
                        }

                        onSaveToFilesClicked: {
                            downloadFileDialog.open()
                            close()
                        }

                        onManageTagsClicked: {
                            manageTagsPopup.open()
                            close()
                        }

                        onMarkAsReadClicked: {
                            close()
                        }

                        onRemoveClicked: {
                            acceptDeletionPopup.open()
                            close()
                        }
                    }


                    /*
                  The options menu when e.g. right-clicking a book while multi selection is enabled
                  */
                    MBookMultiSelectRightClickPopup {
                        id: bookMultiSelectOptionsPopup

                        onMarkAsReadClicked: {

                            toolbar.selectBooksCheckBoxActivated = false
                            close()
                        }

                        onUninstallClicked: {
                            for (var i = 0; i < Globals.selectedBooks.length; i++) {
                                LibraryController.uninstallBook(
                                            Globals.selectedBooks[i])
                            }

                            toolbar.selectBooksCheckBoxActivated = false
                            close()
                        }

                        onDeleteClicked: {
                            for (var i = 0; i < Globals.selectedBooks.length; i++) {
                                LibraryController.deleteBook(
                                            Globals.selectedBooks[i])
                            }

                            toolbar.selectBooksCheckBoxActivated = false
                            close()
                        }
                    }
                }

                ScrollBar {
                    id: verticalScrollbar
                    width: pressed ? 14 : 12
                    hoverEnabled: true
                    active: true
                    policy: ScrollBar.AlwaysOff
                    visible: bookGrid.contentHeight > bookGrid.height
                    orientation: Qt.Vertical
                    size: bookGrid.height / bookGrid.contentHeight
                    minimumSize: 0.04
                    position: (bookGrid.contentY - bookGrid.originY) / bookGrid.contentHeight
                    onPositionChanged: if (pressed)
                                           bookGrid.contentY = position
                                                   * bookGrid.contentHeight + bookGrid.originY
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.rightMargin: -20
                    anchors.bottomMargin: 16
                    anchors.bottom: parent.bottom
                    horizontalPadding: 4

                    contentItem: Rectangle {
                        color: Style.colorScrollBarHandle
                        opacity: verticalScrollbar.pressed ? 0.8 : 1
                        radius: 4
                    }

                    background: Rectangle {
                        implicitWidth: 26
                        implicitHeight: 200
                        color: "transparent"
                    }
                }
            }

            MEmptyScreenContent {
                id: emptyScreenContent
                visible: internal.libraryIsEmpty
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.topMargin: 32

                onClicked: importFilesDialog.open()
            }

            MNoBookSatisfiesFilterItem {
                id: noBookSatisfiesFilterItem
                Layout.alignment: Qt.AlignHCenter
                Layout.leftMargin: -sidebar.width
                Layout.topMargin: Math.round(root.height / 3) - implicitHeight
                visible: bookGrid.count == 0
                         && LibraryController.bookCount !== 0

                onClearFilters: {
                    toolbar.resetFilters()
                    toolbar.resetTags()
                }
            }

            Item {
                id: bottomHeightFillter
                Layout.fillHeight: true
            }
        }
    }

    MWarningPopup {
        id: acceptDeletionPopup
        x: Math.round(
               root.width / 2 - implicitWidth / 2 - sidebar.width / 2 - root.horizontalPadding)
        y: Math.round(
               root.height / 2 - implicitHeight / 2 - root.topPadding - 50)
        visible: false
        title: qsTr("Remove Book?")
        message: qsTr("Deleting a book is a permanent action, no one will be\n able to restore it afterwards!")
        leftButtonText: qsTr("Remove from Device")
        rightButtonText: qsTr("Delete Everywhere")
        messageBottomSpacing: 10
        rightButtonRed: true

        onOpenedChanged: if (opened)
                             acceptDeletionPopup.giveFocus()
        onDecisionMade: close()
        onLeftButtonClicked: LibraryController.uninstallBook(
                                 Globals.selectedBook.uuid)
        onRightButtonClicked: {
            let uuid = Globals.selectedBook.uuid
            let projectGutenbergId = Globals.selectedBook.projectGutenbergId
            let status = LibraryController.deleteBook(uuid)
            if (status === BookOperationStatus.Success) {
                FreeBooksController.unmarkBookAsDownloaded(projectGutenbergId)
            }
        }
    }

    MBookDetailsPopup {
        id: bookDetailsPopup
        x: Math.round(
               root.width / 2 - implicitWidth / 2 - sidebar.width / 2 - root.horizontalPadding)
        y: Math.round(
               root.height / 2 - implicitHeight / 2 - root.topPadding - 30)
    }

    MManageTagsPopup {
        id: manageTagsPopup
        x: Math.round(
               root.width / 2 - implicitWidth / 2 - sidebar.width / 2 - root.horizontalPadding)
        y: Math.round(
               root.height / 2 - implicitHeight / 2 - root.topPadding - 30)
    }

    FolderDialog {
        id: downloadFileDialog
        acceptLabel: qsTr("Save")
        options: FolderDialog.ShowDirsOnly
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)

        onAccepted: LibraryController.saveBookToFile(Globals.selectedBook.uuid,
                                                     folder)
    }

    MWarningPopup {
        id: uploadLimitReachedPopup
        x: Math.round(
               root.width / 2 - implicitWidth / 2 - sidebar.width / 2 - root.horizontalPadding)
        y: Math.round(
               root.height / 2 - implicitHeight / 2 - root.topPadding - 50)
        visible: false
        title: qsTr("Limit Reached")
        message: qsTr("You have reached your upload limit.\nDelete unused books to free up space or upgrade.")
        leftButtonText: qsTr("Ok")
        rightButtonText: qsTr("Upgrade")
        messageBottomSpacing: 16
        minButtonWidth: 180
        onOpenedChanged: if (opened)
                             uploadLimitReachedPopup.giveFocus()
        onDecisionMade: close()
    }

    MWarningPopup {
        id: bookAlreadyExistsPopup
        x: Math.round(
               root.width / 2 - implicitWidth / 2 - sidebar.width / 2 - root.horizontalPadding)
        y: Math.round(
               root.height / 2 - implicitHeight / 2 - root.topPadding - 50)
        visible: false
        title: qsTr("Book already exists")
        message: qsTr("It looks like this book already exists in your library:")
                 + "<br>" + "<font color='" + Style.colorBasePurple + "'>"
                 + internal.lastAddedBookPath.split("/").slice(
                     -1)[0] + "</font> <br>" + qsTr(
                     "Are you sure you that want to add it again?\n")
        richText: true
        leftButtonText: qsTr("Add")
        rightButtonText: qsTr("Don't add")
        messageBottomSpacing: 16
        minButtonWidth: 180
        onOpenedChanged: if (opened)
                             bookAlreadyExistsPopup.giveFocus()
        onDecisionMade: {
            close()
            internal.continueAddingBooks()
        }

        onLeftButtonClicked: LibraryController.addBook(
                                 internal.lastAddedBookPath, true)
    }

    MWarningPopup {
        id: unsupportedFilePopup
        x: Math.round(
               root.width / 2 - implicitWidth / 2 - sidebar.width / 2 - root.horizontalPadding)
        y: Math.round(
               root.height / 2 - implicitHeight / 2 - root.topPadding - 50)
        visible: false
        title: qsTr("Unsupported File")
        message: qsTr("Oops! This file is not supported by Librum.")
        leftButtonText: qsTr("Ok")
        messageBottomSpacing: 24
        singleButton: true

        onOpenedChanged: if (opened)
                             giveFocus()

        onDecisionMade: {
            close()
            internal.continueAddingBooks()
        }
    }

    FileDialog {
        id: importFilesDialog
        acceptLabel: qsTr("Import")
        fileMode: FileDialog.FileMode.OpenFiles
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        nameFilters: [qsTr(
                "All files") + " (*)", "PDF " + qsTr("files") + " (*.pdf)", "EPUB " + qsTr(
                "files") + " (*.epub)", "MOBI " + qsTr("files") + " (*.mobi)", "HTML " + qsTr(
                "files") + " (*.html *.htm)", "Text " + qsTr("files") + " (*.txt)"]

        onAccepted: internal.addBooks(files)
    }

    Keys.onPressed: event => {
                        if (event.key === Qt.Key_Control) {
                            toolbar.selectBooksCheckBoxActivated = true
                            event.accepted = true
                        } else if (event.key === Qt.Key_Escape) {
                            toolbar.selectBooksCheckBoxActivated = false
                            event.accepted = true
                        }
                    }

    Keys.onReleased: event => {
                         if (event.key === Qt.Key_Control) {
                             toolbar.selectBooksCheckBoxActivated = false
                             event.accepted = true
                         }
                     }

    Component.onDestruction: toolbar.selectBooksCheckBoxActivated = false

    QtObject {
        id: internal
        property bool libraryIsEmpty: LibraryController.bookCount === 0
        property int bookWidth: 190
        property int bookHeight: 300
        property int horizontalBookSpacing: 64
        property int verticalBookSpacing: 48
        property var booksCurrentlyAdding: []
        property string lastAddedBookPath: ""

        function openBookOptionsPopup(item) {
            Globals.selectedBook = LibraryController.getBook(item.uuid)
            Globals.bookTags = Qt.binding(function () {
                return item.tags
            })

            if (Globals.bookSelectionModeEnabled)
                bookMultiSelectOptionsPopup.open()
            else
                bookOptionsPopup.open()
        }

        function openBook() {
            if (bookOptionsPopup.opened)
                bookOptionsPopup.close()
            else if (bookMultiSelectOptionsPopup.opened)
                bookMultiSelectOptionsPopup.close()

            BookController.setUp(Globals.selectedBook.uuid)

            LibraryController.refreshLastOpenedFlag(Globals.selectedBook.uuid)
            loadPage(readingPage)
        }

        // This function adds books to the library. When errors happen in the process,
        // it interrupts the adding and shows a popup. After the popup is closed, the
        // adding is resumed by calling continueAddingBooks().
        function addBooks(container) {
            internal.booksCurrentlyAdding = container
            for (var i = container.length - 1; i >= 0; i--) {
                internal.lastAddedBookPath = container[i]
                let result = LibraryController.addBook(
                        internal.lastAddedBookPath)

                // Remove the already added book
                container.splice(i, 1)

                if (result === BookOperationStatus.OpeningBookFailed) {
                    unsupportedFilePopup.open()
                    return
                }

                if (result === BookOperationStatus.BookAlreadyExists) {
                    bookAlreadyExistsPopup.open()
                    return
                }
            }
        }

        // When an error occurs while adding multiple books, this method is called
        // after the error was dealt with to continue adding the rest of the books.
        function continueAddingBooks() {
            internal.addBooks(internal.booksCurrentlyAdding)
        }
    }
}

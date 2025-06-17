import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import CustomComponents
import Librum.style
import Librum.elements
import Librum.controllers
import Librum.globals
import "readingToolbar"
import "readingSearchbar"

Page {
    id: root
    background: Rectangle {
        anchors.fill: parent
        color: Style.colorPageBackground
    }

    // Prevent right-alignment
    LayoutMirroring.enabled: false
    LayoutMirroring.childrenInherit: true

    Component.onCompleted: root.forceActiveFocus()
    Component.onDestruction: internal.saveCurrentPage()

    // Save the current page every 5s automatically
    Timer {
        id: currentPageSaver
        interval: 5000
        repeat: true
        running: true

        onTriggered: internal.saveCurrentPage()
    }

    Shortcut {
        id: nextPage
        sequences: [SettingsController.shortcuts.NextPage]
        onActivated: documentView.nextPage()
    }

    Shortcut {
        id: previousPage
        sequences: [SettingsController.shortcuts.PreviousPage]
        onActivated: documentView.previousPage()
    }

    Shortcut {
        id: stopFullScreenMode
        sequences: [SettingsController.shortcuts.ExitFullScreenMode]
        onActivated: internal.stopFullScreenMode()
        enabled: internal.fullScreen
    }

    Shortcut {
        id: startOfDocument
        sequences: [SettingsController.shortcuts.StartOfDocument]
        onActivated: internal.goToStart()
    }

    Shortcut {
        id: endOfDocument
        sequences: [SettingsController.shortcuts.EndOfDocument]
        onActivated: internal.goToEnd()
    }

    Shortcut {
        id: createBookmark
        sequences: [SettingsController.shortcuts.CreateBookmarkHere]
        onActivated: {
            if (chapterSidebar.active)
                chapterSidebar.close()

            if (!bookmarksSidebar.active)
                bookmarksSidebar.open()

            bookmarksSidebar.createNewBookmark()
        }
    }


    /*
      An invisible component at the top of the screen, when hovering over it
      while the fullscreen mode is enabled, the fullscreen mode will be disabled.
      */
    Item {
        id: toolbarReactivationContainer
        width: parent.width
        height: toolbar.height
        z: 1

        HoverHandler {
            id: toolbarReactivationArea
            enabled: true
            onHoveredChanged: if (hovered)
                                  internal.stopFullScreenMode()
        }
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 0

        MReadingToolBar {
            id: toolbar
            Layout.fillWidth: true
            pageCount: BookController.pageCount
            bookTitle: Globals.selectedBook.title

            onBackButtonClicked: {
                loadPage(homePage, sidebar.homeItem, false)
            }

            onChapterButtonClicked: {
                if (chapterSidebar.active) {
                    chapterSidebar.close()
                    return
                }

                if (bookmarksSidebar.active)
                    bookmarksSidebar.close()

                chapterSidebar.open()
            }

            onBookMarkButtonClicked: {
                if (bookmarksSidebar.active) {
                    bookmarksSidebar.close()
                    return
                }

                if (chapterSidebar.active)
                    chapterSidebar.close()

                bookmarksSidebar.open()
            }

            onFullScreenButtonClicked: {
                if (internal.fullScreen)
                    internal.stopFullScreenMode()
                else
                    internal.startFullScreenMode()
            }

            onSearchButtonClicked: {
                searchbar.visible = !searchbar.visible
            }

            onOptionsPopupVisibileChanged: {
                optionsButton.active = !optionsButton.active
            }

            onZoomChanged: newZoom => {
                               documentView.setZoom(newZoom)
                           }

            PropertyAnimation {
                id: hideToolbar
                target: toolbar
                property: "opacity"
                to: 0
                duration: 100

                onFinished: toolbar.visible = false
            }

            PropertyAnimation {
                id: showToolbar
                target: toolbar
                property: "opacity"
                to: 1
                duration: 200

                onStarted: toolbar.visible = true
            }
        }


        /*
          Contains the reizeable sidebars, e.g. chapter or book-sidebar
          */
        SplitView {
            id: sidebarSplitView
            Layout.fillHeight: true
            Layout.fillWidth: true
            orientation: Qt.Horizontal
            padding: 0
            spacing: 10
            handle: Rectangle {
                implicitWidth: 8
                color: "transparent"
            }
            smooth: true


            /*
              Need to combine sidebar items into one item, else rezising doesn't
              work properly, since the SplitView thinks they are 3 different items
              */
            Item {
                id: sidebarItem
                visible: chapterSidebar.active || bookmarksSidebar.active
                // Load in the last width of the correct sidebar
                SplitView.preferredWidth: chapterSidebar.visible ? chapterSidebar.lastWidth : bookmarksSidebar.visible ? bookmarksSidebar.lastWidth : 0
                SplitView.minimumWidth: chapterSidebar.visible
                                        || bookmarksSidebar.visible ? 140 : 0
                SplitView.maximumWidth: 480

                MChapterSidebar {
                    id: chapterSidebar
                    property int lastWidth: 370
                    property bool active: false
                    anchors.fill: parent
                    visible: false
                    model: BookController.tableOfContents

                    // Save the last width to restore it if re-enabled
                    onVisibleChanged: if (!visible)
                                          lastWidth = width
                    onSwitchPage: (pageNumber, yOffset) => documentView.setPage(
                                      pageNumber, yOffset)

                    Rectangle {
                        id: rightChaptersBorder
                        width: 1
                        height: parent.height
                        color: Style.colorDarkSeparator
                        anchors.right: parent.right
                    }

                    function open() {
                        chapterSidebar.active = true
                        chapterSidebar.visible = true
                        toolbar.chapterButton.active = true
                    }

                    function close() {
                        chapterSidebar.active = false
                        chapterSidebar.visible = false
                        toolbar.chapterButton.active = false
                        documentView.forceActiveFocus()
                    }
                }

                MBookmarksSidebar {
                    id: bookmarksSidebar
                    property int lastWidth: 370
                    property bool active: false

                    anchors.fill: parent
                    visible: false

                    // Save the last width to restore it if re-enabled
                    onVisibleChanged: if (!visible)
                                          lastWidth = width

                    Rectangle {
                        id: rightBookmarksBorder
                        width: 1
                        height: parent.height
                        color: Style.colorDarkSeparator
                        anchors.right: parent.right
                    }

                    function open() {
                        bookmarksSidebar.active = true
                        bookmarksSidebar.visible = true
                        toolbar.bookmarksButton.active = true
                    }

                    function close() {
                        bookmarksSidebar.active = false
                        bookmarksSidebar.visible = false
                        toolbar.bookmarksButton.active = false
                    }
                }
            }

            RowLayout {
                id: documentLayout
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                spacing: 0
                clip: true

                MDocumentView {
                    id: documentView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    bookController: BookController
                }
            }
        }

        MReadingSearchbar {
            id: searchbar
            visible: false
            bookController: BookController
            Layout.fillWidth: true

            onVisibleChanged: toolbar.searchButton.active = visible
            onSearchQueried: if (query.length)
                                 BookController.search(query)
            onClearQuery: BookController.clearSearch()
            onNextButtonClicked: BookController.goToNextSearchHit()
            onPreviousButtonClicked: BookController.goToPreviousSearchHit()
        }
    }

    function getYOffset() {
        let yOffset = documentView.getYOffset()
        let restoredYOffset = yOffset / BookController.zoom

        return restoredYOffset
    }

    QtObject {
        id: internal
        property bool fullScreen: false
        property int prevCurrentPage: -1

        Component.onCompleted: prevCurrentPage = documentView.documentView.currentPage

        function startFullScreenMode() {
            if (internal.fullScreen)
                return

            internal.fullScreen = true
            hideToolbar.start()
        }

        function stopFullScreenMode() {
            if (!internal.fullScreen)
                return

            internal.fullScreen = false
            showToolbar.start()
        }

        function goToStart() {
            documentView.setPage(0)
        }

        function goToEnd() {
            documentView.setPage(BookController.pageCount - 1)
        }

        function saveCurrentPage() {
            let currentPage = documentView.documentView.currentPage
            if (currentPage === internal.prevCurrentPage)
                return

            var operationsMap = {}
            operationsMap[LibraryController.MetaProperty.CurrentPage] = currentPage
            LibraryController.updateBook(Globals.selectedBook.uuid,
                                         operationsMap)
            internal.prevCurrentPage = currentPage
        }
    }
}

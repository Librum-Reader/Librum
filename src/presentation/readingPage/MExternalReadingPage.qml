import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import CustomComponents
import Librum.style
import Librum.elements
import Librum.controllers
import Librum.globals
import "externalReadingToolbar"
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

    Shortcut {
        id: zoomIn
        // sequences: root.loggedIn ? [SettingsController.shortcuts.ZoomIn] :
        onActivated: documentView.changeZoomBy(1.13)
    }

    Shortcut {
        id: zoomOut
        sequences: [SettingsController.shortcuts.ZoomOut]
        onActivated: documentView.changeZoomBy(0.87)
    }

    Shortcut {
        id: flickUp
        sequences: [SettingsController.shortcuts.MoveUp]
        onActivated: documentView.flick("up")
    }

    Shortcut {
        id: flickDown
        sequences: [SettingsController.shortcuts.MoveDown]
        onActivated: documentView.flick("down")
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

        MExternalReadingToolBar {
            id: toolbar
            Layout.fillWidth: true
            currentPage: ExternalBookController.currentPage
            pageCount: ExternalBookController.pageCount
            bookTitle: "Some"

            onBackButtonClicked: {
                loadPage(homePage, sidebar.homeItem, false)

                baseRoot.externalBookMode = false
            }

            onChapterButtonClicked: {
                if (chapterSidebar.active) {
                    chapterSidebar.close()
                    return
                }

                chapterSidebar.open()
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
                visible: chapterSidebar.active
                // Load in the last width of the correct sidebar
                SplitView.preferredWidth: chapterSidebar.visible ? chapterSidebar.lastWidth : 0
                SplitView.minimumWidth: chapterSidebar.visible ? 140 : 0
                SplitView.maximumWidth: 480

                MChapterSidebar {
                    id: chapterSidebar
                    property int lastWidth: 370
                    property bool active: false
                    anchors.fill: parent
                    visible: false
                    model: ExternalBookController.tableOfContents

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
                    bookController: ExternalBookController
                }
            }
        }

        MReadingSearchbar {
            id: searchbar
            visible: false
            Layout.fillWidth: true
            bookController: ExternalBookController

            // onVisibleChanged: toolbar.searchButton.active = visible
            onSearchQueried: if (query.length)
                                 ExternalBookController.search(query)
            onClearQuery: ExternalBookController.clearSearch()
            onNextButtonClicked: ExternalBookController.goToNextSearchHit()
            onPreviousButtonClicked: ExternalBookController.goToPreviousSearchHit()
        }
    }

    function getYOffset() {
        let yOffset = documentView.getYOffset()
        let restoredYOffset = yOffset / ExternalBookController.zoom

        return restoredYOffset
    }

    QtObject {
        id: internal
        property bool fullScreen: false

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
            documentView.setPage(ExternalBookController.pageCount - 1)
        }
    }
}

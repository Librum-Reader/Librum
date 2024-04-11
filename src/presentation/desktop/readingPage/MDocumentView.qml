import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.elements
import Librum.style
import Librum.globals
import Librum.controllers
import "DocumentNavigation.js" as NavigationLogic


/*
  A view on the document's pages in a certain layout (e.g. vertical)
  */
Pane {
    id: root
    property var lastSelectedPage
    signal clicked
    signal zoomFactorChanged(real factor)
    property var bookController

    padding: 0
    background: Rectangle {
        color: "transparent"
    }
    // Disable pressing tab to focus other elements
    Keys.onTabPressed: event => {
                           event.accepted = true
                       }

    Component.onCompleted: root.bookController.zoom
                           = SettingsController.appearanceSettings.DefaultZoom / 100
    Component.onDestruction: root.bookController.zoom = 1

    Connections {
        target: root.bookController

        function onGoToPosition(pageNumber, y) {
            root.setPage(pageNumber, y - pageView.currentItem.yOffset)
        }

        function onTextSelectionFinished(centerX, topY) {
            selectionOptionsPopup.highlight = ""

            internal.openSelectionOptionsPopup(centerX, topY)
        }

        function onHighlightSelected(centerX, topY, highlightUuid) {
            // Remove selection if there is one when selecting a highlight
            activeFocusItem.removeSelection()
            selectionOptionsPopup.highlight = highlightUuid

            internal.openSelectionOptionsPopup(centerX, topY)
        }

        function onZoomChanged(newZoom) {
            let normMaxWidth = pageView.widestItem / pageView.prevZoom
            pageView.widestItem = normMaxWidth * root.bookController.zoom

            pageView.prevZoom = root.bookController.zoom
        }
    }

    Connections {
        target: DictionaryController

        function onGettingDefinitionFailed() {}

        function onStartedGettingDefinition(word) {
            if (!dictionaryPopup.opened) {
                dictionaryPopup.x = Qt.binding(function () {
                    return root.width / 2 - dictionaryPopup.width / 2
                })
                dictionaryPopup.y = Qt.binding(function () {
                    return root.height / 2 - dictionaryPopup.height / 2
                })
                dictionaryPopup.open()
            }

            dictionaryPopup.word = word
        }
    }

    Timer {
        id: hideCursorTimer
        interval: SettingsController.behaviorSettings.HideCursorAfterDelay
        repeat: true
        running: true

        onTriggered: {
            if (SettingsController.behaviorSettings.CursorMode
                    !== internal.optionNameCursorModeHiddenAfterDelay) {
                return
            }

            pageView.currentItem.resetCursorToDefault()
            mouseArea.cursorShape = Qt.BlankCursor
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        // Handle scrolling customly
        onWheel: NavigationLogic.handleWheel(wheel)

        onPositionChanged: internal.showCursor()

        onPressed: mouse.accepted = false
        onReleased: mouse.accepted = false

        ListView {
            id: pageView
            readonly property int scrollSpeed: 5500
            property int pageSpacing: pageView.getPageSpacing(
                                          root.bookController.zoom)
            property int widestItem: 0
            property real prevZoom: 1

            height: parent.height
            width: contentWidth <= root.width ? contentWidth : root.width
            contentWidth: pageView.widestItem
            anchors.centerIn: parent
            flickableDirection: Flickable.AutoFlickDirection
            flickDeceleration: 150000
            interactive: false
            clip: true
            cacheBuffer: 1000
            maximumFlickVelocity: scrollSpeed
            boundsMovement: Flickable.StopAtBounds
            boundsBehavior: Flickable.StopAtBounds
            model: root.bookController.pageCount
            spacing: pageSpacing
            delegate: PageView {
                id: page
                pageNumber: modelData
                bookController: root.bookController
                height: implicitHeight
                width: implicitWidth
                colorInverted: root.bookController.colorTheme === "Inverted"
                includeNewLinesInCopiedText: SettingsController.behaviorSettings.IncludeNewLinesInCopiedText === "ON"
                anchors.horizontalCenter: if (parent != null)
                                              parent.horizontalCenter

                Component.onCompleted: {
                    if (implicitWidth > pageView.contentWidth)
                        pageView.widestItem = page.implicitWidth
                }

                onMouseHoverMoved: internal.showCursor()
            }

            // Set the book's current page once the model is loaded
            onContentYChanged: {
                NavigationLogic.updateCurrentPageCounter()
                selectionOptionsPopup.close()
            }

            onContentXChanged: selectionOptionsPopup.close()

            Component.onCompleted: root.setPage(root.bookController.currentPage)

            function getPageSpacing(zoom) {
                return Math.round(
                            SettingsController.appearanceSettings.PageSpacing
                            * (zoom + 0.4 * (1 - zoom)))
            }
        }
    }

    ScrollBar {
        id: verticalScrollbar
        width: pressed ? 14 : 12
        hoverEnabled: true
        active: true
        policy: ScrollBar.AlwaysOn
        orientation: Qt.Vertical
        size: pageView.height / pageView.contentHeight
        minimumSize: 0.04
        position: (pageView.contentY - pageView.originY) / pageView.contentHeight
        onPositionChanged: if (pressed)
                               pageView.contentY = position
                                       * pageView.contentHeight + pageView.originY
        anchors.top: parent.top
        anchors.right: parent.right
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
            color: verticalScrollbar.pressed
                   || verticalScrollbar.hovered ? Style.colorContainerBackground : "transparent"
        }
    }

    ScrollBar {
        id: horizontalScrollbar
        height: hovered ? 12 : 10
        hoverEnabled: true
        active: true
        policy: ScrollBar.AlwaysOn
        visible: pageView.contentWidth > pageView.width
        orientation: Qt.Horizontal
        size: pageView.width / pageView.contentWidth
        minimumSize: 0.04
        position: (pageView.contentX - pageView.originX) / pageView.contentWidth
        onPositionChanged: pageView.contentX = position * pageView.contentWidth + pageView.originX
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        horizontalPadding: 4

        contentItem: Rectangle {
            color: Style.colorScrollBarHandle
            opacity: horizontalScrollbar.pressed ? 0.8 : 1
            radius: 4
        }

        background: Rectangle {
            implicitWidth: 26
            implicitHeight: 200
            color: "transparent"
        }
    }

    MSelectionOptionsPopup {
        id: selectionOptionsPopup
        property real highlightCenterX
        property real highlightBottomY

        onNewWidth: internal.openSelectionOptionsPopup(-1, -1)

        onExplanationOptionSelected: text => {
                                         explanationPopup.question = text
                                         explanationPopup.open()
                                     }
    }

    MDictionaryPopup {
        id: dictionaryPopup
    }

    MExplanationPopup {
        id: explanationPopup
        x: root.width / 2 - explanationPopup.width / 2
        y: root.height / 2 - explanationPopup.height / 2
    }

    function zoom(factor) {
        NavigationLogic.zoom(factor)
    }

    function changeZoomBy(factor) {
        let newZoomFactor = root.bookController.zoom * factor
        NavigationLogic.zoom(newZoomFactor)
    }

    function flick(direction) {
        let up = direction === "up"
        NavigationLogic.flick(0, (pageView.scrollSpeed / 1.4) * (up ? 1 : -1))
    }

    function nextPage() {
        // Prevent trying to go over the end
        let newPage = root.bookController.currentPage + 1
        if (newPage > root.bookController.pageCount - 1)
            return

        NavigationLogic.setPage(root.bookController.currentPage + 1)
    }

    function previousPage() {
        NavigationLogic.setPage(root.bookController.currentPage - 1)
    }

    function setPage(pageNumber, yOffset = 0) {
        NavigationLogic.setPage(pageNumber)

        pageView.contentY += yOffset * root.bookController.zoom
    }

    function getYOffset() {
        let currentPageHeight = pageView.currentItem.height + pageView.getPageSpacing(
                root.bookController.zoom)
        let currentPos = pageView.contentY - pageView.originY
        return currentPos - (currentPageHeight * root.bookController.currentPage)
    }

    QtObject {
        id: internal
        property string optionNameCursorModeHiddenAfterDelay: "Hidden after delay"

        function openSelectionOptionsPopup(centerX, bottomY) {
            if (centerX === -1 && bottomY === -1) {
                centerX = selectionOptionsPopup.highlightCenterX
                bottomY = selectionOptionsPopup.highlightBottomY
            }
            selectionOptionsPopup.highlightCenterX = centerX
            selectionOptionsPopup.highlightBottomY = bottomY

            internal.openPopupAt(selectionOptionsPopup, centerX, bottomY)
        }

        function openPopupAt(popup, centerX, bottomY) {
            let pageYOffset = pageView.contentY - activeFocusItem.y
            let pageXOffset = pageView.contentX - activeFocusItem.x

            let posY = bottomY + -pageYOffset + 6
            let spaceToBottom = (pageView.y + root.height) - (posY + popup.height)
            if (spaceToBottom < 0)
                posY = posY + spaceToBottom

            let posX = centerX + pageView.x - popup.width / 2 - pageXOffset
            let spaceToRight = (pageView.x + pageView.width) - (posX + popup.width)
            if (spaceToRight < 0)
                posX = posX + spaceToRight

            let spaceToLeft = (pageView.x + pageView.width) - (posX + popup.width)

            popup.x = posX
            popup.y = posY

            popup.open()
        }

        function showCursor() {
            mouseArea.cursorShape = Qt.ArrowCursor
            hideCursorTimer.restart()
        }
    }
}

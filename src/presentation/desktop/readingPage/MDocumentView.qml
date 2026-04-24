import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.elements
import Librum.style
import Librum.globals
import Librum.controllers

Pane {
    id: root
    property var openedBookController
    property var lastSelectedPage

    signal zoomChanged(real zoom)
    signal mouseMoved(real x, real y)

    // Read only access to the document's properties to allow bindings
    readonly property alias currentPage: document.currentPage
    readonly property alias currentZoom: document.currentZoom
    readonly property alias tableOfContents: document.tableOfContents
    readonly property alias documentSearcher: document.documentSearcher
    readonly property alias topPage: document.topPage

    padding: 0
    background: Rectangle {
        color: "transparent"
    }
    // Disable pressing tab to focus other elements
    Keys.onTabPressed: event => {
                           event.accepted = true
                       }

    Connections {
        target: root.openedBookController

        // When bookmark was clicked
        function onGoToBookmarkPosition(pageNumber, y) {
            setPage(pageNumber, y * document.currentZoom)
        }
    }

    Document {
        id: document
        height: parent.height
        width: Math.min(implicitWidth, parent.width)
        anchors.centerIn: parent
        openedBookController: root.openedBookController
        settingsController: SettingsController
        colorInverted: root.openedBookController.colorTheme === "Inverted"
        includeNewLinesInCopiedText: SettingsController.behaviorSettings.IncludeNewLinesInCopiedText
                                     === "ON"
        clip: true

        onCurrentZoomChanged: root.zoomChanged(document.currentZoom)
        onMouseMoved: (x, y) => root.mouseMoved(x, y)

        // Close the selection popup when moving
        onContentYChanged: selectionOptionsPopup.close()

        onTextSelectionFinished: (centerX, topY) => {
                                     selectionOptionsPopup.highlight = ""

                                     internal.openSelectionOptionsPopup(
                                         centerX, topY)
                                 }

        onHighlightSelected: (centerX, topY, highlightUuid) => {
                                 // Remove selection if there is one when selecting a highlight
                                 activeFocusItem.removeSelection()
                                 selectionOptionsPopup.highlight = highlightUuid

                                 internal.openSelectionOptionsPopup(centerX,
                                                                    topY)
                             }

        Component.onCompleted: document.forceActiveFocus()
    }

    ScrollBar {
        id: verticalScrollbar
        width: pressed ? 14 : 12
        hoverEnabled: true
        active: true
        policy: ScrollBar.AlwaysOn
        orientation: Qt.Vertical
        size: document.height / document.contentHeight
        minimumSize: 0.04
        position: document.contentY / document.contentHeight
        onPositionChanged: if (pressed)
                               document.contentY = position * document.contentHeight
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
        visible: document.contentWidth > document.width
        orientation: Qt.Horizontal
        size: document.width / document.contentWidth
        minimumSize: 0.04
        position: document.contentX / document.contentWidth
        onPositionChanged: document.contentX = position * document.contentWidth
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

        onTranslationOptionSelected: text => {
                                         translationPopup.input = text.replace(
                                             /\r|\n/g, '')
                                         translationPopup.open()
                                     }

        onDictionaryOptionSelected: text => {
                                        dictionaryPopup.open()
                                        dictionaryPopup.lookupWord(text)
                                    }
    }

    MDictionaryPopup {
        id: dictionaryPopup
        x: root.width / 2 - dictionaryPopup.width / 2
        y: root.height / 2 - dictionaryPopup.height / 2
    }

    MExplanationPopup {
        id: explanationPopup
        x: root.width / 2 - explanationPopup.width / 2
        y: root.height / 2 - explanationPopup.height / 2
    }

    MTranslationPopup {
        id: translationPopup
        x: root.width / 2 - translationPopup.width / 2
        y: root.height / 2 - translationPopup.height / 2
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
            let pageYOffset = document.contentY - activeFocusItem.y
            let pageXOffset = document.contentX - activeFocusItem.x

            let posY = bottomY + -pageYOffset + 6
            let spaceToBottom = (document.y + root.height) - (posY + popup.height)
            if (spaceToBottom < 0)
                posY = posY + spaceToBottom

            let posX = centerX + document.x - popup.width / 2 - pageXOffset
            let spaceToRight = (document.x + document.width) - (posX + popup.width)
            if (spaceToRight < 0)
                posX = posX + spaceToRight

            let spaceToLeft = (document.x + document.width) - (posX + popup.width)

            popup.x = posX
            popup.y = posY

            popup.open()
        }
    }

    function setPage(pageNumber, offsetY = 0) {
        document.currentPage = pageNumber

        if (offsetY !== 0) {
            let newContentY = document.contentY + offsetY
            document.contentY = newContentY
        }
    }

    function setZoom(newZoom) {
        document.currentZoom = newZoom
    }

    function getTopPage() {
        return document.topPage
    }

    function getTopPageYOffset() {
        return document.topPageYOffset
    }

    function removeSelectionsFromPage(pageNumber) {
        document.removeSelectionFromPage(pageNumber)
    }

    function restoreCursor() {
        document.restoreCursor()
    }

    function hideCursor() {
        document.hideCursor()
    }
}

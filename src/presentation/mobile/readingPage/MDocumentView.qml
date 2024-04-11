import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.fonts
import Librum.globals
import Librum.elements
import Librum.controllers

Pane {
    id: root
    padding: 0
    background: Rectangle {
        color: "transparent"
    }

    // Rectangle {
    //     width: 30
    //     height: 30
    //     color: "green"
    //     x: point1.x
    //     y: point1.y
    //     z: 3
    // }

    // Rectangle {
    //     width: 30
    //     height: 30
    //     color: "yellow"
    //     x: point2.x
    //     y: point2.y
    //     z: 3
    // }
    ListView {
        id: pageView
        readonly property int scrollSpeed: 5500
        property int pageSpacing: pageView.getPageSpacing(BookController.zoom)
        property int widestItem: 0
        property real prevZoom: 1

        height: parent.height
        width: contentWidth <= root.width ? contentWidth : root.width
        contentWidth: pageView.widestItem
        anchors.centerIn: parent
        flickableDirection: Flickable.AutoFlickDirection
        flickDeceleration: 150000
        interactive: true
        clip: true
        cacheBuffer: 0
        maximumFlickVelocity: scrollSpeed
        boundsMovement: Flickable.StopAtBounds
        boundsBehavior: Flickable.StopAtBounds
        model: BookController.pageCount
        spacing: pageSpacing
        delegate: PageView {
            id: page
            pageNumber: modelData
            bookController: BookController
            height: implicitHeight
            width: implicitWidth
            colorInverted: BookController.colorTheme === "Inverted"
            includeNewLinesInCopiedText: SettingsController.behaviorSettings.IncludeNewLinesInCopiedText === "ON"
            anchors.horizontalCenter: if (parent != null)
                                          parent.horizontalCenter

            Component.onCompleted: {
                if (implicitWidth > pageView.contentWidth)
                    pageView.widestItem = page.implicitWidth
            }
        }

        PinchArea {
            property real initialWidth
            property real initialHeight

            anchors.fill: parent
            z: 3
            enabled: true

            onPinchStarted: {
                initialWidth = pageView.contentWidth
                initialHeight = pageView.contentHeight
            }

            onPinchUpdated: pinch => {
                                root.zoom(BookController.zoom * pinch.scale)
                            }
        }

        // MultiPointTouchArea {
        //     anchors.fill: parent
        //     z: 3
        //     mouseEnabled: false
        //     enabled: true
        //     minimumTouchPoints: 2

        //     touchPoints: [
        //         TouchPoint {
        //             id: point1
        //         },
        //         TouchPoint {
        //             id: point2
        //         }
        //     ]

        //     onUpdated: {

        //     }
        // }
        function getPageSpacing(zoom) {
            return Math.round(SettingsController.appearanceSettings.PageSpacing
                              * (zoom + 0.4 * (1 - zoom)))
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

    function getYOffset() {
        let currentPageHeight = pageView.currentItem.height + pageView.getPageSpacing(
                BookController.zoom)
        let currentPos = pageView.contentY - pageView.originY
        return currentPos - (currentPageHeight * BookController.currentPage)
    }

    function zoom(newZoomFactor) {
        // Clamp to max / min zoom factors
        newZoomFactor = Math.max(0.15, Math.min(newZoomFactor, 10))
        if (newZoomFactor === BookController.zoom)
            return

        if (!newZoomFactor > BookController.zoom + 0.5
                && !newZoomFactor < BookController.zoom - 0.5) {
            return
        }

        print("Zooming to " + newZoomFactor)

        let defaultPageHeight = Math.round(
                pageView.currentItem.height / BookController.zoom)
        let newPageHeight = Math.round(
                defaultPageHeight * newZoomFactor) + pageView.getPageSpacing(
                newZoomFactor)

        let pageOffset = getYOffset()

        BookController.zoom = newZoomFactor
        pageView.forceLayout()
        pageView.contentY = 0
    }
}

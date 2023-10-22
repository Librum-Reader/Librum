import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.controllers
import Librum.style
import Librum.icons
import Librum.fonts
import CustomComponents
import "explorerToolbar"

Page {
    id: root
    background: Rectangle {
        anchors.fill: parent
        color: Style.colorPageBackground
    }

    Connections {
        target: FreeBooksController

        function onFoundNoBooks() {
            errorMessageLabel.text = "No books were found"
            errorMessageLabel.visible = true
        }

        function onFetchingFirstMetadataPageSuccessful(success) {
            if (!success) {
                errorMessageLabel.text
                        = "Couldn't load free books. Please, check your network connection"
                errorMessageLabel.visible = true
            }

            loadingAnimation.playing = false
            loadingAnimation.visible = false
        }

        function onStartedFetchingFistMetadataPage() {
            loadingAnimation.playing = true
            loadingAnimation.visible = true
        }
    }

    Component.onCompleted: {
        if (FreeBooksController.getFilterAuthorsAndTitle() !== "") {
            toolbar.openSearchingBar()
            toolbar.searchingBarText = FreeBooksController.getFilterAuthorsAndTitle()
        }
        FreeBooksController.fetchFirstBooksMetadataPage()
    }

    AnimatedImage {
        id: loadingAnimation
        anchors.centerIn: parent
        playing: false
        visible: false
        source: Icons.loadingAnimation
        width: 120
        fillMode: Image.PreserveAspectFit
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        anchors.rightMargin: internal.windowRightMargin
        anchors.leftMargin: internal.windowLeftMargin
        spacing: 0

        MTitle {
            id: pageTitle
            Layout.topMargin: 44
            titleText: "Free books"
            descriptionText: "Choose from over 70,000 books"
        }

        MExplorerToolbar {
            id: toolbar
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft
            Layout.topMargin: 45
            onSearchForAuthorsAndTitleTriggered: authorsAndTitle => {
                                                     errorMessageLabel.visible = false
                                                     FreeBooksController.setFilterAuthorsAndTitle(
                                                         authorsAndTitle)
                                                     FreeBooksController.fetchFirstBooksMetadataPage()
                                                 }
        }

        Label {
            id: errorMessageLabel
            Layout.alignment: Qt.AlignHCenter
            Layout.leftMargin: -sidebar.width
            Layout.topMargin: Math.round(root.height / 3) - implicitHeight
            color: Style.colorTitle
            font.pointSize: Fonts.largeTitleSize
            font.weight: Font.Medium
            visible: false
        }

        Pane {
            id: bookGridContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 30
            visible: !root.empty
            padding: 0
            background: Rectangle {
                color: "transparent"
            }

            GridView {
                id: bookGrid
                anchors.fill: parent
                cellWidth: internal.bookWidth + internal.horizontalBookSpacing
                cellHeight: internal.bookHeight + internal.verticalBookSpacing
                // Negative margin removes the extra spacing at the right of the grid
                rightMargin: -internal.horizontalBookSpacing
                interactive: true
                boundsBehavior: Flickable.StopAtBounds
                flickDeceleration: 19500
                maximumFlickVelocity: 3300
                clip: true

                cacheBuffer: 1000
                model: FreeBooksController.freeBooksModel
                delegate: MFreeBook {
                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            downloadBookPopup.bookId = model.id
                            downloadBookPopup.title = model.title
                            downloadBookPopup.authors = model.authors
                            downloadBookPopup.languages = model.languages
                            downloadBookPopup.cover = model.cover
                            downloadBookPopup.downloadCount = model.downloadCount
                            downloadBookPopup.downloadLink = model.mediaDownloadLink
                            downloadBookPopup.open()
                        }
                    }

                    Component.onCompleted: FreeBooksController.getBookCover(
                                               model.id)
                    Component.onDestruction: FreeBooksController.deleteBookCover(
                                                 model.id)
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
    }

    MDownloadBookPopup {
        id: downloadBookPopup

        x: Math.round(
               root.width / 2 - implicitWidth / 2 - sidebar.width / 2 - root.horizontalPadding)
        y: Math.round(
               root.height / 2 - implicitHeight / 2 - root.topPadding - 30)
    }

    QtObject {
        id: internal
        property int windowLeftMargin: 64
        property int windowRightMargin: 70

        property int bookWidth: 190
        property int bookHeight: 300
        property int horizontalBookSpacing: 64
        property int verticalBookSpacing: 48
    }
}

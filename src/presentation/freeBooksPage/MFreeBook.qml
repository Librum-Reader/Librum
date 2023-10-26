import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.icons
import CustomComponents
import Librum.fonts

Item {
    id: root

    implicitWidth: 190
    implicitHeight: 304

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 0


        /**
          A rectangle with rounded corners which is overlapping with the top half of
          the book to create a rounded top, while the rest of the book is rectangluar
          */
        Rectangle {
            id: upperBookPartRounding
            Layout.preferredHeight: 16
            Layout.fillWidth: true
            radius: 4
            color: Style.colorBookImageBackground
        }


        /**
          An overlay over the upper-book-rounding to get it to be transparent and modal,
          when the book is already downloaded.
          */
        Item {
            id: upperBookRoundingDimmer
            Layout.topMargin: -upperBookPartRounding.height - 6
            Layout.preferredHeight: 12
            Layout.fillWidth: true
            visible: model.downloaded
            clip: true
            z: 2

            Rectangle {
                id: dimmerRect
                height: upperBookPartRounding.height
                width: upperBookPartRounding.width
                color: Style.colorBookCoverDim
                opacity: 0.5
                radius: 4
            }
        }


        /**
          The upper book half which contains the book cover
          */
        Rectangle {
            id: bookCoverRect
            Layout.fillWidth: true
            Layout.preferredHeight: 230
            Layout.topMargin: -5
            color: Style.colorBookImageBackground
            clip: true

            MProgressBar {
                id: downloadProgressBar
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 1
                anchors.rightMargin: 1
                visible: false
                z: 3
                progress: model.mediaDownloadProgress
                onProgressChanged: {
                    if (progress === 1)
                        visible = false
                    else
                        visible = true
                }
            }

            Rectangle {
                id: bookCoverDimmer
                anchors.fill: parent
                visible: model.downloaded
                color: Style.colorBookCoverDim
                opacity: 0.5
                z: 2
            }

            Image {
                id: alreadyDownloadedIndicator
                anchors.centerIn: bookCoverDimmer
                visible: model.downloaded
                sourceSize.width: 52
                fillMode: Image.PreserveAspectFit
                source: Icons.checkWhite
                opacity: 1
                z: 3
            }

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 0

                Image {
                    id: bookCover
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: -8
                    sourceSize.height: 241
                    source: model.cover
                    fillMode: Image.PreserveAspectFit
                }


                /*
                  The item displaying when no book cover exists (usually a ".format" label)
                 */
                Label {
                    id: noImageLabel
                    Layout.alignment: Qt.AlignCenter
                    visible: bookCover.source == ""
                    text: ".jpeg"
                    color: Style.colorNoImageLabel
                    font.pointSize: Fonts.veryBigTitleSize
                    font.bold: true
                }
            }
        }


        /**
          The lower book half which contains the book information
          */
        Rectangle {
            id: informationRect
            Layout.fillWidth: true
            Layout.preferredHeight: 65
            color: Style.colorBookBackground
            border.width: 1
            border.color: Style.colorBookBorder

            ColumnLayout {
                id: informationRectLayout
                width: parent.width - internal.paddingInsideBook * 2
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 0

                Label {
                    id: bookName
                    Layout.fillWidth: true
                    Layout.preferredHeight: 32
                    Layout.topMargin: 6
                    text: model.title
                    font.weight: Font.Medium
                    color: Style.colorTitle
                    font.pointSize: Fonts.baseSize
                    lineHeight: 0.8
                    elide: Label.ElideRight
                    wrapMode: TextInput.WordWrap
                }

                Label {
                    id: authors
                    Layout.fillWidth: true
                    Layout.topMargin: 5
                    text: model.authors
                    color: Style.colorLightText
                    font.pointSize: Fonts.smallSize
                    elide: Label.ElideRight
                }
            }
        }
    }

    QtObject {
        id: internal
        property int paddingInsideBook: 14
    }

    function giveFocus() {
        root.forceActiveFocus()
    }
}

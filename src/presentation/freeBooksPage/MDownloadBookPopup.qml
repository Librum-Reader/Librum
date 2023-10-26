import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.controllers
import Librum.style
import Librum.icons
import Librum.fonts

Popup {
    id: root
    property int bookId
    property string title
    property string authors
    property string languages
    property var cover
    property int downloadCount
    property string downloadLink

    implicitWidth: 751
    implicitHeight: layout.height
    padding: 0
    background: Rectangle {
        radius: 6
        color: Style.colorPopupBackground
    }

    modal: true
    Overlay.modal: Rectangle {
        color: Style.colorPopupDim
        opacity: 1
    }

    onOpened: downloadButton.forceActiveFocus()

    MFlickWrapper {
        id: flickWrapper
        anchors.fill: parent
        contentHeight: layout.height

        ColumnLayout {
            id: layout
            width: parent.width
            spacing: 0

            MButton {
                id: closePopupButton
                Layout.preferredHeight: 32
                Layout.preferredWidth: 32
                Layout.topMargin: 12
                Layout.rightMargin: 14
                Layout.alignment: Qt.AlignRight
                backgroundColor: "transparent"
                borderColor: "transparent"
                opacityOnPressed: 0.7
                radius: 6
                borderColorOnPressed: Style.colorButtonBorder
                imagePath: Icons.closePopup
                imageSize: 14

                onClicked: root.close()
            }

            Pane {
                id: content
                Layout.fillWidth: true
                horizontalPadding: 52
                bottomPadding: 42
                background: Rectangle {
                    color: "transparent"
                    radius: 6
                }

                ColumnLayout {
                    id: contentLayout
                    width: parent.width
                    spacing: 0

                    Label {
                        id: pageTitle
                        Layout.topMargin: 6
                        text: "Download book"
                        font.weight: Font.Bold
                        font.pointSize: Fonts.bigTitleSize
                        color: Style.colorTitle
                    }

                    RowLayout {
                        id: bookInformationLayout
                        spacing: 28
                        Layout.fillWidth: true
                        Layout.topMargin: 32

                        Rectangle {
                            id: bookCoverArea
                            Layout.preferredWidth: 198
                            Layout.preferredHeight: 258
                            color: Style.colorBookImageBackground
                            radius: 4
                            clip: true

                            Image {
                                id: bookCover
                                anchors.centerIn: parent
                                Layout.alignment: Qt.AlignHCenter
                                sourceSize.height: bookCoverArea.height - 2
                                source: root.cover !== undefined ? root.cover : ""
                                fillMode: Image.PreserveAspectFit
                            }
                        }

                        ScrollView {
                            id: bookInformation
                            Layout.preferredHeight: 263
                            Layout.fillWidth: true
                            Layout.topMargin: -4
                            contentWidth: width
                            clip: true
                            ScrollBar.vertical.policy: ScrollBar.AlwaysOn

                            // contentItem is the underlying flickable of ScrollView
                            Component.onCompleted: {
                                contentItem.boundsBehavior = Flickable.StopAtBounds
                                contentItem.maximumFlickVelocity = 600
                            }

                            ColumnLayout {
                                id: bookDetails
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.rightMargin: 16
                                spacing: 16

                                MLabeledInputBox {
                                    id: titleField
                                    Layout.fillWidth: true
                                    boxHeight: 34
                                    headerText: "Title"
                                    headerFontWeight: Font.Bold
                                    headerFontSize: Fonts.bigSize
                                    text: root.title
                                    headerToBoxSpacing: 3
                                    inputFontSize: Fonts.bigSize
                                    inputFontColor: Style.colorReadOnlyInputText
                                    textPadding: 12
                                    borderWidth: 1
                                    borderRadius: 4
                                    readOnly: true
                                }

                                MLabeledInputBox {
                                    id: authorField
                                    Layout.fillWidth: true
                                    boxHeight: 34
                                    headerText: "Authors"
                                    headerFontWeight: Font.Bold
                                    headerFontSize: Fonts.bigSize
                                    text: root.authors
                                    headerToBoxSpacing: 3
                                    inputFontSize: Fonts.bigSize
                                    inputFontColor: Style.colorReadOnlyInputText
                                    textPadding: 12
                                    borderWidth: 1
                                    borderRadius: 4
                                    readOnly: true
                                }

                                MLabeledInputBox {
                                    id: languageField
                                    Layout.fillWidth: true
                                    boxHeight: 34
                                    headerText: "Language"
                                    headerFontWeight: Font.Bold
                                    headerFontSize: Fonts.bigSize
                                    text: root.languages
                                    headerToBoxSpacing: 3
                                    inputFontSize: Fonts.bigSize
                                    inputFontColor: Style.colorReadOnlyInputText
                                    textPadding: 12
                                    borderWidth: 1
                                    borderRadius: 4
                                    readOnly: true
                                }

                                MLabeledInputBox {
                                    id: publicationField
                                    Layout.fillWidth: true
                                    boxHeight: 34
                                    headerText: "Downloads"
                                    headerFontWeight: Font.Bold
                                    headerFontSize: Fonts.bigSize
                                    text: root.downloadCount
                                    headerToBoxSpacing: 3
                                    inputFontSize: Fonts.bigSize
                                    inputFontColor: Style.colorReadOnlyInputText
                                    textPadding: 12
                                    borderWidth: 1
                                    borderRadius: 4
                                    readOnly: true
                                }

                                MLabeledInputBox {
                                    id: formatField
                                    Layout.fillWidth: true
                                    Layout.bottomMargin: 3
                                    boxHeight: 34
                                    headerText: "Format"
                                    headerFontWeight: Font.Bold
                                    headerFontSize: Fonts.bigSize
                                    text: "EPUB"
                                    headerToBoxSpacing: 3
                                    inputFontSize: Fonts.bigSize
                                    inputFontColor: Style.colorReadOnlyInputText
                                    textPadding: 12
                                    borderWidth: 1
                                    borderRadius: 4
                                    readOnly: true
                                }
                            }
                        }
                    }

                    RowLayout {
                        id: buttonsLayout
                        Layout.topMargin: 42
                        spacing: 16


                        /*
                          Download button, the color/border changes if the button is focused
                          */
                        MButton {
                            id: downloadButton
                            Layout.preferredWidth: 140
                            Layout.preferredHeight: 38
                            active: true
                            text: "Download"
                            textColor: active ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                            fontWeight: Font.Bold
                            fontSize: Fonts.bigSize
                            borderWidth: active ? 0 : 1
                            backgroundColor: active ? Style.colorBasePurple : "transparent"
                            imagePath: active ? Icons.downloadSelected : Icons.download
                            imageSize: 18

                            onClicked: {
                                internal.downloadBook()
                                root.close()
                            }
                            Keys.onReturnPressed: internal.downloadBook()
                            Keys.onRightPressed: internal.giveFocusToCancelButton()
                            Keys.onTabPressed: internal.giveFocusToCancelButton(
                                                   )
                        }

                        MButton {
                            id: cancelButton
                            Layout.preferredWidth: 140
                            Layout.preferredHeight: 38
                            borderWidth: active ? 0 : 1
                            backgroundColor: active ? Style.colorBasePurple : "transparent"
                            opacityOnPressed: 0.7
                            text: "Cancel"
                            textColor: active ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                            fontWeight: Font.Bold
                            fontSize: Fonts.bigSize

                            onClicked: root.close()
                            Keys.onReturnPressed: root.close()
                            Keys.onLeftPressed: internal.giveFocusToDownloadButton()
                            Keys.onTabPressed: internal.giveFocusToDownloadButton()
                        }
                    }
                }
            }
        }
    }

    QtObject {
        id: internal

        function downloadBook() {
            FreeBooksController.getBookMedia(root.bookId, root.downloadLink)
        }

        function giveFocusToCancelButton() {
            downloadButton.active = false
            cancelButton.active = true
            cancelButton.giveFocus()
        }

        function giveFocusToDownloadButton() {
            cancelButton.active = false
            downloadButton.active = true
            downloadButton.giveFocus()
        }
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons
import Librum.controllers
import Librum.fonts

Item {
    id: root
    signal searchQueried(string query)
    signal clearQuery
    signal nextButtonClicked
    signal previousButtonClicked

    implicitHeight: 48
    implicitWidth: 1000

    // Make sure to remove the focus from the textinput when the searchbar is closed
    onVisibleChanged: {
        if (visible) {
            inputField.forceActiveFocus()
        } else {
            root.clearQuery()
            inputField.clear()
            inputField.previousText = ""
            root.forceActiveFocus()
            optionsPopup.close()
        }
    }

    // Close on pressing escape
    Keys.onPressed: event => {
                        if (event.key === Qt.Key_Escape) {
                            root.visible = false
                            event.accepted = true
                        }
                    }

    Connections {
        target: BookController
        function onNoSearchHitsFound() {
            internal.setSearchError()
        }
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: topBorder
            color: Style.colorDarkSeparator
            Layout.preferredHeight: 1
            Layout.fillWidth: true
        }

        Pane {
            id: container
            Layout.fillHeight: true
            Layout.fillWidth: true
            padding: 8
            background: Rectangle {
                color: Style.colorContainerBackground
            }

            RowLayout {
                id: contentLayout
                spacing: 12
                anchors.fill: parent

                MButton {
                    id: closeButton
                    Layout.preferredWidth: 32
                    Layout.preferredHeight: 30
                    backgroundColor: Style.colorHighlight
                    opacityOnPressed: 0.8
                    borderWidth: 0
                    radius: 4
                    imagePath: Icons.readingSearchbarCancel
                    imageSize: 12

                    onClicked: root.visible = false
                }

                MButton {
                    id: optionsButton
                    Layout.preferredWidth: 82
                    Layout.preferredHeight: 30
                    text: qsTr("Options")
                    fontSize: Fonts.size12
                    textColor: Style.colorUnfocusedButtonText
                    fontWeight: Font.Normal
                    backgroundColor: Style.colorHighlight
                    opacityOnPressed: 0.8
                    borderWidth: 0
                    radius: 4

                    onClicked: optionsPopup.opened ? optionsPopup.close(
                                                         ) : optionsPopup.open()
                }

                Pane {
                    id: inputFieldContainer
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    horizontalPadding: 12
                    verticalPadding: 0
                    background: Rectangle {
                        color: Style.colorControlBackground
                        border.color: Style.colorContainerBorder
                        border.width: 1
                        radius: 4
                    }

                    RowLayout {
                        id: inputFieldLayout
                        anchors.fill: parent
                        spacing: 6

                        Image {
                            id: searchIcon
                            source: Icons.readingSearchbarSearch
                            sourceSize.width: 14
                            fillMode: Image.PreserveAspectFit
                        }

                        TextField {
                            id: inputField
                            property string previousText

                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignVCenter
                            leftPadding: 4
                            selectByMouse: true
                            color: Style.colorBaseInputText
                            font.pointSize: Fonts.size12
                            placeholderText: qsTr("Find")
                            placeholderTextColor: Style.colorPlaceholderText
                            background: Rectangle {
                                anchors.fill: parent
                                color: "transparent"
                            }

                            onTextEdited: internal.resetSearchError()

                            Keys.onReturnPressed: {
                                // When clicking Enter without changing the text, go to the next hit
                                if (previousText === text) {
                                    root.nextButtonClicked()
                                    return
                                }

                                root.searchQueried(text)
                                previousText = text
                            }
                        }
                    }
                }

                MButton {
                    id: nextButton
                    Layout.preferredWidth: 81
                    Layout.preferredHeight: 30
                    text: qsTr("Next")
                    fontSize: Fonts.size12
                    textColor: Style.colorUnfocusedButtonText
                    fontWeight: Font.Normal
                    backgroundColor: Style.colorHighlight
                    opacityOnPressed: 0.8
                    borderWidth: 0
                    radius: 4
                    imagePath: Icons.readingSearchbarNext
                    imageSize: 13
                    imageSpacing: 10

                    onClicked: root.nextButtonClicked()
                }

                MButton {
                    id: previousButton
                    Layout.preferredWidth: 110
                    Layout.preferredHeight: 30
                    text: qsTr("Previous")
                    fontSize: Fonts.size12
                    textColor: Style.colorUnfocusedButtonText
                    fontWeight: Font.Normal
                    backgroundColor: Style.colorHighlight
                    opacityOnPressed: 0.8
                    borderWidth: 0
                    radius: 4
                    imagePath: Icons.readingSearchbarPrevious
                    imageSize: 13
                    imageSpacing: 10

                    onClicked: root.previousButtonClicked()
                }
            }
        }
    }

    MReadingSearchbarOptionsPopup {
        id: optionsPopup
        x: optionsButton.x + 8
        y: -optionsPopup.height - 1

        onSettingsChanged: if (inputField.text !== "")
                               root.searchQueried(inputField.text)
    }

    QtObject {
        id: internal
        property bool error: false
        property color previousTextColor: inputField.color
        property color previousBackgroundColor: inputFieldContainer.background.color
        property color previousBorderColor: inputFieldContainer.background.border.color

        function setSearchError() {
            previousTextColor = inputField.color
            previousBackgroundColor = inputFieldContainer.background.color
            previousBorderColor = inputFieldContainer.background.border.color

            inputField.color = Style.colorErrorText
            inputFieldContainer.background.color = Style.colorErrorBackground
            inputFieldContainer.background.border.color = Style.colorErrorBorder

            error = true
        }

        function resetSearchError() {
            if (!error)
                return

            inputField.color = previousTextColor
            inputFieldContainer.background.color = previousBackgroundColor
            inputFieldContainer.background.border.color = previousBorderColor

            error = false
        }
    }

    function giveFocus() {
        inputField.forceActiveFocus()
    }
}

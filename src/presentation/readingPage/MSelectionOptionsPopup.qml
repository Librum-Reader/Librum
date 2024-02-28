import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.controllers
import Librum.fonts

Popup {
    id: root
    property string highlight: ""
    signal newWidth
    signal highlightOptionSelected(string uuid)
    signal dictionaryOptionSelected(string word)
    signal explanationOptionSelected(string word)

    width: selectionOptionsListView.width
    height: 32
    padding: 0
    background: Rectangle {
        color: Style.colorControlBackground
        radius: 4
        border.width: 1
        border.color: Style.colorContainerBorder
    }

    onOpened: selectionOptionsListView.model
              = (baseRoot.externalBookMode ? externalBookActionPage : firstActionsPage)
    onClosed: root.highlight = ""

    Shortcut {
        sequence: SettingsController.shortcuts.CreateHighlight
        onActivated: {
            internal.createHighlight()
            root.close()
        }
    }

    Shortcut {
        sequence: SettingsController.shortcuts.RemoveHighlight
        onActivated: {
            if (root.highlight != "")
                activeFocusItem.removeHighlight(root.highlight)

            root.close()
        }
    }

    Image {
        id: triangleDecoration
        x: parent.width / 2 - implicitWidth / 2
        y: parent.y + parent.height - 1
        source: Icons.popupDroplet
        rotation: 180
    }

    ListView {
        id: selectionOptionsListView
        property bool firstTimeOpened: true

        width: contentWidth
        height: parent.height
        orientation: ListView.Horizontal
        spacing: 2
        model: baseRoot.externalBookMode ? externalBookActionPage : firstActionsPage
        boundsBehavior: Flickable.StopAtBounds

        onModelChanged: {
            // Prevent the selection popup being opened on initialization
            if (firstTimeOpened) {
                firstTimeOpened = false
                return
            }

            selectionOptionsListView.forceLayout()
            root.newWidth()
        }
    }

    component SelectionOptionsPopupItem: Rectangle {
        id: action
        property string text
        property color textColor: Style.colorText
        property var clickedFunction: function () {}

        height: selectionOptionsListView.height
        implicitWidth: actionText.implicitWidth
        color: "transparent"
        opacity: actionArea.pressed ? 0.8 : 1

        Label {
            id: actionText
            height: parent.height
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            padding: 10
            text: action.text
            color: action.textColor
            font.pointSize: Fonts.size12
        }

        MouseArea {
            id: actionArea
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            hoverEnabled: true

            onClicked: {
                action.clickedFunction()

                root.close()
            }
        }
    }

    component Separator: Rectangle {
        y: 1 // Move one down
        height: selectionOptionsListView.height - 2
        implicitWidth: 2
        color: Style.colorSeparator
    }

    ObjectModel {
        id: firstActionsPage

        SelectionOptionsPopupItem {
            id: copyAction
            text: qsTr("Copy")
            clickedFunction: function () {
                if (root.highlight == "")
                    activeFocusItem.copySelectedText()
                else
                    activeFocusItem.copyHighlightedText(root.highlight)
            }
        }

        Separator {}

        SelectionOptionsPopupItem {
            id: highlightAction
            text: qsTr("Highlight")
            visible: !baseRoot.externalBookMode
            clickedFunction: function () {
                // We do not want to create a new highlight when clicking on
                // one and selecting the "Highlight" option to edit it.
                let uuid = root.highlight
                if (root.highlight == "") {
                    uuid = internal.createHighlight()
                }

                root.highlightOptionSelected(uuid)
            }
        }

        Separator {}

        SelectionOptionsPopupItem {
            id: lookUpAction
            text: qsTr("Look Up")
            clickedFunction: function () {
                let text = ""
                if (root.highlight == "")
                    text = activeFocusItem.getSelectedText()
                else
                    text = activeFocusItem.getHighlightedText(root.highlight)

                // Removing any . or , from the start and the end of the word
                text = text.replace(/^[,.]+|[,.]+$/g, '')

                // Make the first letter lower case. When the word is at the start of a sentence,
                // the first letter will be upper case, which in turn causes the dictionary to fail.
                const firstLetter = text.charAt(0).toLowerCase()
                const restOfString = text.slice(1)
                // Get the rest of the string
                text = firstLetter + restOfString

                DictionaryController.getDefinitionForWord(text)
                root.dictionaryOptionSelected(text)
            }
        }

        Separator {}

        Rectangle {
            height: selectionOptionsListView.height
            width: 26
            color: "transparent"

            RowLayout {
                anchors.fill: parent

                Image {
                    id: goToPage2Button
                    Layout.alignment: Qt.AlignCenter
                    Layout.rightMargin: 4
                    source: Icons.arrowheadNextIcon
                }
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked: selectionOptionsListView.model = secondActionsPage
            }
        }
    }

    ObjectModel {
        id: secondActionsPage

        Rectangle {
            height: selectionOptionsListView.height
            width: 26
            color: "transparent"

            RowLayout {
                anchors.fill: parent

                Image {
                    id: goToPage1Button
                    Layout.alignment: Qt.AlignCenter
                    Layout.leftMargin: 2
                    rotation: 180
                    source: Icons.arrowheadNextIcon
                }
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked: selectionOptionsListView.model = firstActionsPage
            }
        }

        Separator {}

        SelectionOptionsPopupItem {
            id: explainAction
            text: qsTr("Explain")
            clickedFunction: function () {
                let text = ""
                if (root.highlight == "")
                    text = activeFocusItem.getSelectedText()
                else
                    text = activeFocusItem.getHighlightedText(root.highlight)

                root.explanationOptionSelected(text)
            }
        }

        Separator {
            visible: root.highlight != ""
            width: visible ? implicitWidth : 0
        }

        SelectionOptionsPopupItem {
            id: removeAction
            width: visible ? implicitWidth : 0
            text: qsTr("Remove")
            textColor: Style.colorErrorText
            visible: root.highlight != ""

            clickedFunction: function () {
                activeFocusItem.removeHighlight(root.highlight)
            }
        }
    }

    ObjectModel {
        id: externalBookActionPage

        SelectionOptionsPopupItem {
            text: qsTr("Copy")
            clickedFunction: function () {
                if (root.highlight == "")
                    activeFocusItem.copySelectedText()
                else
                    activeFocusItem.copyHighlightedText(root.highlight)
            }
        }

        Separator {}

        SelectionOptionsPopupItem {
            text: qsTr("Look Up")
            clickedFunction: function () {
                let text = ""
                if (root.highlight == "")
                    text = activeFocusItem.getSelectedText()
                else
                    text = activeFocusItem.getHighlightedText(root.highlight)

                // Removing any . or , from the start and the end of the word
                text = text.replace(/^[,.]+|[,.]+$/g, '')

                // Make the first letter lower case. When the word is at the start of a sentence,
                // the first letter will be upper case, which in turn causes the dictionary to fail.
                const firstLetter = text.charAt(0).toLowerCase()
                const restOfString = text.slice(1)
                // Get the rest of the string
                text = firstLetter + restOfString

                DictionaryController.getDefinitionForWord(text)
                root.dictionaryOptionSelected(text)
            }
        }

        Separator {}

        SelectionOptionsPopupItem {
            text: qsTr("Explain")
            clickedFunction: function () {
                let text = ""
                if (root.highlight == "")
                    text = activeFocusItem.getSelectedText()
                else
                    text = activeFocusItem.getHighlightedText(root.highlight)

                root.explanationOptionSelected(text)
            }
        }
    }

    QtObject {
        id: internal

        function createHighlight() {
            let defaultColorName = SettingsController.appearanceSettings.DefaultHighlightColorName
            let uuid = activeFocusItem.createHighlightFromCurrentSelection(
                    SettingsController.appearanceSettings[defaultColorName],
                    SettingsController.appearanceSettings.HighlightOpacity)

            return uuid
        }
    }
}

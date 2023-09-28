import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.controllers

Popup {
    id: root
    property string highlight: ""
    signal highlightOptionSelected(string uuid)
    signal dictionaryOptionSelected(string word)

    width: internal.getWidth()
    height: 32
    padding: 0
    background: Rectangle {
        color: Style.colorControlBackground
        radius: 4
        border.width: 1
        border.color: Style.colorContainerBorder
    }

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

    RowLayout {
        id: selectionOptionsLayout
        height: parent.height
        // We need to manually calculate the width because QML bugs lead to the RowLayout not being
        // updated properly when an item inside of it changes its visibility.
        spacing: 2

        component SelectionOptionsPopupItem: Rectangle {
            id: action
            property string text
            property color textColor: Style.colorText
            property var clickedFunction: function () {}

            Layout.fillHeight: true
            Layout.preferredWidth: actionText.implicitWidth
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
                font.pointSize: 12
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
            Layout.fillHeight: true
            Layout.preferredWidth: 2
            color: Style.colorSeparator
        }

        SelectionOptionsPopupItem {
            id: copyAction
            text: "Copy"
            clickedFunction: function () {
                if (root.highlight == "")
                    activeFocusItem.copySelectedText()
                else
                    activeFocusItem.copyHighlightedText(root.highlight)
            }
        }

        Separator {
            id: separator1
        }

        SelectionOptionsPopupItem {
            id: highlightAction
            text: "Highlight"
            clickedFunction: function () {
                let uuid = internal.createHighlight()
                root.highlightOptionSelected(uuid)
            }
        }

        Separator {}

        SelectionOptionsPopupItem {
            id: lookUpAction
            text: "Look Up"
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
            id: explainAction
            text: "Explain"
            clickedFunction: function () {
                let text = ""
                if (root.highlight == "")
                    text = activeFocusItem.getSelectedText()
                else
                    text = activeFocusItem.getHighlightedText(root.highlight)

                AiExplanationController.getExplanation(text)
                // root.dictionaryOptionSelected(text)
            }
        }

        Separator {
            visible: root.highlight != ""
        }

        SelectionOptionsPopupItem {
            id: removeAction
            text: "Remove"
            textColor: Style.colorErrorText
            visible: root.highlight != ""
            clickedFunction: function () {
                activeFocusItem.removeHighlight(root.highlight)
            }
        }
    }

    QtObject {
        id: internal

        function getWidth() {
            return separator1.width * 4 + selectionOptionsLayout.spacing * 7
                    + copyAction.width + highlightAction.width + lookUpAction.width
                    + explainAction.width + (root.highlight === "" ? 0 : removeAction.width)
        }

        function createHighlight() {
            let defaultColorName = SettingsController.appearanceSettings.DefaultHighlightColorName
            let uuid = activeFocusItem.createHighlightFromCurrentSelection(
                    SettingsController.appearanceSettings[defaultColorName],
                    SettingsController.appearanceSettings.HighlightOpacity)

            return uuid
        }
    }
}

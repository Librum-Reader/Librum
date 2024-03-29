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
    signal dictionaryOptionSelected(string word)
    signal explanationOptionSelected(string word)

    padding: 0
    width: layout.width
    height: layout.height
    background: Rectangle {
        color: Style.colorControlBackground
        radius: 4
        border.width: 1
        border.color: Style.colorContainerBorder
    }

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
        y: -5 - root.verticalPadding
        source: Icons.popupDroplet
    }

    ColumnLayout {
        id: layout
        spacing: 6

        RowLayout {
            id: highlightRow
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.topMargin: 8
            spacing: 0

            component ColorItem: Rectangle {
                id: colorItem
                property string colorName
                Layout.preferredHeight: 26
                Layout.preferredWidth: 26
                color: "transparent"

                Rectangle {
                    width: 15
                    height: width
                    anchors.centerIn: parent
                    radius: width
                    color: SettingsController.appearanceSettings[colorItem.colorName]
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onContainsMouseChanged: activeFocusItem.setPointingCursor()

                    onClicked: {
                        let uuid = root.highlight
                        if (root.highlight == "") {
                            uuid = internal.createHighlight()
                        }

                        // Change highlight color
                        activeFocusItem.changeHighlightColor(
                                    uuid,
                                    SettingsController.appearanceSettings[colorItem.colorName],
                                    SettingsController.appearanceSettings.HighlightOpacity)

                        // Remember the last color used
                        SettingsController.setSetting(
                                    SettingKeys.DefaultHighlightColorName,
                                    colorItem.colorName,
                                    SettingGroups.Appearance)

                        root.close()
                    }
                }
            }

            ColorItem {
                colorName: "HighlightColorA"
            }

            ColorItem {
                colorName: "HighlightColorB"
            }

            ColorItem {
                colorName: "HighlightColorC"
            }

            ColorItem {
                colorName: "HighlightColorD"
            }

            ColorItem {
                colorName: "HighlightColorE"
            }
        }

        Label {
            id: copyText
            Layout.fillWidth: true
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            Layout.topMargin: -2
            text: "Copy"
            color: Style.colorText
            font.weight: Font.Medium
            font.pointSize: Fonts.size12dot5

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked: {
                    if (root.highlight == "")
                        activeFocusItem.copySelectedText()
                    else
                        activeFocusItem.copyHighlightedText(root.highlight)

                    root.close()
                }
            }
        }

        Label {
            id: lookUpText
            Layout.fillWidth: true
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            text: "Look Up"
            color: Style.colorText
            font.weight: Font.Medium
            font.pointSize: Fonts.size12dot5

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked: {
                    let text = ""
                    if (root.highlight == "")
                        text = activeFocusItem.getSelectedText()
                    else
                        text = activeFocusItem.getHighlightedText(
                                    root.highlight)
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
        }

        Label {
            id: explainText
            Layout.fillWidth: true
            Layout.leftMargin: 16
            Layout.rightMargin: 16
            Layout.bottomMargin: 10
            text: "Ai Explain"
            color: Style.colorText
            font.pointSize: Fonts.size12dot5
            font.weight: Font.Medium

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked: {
                    let text = ""
                    if (root.highlight == "")
                        text = activeFocusItem.getSelectedText()
                    else
                        text = activeFocusItem.getHighlightedText(
                                    root.highlight)
                    root.explanationOptionSelected(text)
                }
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

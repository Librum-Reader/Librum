import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons
import Librum.fonts
import Librum.controllers

Popup {
    id: root
    property int preselectedSettingIndex: -1
    signal applied(string shortcut, string value)

    implicitWidth: 594
    implicitHeight: layout.implicitHeight
    closePolicy: Popup.CloseOnPressOutside
    padding: 0
    focus: true
    modal: true
    background: Rectangle {
        color: Style.colorPopupBackground
        radius: 6
    }
    Overlay.modal: Rectangle {
        color: Style.colorPopupDim
        opacity: 1
    }

    onOpened: {
        applyButton.forceActiveFocus()
        internal.setPreselectedItem()
    }

    onClosed: {
        internal.resetPopupData()

        // Reset the buttons, so that "Apply" has the focus again
        cancelButton.active = false
        applyButton.active = true
    }

    Shortcut {
        sequence: "Ctrl+R"
        enabled: root.opened
        onActivated: recordKeyBox.startRecording()
    }

    Shortcut {
        sequence: "Escape"
        enabled: root.opened
        onActivated: if (!recordKeyBox.recording)
                         root.close()
    }

    ColumnLayout {
        id: layout
        width: parent.width
        spacing: 0

        MButton {
            id: closeButton
            Layout.preferredHeight: 32
            Layout.preferredWidth: 32
            Layout.topMargin: 12
            Layout.rightMargin: 14
            Layout.alignment: Qt.AlignRight
            backgroundColor: "transparent"
            opacityOnPressed: 0.7
            borderColor: "transparent"
            radius: 6
            borderColorOnPressed: Style.colorButtonBorder
            imagePath: Icons.closePopup
            imageSize: 14

            onClicked: root.close()
        }

        Pane {
            id: content
            Layout.fillWidth: true
            Layout.topMargin: 18
            topPadding: 0
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
                    id: popupTitle
                    text: "Edit Shortcuts"
                    font.weight: Font.Bold
                    font.pointSize: Fonts.mediumTitleSize
                    color: Style.colorTitle
                }

                RowLayout {
                    id: selectionLayout
                    Layout.fillWidth: true
                    Layout.topMargin: 42
                    spacing: 26

                    MComboBox {
                        id: actionsComboBox
                        Layout.fillWidth: true
                        Layout.preferredHeight: 60
                        itemHeight: 32
                        headerText: "Action"
                        headerFontSize: Fonts.bigSize
                        headerFontColor: Style.colorTitle
                        headerFontWeight: Font.DemiBold
                        selectedItemFontColor: Style.colorReadOnlyInputText
                        selectedItemFontSize: Fonts.bigSize
                        selectedItemPadding: 3
                        emptyText: "None selected"
                        dropdownIconSize: 11
                        contentPropertyName: "shortcut"

                        fontSize: Fonts.bigSize
                        checkBoxStyle: false
                        model: SettingsController.shortcutsModel
                    }

                    MRecordKeyBox {
                        id: recordKeyBox
                        Layout.fillWidth: true
                        Layout.preferredHeight: 60
                        itemToRedirectFocusTo: applyButton

                        onTextChanged: {
                            let conflictingShortcut = SettingsController.checkIfShortcutIsInUse(
                                    recordKeyBox.text)
                            if (conflictingShortcut.length !== 0) {
                                shortcutIsAlreadyInUseLabel.conflictingShortcut
                                        = conflictingShortcut
                                shortcutIsAlreadyInUseLabel.visible = true
                            } else {
                                shortcutIsAlreadyInUseLabel.visible = false
                            }
                        }
                    }
                }

                Label {
                    id: shortcutIsAlreadyInUseLabel
                    property string conflictingShortcut: ""

                    Layout.preferredWidth: parent.width
                    Layout.topMargin: 16
                    visible: false
                    text: "The shortcut '" + recordKeyBox.text
                          + "' is already used for '" + conflictingShortcut + "'."
                    wrapMode: Text.WordWrap
                    font.pointSize: Fonts.baseSize
                    color: Style.colorErrorText
                }

                RowLayout {
                    id: buttonLayout
                    Layout.topMargin: 96
                    spacing: 16

                    MButton {
                        id: applyButton
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 38
                        active: true
                        borderWidth: active ? 0 : 1
                        backgroundColor: active ? Style.colorBasePurple : "transparent"
                        text: "Apply"
                        textColor: active ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                        fontWeight: Font.Bold
                        fontSize: Fonts.bigSize
                        KeyNavigation.right: cancelButton
                        KeyNavigation.tab: cancelButton

                        onClicked: apply()

                        // Key navigation
                        Keys.onPressed: event => {
                                            if (event.key === Qt.Key_Right
                                                || event.key === Qt.Key_Tab) {
                                                applyButton.active = false
                                                cancelButton.active = true
                                            } else if (event.key === Qt.Key_Return) {
                                                apply()
                                            }
                                        }

                        function apply() {
                            root.applied(actionsComboBox.text,
                                         recordKeyBox.text)
                            root.close()
                        }
                    }

                    MButton {
                        id: cancelButton
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 38
                        borderWidth: active ? 0 : 1
                        backgroundColor: active ? Style.colorBasePurple : "transparent"
                        opacityOnPressed: 0.7
                        text: "Cancel"
                        textColor: active ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                        fontWeight: Font.Bold
                        fontSize: Fonts.bigSize
                        KeyNavigation.left: applyButton
                        KeyNavigation.tab: applyButton

                        onClicked: root.close()

                        // Key navigation
                        Keys.onPressed: event => {
                                            if (event.key === Qt.Key_Left
                                                || event.key === Qt.Key_Tab) {
                                                cancelButton.active = false
                                                applyButton.active = true
                                            } else if (event.key === Qt.Key_Return) {
                                                root.close()
                                            }
                                        }
                    }
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent

        propagateComposedEvents: true
        // If clicking anywhere on the popup, stop the recordKeyBox
        onClicked: mouse => {
                       recordKeyBox.stopRecording()
                       applyButton.forceActiveFocus()
                       mouse.accepted = false
                   }
    }

    QtObject {
        id: internal

        function setPreselectedItem() {
            if (root.preselectedSettingIndex !== -1)
                actionsComboBox.selectItem(root.preselectedSettingIndex)
        }

        function resetPopupData() {
            actionsComboBox.deselectCurrenItem()
            recordKeyBox.stopRecording()
            recordKeyBox.clear()
        }
    }
}

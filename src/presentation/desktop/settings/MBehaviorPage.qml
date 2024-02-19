import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.fonts
import Librum.controllers

MFlickWrapper {
    id: root
    contentHeight: page.implicitHeight
    flickDeceleration: 3500

    Page {
        id: page
        width: parent.width
        horizontalPadding: 48
        bottomPadding: 22
        background: Rectangle {
            anchors.fill: parent
            color: Style.colorPageBackground
        }

        ColumnLayout {
            id: layout
            width: parent.width
            spacing: 0

            RowLayout {
                id: titleRow
                Layout.fillWidth: true
                spacing: 0

                MTitle {
                    id: pageTitle
                    Layout.topMargin: 64
                    titleText: qsTr("Behavior")
                    descriptionText: qsTr("Change the way Librum works")
                    titleSize: Fonts.size25
                    descriptionSize: Fonts.size13dot25
                }

                Item {
                    Layout.fillWidth: true
                }

                MButton {
                    id: resetButton
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignBottom
                    horizontalMargins: 12
                    borderWidth: 0
                    backgroundColor: Style.colorBasePurple
                    text: qsTr("Restore Defaults")
                    fontSize: Fonts.size12
                    fontWeight: Font.Bold
                    textColor: Style.colorFocusedButtonText

                    onClicked: resetSettingsPopup.open()
                }
            }

            Pane {
                id: behaviorBlock
                Layout.fillWidth: true
                Layout.topMargin: 32
                verticalPadding: 24
                horizontalPadding: internal.pagePadding
                background: Rectangle {
                    color: Style.colorContainerBackground
                    border.color: Style.colorContainerBorder
                    radius: 4
                    antialiasing: true
                }

                ColumnLayout {
                    id: behaviorLayout
                    anchors.fill: parent
                    spacing: 0

                    Label {
                        id: behaviorTitle
                        Layout.fillWidth: true
                        text: qsTr("Books")
                        font.pointSize: Fonts.size17
                        font.weight: Font.DemiBold
                        color: Style.colorText
                    }

                    Label {
                        id: cursorModeTitle
                        Layout.fillWidth: true
                        Layout.topMargin: 24
                        text: qsTr("Cursor mode")
                        font.pointSize: Fonts.size13
                        font.weight: Font.DemiBold
                        color: Style.colorText
                    }

                    MRadioButtonSelector {
                        id: cursorModeSelector
                        property string savedValue: SettingsController.behaviorSettings.CursorMode

                        Layout.fillWidth: true
                        Layout.topMargin: 6
                        displayOptions: [qsTr("Hidden after delay"), qsTr(
                                "Always visible")]
                        options: ["Hidden after delay", "Always visible"]
                        currentSelected: changeSelected(options.indexOf(
                                                            savedValue))

                        // Need rebinding on reset
                        onSavedValueChanged: {
                            if (currentSelected !== savedValue)
                                changeSelected(options.indexOf(savedValue))
                        }
                        onNewCurrentSelected: internal.saveSetting(
                                                  SettingKeys.CursorMode,
                                                  currentSelected)
                    }

                    ColumnLayout {
                        id: hideCursorAfterDelayFieldLayout
                        visible: cursorModeSelector.currentSelected
                                 === "Hidden after delay" ? true : false

                        Label {
                            id: hideCursorAfterDelayTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: qsTr("Hide cursor after delay")
                            font.pointSize: Fonts.size13
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        RowLayout {
                            id: hideCursorAfterDelaySpinBoxLayout

                            MSpinbox {
                                id: hideCursorAfterDelaySpinBox
                                property string savedValue: SettingsController.behaviorSettings.HideCursorAfterDelay
                                Layout.preferredWidth: 100
                                Layout.topMargin: 4
                                value: savedValue
                                minVal: 1
                                maxVal: 99999

                                onSavedValueChanged: value = savedValue
                                onNewValueSelected: internal.saveSetting(
                                                        SettingKeys.HideCursorAfterDelay,
                                                        value)
                            }

                            Text {
                                text: qsTr("ms")
                                font.pointSize: Fonts.size13
                                font.weight: Font.DemiBold
                                color: Style.colorText
                            }
                        }
                    }

                    Label {
                        id: includeNewlinesInCopiedText
                        Layout.fillWidth: true
                        Layout.topMargin: 24
                        text: qsTr("Include new lines in copied text")
                        font.pointSize: Fonts.size13
                        font.weight: Font.DemiBold
                        color: Style.colorText
                    }

                    MDualToggle {
                        id: includeNewlinesInCopiedTextToggle

                        property string savedValue: SettingsController.behaviorSettings.IncludeNewLinesInCopiedText
                        Layout.topMargin: 4
                        leftText: "ON"
                        leftDisplayText: qsTr("ON")
                        rightText: "OFF"
                        rightDisplayText: qsTr("OFF")
                        leftSelected: savedValue === leftText
                        rightSelected: savedValue === rightText

                        onSavedValueChanged: savedValue === leftText ? selectLeft(
                                                                           ) : selectRight()
                        onToggled: newSelected => internal.saveSetting(
                                       SettingKeys.IncludeNewLinesInCopiedText,
                                       newSelected)
                    }
                }
            }
        }
    }

    MWarningPopup {
        id: resetSettingsPopup
        x: Math.round(root.width / 2 - implicitWidth / 2 - settingsSidebar.width
                      / 2 - page.horizontalPadding)
        y: Math.round(
               root.height / 2 - implicitHeight / 2 - page.topPadding - 50)
        visible: false
        title: qsTr("Reset settings?")
        message: qsTr("Resetting your settings is a permanent action, there\n will be no way to restore them!")
        leftButtonText: qsTr("No, Keep")
        rightButtonText: qsTr("Yes, Reset")
        messageBottomSpacing: 10
        minButtonWidth: 180

        onOpenedChanged: if (opened)
                             resetSettingsPopup.giveFocus()
        onDecisionMade: close()
        onRightButtonClicked: SettingsController.resetSettingGroup(
                                  SettingGroups.Behavior)
    }

    QtObject {
        id: internal
        property int pagePadding: 40


        /*
          Higher order functions to simplify the call on the SettingsController
          */
        function saveSetting(key, value) {
            SettingsController.setSetting(key, value, SettingGroups.Behavior)
        }
    }
}

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import CustomComponents
import Librum.style
import Librum.icons
import Librum.models
import Librum.fonts
import Librum.controllers

Page {
    id: root
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
                titleText: qsTr("Appearance")
                descriptionText: qsTr("Make your own experience")
                titleSize: Fonts.size25
                descriptionSize: Fonts.size13dot25
            }

            Item {
                Layout.fillWidth: true
            }

            MButton {
                id: resetButton
                Layout.preferredWidth: 160
                Layout.preferredHeight: 38
                Layout.alignment: Qt.AlignBottom
                borderWidth: 0
                backgroundColor: Style.colorBasePurple
                text: qsTr("Restore Defaults")
                fontSize: Fonts.size12
                fontWeight: Font.Bold
                textColor: Style.colorFocusedButtonText

                onClicked: resetSettingsPopup.open()
            }
        }

        MFlickWrapper {
            id: flickWrapper
            Layout.fillWidth: true
            // Calculate the scrollable height (Scrollable only if the page is bigger than the screen)
            Layout.preferredHeight: root.height - titleRow.implicitHeight - root.bottomPadding * 2
            contentHeight: contentLayout.implicitHeight
            Layout.topMargin: 32
            Layout.rightMargin: -internal.scrollbarOffset
            flickDeceleration: 20000
            maximumFlickVelocity: 2300
            clip: true

            ScrollBar.vertical: ScrollBar {
                width: 10
                policy: ScrollBar.AlwaysOn
            }

            ColumnLayout {
                id: contentLayout
                width: parent.width
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: internal.scrollbarOffset
                spacing: 0

                Pane {
                    id: displayBlock
                    Layout.fillWidth: true
                    verticalPadding: 24
                    horizontalPadding: internal.pagePadding
                    background: Rectangle {
                        color: Style.colorContainerBackground
                        border.color: Style.colorContainerBorder
                        radius: 4
                        antialiasing: true
                    }

                    ColumnLayout {
                        id: displayLayout
                        anchors.fill: parent
                        spacing: 0

                        Label {
                            id: displayTitle
                            Layout.fillWidth: true
                            text: qsTr("Display")
                            font.pointSize: Fonts.size17
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        Label {
                            id: themeTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 24
                            text: qsTr("Theme")
                            font.pointSize: Fonts.size13
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        MDualToggle {
                            id: themeSwitch
                            property string savedValue: SettingsController.appearanceSettings.Theme

                            Layout.topMargin: 4
                            leftText: "Dark"
                            leftDisplayText: qsTr("Dark")
                            rightText: "Light"
                            rightDisplayText: qsTr("Light")
                            leftSelected: savedValue === leftText
                            rightSelected: savedValue === rightText

                            // Need rebinding on reset
                            onSavedValueChanged: savedValue === leftText ? selectLeft(
                                                                               ) : selectRight()
                            onToggled: newSelected => internal.saveSetting(
                                           SettingKeys.Theme, newSelected)
                        }

                        Label {
                            id: pageColorModeTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: qsTr("Page Color Mode")
                            font.pointSize: Fonts.size13
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        MDualToggle {
                            id: pageColorModeSwitch
                            property string savedValue: SettingsController.appearanceSettings.PageColorMode

                            Layout.topMargin: 4
                            leftText: "Normal"
                            leftDisplayText: qsTr("Normal")
                            rightText: "Inverted"
                            rightDisplayText: qsTr("Inverted")
                            leftSelected: savedValue === leftText
                            rightSelected: savedValue === rightText

                            // Need rebinding on reset
                            onSavedValueChanged: savedValue === leftText ? selectLeft(
                                                                               ) : selectRight()
                            onToggled: newSelected => internal.saveSetting(
                                           SettingKeys.PageColorMode,
                                           newSelected)
                        }

                        Label {
                            id: languageTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: qsTr("Language")
                            font.pointSize: Fonts.size13
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        MComboBox {
                            id: languageComboBox
                            Layout.topMargin: 4
                            Layout.preferredHeight: 36
                            Layout.fillWidth: true
                            Layout.maximumWidth: 380
                            selectedItemFontSize: Fonts.size12
                            selectedItemPadding: 4
                            defaultIndex: calculateDefaultIndex()
                            dropdownIconSize: 9
                            itemHeight: 32
                            fontSize: Fonts.size12
                            checkBoxStyle: false
                            maxHeight: 200
                            model: LanguageModel

                            onItemChanged: index => {
                                               AppInfoController.switchToLanguage(
                                                   model.get(index).code)

                                               languageComboBox.closePopup()
                                           }

                            Component.onCompleted: {
                                let defaultIndex = calculateDefaultIndex()
                                if (listView.currentIndex === defaultIndex)
                                    return

                                deselectCurrenItem()
                                selectItem(defaultIndex, true)
                            }

                            function calculateDefaultIndex() {
                                let selectedLanguage = AppInfoController.language
                                for (var i = 0; i < model.count; ++i) {
                                    if (model.get(i).text === selectedLanguage)
                                        return i
                                }

                                return -1
                            }
                        }
                    }
                }

                Pane {
                    id: readingBlock
                    Layout.fillWidth: true
                    Layout.topMargin: 24
                    verticalPadding: 24
                    horizontalPadding: internal.pagePadding
                    background: Rectangle {
                        color: Style.colorContainerBackground
                        border.color: Style.colorContainerBorder
                        radius: 4
                        antialiasing: true
                    }

                    ColumnLayout {
                        id: readingLayout
                        anchors.fill: parent
                        spacing: 0

                        Label {
                            id: readingTitle
                            Layout.fillWidth: true
                            text: qsTr("Reading")
                            font.pointSize: Fonts.size17
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        Label {
                            id: pageSpacingTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 24
                            text: qsTr("Page spacing")
                            font.pointSize: Fonts.size13
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        MSpinbox {
                            id: pageSpacingSpinBox
                            property string savedValue: SettingsController.appearanceSettings.PageSpacing

                            Layout.preferredWidth: 76
                            Layout.topMargin: 4
                            value: savedValue
                            minVal: 0
                            maxVal: 999

                            // Need rebinding on reset
                            onSavedValueChanged: value = savedValue
                            onNewValueSelected: internal.saveSetting(
                                                    SettingKeys.PageSpacing,
                                                    value)
                        }

                        Label {
                            id: docTitleDisplayTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: qsTr("Display book title in titlebar")
                            font.pointSize: Fonts.size13
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        MOnOffToggle {
                            id: displayBookTitleInTitlebarToggle
                            property bool savedValue: JSON.parse(
                                                          SettingsController.appearanceSettings.DisplayBookTitleInTitlebar)

                            Layout.topMargin: 4
                            onByDefault: savedValue

                            // Need rebinding on reset
                            onSavedValueChanged: savedValue ? setOn() : setOff()
                            onToggled: value => internal.saveSetting(
                                           SettingKeys.DisplayBookTitleInTitlebar,
                                           value === onText ? true : false)
                        }

                        Label {
                            id: layoutDirectionTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: qsTr("Layout direction")
                            font.pointSize: Fonts.size13
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        MRadioButtonSelector {
                            id: layoutDirectionSelector
                            property string savedValue: SettingsController.appearanceSettings.LayoutDirection

                            Layout.fillWidth: true
                            Layout.topMargin: 6
                            displayOptions: [qsTr("Vertical"), qsTr(
                                    "Horizontal")]
                            options: ["Vertical", "Horizontal"]
                            currentSelected: changeSelected(options.indexOf(
                                                                savedValue))

                            // Need rebinding on reset
                            onSavedValueChanged: changeSelected(options.indexOf(
                                                                    savedValue))
                            onNewCurrentSelected: internal.saveSetting(
                                                      SettingKeys.LayoutDirection,
                                                      currentSelected)
                        }

                        Label {
                            id: displayModeTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: qsTr("Display mode")
                            font.pointSize: Fonts.size13
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        MRadioButtonSelector {
                            id: displayModeSelector
                            property string savedValue: SettingsController.appearanceSettings.DisplayMode

                            Layout.fillWidth: true
                            Layout.topMargin: 6
                            displayOptions: [qsTr("Single Page"), qsTr(
                                    "Double Page")]
                            options: ["Single Page", "Double Page"]
                            currentSelected: changeSelected(options.indexOf(
                                                                savedValue))

                            // Need rebinding on reset
                            onSavedValueChanged: changeSelected(options.indexOf(
                                                                    savedValue))
                            onNewCurrentSelected: internal.saveSetting(
                                                      SettingKeys.DisplayMode,
                                                      currentSelected)
                        }

                        Label {
                            id: pageTransitionTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: qsTr("Page transition")
                            font.pointSize: Fonts.size13
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        MComboBox {
                            id: pageTransitionComboBox
                            property string savedValue: SettingsController.appearanceSettings.PageTransition

                            Layout.topMargin: 4
                            Layout.preferredHeight: 36
                            Layout.fillWidth: true
                            Layout.maximumWidth: 380
                            selectedItemFontSize: Fonts.size12
                            selectedItemPadding: 4
                            defaultIndex: calculateDefaultIndex()
                            dropdownIconSize: 9
                            itemHeight: 32
                            fontSize: Fonts.size12
                            checkBoxStyle: false
                            maxHeight: 200
                            model: ListModel {
                                ListElement {
                                    text: qsTr("Instant")
                                }
                                ListElement {
                                    text: qsTr("Fading")
                                }
                                ListElement {
                                    text: qsTr("Swipe")
                                }
                                ListElement {
                                    text: qsTr("Swap")
                                }
                            }

                            onItemChanged: internal.saveSetting(
                                               SettingKeys.PageTransition, text)
                            // Need rebinding on reset
                            onSavedValueChanged: {
                                let defaultIndex = calculateDefaultIndex()
                                if (listView.currentIndex === defaultIndex)
                                    return

                                deselectCurrenItem()
                                selectItem(defaultIndex, true)
                            }

                            function calculateDefaultIndex() {
                                for (var i = 0; i < model.count; ++i) {
                                    if (model.get(i).text === savedValue)
                                        return i
                                }
                                return -1
                            }
                        }

                        Label {
                            id: defaultZoomTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: qsTr("Default Zoom")
                            font.pointSize: Fonts.size13
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        MSpinbox {
                            id: defaultZoomSpinBox
                            property int savedValue: SettingsController.appearanceSettings.DefaultZoom

                            Layout.preferredWidth: 76
                            Layout.topMargin: 4
                            value: savedValue
                            maxVal: 300
                            minVal: 15

                            // Need rebinding on reset
                            onSavedValueChanged: value = savedValue
                            onNewValueSelected: internal.saveSetting(
                                                    SettingKeys.DefaultZoom,
                                                    value)
                        }
                    }
                }

                Pane {
                    id: highlightsBlock
                    Layout.fillWidth: true
                    Layout.topMargin: 24
                    verticalPadding: 24
                    horizontalPadding: internal.pagePadding
                    background: Rectangle {
                        color: Style.colorContainerBackground
                        border.color: Style.colorContainerBorder
                        radius: 4
                        antialiasing: true
                    }

                    ColumnLayout {
                        id: highlightsLayout
                        anchors.fill: parent
                        spacing: 0

                        Label {
                            id: highlightsTitle
                            Layout.fillWidth: true
                            text: qsTr("Highlights")
                            font.pointSize: Fonts.size17
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        Label {
                            Layout.fillWidth: true
                            Layout.topMargin: 24
                            text: qsTr("Colors")
                            font.pointSize: Fonts.size13
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        Pane {
                            id: highlightColorBox
                            property var currentColorButton

                            Layout.preferredWidth: highlightColorBoxLayout.implicitWidth
                                                   + leftPadding + rightPadding
                            Layout.preferredHeight: 40
                            Layout.topMargin: 8
                            background: Rectangle {
                                anchors.fill: parent
                                border.width: 1
                                border.color: Style.colorContainerBorder
                                color: Style.colorControlBackground
                            }
                            leftPadding: 16
                            rightPadding: 16
                            verticalPadding: 0

                            RowLayout {
                                id: highlightColorBoxLayout
                                height: parent.height
                                spacing: 16

                                component HighlightColorButton: Rectangle {
                                    property string settingName
                                    property string savedValue: SettingsController.appearanceSettings[settingName]

                                    implicitHeight: 18
                                    implicitWidth: 18
                                    Layout.alignment: Qt.AlignVCenter
                                    radius: 16
                                    color: savedValue

                                    MouseArea {
                                        anchors.fill: parent

                                        onClicked: {
                                            highlightColorBox.currentColorButton = parent
                                            colorDialog.selectedColor = savedValue
                                            colorDialog.open()
                                        }
                                    }

                                    function changeColor(color) {
                                        internal.saveSetting(
                                                    SettingKeys[settingName],
                                                    color)
                                    }

                                    function rebind() {
                                        savedValue = Qt.binding(function () {
                                            return SettingsController.appearanceSettings[settingName]
                                        })
                                        color = Qt.binding(function () {
                                            return savedValue
                                        })
                                    }
                                }

                                component Separator: Rectangle {
                                    Layout.fillHeight: true
                                    Layout.preferredWidth: 1
                                    color: Style.colorSeparator
                                }

                                HighlightColorButton {
                                    settingName: "HighlightColorA"
                                }

                                Separator {}

                                HighlightColorButton {
                                    settingName: "HighlightColorB"
                                }

                                Separator {}

                                HighlightColorButton {
                                    settingName: "HighlightColorC"
                                }

                                Separator {}

                                HighlightColorButton {
                                    settingName: "HighlightColorD"
                                }

                                Separator {}

                                HighlightColorButton {
                                    settingName: "HighlightColorE"
                                }
                            }

                            ColorDialog {
                                id: colorDialog

                                onSelectedColorChanged: highlightColorBox.currentColorButton.color
                                                        = selectedColor

                                onAccepted: {
                                    highlightColorBox.currentColorButton.changeColor(
                                                colorDialog.selectedColor)
                                    highlightColorBox.currentColorButton.rebind(
                                                )
                                }
                                onRejected: highlightColorBox.currentColorButton.rebind()
                            }
                        }

                        Label {
                            Layout.fillWidth: true
                            Layout.topMargin: 24
                            text: qsTr("Opacity")
                            font.pointSize: Fonts.size13
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        RowLayout {
                            spacing: 16

                            MSlider {
                                id: highlightOpacitySlider
                                Layout.alignment: Qt.AlignVCenter
                                value: SettingsController.appearanceSettings.HighlightOpacity

                                onControlledValueChanged: value => internal.saveSetting(
                                                              SettingKeys.HighlightOpacity,
                                                              value)
                            }

                            MLabeledInputBox {
                                Layout.preferredWidth: 72
                                Layout.preferredHeight: implicitHeight
                                boxHeight: 36
                                hasHeader: false
                                textPadding: 20
                                text: highlightOpacitySlider.value
                                validator: IntValidator {
                                    bottom: 0
                                    top: 255
                                }

                                onEdited: {
                                    if (text > 255)
                                        text = 255
                                    else if (text < 0)
                                        text = 0

                                    internal.saveSetting(
                                                SettingKeys.HighlightOpacity,
                                                text)
                                }
                            }
                        }
                    }
                }

                Pane {
                    id: behaviorBlock
                    Layout.fillWidth: true
                    Layout.topMargin: 24
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
                            text: qsTr("Behavior")
                            font.pointSize: Fonts.size17
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        Label {
                            id: smoothScrollingTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 24
                            text: qsTr("Smooth scrolling")
                            font.pointSize: Fonts.size13
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        MOnOffToggle {
                            id: smoothScrollingToggle
                            property bool savedValue: JSON.parse(
                                                          SettingsController.appearanceSettings.SmoothScrolling)

                            Layout.topMargin: 4
                            onByDefault: savedValue

                            // Need rebinding on reset
                            onSavedValueChanged: savedValue ? setOn() : setOff()
                            onToggled: value => internal.saveSetting(
                                           SettingKeys.SmoothScrolling,
                                           value === onText ? true : false)
                        }

                        Label {
                            id: loopAfterLastTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: qsTr("Loop after last page")
                            font.pointSize: Fonts.size13
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        MOnOffToggle {
                            id: loopAfterLastToggle
                            property bool savedValue: JSON.parse(
                                                          SettingsController.appearanceSettings.LoopAfterLastPage)

                            Layout.topMargin: 4
                            onByDefault: savedValue

                            // Need rebinding on reset
                            onSavedValueChanged: savedValue ? setOn() : setOff()
                            onToggled: value => internal.saveSetting(
                                           SettingKeys.LoopAfterLastPage,
                                           value === onText ? true : false)
                        }

                        Label {
                            id: cursorModeTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: qsTr("Cursor mode")
                            font.pointSize: Fonts.size13
                            font.weight: Font.DemiBold
                            color: Style.colorText
                        }

                        MRadioButtonSelector {
                            id: cursorModeSelector
                            property string savedValue: SettingsController.appearanceSettings.CursorMode

                            Layout.fillWidth: true
                            Layout.topMargin: 6
                            displayOptions: [qsTr("Hidden after delay"), qsTr(
                                    "Always visible")]
                            options: ["Hidden after delay", "Always visible"]
                            currentSelected: changeSelected(options.indexOf(
                                                                savedValue))

                            // Need rebinding on reset
                            onSavedValueChanged: changeSelected(options.indexOf(
                                                                    savedValue))
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
                                text: "Hide cursor after delay"
                                font.pointSize: Fonts.size13
                                font.weight: Font.DemiBold
                                color: Style.colorText
                            }

                            RowLayout {
                                id: hideCursorAfterDelaySpinBoxLayout

                                MSpinbox {
                                    id: hideCursorAfterDelaySpinBox
                                    property string savedValue: SettingsController.appearanceSettings.HideCursorAfterDelay
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
                                    text: "ms"
                                    font.pointSize: Fonts.size13
                                    font.weight: Font.DemiBold
                                    color: Style.colorText
                                }
                            }

                            onVisibleChanged: {
                                if (visible)
                                    flickWrapper.flick(0, -2000)
                            }
                        }
                    }
                }
            }
        }
    }

    MWarningPopup {
        id: resetSettingsPopup
        x: Math.round(root.width / 2 - implicitWidth / 2 - settingsSidebar.width
                      / 2 - root.horizontalPadding)
        y: Math.round(
               root.height / 2 - implicitHeight / 2 - root.topPadding - 50)
        visible: false
        title: qsTr("Reset settings?")
        message: qsTr("Resetting your settings is a permanent action, there\n will be no way to restore them!")
        leftButtonText: qsTr("No, Keep")
        rightButtonText: qsTr("Yes, Reset")
        messageBottomSpacing: 10

        onOpenedChanged: if (opened)
                             resetSettingsPopup.giveFocus()
        onDecisionMade: close()
        onRightButtonClicked: SettingsController.resetSettingGroup(
                                  SettingGroups.Appearance)
    }

    QtObject {
        id: internal
        property int pagePadding: 40
        property int scrollbarOffset: 22


        /*
          Higher order functions to simplify the call on the SettingsController
          */
        function saveSetting(key, value) {
            SettingsController.setSetting(key, value, SettingGroups.Appearance)
        }
    }
}

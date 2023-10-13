import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.controllers

Popup {
    id: root
    property string answer
    property string question

    implicitWidth: 800
    implicitHeight: 700
    padding: 32
    bottomPadding: 28
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    background: Rectangle {
        color: Style.colorPopupBackground
        radius: 6
        border.width: 1
        border.color: Style.colorContainerBorder
    }

    onOpened: {
        root.answer = ""
        internal.sendExplanationRequest()
    }

    onClosed: {
        root.answer = ""
        root.question = ""

        internal.dataChanged = false
    }

    Connections {
        target: AiExplanationController

        function onWordReady(word) {
            root.answer += word
        }

        function onLimitReached() {
            limitReachedWarning.visible = true
        }
    }

    ListModel {
        id: explanationModes

        ListElement {
            name: "Explain"
            query: "Shortly explain the following: "
        }
        ListElement {
            name: "Explain like I'm five"
            query: "Shortly explain the following like I am a kid: "
        }
        ListElement {
            name: "Summarize"
            query: "Shortly summarize the following: "
        }
        ListElement {
            name: "Give more information"
            query: "Shortly give more information on the following: "
        }
        ListElement {
            name: "Explain visually"
            query: "Shortly explain the following visually: "
        }
    }

    ColumnLayout {
        anchors.fill: parent

        MButton {
            id: closeButton
            Layout.preferredHeight: 32
            Layout.preferredWidth: 32
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

        MComboBox {
            id: modesComboBox
            Layout.preferredWidth: 300
            Layout.preferredHeight: 60
            itemHeight: 32
            selectedItemFontColor: Style.colorBaseInputText
            headerText: "Mode"
            headerFontSize: 12
            headerFontColor: Style.colorTitle
            headerFontWeight: Font.DemiBold
            fontSize: 12
            selectedItemFontSize: 13
            selectedItemPadding: 3
            defaultIndex: 0
            emptyText: "None selected"
            dropdownIconSize: 11
            contentPropertyName: "name"
            checkBoxStyle: false
            model: explanationModes

            onItemChanged: {
                modesComboBox.closePopup()
                internal.dataChanged = true
            }
        }

        MLabeledInputBox {
            id: request
            Layout.fillWidth: true
            Layout.topMargin: 24
            headerFontColor: Style.colorLightText
            headerFontSize: 12
            borderColor: Style.colorContainerBorder
            borderWidth: 1
            text: question
            backgroundColor: Style.colorContainerBackground
            inputFontSize: 13
            headerText: "Request"
            readOnly: true
        }

        Pane {
            id: answerContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 32
            rightPadding: 16
            clip: true
            background: Rectangle {
                color: Style.colorContainerBackground
                radius: 6
                border.width: 1
                border.color: Style.colorContainerBorder
            }

            Flickable {
                id: answerFlick
                anchors.fill: parent
                contentWidth: answerField.contentWidth
                contentHeight: answerField.contentHeight
                clip: true
                boundsBehavior: Flickable.StopAtBounds
                boundsMovement: Flickable.StopAtBounds

                onContentHeightChanged: contentY = contentHeight - height

                TextEdit {
                    id: answerField
                    width: answerFlick.width
                    focus: true
                    text: root.answer
                    font.pointSize: 13
                    color: Style.colorText
                    readOnly: true
                    wrapMode: Text.WordWrap
                    selectionColor: Style.colorTextSelection
                }
            }

            Item {
                id: limitReachedWarning
                visible: false
                anchors.fill: parent

                ColumnLayout {
                    width: parent.width
                    anchors.centerIn: parent

                    Image {
                        id: limitReachedIllustration
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: -30
                        source: Icons.attentionPurple
                        sourceSize.width: 270
                        fillMode: Image.PreserveAspectFit
                    }

                    Label {
                        id: limitReachedText
                        Layout.alignment: Qt.AlignHCenter
                        textFormat: Text.RichText
                        text: 'You have reached your daily limit. Click <a href="update" style="color: ' + Style.colorBasePurple
                              + '; text-decoration: none;">here</a> to learn more.'
                        color: Style.colorTitle
                        font.weight: Font.Medium
                        font.pointSize: 14
                        onLinkActivated: Qt.openUrlExternally(
                                             AppInfoController.website + "/whyAiLimits")

                        // Switch to the proper cursor when hovering above the link
                        MouseArea {
                            id: mouseArea
                            acceptedButtons: Qt.NoButton // Don't eat the mouse clicks
                            anchors.fill: parent
                            cursorShape: limitReachedText.hoveredLink
                                         !== "" ? Qt.PointingHandCursor : Qt.ArrowCursor
                        }
                    }
                }
            }

            ScrollBar {
                id: verticalScrollbar
                width: pressed ? 14 : 12
                hoverEnabled: true
                active: true
                policy: ScrollBar.AlwaysOff
                visible: answerFlick.contentHeight > answerFlick.height
                orientation: Qt.Vertical
                size: answerFlick.height / answerFlick.contentHeight
                minimumSize: 0.04
                position: (answerFlick.contentY - answerFlick.originY) / answerFlick.contentHeight
                onPositionChanged: if (pressed)
                                       answerFlick.contentY = position
                                               * answerFlick.contentHeight + answerFlick.originY
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.rightMargin: -12
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

        Item {
            id: aiWarningItem
            Layout.preferredWidth: warningLayout.width
            Layout.preferredHeight: 20
            Layout.alignment: Qt.AlignRight

            RowLayout {
                id: warningLayout
                height: parent.height
                spacing: 4

                Image {
                    id: actionImage
                    Layout.alignment: Qt.AlignVCenter
                    source: Icons.warningCircle
                    sourceSize.width: 16
                    fillMode: Image.PreserveAspectFit
                }

                Label {
                    id: actionText
                    Layout.topMargin: -1
                    text: "Note: AI responses can be inaccurate"
                    font.pointSize: 10
                    color: Style.colorBasePurple
                }
            }
        }

        MButton {
            id: askButton
            Layout.preferredWidth: 140
            Layout.preferredHeight: 38
            Layout.topMargin: 4
            Layout.alignment: Qt.AlignLeft
            borderWidth: internal.dataChanged ? 0 : 1
            backgroundColor: internal.dataChanged ? Style.colorBasePurple : "transparent"
            opacityOnPressed: 0.7
            text: "Ask"
            textColor: internal.dataChanged ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
            fontWeight: Font.Bold
            fontSize: 12

            onClicked: {
                if (internal.dataChanged) {
                    root.answer = ""
                    internal.sendExplanationRequest()
                    internal.dataChanged = false
                }
            }
        }
    }

    QtObject {
        id: internal
        property bool dataChanged: false

        function sendExplanationRequest() {
            AiExplanationController.getExplanation(
                        root.question, explanationModes.get(
                            modesComboBox.listView.currentIndex).query)
        }
    }
}

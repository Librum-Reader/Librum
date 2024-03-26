import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.fonts
import Librum.style
import Librum.icons
import Librum.controllers
import CustomComponents
import Librum.globalSettings

Popup {
    id: root
    implicitWidth: 560
    implicitHeight: layout.height + 32
    horizontalPadding: 36
    verticalPadding: 24
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape
    focus: true
    background: Rectangle {
        color: Style.colorPopupBackground
        border.width: 1
        border.color: Style.colorContainerBorder
        radius: 6
        antialiasing: true
    }

    ColumnLayout {
        id: layout
        width: parent.width
        spacing: 0

        LayoutMirroring.enabled: baseRoot.rightAlign
        LayoutMirroring.childrenInherit: true

        Label {
            id: popupTitle
            Layout.alignment: Qt.AlignLeft
            Layout.topMargin: 12
            Layout.fillWidth: true
            text: qsTr("Got some feedback?")
            elide: Text.ElideRight
            font.weight: Font.Medium
            font.pointSize: Fonts.size18
            color: Style.colorTitle
        }

        Label {
            id: infoText
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 16
            font.pointSize: Fonts.size12dot25
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            color: Style.colorText
            text: qsTr("Your feedback is crucial for improving Librum. Would you mind sharing your experience by taking a quick survey?")
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: 36
            Layout.bottomMargin: 18
            height: cancelButton.height
            spacing: 12

            MButton {
                id: cancelButton
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                borderWidth: 1
                backgroundColor: "transparent"
                fontSize: Fonts.size11
                opacityOnPressed: 0.85
                textColor: Style.colorText
                text: qsTr("No, Thanks")

                onClicked: root.close()
            }

            MButton {
                id: acceptButton
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                borderWidth: 0
                backgroundColor: Style.colorBasePurple
                fontSize: Fonts.size11
                opacityOnPressed: 0.85
                textColor: Style.colorFocusedButtonText
                text: qsTr("Let's do this!")

                onClicked: {
                    Qt.openUrlExternally(AppInfoController.feedbackLink)

                    // Ask the user again in half a year after they've taken the survey
                    var date = new Date()
                    date.setDate((new Date()).getDate() + 183)
                    GlobalSettings.lastFeedbackQuery = date.toString()
                    root.close()
                }
            }
        }
    }

    function giveFocus() {
        root.forceActiveFocus()
    }
}

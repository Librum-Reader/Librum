import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.elements
import Librum.style
import Librum.icons
import Librum.fonts

Item {
    id: root
    property var itemToRedirectFocusTo
    property bool recording: false
    property string originalSequence: ""
    property alias text: recordLabel.text

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 2

        Label {
            id: header
            //: As in key on the keyboard
            text: qsTr("Key")
            font.weight: Font.DemiBold
            font.pointSize: Fonts.size12
            color: Style.colorTitle
        }

        Rectangle {
            id: button
            Layout.fillWidth: true
            Layout.preferredHeight: 38
            radius: 4
            color: Style.colorControlBackground
            border.color: root.recording ? Style.colorRecordActiveBorder : Style.colorButtonBorder
            border.width: root.recording ? 2 : 1
            opacity: mouseArea.pressed ? 0.8 : 1

            onActiveFocusChanged: activeFocus ? root.startRecording(
                                                    ) : root.stopRecording()

            KeySequenceRecorder {
                id: keySequenceRecorder
                originalSequence: root.originalSequence

                onReturnPressed: stopRecording()
            }

            RowLayout {
                id: contentLayout
                anchors.fill: parent
                spacing: 4

                Label {
                    id: recordLabel
                    Layout.fillWidth: true
                    Layout.leftMargin: 12
                    text: internal.getRecordText()
                    font.pointSize: Fonts.size12
                    color: Style.colorLightInputText
                    elide: Text.ElideLeft
                }

                Image {
                    id: microphoneIcon
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin: 12
                    source: root.recording ? Icons.activeMicrophone : Icons.microphone
                    sourceSize.width: 18
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: {
            if (root.recording) {
                stopRecording()
                root.forceActiveFocus()
                return
            }

            startRecording()
        }
    }

    QtObject {
        id: internal

        function getRecordText() {
            if (keySequenceRecorder.currentSequence)
                return keySequenceRecorder.currentSequence

            if (keySequenceRecorder.originalSequence)
                return keySequenceRecorder.originalSequence

            //: As in recording a key that is pressed on the keyboard
            return qsTr("Press to record")
        }
    }

    function clear() {
        keySequenceRecorder.resetSequence()
    }

    function startRecording() {
        keySequenceRecorder.forceActiveFocus()
        root.recording = true
    }

    function stopRecording() {
        itemToRedirectFocusTo.forceActiveFocus()
        root.recording = false
    }
}

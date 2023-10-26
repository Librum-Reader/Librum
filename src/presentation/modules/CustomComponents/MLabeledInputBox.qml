import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.fonts

Item {
    id: root
    property alias text: inputField.text
    property int boxHeight: 40
    property string placeholderContent
    property color placeholderColor: Style.colorPlaceholderText
    property double inputFontSize: Fonts.bigSize
    property color inputFontColor: Style.colorBaseInputText
    property bool readOnly: false
    property int inputFontWeight: Font.Normal
    property bool hasError: false
    property bool clearErrorOnEdit: true
    property string errorText
    property double errorFontSize: Fonts.baseSize
    property bool hasHeader: true
    property string headerText
    property int headerFontWeight: Font.Medium
    property double headerFontSize: Fonts.baseSize
    property color headerFontColor: Style.colorTitle
    property int headerToBoxSpacing: 2
    property color borderColor: Style.colorContainerBorder
    property color backgroundColor: Style.colorControlBackground
    property int borderWidth: 2
    property int borderRadius: 5
    property int textPadding: 15
    property string image
    property string toggledImage
    property bool textHidden: toggledImage.length > 0 ? true : false
    property var validator: null
    signal edited

    implicitWidth: 100
    implicitHeight: layout.height

    ColumnLayout {
        id: layout
        width: parent.width
        height: header.implicitHeight + inputBox.height
                + (errorText.visible
                   && root.errorText != "" ? errorText.implicitHeight : 0)
        spacing: root.headerToBoxSpacing

        Label {
            id: header
            Layout.fillWidth: true
            visible: root.hasHeader
            text: root.headerText
            font.pointSize: root.headerFontSize
            font.weight: root.headerFontWeight
            color: root.headerFontColor
        }

        Pane {
            id: inputBox
            Layout.fillWidth: true
            Layout.preferredHeight: root.boxHeight
            padding: 0
            background: Rectangle {
                id: backgroundRect
                border.width: root.borderWidth
                border.color: root.borderColor
                radius: root.borderRadius
                color: root.backgroundColor
            }

            RowLayout {
                id: inBoxLayout
                anchors.fill: parent
                spacing: 0

                TextField {
                    id: inputField
                    Layout.fillWidth: true
                    leftPadding: root.textPadding
                    rightPadding: root.textPadding
                    selectByMouse: true
                    readOnly: root.readOnly
                    color: root.inputFontColor
                    font.pointSize: root.inputFontSize
                    font.weight: root.inputFontWeight
                    placeholderText: root.placeholderContent
                    placeholderTextColor: root.placeholderColor
                    echoMode: root.textHidden ? TextInput.Password : TextInput.Normal
                    selectionColor: Style.colorTextSelection
                    validator: root.validator
                    selectedTextColor: root.inputFontColor
                    background: Rectangle {
                        anchors.fill: parent
                        radius: root.borderRadius
                        color: "transparent"
                    }

                    // Make sure the cursor is at the start
                    onActiveFocusChanged: resetCursorPositionToStart()
                    onTextChanged: resetCursorPositionToStart()
                    onTextEdited: {
                        root.edited()

                        if (clearErrorOnEdit)
                            root.clearError()
                    }


                    /**
                     In certain text-size / container-height proportions the text position
                     is too low, this makes sure that the text is always centered
                     */
                    Component.onCompleted: {
                        // If the size difference is too big, move the input field up by 1px
                        if ((inputBox.height / inputField.implicitHeight) > 0.2) {
                            inputField.Layout.topMargin -= 1
                        }
                    }


                    /**
                     If the text is longer than the container-width, the text is automatically
                     scrolled to the right. This resets the cursor to the start of the text
                     */
                    function resetCursorPositionToStart() {
                        if (!inputField.activeFocus)
                            inputField.cursorPosition = 0
                    }
                }

                Image {
                    id: passwordVisibilityTogglerIcon
                    Layout.preferredWidth: 20
                    Layout.preferredHeight: 18
                    Layout.rightMargin: 10
                    Layout.alignment: Qt.AlignVCenter
                    visible: root.image.length > 0
                    source: root.textHidden ? root.image : root.toggledImage
                    opacity: imageArea.pressed ? 0.75 : 1

                    MouseArea {
                        id: imageArea
                        anchors.fill: parent

                        onClicked: root.textHidden = !root.textHidden
                    }
                }
            }
        }

        Label {
            id: errorText
            Layout.fillWidth: true
            Layout.topMargin: 2
            visible: root.hasError
            text: root.errorText
            font.pointSize: root.errorFontSize
            color: Style.colorErrorText
        }
    }

    function giveFocus() {
        inputField.forceActiveFocus()
    }

    function clearText() {
        inputField.clear()
    }

    function setError() {
        root.hasError = true
        backgroundRect.border.color = Style.colorErrorBorder
        backgroundRect.border.width = 2
        backgroundRect.color = Style.colorErrorBackground
    }

    function clearError() {
        root.hasError = false
        backgroundRect.border.color = root.borderColor
        backgroundRect.border.width = root.borderWidth
        backgroundRect.color = root.backgroundColor
    }
}

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.controllers
import Librum.style
import Librum.fonts

Item {
    id: root
    property bool activated: acceptCheckBox.checked
    property bool hasError: false
    signal keyUp
    signal keyDown

    implicitWidth: 200
    implicitHeight: layout.height

    onActivatedChanged: if (root.hasError)
                            root.clearError()

    RowLayout {
        id: layout
        width: parent.width
        spacing: 10

        MCheckBox {
            id: acceptCheckBox
            Layout.preferredWidth: 20
            Layout.preferredHeight: 20

            Keys.onPressed: event => {
                                if (event.key === Qt.Key_Return) {
                                    acceptCheckBox.toggle()
                                } else if (event.key === Qt.Key_Down) {
                                    root.keyDown()
                                } else if (event.key === Qt.Key_Up) {
                                    root.keyUp()
                                }
                            }
        }

        Label {
            id: text
            Layout.fillWidth: true
            textFormat: Text.RichText
            //: Make sure to translate the following words together to make a logical sentence
            text: qsTr('I accept the') + '<a href="' + AppInfoController.website
                  + '/TermsOfService" style="text-decoration: none; color: '
                  + Style.colorBasePurple + ';"> ' + qsTr('Terms of Service') + '</a>'
                  + ' ' + qsTr('and the') + ' <a href="' + AppInfoController.website
                  + '/privacypolicy" style="text-decoration: none; color: '
                  + Style.colorBasePurple + ';">' + qsTr('Privacy Policy') + '</a>'
            onLinkActivated: link => Qt.openUrlExternally(link)
            wrapMode: Text.WordWrap
            font.pointSize: Fonts.size11
            color: Style.colorText

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                cursorShape: text.hoveredLink != "" ? Qt.PointingHandCursor : Qt.ArrowCursor

                onClicked: {
                    if (text.hoveredLink != "") {
                        Qt.openUrlExternally(text.hoveredLink)
                    } else {
                        acceptCheckBox.toggle()
                    }
                }
            }
        }
    }

    function giveFocus() {
        acceptCheckBox.forceActiveFocus()
    }

    function setError() {
        root.hasError = true
        acceptCheckBox.borderWidth = 2
        acceptCheckBox.borderColor = Style.colorErrorBorder
        acceptCheckBox.uncheckedBackgroundColor = Style.colorErrorBackground
    }

    function clearError() {
        root.hasError = false
        acceptCheckBox.borderWidth = 1
        acceptCheckBox.borderColor = Style.colorCheckboxBorder
        acceptCheckBox.uncheckedBackgroundColor = "transparent"
    }
}

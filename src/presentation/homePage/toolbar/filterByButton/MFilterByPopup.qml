import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons
import Librum.fonts
import Librum.models

Popup {
    id: root
    property alias authors: authorsInput.text
    property alias format: formatInputComboBox.text
    property alias language: languageInputComboBox.text
    property alias onlyBooks: onlyBooksBox.checked
    property alias onlyFiles: onlyFilesBox.checked
    property alias read: readBox.checked
    property alias unread: unreadBox.checked
    signal filterQuerySent

    padding: 0
    implicitWidth: 273
    implicitHeight: layout.height
    background: Rectangle {
        color: "transparent"
    }

    Shortcut {
        sequence: "Return"
        enabled: root.opened
        onActivated: root.filterQuerySent()
    }

    MFlickWrapper {
        id: flickWrapper
        anchors.fill: parent
        contentHeight: layout.height

        ColumnLayout {
            id: layout
            width: parent.width
            spacing: 0

            Image {
                id: triangleDecoration
                Layout.leftMargin: 14
                Layout.bottomMargin: -1
                source: Icons.popupDroplet
            }

            Pane {
                id: container
                Layout.fillWidth: true
                padding: 14
                background: Rectangle {
                    color: Style.colorPopupBackground
                    border.width: 1
                    border.color: Style.colorContainerBorder
                    radius: 6
                    antialiasing: true
                }

                ColumnLayout {
                    id: itemLayout
                    width: parent.width
                    spacing: 11

                    MLabeledInputBox {
                        id: authorsInput
                        Layout.fillWidth: true
                        boxHeight: 30
                        headerToBoxSpacing: 2
                        placeholderContent: qsTr("e.g. Uncle bob")
                        inputFontSize: Fonts.size11
                        placeholderColor: Style.colorPlaceholderText
                        headerText: qsTr("Authors")
                        headerFontSize: Fonts.size10dot5
                        headerFontWeight: Font.Bold
                        headerFontColor: Style.colorLightText
                        textPadding: 8
                        borderWidth: 1
                        borderRadius: 4
                    }

                    RowLayout {
                        id: horizontalInputLayout
                        Layout.fillWidth: true
                        spacing: 17

                        ColumnLayout {
                            id: leftColumnLayout
                            Layout.fillWidth: true

                            MComboBox {
                                id: languageInputComboBox
                                Layout.fillWidth: true
                                Layout.preferredHeight: 49
                                Layout.topMargin: 2
                                headerText: qsTr("Language")
                                emptyText: qsTr("Any")
                                dropdownIconSize: 9

                                checkBoxStyle: false
                                checkBoxSize: 17
                                checkBoxImageSize: 9
                                itemHeight: 29
                                fontSize: Fonts.size11
                                model: MLanguageModel

                                onItemChanged: languageInputComboBox.closePopup(
                                                   )
                            }

                            MLabeledCheckBox {
                                id: readBox
                                Layout.topMargin: 16
                                Layout.fillWidth: true
                                boxWidth: 17
                                boxHeight: 17
                                spacing: 7
                                text: qsTr("Read")
                                fontSize: Fonts.size10dot5
                                fontWeight: Font.Normal
                                fontColor: Style.colorLightText
                                imageSize: 9

                                onCheckedChanged: if (checked
                                                          && unreadBox.checked)
                                                      unreadBox.checked = false
                            }

                            MLabeledCheckBox {
                                id: unreadBox
                                Layout.topMargin: 10
                                Layout.fillWidth: true
                                boxWidth: 17
                                boxHeight: 17
                                spacing: 7
                                text: qsTr("Unread")
                                fontSize: Fonts.size10dot5
                                fontWeight: Font.Normal
                                fontColor: Style.colorLightText
                                imageSize: 9

                                onCheckedChanged: if (checked
                                                          && readBox.checked)
                                                      readBox.checked = false
                            }
                        }

                        ColumnLayout {
                            id: rightColumnlayout
                            Layout.fillWidth: true

                            MComboBox {
                                id: formatInputComboBox
                                Layout.fillWidth: true
                                Layout.preferredHeight: 49
                                Layout.topMargin: 2
                                headerText: qsTr("Format")
                                emptyText: qsTr("Any")
                                dropdownIconSize: 9

                                checkBoxStyle: false
                                checkBoxSize: 17
                                checkBoxImageSize: 9
                                itemHeight: 29
                                fontSize: Fonts.size11
                                model: ListModel {
                                    ListElement {
                                        text: qsTr("Pdf")
                                    }
                                    ListElement {
                                        text: qsTr("Epub")
                                    }
                                    ListElement {
                                        text: qsTr("Mobi")
                                    }
                                    ListElement {
                                        text: qsTr("Txt")
                                    }
                                }

                                onItemChanged: formatInputComboBox.closePopup()
                            }

                            MLabeledCheckBox {
                                id: onlyBooksBox
                                Layout.topMargin: 14
                                Layout.fillWidth: true
                                boxWidth: 17
                                boxHeight: 17
                                spacing: 7
                                text: qsTr("Only Books")
                                fontSize: Fonts.size10dot5
                                fontWeight: Font.Normal
                                fontColor: Style.colorLightText
                                imageSize: 9

                                onCheckedChanged: if (checked
                                                          && onlyFilesBox.checked)
                                                      onlyFilesBox.checked = false
                            }

                            MLabeledCheckBox {
                                id: onlyFilesBox
                                Layout.topMargin: 10
                                Layout.fillWidth: true
                                boxWidth: 17
                                boxHeight: 17
                                spacing: 7
                                text: qsTr("Only Files")
                                fontSize: Fonts.size10dot5
                                fontWeight: Font.Normal
                                fontColor: Style.colorLightText
                                imageSize: 9

                                onCheckedChanged: if (checked
                                                          && onlyBooksBox.checked)
                                                      onlyBooksBox.checked = false
                            }
                        }
                    }

                    MButton {
                        id: applyButton
                        Layout.fillWidth: true
                        Layout.preferredHeight: 28
                        Layout.topMargin: 8
                        radius: 3
                        borderWidth: 0
                        backgroundColor: Style.colorBasePurple
                        text: qsTr("Apply Filter")
                        fontSize: Fonts.size10dot5
                        textColor: Style.colorFocusedButtonText
                        fontWeight: Font.Bold

                        onClicked: root.filterQuerySent()
                    }
                }
            }
        }
    }

    function resetFilter() {
        authorsInput.clearText()
        addedInput.clearText()
        formatInputComboBox.deselectCurrenItem()
        unreadBox.checked = false
        readBox.checked = false
        onlyFilesBox.checked = false
        onlyBooksBox.checked = false
    }
}

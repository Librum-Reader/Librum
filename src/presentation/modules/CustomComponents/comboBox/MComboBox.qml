import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.icons
import Librum.fonts

Item {
    id: root
    property alias text: selectionPopup.selectedContent
    property bool multiSelect: false
    property string boxBackgroundColor: Style.colorContainerBackground

    property alias model: selectionPopup.model

    // Not all models have their property called "text", thus we can't use model.get(i).text or similar,
    // this allows us to specify the property name by which we can get the content
    property alias contentPropertyName: selectionPopup.contentPropertyName

    property alias listView: selectionPopup.listView
    property alias checkBoxStyle: selectionPopup.checkBoxStyle
    property alias checkBoxSize: selectionPopup.checkBoxSize
    property alias checkBoxImageSize: selectionPopup.checkBoxImageSize
    property alias itemHeight: selectionPopup.itemHeight
    property alias maxHeight: selectionPopup.maxHeight
    property alias defaultIndex: selectionPopup.defaultIndex
    property alias fontSize: selectionPopup.fontSize
    property alias fontWeight: selectionPopup.fontWeight
    property alias popupSpacing: selectionPopup.popupSpacing
    property bool dropDownIconLeft: false
    property int spacing: 4
    property int borderWidth: 1

    property string headerText
    property int headerFontWeight: Font.Bold
    property double headerFontSize: Fonts.size10dot5
    property color headerFontColor: Style.colorLightText

    property string emptyText: qsTr("Any")
    property double selectedItemFontSize: Fonts.size11
    property color selectedItemFontColor: Style.colorText
    property int selectedItemPadding: 0

    property string dropdownIcon: Icons.dropdownLight
    property int dropdownIconSize: 6
    signal itemChanged(int index)

    implicitHeight: 47

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 2

        Label {
            id: header
            Layout.fillWidth: true
            visible: root.headerText.length > 0
            text: root.headerText
            font.pointSize: root.headerFontSize
            font.weight: root.headerFontWeight
            color: root.headerFontColor
        }

        Pane {
            id: container
            Layout.fillWidth: true
            Layout.fillHeight: true
            verticalPadding: 6
            horizontalPadding: 10
            background: Rectangle {
                color: root.boxBackgroundColor
                border.width: root.borderWidth
                border.color: Style.colorContainerBorder
                radius: 4
            }

            RowLayout {
                id: contentLayout
                width: parent.width
                anchors.centerIn: parent
                spacing: root.spacing

                Image {
                    id: dropDownIconLeft
                    visible: root.dropDownIconLeft
                    Layout.alignment: Qt.AlignRight
                    sourceSize.width: root.dropdownIconSize
                    source: root.dropdownIcon
                    rotation: -180
                    fillMode: Image.PreserveAspectFit

                    NumberAnimation {
                        id: closeAnimLeft
                        target: dropDownIconLeft
                        property: "rotation"
                        to: -180
                        duration: 175
                        easing.type: Easing.InOutQuad
                    }

                    NumberAnimation {
                        id: openAnimLeft
                        target: dropDownIconLeft
                        property: "rotation"
                        to: 0
                        duration: 175
                        easing.type: Easing.InOutQuad
                    }
                }

                Label {
                    id: selectedItem
                    Layout.fillWidth: true
                    leftPadding: root.selectedItemPadding
                    rightPadding: root.selectedItemPadding
                    text: root.text ? root.text : root.emptyText
                    font.pointSize: root.selectedItemFontSize
                    font.weight: Font.Normal
                    color: root.text ? root.selectedItemFontColor : Style.colorLightText
                    elide: Text.ElideRight
                }

                Image {
                    id: dropDownIconRight
                    visible: !root.dropDownIconLeft
                    Layout.alignment: Qt.AlignRight
                    sourceSize.width: root.dropdownIconSize
                    source: root.dropdownIcon
                    rotation: -180
                    fillMode: Image.PreserveAspectFit

                    NumberAnimation {
                        id: closeAnimRight
                        target: dropDownIconRight
                        property: "rotation"
                        to: -180
                        duration: 175
                        easing.type: Easing.InOutQuad
                    }

                    NumberAnimation {
                        id: openAnimRight
                        target: dropDownIconRight
                        property: "rotation"
                        to: 0
                        duration: 175
                        easing.type: Easing.InOutQuad
                    }
                }
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true

        onClicked: selectionPopup.opened ? selectionPopup.close(
                                               ) : selectionPopup.open()
    }

    MComboBoxPopup {
        id: selectionPopup
        property int popupSpacing: 5
        property bool fitsToBottom: false

        model: root.model
        y: layout.y + (fitsToBottom ? layout.height + popupSpacing : -popupSpacing - implicitHeight)
        width: parent.width
        multiSelect: root.multiSelect

        onClosed: root.dropDownIconLeft ? closeAnimLeft.start(
                                              ) : closeAnimRight.start()
        onItemChanged: index => root.itemChanged(index)
        onAboutToShow: {
            fitsToBottom = internal.popupFitsBelowComboBox()
            root.dropDownIconLeft ? openAnimLeft.start() : openAnimRight.start()
        }
    }

    QtObject {
        id: internal


        /**
         Returns true if the popup would fit below the combobox, returns
         false if the popup would be out of screen
         */
        function popupFitsBelowComboBox() {
            let globalMousePos = mapToGlobal(mouseArea.mouseX, mouseArea.mouseY)
            if ((globalMousePos.y + selectionPopup.height + selectionPopup.popupSpacing
                 + mouseArea.mouseY) >= baseRoot.height) {
                return false
            }

            return true
        }
    }

    function selectItem(index, initialSelect = false) {
        selectionPopup.selectItem(index, initialSelect)
    }

    function deselectCurrenItem() {
        selectionPopup.deselectCurrenItem()
    }

    function closePopup() {
        selectionPopup.close()
    }

    function setDefaultItem(itemName) {
        for (var i = 0; i < selectionPopup.model.count; i++) {
            if (selectionPopup.model.get(i)[contentPropertyName] === itemName) {
                selectItem(i)
                return
            }
        }

        selectionPopup.defaultIndex = -1
    }

    function giveFocus() {
        root.forceActiveFocus()
    }
}

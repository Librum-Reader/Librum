import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.fonts
import "ComboBoxLogic.js" as Logic

Popup {
    id: root
    property string selectedContent
    property string contentPropertyName: "text"
    property alias model: listView.model
    property alias listView: listView
    property int itemHeight: 28
    property int maxHeight: 208
    property int defaultIndex: -1
    property bool checkBoxStyle: true
    property int checkBoxSize: 18
    property int checkBoxImageSize: 9
    property bool multiSelect: false
    property double fontSize: Fonts.size11
    property int fontWeight: Font.Normal
    signal itemChanged(int index)

    padding: 8
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    focus: true
    background: Rectangle {
        color: Style.colorPopupBackground
        border.width: 1
        border.color: Style.colorContainerBorder
        radius: 5
        antialiasing: true
    }

    ColumnLayout {
        id: layout
        width: parent.width

        ListView {
            id: listView
            property MBaseListItem currentSelected

            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            Layout.maximumHeight: root.maxHeight
            maximumFlickVelocity: 550
            keyNavigationEnabled: true
            currentIndex: root.defaultIndex
            clip: true
            focus: true
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar {}
            delegate: MBaseListItem {
                width: listView.width
                height: root.itemHeight
                containingListview: listView
                fontSize: root.fontSize
                fontColor: Style.colorLightText
                checkBoxStyle: root.checkBoxStyle
                checkBoxImageSize: root.checkBoxImageSize
                checkBoxSize: root.checkBoxSize

                onClicked: (mouse, index) => root.selectItem(index)

                // Overwrite the way the model's text is queried
                function getContent() {
                    return model[root.contentPropertyName]
                }
            }

            Keys.onReturnPressed: if (listView.currentIndex !== -1)
                                      root.selectItem(listView.currentIndex)
            Component.onCompleted: if (root.defaultIndex != -1)
                                       root.selectItem(listView.currentIndex,
                                                       true)
        }
    }

    QtObject {
        id: internal
        // If multi select is turned on, multiple items can be selected, thus
        // this is a storage for all the currently selected items
        property var selectedItems: []
    }

    function selectItem(index, initialSelect = false) {
        if (root.multiSelect)
            Logic.addItemToSelectedItems(index)

        Logic.selectItem(index)
        if (!initialSelect)
            root.itemChanged(index)
    }

    function deselectCurrenItem() {
        if (listView.currentItem)
            listView.currentItem.selected = false

        listView.currentIndex = -1
        root.selectedContent = ""
        Logic.deselectCurrentItem()
    }
}

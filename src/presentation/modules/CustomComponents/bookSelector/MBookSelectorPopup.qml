import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons
import Librum.fonts

Popup {
    id: popup
    implicitWidth: 200
    implicitHeight: 360
    background: Rectangle {
        color: Style.colorControlBackground
        border.color: Style.colorContainerBorder
        radius: 5
        border.width: 2
    }

    ColumnLayout {
        id: layout
        width: parent.width

        ListView {
            id: listView
            property MBaseListItem currentSelected

            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            Layout.maximumHeight: 360
            maximumFlickVelocity: 550
            keyNavigationEnabled: true
            currentIndex: 0
            clip: true
            focus: true
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar {}

            model: ListModel {
                ListElement {
                    text: "First"
                }
                ListElement {
                    text: "Second"
                }
                ListElement {
                    text: "Third"
                }
                ListElement {
                    text: "Fourth"
                }
                ListElement {
                    text: "Fifth"
                }
                ListElement {
                    text: "Sixth"
                }
            }

            delegate: MBaseListItem {
                width: listView.width
                height: 28
                containingListview: listView
                fontSize: Fonts.size11
                fontColor: Style.colorLightText

                checkBoxStyle: false

                // checkBoxStyle: root.checkBoxStyle
                // checkBoxImageSize: root.checkBoxImageSize
                // checkBoxSize: root.checkBoxSize
                // onClicked: (mouse, index) => root.selectItem(index)

                // Overwrite the way the model's text is queried
                // function getContent() {
                //     return model[root.contentPropertyName]
                // }
            }

            // Keys.onReturnPressed: if (listView.currentIndex !== -1)
            // root.selectItem(listView.currentIndex)
            // Component.onCompleted: if (root.defaultIndex != -1)
            // root.selectItem(listView.currentIndex,
            // true)
        }
    }
}

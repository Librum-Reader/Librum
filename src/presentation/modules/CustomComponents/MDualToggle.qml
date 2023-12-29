import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import Librum.fonts


/**
 A rectangular component which switches between two states which are represented
 as texts via @leftText and @rightText
 */
Item {
    id: root
    property string leftText: "Left"
    property string leftDisplayText: qsTr("Left")
    property string rightText: "Right"
    property string rightDisplayText: qsTr("Right")
    property bool leftSelected: false
    property bool rightSelected: true // default
    property int minWidth: 178
    signal toggled(string newSelected)

    implicitHeight: 38
    implicitWidth: {
        let biggestText = Math.max(leftLabel.implicitWidth,
                                   rightLabel.implicitWidth)

        return Math.max(minWidth, biggestText * 2 + 42)
    }

    Pane {
        id: container
        anchors.fill: parent
        padding: 0
        background: Rectangle {
            color: Style.colorControlBackground
            border.color: Style.colorContainerBorder
            radius: 4
        }

        RowLayout {
            id: layout
            anchors.fill: parent
            spacing: 0

            Label {
                id: leftLabel
                Layout.fillHeight: true
                Layout.preferredWidth: (root.width - separator.width) / 2
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: root.leftDisplayText
                color: root.leftSelected ? Style.colorBasePurple : Style.colorLightText
                font.pointSize: Fonts.size12
                font.weight: root.leftSelected ? Font.Bold : Font.DemiBold
                font.letterSpacing: root.leftSelected ? -0.4 : 0
                background: Rectangle {
                    id: leftLabelBackground
                    anchors.fill: parent
                    anchors.margins: 1
                    color: Style.colorLightHighlight
                    opacity: internal.leftRectOpacity
                    radius: 4
                }


                /**
                 Due to @leftLabel having rounder corners it doesn't fill up the
                 space towards the middle (infront of the @separator).
                 This Rectangle just fills the remaining space.
                 */
                Rectangle {
                    id: leftBackgroundFiller
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: 1
                    anchors.bottomMargin: 1
                    width: 3
                    opacity: leftLabelBackground.opacity
                    color: leftLabelBackground.color
                }

                MouseArea {
                    id: leftMouseArea
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: leftMouseArea.cursorShape = Qt.PointingHandCursor

                    onClicked: root.selectLeft()
                }
            }

            Rectangle {
                id: separator
                Layout.fillHeight: true
                Layout.preferredWidth: 2
                color: Style.colorSeparator
            }

            Label {
                id: rightLabel
                Layout.fillHeight: true
                Layout.preferredWidth: (root.width - separator.width) / 2
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: root.rightDisplayText
                color: root.rightSelected ? Style.colorBasePurple : Style.colorLightText
                font.pointSize: Fonts.size12
                font.weight: root.rightSelected ? Font.Bold : Font.DemiBold
                font.letterSpacing: root.rightSelected ? -0.4 : 0
                background: Rectangle {
                    id: rightLabelBackground
                    anchors.fill: parent
                    anchors.margins: 1
                    opacity: internal.rightRectOpacity
                    color: Style.colorLightHighlight
                    radius: 4
                }


                /**
                 Due to @rightLabel having rounder corners it doesn't fill up the
                 space towards the middle (after the @separator).
                 This Rectangle just fills the remaining space.
                 */
                Rectangle {
                    id: rightBackgroundFiller
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.topMargin: 1
                    anchors.bottomMargin: 1
                    width: 3
                    opacity: rightLabelBackground.opacity
                    color: rightLabelBackground.color
                }

                MouseArea {
                    id: rightMouseArea
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: rightMouseArea.cursorShape = Qt.PointingHandCursor

                    onClicked: root.selectRight()
                }
            }
        }
    }

    SequentialAnimation {
        id: selectRightAnimation

        NumberAnimation {
            target: internal
            property: "leftRectOpacity"
            duration: 75
            to: 0
        }

        NumberAnimation {
            target: internal
            property: "rightRectOpacity"
            duration: 75
            to: 1
        }

        onFinished: {
            root.leftSelected = false
            root.rightSelected = true
            root.toggled(root.rightText)
        }
    }

    SequentialAnimation {
        id: selectLeftAnimation

        NumberAnimation {
            target: internal
            property: "rightRectOpacity"
            duration: 75
            to: 0
        }

        NumberAnimation {
            target: internal
            property: "leftRectOpacity"
            duration: 75
            to: 1
        }

        onFinished: {
            root.leftSelected = true
            root.rightSelected = false
            root.toggled(root.leftText)
        }
    }

    QtObject {
        id: internal
        property double leftRectOpacity
        property double rightRectOpacity

        Component.onCompleted: {
            // Hard-set it once during object creation, after that it
            // is managed by fluent animations
            internal.leftRectOpacity = root.leftSelected ? 1 : 0
            internal.rightRectOpacity = root.rightSelected ? 1 : 0
        }
    }

    function selectLeft() {
        if (root.leftSelected)
            return

        selectLeftAnimation.start()
    }

    function selectRight() {
        if (root.rightSelected)
            return

        selectRightAnimation.start()
    }
}

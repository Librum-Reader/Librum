import QtQuick

MProgressArc {
    id: root
    property int animDuration: 540

    implicitWidth: width
    implicitHeight: height
    width: 18
    height: 18
    value: 0.8
    arcWidth: 2
    arcCap: "round"
    arcColor: "#FFFFFF"

    NumberAnimation {
        target: root
        property: "rotation"
        duration: root.animDuration
        from: 0
        to: 360
        easing.type: Easing.Linear
        running: true
        loops: Animation.Infinite
    }
}

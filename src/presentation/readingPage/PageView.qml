import QtQuick 2.15
import QtGraphicalEffects 1.15
import org.kde.kirigami 2.17 as Kirigami
import Librum.elements 1.0
import Librum.style 1.0


Item
{
    id: root
    readonly property PageItem pageItem: page
    property alias document: page.document
    property alias pageNumber: page.pageNumber
    implicitWidth: page.implicitWidth
    implicitHeight: page.implicitHeight
    readonly property real pageRatio: page.implicitWidth / page.implicitHeight
    readonly property real scaleFactor: page.width / page.implicitWidth

    
    PageItem
    {
        id: page
        property bool sameOrientation: parent.width / parent.height > pageRatio
        anchors.centerIn: parent
        width: sameOrientation ? parent.height * pageRatio : parent.width
        height: !sameOrientation ? parent.width / pageRatio : parent.height
    }

    Rectangle
    {
        id: backgroundRectangle
        anchors
        {
            top: parent.top
            bottom: parent.bottom
            left: page.left
            right: page.right
            topMargin: -Math.round(root.height / 96)
            bottomMargin: -Math.round(root.height / 96)
        }
        z: -1
        color: Style.pagesBackground
    }
}

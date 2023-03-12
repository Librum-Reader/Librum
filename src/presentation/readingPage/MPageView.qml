import QtQuick 2.15
import QtQuick.Controls 2.15
import Librum.elements 1.0
import Librum.style 1.0

/*
  The book page
  */
Item
{
    id: root
    property var container
    property bool isLastPage: pageNumber == container.count - 1
    readonly property PageItem pageItem: page
    readonly property int pageSpacing: root.isLastPage ? 0 : Math.round(10 * scaleFactor)
    property alias document: page.document
    property alias pageNumber: page.pageNumber
    readonly property real pageRatio: page.implicitWidth / page.implicitHeight
    readonly property real scaleFactor: page.width / page.implicitWidth
    
    
    PageItem
    {
        id: page
        height: root.height - root.pageSpacing
        width: root.width
        
        
        Rectangle
        {
            id: pageBackground
            z: -1
            anchors.fill: parent
            color: Style.colorReadingViewBackground
        }
    }
    
    Rectangle
    {
        id: bottomSpacing
        anchors.top: page.bottom
        height: root.pageSpacing
        width: root.width
        color: "transparent"
    }
}
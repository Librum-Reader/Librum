import QtQuick 2.15
import QtQuick.Controls 2.15
import Librum.elements 1.0
import Librum.style 1.0

/**
  The book page
  */
Item
{
    id: root
    property var container
    readonly property PageItem pageItem: page
    property alias document: page.document
    property alias pageNumber: page.pageNumber
    property real pageRatio: page.implicitWidth / page.implicitHeight
    readonly property real scaleFactor: page.width / page.implicitWidth
    
    PageItem
    {
        id: page
        anchors.fill: parent
        
        
        Rectangle
        {
            id: pageBackground
            z: -1
            anchors.fill: parent
            color: Style.colorReadingViewBackground
        }
    }
}
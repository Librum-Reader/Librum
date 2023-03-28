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
    readonly property PageItem pageItem: page
    property alias document: page.document
    property alias pageNumber: page.pageNumber
    readonly property bool isLastPage: pageNumber == document.pageCount - 1
    readonly property real pageRatio: page.implicitWidth / page.implicitHeight
    property int pageSpacing
    readonly property int adaptedWidth: page.width
    
    PageItem
    {
        id: page
        height: parent.height - root.pageSpacing
        width: root.height * root.pageRatio
        
        
        Rectangle
        {
            id: pageBackground
            z: -1
            anchors.fill: parent
            color: Style.colorReadingViewBackground
        }
    }
    
    // TODO: Fix the way we handle the last item spacing
    Rectangle
    {
        id: bottomSpacing
        anchors.top: page.bottom
        height: root.pageSpacing
        width: root.width
        // If the current page is the book's last page, we dont want a space at the
        // bottom, so just fill it with the page's background color.
        color: root.isLastPage ? Style.colorReadingViewBackground : "transparent"
    }
}
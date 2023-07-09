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
    property int pageNumber: 0
    readonly property bool isLastPage: pageNumber == 0
    readonly property real pageRatio: page.implicitWidth / page.implicitHeight
    readonly property int adaptedWidth: page.width
    
    
    Item
    {
        id: page
        height: parent.height
        width: root.height * root.pageRatio
        
        
        Rectangle
        {
            id: pageBackground
            z: -1
            anchors.fill: parent
            color: Style.colorReadingViewBackground
        }
    }
}
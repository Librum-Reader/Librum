import QtQuick
import Librum.style

Rectangle
{
    id: root
    property double progress
    
    implicitWidth: 110
    implicitHeight: 12
    color: Style.colorBookBackground
    opacity: 0.75
    
    Rectangle
    {
        id: fillRect
        width: root.width * root.progress
        height: parent.height
        color: Style.colorBasePurple
        opacity: 1.0
    }
}
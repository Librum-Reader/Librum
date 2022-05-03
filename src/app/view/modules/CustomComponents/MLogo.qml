import QtQuick
import QtQuick.Layouts

FocusScope
{
    id: root
    implicitWidth: 72
    implicitHeight: 72
    
    Rectangle
    {
        id: outerRect
        anchors.fill: parent
        color: properties.colorBasePurple
        radius: 8
        antialiasing: true
        
        Rectangle
        {
            id: innerRect
            width: parent.width / 2
            height: parent.height / 2
            anchors.centerIn: parent
            color: properties.loginContentBackground
            radius: width / 4
            antialiasing: true
        }
    }
}
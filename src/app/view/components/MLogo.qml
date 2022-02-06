import QtQuick
import QtQuick.Layouts

FocusScope
{
    id: root
    height: 72
    width: 72
    
    Rectangle
    {
        id: logo
        width: root.height
        height: root.width
        color: properties.colorBasePurple
        radius: 8
        antialiasing: true
        
        Rectangle
        {
            anchors.centerIn: parent
            width: parent.width / 2
            height: parent.height / 2
            radius: width / 4
            color: properties.loginContentBackground
        }
    }
}
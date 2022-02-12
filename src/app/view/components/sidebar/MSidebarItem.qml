import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

FocusScope
{
    id: root
    implicitWidth: 52
    implicitHeight: 44
    
    property int imageWidth : 30
    property int imageHeight : 30
    property string imageSource : ""
    
    Rectangle
    {
        width: parent.implicitWidth
        height: parent.implicitHeight
        radius: 4
        color: (focus ? properties.colorSidebarMark : "transparent")
      
        Image
        {
            sourceSize.height: root.imageHeight
            sourceSize.width: root.imageWidth
            anchors.centerIn: parent
            source: root.imageSource
            antialiasing: false                    
        }
    }
}
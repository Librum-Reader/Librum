import QtQuick
import QtQuick.Controls


FocusScope
{
    id: root
    property bool toLeft : false
    signal clicked
    
    implicitWidth: image.implicitWidth
    implicitHeight: image.implicitHeight
    
    Image
    {
        id: image
        rotation: (root.toLeft ? 180 : 0)
        source: "/resources/images/black_right_icon.svg"
        sourceSize.width: 30
        
        MouseArea
        {
            anchors.fill: parent
            
            onClicked: root.clicked()
        }
    }
}

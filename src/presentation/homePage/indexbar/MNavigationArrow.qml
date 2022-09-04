import QtQuick 2.15
import QtQuick.Controls 2.15
import Librum.icons 1.0


Item
{
    id: root
    property bool toLeft : false
    signal clicked
    
    implicitWidth: image.implicitWidth + 5
    implicitHeight: image.implicitHeight
    
    Image
    {
        id: image
        anchors.right: parent.right
        rotation: (root.toLeft ? 180 : 0)
        source: Icons.arrowtopGrayRight
        sourceSize.width: 9
    }
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked: root.clicked()
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}

import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Item 
{
    id: root
    property bool onByDefault: false
    property bool currentlyOn: dualToggle.rightSelected
    property alias onText: dualToggle.rightProperty
    property alias offText: dualToggle.leftProperty
    signal toggled
 
    implicitHeight: 38
    implicitWidth: 178
    
    MDualToggle
    {
        id: dualToggle
        anchors.fill: parent
        leftProperty: "OFF"
        rightProperty: "ON"
        leftSelected: !onByDefault
        rightSelected: onByDefault
        
        onSelectedChanged: root.toggled()
    }
}
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

/**
 A specialisation of MDualToggle to create a more specific interface
 */
Item 
{
    id: root
    property bool onByDefault: false
    property bool currentlyOn: dualToggle.rightSelected
    property alias onText: dualToggle.rightText
    property alias offText: dualToggle.leftText
    signal toggled
 
    implicitHeight: 38
    implicitWidth: 178
    
    MDualToggle
    {
        id: dualToggle
        anchors.fill: parent
        leftText: "OFF"
        rightText: "ON"
        leftSelected: !onByDefault
        rightSelected: onByDefault
        
        onToggled: root.toggled()
    }
    
    
    function setOn()
    {
        dualToggle.selectRight();
    }
    
    function setOff()
    {
        dualToggle.selectLeft();
    }
}
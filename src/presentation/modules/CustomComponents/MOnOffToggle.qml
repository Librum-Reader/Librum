import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

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
    signal toggled(string value)
 
    implicitHeight: 38
    implicitWidth: 178
    
    MDualToggle
    {
        id: dualToggle
        anchors.fill: parent
        leftText: qsTr("OFF")
        rightText: qsTr("ON")
        leftSelected: !onByDefault
        rightSelected: onByDefault
        
        onToggled: root.toggled(root.currentlyOn ? root.onText : root.offText)
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

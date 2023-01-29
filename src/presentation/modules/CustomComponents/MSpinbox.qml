import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0
import Librum.icons 1.0

/**
 A box with an editable text input which contains a number and can also
 be changed by arrows next to the box.
 */
Item
{
    id: root
    property bool invalid: false
    property int value: 14
    property int maxVal: 99
    property int minVal: 1
    signal newValueSelected()
    
    implicitWidth: 72
    implicitHeight: 32
    
    Keys.onPressed:(event) => internal.handleKeyInput(event)
    
    
    Pane
    {
        id: container
        anchors.fill: parent
        padding: 0
        background: Rectangle
        {
            color: Style.colorBackground
            border.color: root.invalid ? Style.colorBaseRed : Style.colorLightBorder
            border.width: root.invalid ? 2 : 1
            radius: 4
        }
        
        
        RowLayout
        {
            id: layout
            anchors.fill: parent
            spacing: 0
            
            
            TextField
            {
                id: inputField
                Layout.fillHeight: true
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                selectByMouse: true
                color: Style.colorLightText3
                font.pointSize: 12
                font.weight: Font.Bold
                validator: IntValidator { bottom: root.minVal; top: root.maxVal }
                text: root.value.toString()
                background: Rectangle   
                {   
                    anchors.fill: parent
                    radius: 4
                    color: "transparent"
                }
                
                // Validate new value before applying
                onTextEdited:
                {
                    if(!internal.isValid())
                    {
                        root.invalid = true;
                    }
                    else
                    {
                        root.value = text;
                        root.invalid = false;
                        root.newValueSelected();
                    }
                }
            }
            
            ColumnLayout
            {
                id: arrowLayout
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                Layout.rightMargin: 14
                spacing: 4
                
                
                Image
                {
                    id: upArrow
                    source: Icons.dropdownGray
                    sourceSize.width: 9
                    fillMode: Image.PreserveAspectFit
                    rotation: 180
                    
                    MouseArea
                    {
                        anchors.fill: parent
                        
                        onClicked: internal.increaseValue()
                    }
                }
                
                Image
                {
                    id: downArrow
                    source: Icons.dropdownGray
                    sourceSize.width: 9
                    fillMode: Image.PreserveAspectFit
                    
                    MouseArea
                    {
                        anchors.fill: parent
                        
                        onClicked: internal.decreaseValue()
                    }
                }
            }
        }
    }
    
    QtObject
    {
        id: internal
        
        function handleKeyInput(event)
        {
            if(event.key === Qt.Key_Up)
            {
                if(value < maxVal)
                    value += 1;
            }
            else if(event.key === Qt.Key_Down)
            {
                if(value > minVal)
                    value -= 1;
            }
        }
        
        function isValid()
        {
            if(inputField.text < root.minVal || inputField.text > root.maxVal)
                return false;
            
            return true;
        }
        
        function increaseValue()
        {
            root.forceActiveFocus();
            if(root.value > root.maxVal)
                return;
            
            root.value += 1;
            root.newValueSelected();
            root.invalid = false;
        }
        
        function decreaseValue()
        {
            root.forceActiveFocus();
            if(root.value < root.minVal)
                return;
            
            root.value -= 1;
            root.newValueSelected();
            root.invalid = false;
        }
    }
}
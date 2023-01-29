import QtQuick 2.15
import CustomComponents 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0

/**
 A radio button selector for string items
 */
Item
{
    id: root
    property var options: []
    property string currentSelected: options[0]
    signal newCurrentSelected()
    
    implicitWidth: 120
    implicitHeight: layout.height
    
    
    ColumnLayout
    {
        id: layout
        width: parent.width
        spacing: 10
        
        
        Repeater
        {
            model: options
            delegate: MRadioButton
            {
                required property int index
                
                selected: root.currentSelected === root.options[index]
                text: root.options[index]
                
                onClicked: internal.changeSelected(index)
            }
        }
    }
    
    QtObject
    {
        id: internal
        
        function changeSelected(index)
        {
            root.currentSelected = root.options[index];
            root.newCurrentSelected();
        }
    }
}
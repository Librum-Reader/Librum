import QtQuick
import CustomComponents
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style


Item
{
    id: root
    property var options: []
    property string currentSelected
    
    implicitWidth: 120
    implicitHeight: layout.height
    
    
    ColumnLayout
    {
        id: layout
        property var selectedItem
        
        width: parent.width
        
        
        Repeater
        {
            model: options
            delegate: Rectangle
            {
                id: delRoot
                required property int index
                property bool selected: false
                
                width: layout.width
                height: componentLayout.height
                
                
                RowLayout
                {
                    id: componentLayout
                    width: parent.width
                    height: button.implicitHeight
                    spacing: 8
                    
                    
                    MRadioButton
                    {
                        id: button
                        selected: delRoot.selected
                        
                        onClicked: layout.changeSelected(delRoot)
                    }
                    
                    Label
                    {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter
                        text: root.options[index]
                        font.family: Style.defaultFontFamily
                        font.pointSize: 12
                        font.weight: delRoot.selected ? Font.DemiBold : Font.Medium
                        color: delRoot.selected ? Style.colorBasePurple : Style.colorBaseText
                    }
                }
            }
        }
        
        function changeSelected(item)
        {
            if(layout.selectedItem)
                layout.selectedItem.selected = false;
            
            item.selected = true;
            layout.selectedItem = item;
            root.currentSelected = root.options[item.index]
        }
    }
}
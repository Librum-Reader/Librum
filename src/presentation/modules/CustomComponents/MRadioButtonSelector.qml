import QtQuick
import CustomComponents
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style


Item
{
    id: root
    property var options: []
    property string currentSelected: options[0]
    
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
            delegate: Rectangle
            {
                id: delRoot
                required property int index
                property bool selected: root.currentSelected === options[index]
                
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
                        
                        onClicked: layout.changeSelected(index)
                    }
                    
                    Label
                    {
                        Layout.fillWidth: true
                        Layout.maximumWidth: 200
                        Layout.alignment: Qt.AlignVCenter
                        text: root.options[index]
                        font.family: Style.defaultFontFamily
                        font.pointSize: 12
                        font.weight: delRoot.selected ? Font.Medium : Font.Normal
                        color: Style.colorBaseText
                        
                        MouseArea
                        {
                            anchors.fill: parent
                            
                            onClicked: layout.changeSelected(index)
                        }
                    }
                    
                    Item { Layout.fillWidth: true }
                }
            }
        }
        
        function changeSelected(index)
        {
            root.currentSelected = root.options[index];
        }
    }
}
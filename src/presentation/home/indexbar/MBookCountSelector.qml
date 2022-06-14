import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import Librum.icons


Item
{
    id: root
    property int selectedAmountOfBooks: 12
    signal selected
    
    implicitWidth: 58
    implicitHeight: 32
    
    
    Rectangle
    {
        id: container
        anchors.fill: parent
        color: Style.colorBackground
        border.color: Style.colorLightBorder
        radius: 4
        antialiasing: true
        
        
        RowLayout
        {
            id: layout
            anchors.centerIn: parent
            spacing: 5
            
            
            Label
            {
                id: bookCountLabel
                text: root.selectedAmountOfBooks.toString()
                color: Style.colorBaseText
                font.family: Style.defaultFontFamily
                font.pointSize: 11
                font.bold: true
            }
            
            Image
            {
                id: dropDownArrowImage
                source: Icons.dropDownGray
                sourceSize.width: 8
                fillMode: Image.PreserveAspectFit
            }
        }
    }
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked: selected()
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}

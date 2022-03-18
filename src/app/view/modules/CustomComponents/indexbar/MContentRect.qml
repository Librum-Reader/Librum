import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


FocusScope
{
    id: root
    property int pageNumber : -1
    property bool selected : false
    property int selectedFontSize : 12
    property int normalFontSize : 11
    signal clicked
    
    implicitWidth: 36
    implicitHeight: 36
    
    
    Rectangle
    {
        id: contentRect1
        width: parent.width
        height: parent.height
        color: (root.selected ? properties.colorBackground : "transparent")
        border.color: (root.selected ? properties.colorLightBorder : "transparent")
        radius: 5
        
        Label
        {
            anchors.centerIn: parent
            text: root.pageNumber.toString()
            font.pointSize: (root.selected ? root.selectedFontSize : root.normalFontSize)
            font.family: properties.defaultFontFamily
            font.bold: root.selected
            color: (root.selected ? properties.colorBasePurple : properties.colorBaseText)
        }
    }
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        
        onClicked: root.clicked()
    }
}
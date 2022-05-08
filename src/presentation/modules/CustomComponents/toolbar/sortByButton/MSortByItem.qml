import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope
{
    id: root
    property bool selected : false
    property string text : "My item"
    signal clicked
    
    implicitWidth: 137
    implicitHeight: 30
    
    
    Pane
    {
        id: container
        anchors.fill: parent
        verticalPadding: 0
        horizontalPadding: 10
        background: Rectangle
        {
            color: (root.selected) ? properties.colorSidebarMark : properties.colorBackground
            radius: 4
            antialiasing: true
        }
        
        
        Label
        {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            color: properties.colorLightText3
            text: root.text
            font.pointSize: 11.5
            font.family: properties.defaultFontFamily
            font.weight: Font.Bold
        }
    }
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked: root.clicked()
    }
}

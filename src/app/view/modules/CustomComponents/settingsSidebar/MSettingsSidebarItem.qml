import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

FocusScope
{
    id: root
    property bool selected : false
    property int imageLeftMargin : 0
    property int imageWidth : 0
    property int labelLeftMargin : 0
    property int labelTopMargin : 0
    property int textVerticalAlignment: Text.AlignVCenter
    property string textContent : "<Text here>"
    property string defaultIcon : "<icon here>"
    property string selectedIcon : "<icon here>"
    
    implicitWidth: 235
    implicitHeight: 32
    
    
    Rectangle
    {
        id: container
        anchors.fill: parent
        color: (root.selected ? "#F0F0FF" : "transparent")
        
        RowLayout
        {
            id: layout
            height: parent.height
            width: parent.width
            spacing: 0
            
            
            Image
            {
                id: icon
                Layout.leftMargin: root.imageLeftMargin
                source: (root.selected ? root.selectedIcon : root.defaultIcon)
                fillMode: Image.PreserveAspectFit
                sourceSize.width: root.imageWidth
            }
            
            Label
            {
                id: text
                Layout.topMargin: root.labelTopMargin
                Layout.leftMargin: root.labelLeftMargin
                verticalAlignment: textVerticalAlignment
                text: root.textContent
                color: (root.selected ? "#271FE0" : properties.colorBaseText)
                font.pointSize: 13
                font.family: properties.defaultFontFamily
                font.weight: (root.selected ? Font.DemiBold : Font.Normal)
            }
            
            Item { Layout.fillWidth: true }
            
            Rectangle
            {
                id: sideBorder
                visible: root.selected
                Layout.preferredWidth: 2
                Layout.preferredHeight: parent.height
                color: "#271FE0"
            }
        }
    }
}
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Librum.style 1.0


Item
{
    id: root
    property bool selected : false
    property int imageLeftMargin: 0
    property int imageWidth: 0
    property int labelLeftMargin: 0
    property int labelTopMargin: 0
    property int textVerticalAlignment: Text.AlignVCenter
    property string textContent
    property string defaultIcon
    property string selectedIcon
    signal clicked
    
    implicitWidth: 235
    implicitHeight: 32
    
    
    Rectangle
    {
        id: container
        anchors.fill: parent
        color: root.selected ? Style.colorLightHighlight : "transparent"
        
        
        RowLayout
        {
            id: layout
            anchors.fill: parent
            spacing: 0
            
            
            Image
            {
                id: icon
                Layout.leftMargin: root.imageLeftMargin
                source: root.selected ? root.selectedIcon : root.defaultIcon
                fillMode: Image.PreserveAspectFit
                sourceSize.width: root.selected ? root.imageWidth + 1 : root.imageWidth
            }
            
            Label
            {
                id: text
                Layout.topMargin: root.labelTopMargin
                Layout.leftMargin: root.labelLeftMargin
                verticalAlignment: textVerticalAlignment
                text: root.textContent
                color: root.selected ? Style.colorBasePurple : Style.colorText
                font.pointSize: 13
                font.weight: root.selected ? Font.DemiBold : Font.Normal
            }
            
            Item { id: widthFiller; Layout.fillWidth: true }
            
            Rectangle
            {
                id: sideBorder
                visible: root.selected
                Layout.preferredWidth: 2
                Layout.fillHeight: true
                color: Style.colorBasePurple
            }
        }
       
        MouseArea
        {
            anchors.fill: parent
            
            onClicked: root.clicked()
        }
    }
}
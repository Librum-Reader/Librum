import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style


Item
{
    id: root
    property bool selected
    property string imagePath
    property int imageSize: 18
    property string text
    signal clicked    
    
    implicitHeight: 26
    implicitWidth: 100
    
    
    Pane
    {
        id: container
        anchors.fill: parent
        horizontalPadding: 12
        verticalPadding: 4
        background: Rectangle
        {
            color: root.selected ? Style.colorSidebarMark : "transparent"
            radius: 4
        }
        
        
        RowLayout
        {
            id: mainLayout
            anchors.fill: parent
            spacing: 10
            
            
            Item
            {
                id: imageContainer
                Layout.preferredHeight: 18
                Layout.preferredWidth: 18
                Layout.alignment: Qt.AlignLeft
                
                
                Image
                {
                    id: actionImage
                    anchors.centerIn: parent
                    source: root.imagePath
                    sourceSize.width: root.imageSize
                    fillMode: Image.PreserveAspectFit
                }
            }
            
            Label
            {
                id: actionText
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                text: root.text
                font.weight: Font.Medium
                font.pointSize: 11
                font.family: Style.defaultFontFamily
                color: Style.colorLightText3
                elide: Text.ElideRight
            }
        }
    }
}
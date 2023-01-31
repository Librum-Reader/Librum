import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0


Item
{
    id: root
    property bool selected
    property string imagePath
    property int imageSize: 18
    property string text
    signal clicked    
    
    implicitHeight: 32
    implicitWidth: 100
    
    onVisibleChanged: selected = false
    
    
    Pane
    {
        id: container
        width: parent.width - 2
        height: parent.height
        anchors.centerIn: parent
        horizontalPadding: 12
        verticalPadding: 4
        background: Rectangle
        {
            color: root.selected ? Style.colorSidebarMark : "transparent"
            radius: 3
        }
        
        
        RowLayout
        {
            id: layout
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
                Layout.topMargin: -1
                text: root.text
                font.weight: Font.Medium
                font.pointSize: 11
                color: Style.colorLightText
                elide: Text.ElideRight
            }
        }
    }
    
    MouseArea
    {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: root.selected = true;
        onExited: root.selected = false;
        
        onClicked: root.clicked()
    }
}
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0


Item
{
    id: root
    property bool selected: false
    property string text
    property string image
    property int imageWidth: 30
    property int imageHeight
    property bool preserveImageFit: true
    property alias openAnimation: openAnim
    property alias closeAnimation: closeAnim
    property alias labelVisibility: label.visible
    property real textOpacity: 0
    signal clicked()
    
    implicitWidth: (labelVisibility ? internal.openedWidth : internal.closedWidth)
    implicitHeight: 44
    
    
    Pane
    {
        id: container
        anchors.fill: parent
        padding: 0
        background: Rectangle
        {
            color: root.selected ? Style.colorSidebarMark : "transparent"
            radius: 4
        }
        
        
        RowLayout
        {
            id: layout
            height: parent.height
            spacing: 0
            
            /*
              Needs a container because the actual item is bigger than just the icon
              */
            Rectangle
            {
                id: iconContainer
                Layout.preferredWidth: 52
                Layout.preferredHeight: 44
                radius: 4
                color: "transparent"
                
                Image
                {
                    id: icon
                    sourceSize.width: root.imageWidth
                    sourceSize.height: root.imageHeight
                    fillMode: root.preserveImageFit ? Image.PreserveAspectFit : Image.Stretch
                    anchors.centerIn: parent
                    source: root.image
                    antialiasing: false                    
                }
            }
            
            Label
            {
                id: label
                Layout.leftMargin: 10
                visible: false
                opacity: root.textOpacity
                text: root.text
                font.weight: Font.Medium
                font.pointSize: 13
                color: Style.colorLightText3
            }
        }
    }
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked: root.clicked();
    }
    
    
    PropertyAnimation
    {
        id: openAnim
        target: container
        property: "width"
        to: internal.openedWidth
        duration: 250
        easing.type: Easing.InOutQuad
    }
    
    PropertyAnimation
    {
        id: closeAnim
        target: container
        property: "width"
        to: internal.closedWidth
        duration: 250
        easing.type: Easing.InOutQuad
    }
    
    QtObject
    {
        id: internal
        property int closedWidth: 52
        property int openedWidth: 177
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }    
}
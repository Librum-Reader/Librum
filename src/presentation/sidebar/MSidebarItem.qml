import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style


Item
{
    id: root
    property int closedWidth : 52
    property int openedWidth : 177
    property int imageWidth : 30
    property int imageHeight : 30
    property string imageSource : ""
    property string labelContent : "Content here"
    property alias  labelVisibility : label.visible
    property alias  openAnimation : openAnim
    property alias  closeAnimation : closeAnim
    property real   textOpacity : 0
    property bool   selected : false
    signal clicked()
    
    implicitWidth: (labelVisibility ? openedWidth : closedWidth)
    implicitHeight: 44
    
    
    Pane
    {
        id: content
        anchors.fill: parent
        padding: 0
        background: Rectangle
        {
            color: (root.selected ? Style.colorSidebarMark : "transparent")
            radius: 4
        }
        
        
        RowLayout
        {
            id: layout
            height: parent.height
            spacing: 0
            
            Rectangle
            {
                id: iconBox
                Layout.preferredWidth: 52
                Layout.preferredHeight: 44
                radius: 4
                color: "transparent"
                
                Image
                {
                    id: icon
                    sourceSize.width: root.imageWidth
                    sourceSize.height: root.imageHeight
                    anchors.centerIn: parent
                    source: root.imageSource
                    antialiasing: false                    
                }
            }
            
            Label
            {
                id: label
                Layout.leftMargin: 10
                visible: false
                opacity: root.textOpacity
                text: root.labelContent
                font.family: Style.defaultFontFamily
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
        target: content
        property: "width"
        to: openedWidth
        duration: 250
        easing.type: Easing.InOutQuad
    }
    
    PropertyAnimation
    {
        id: closeAnim
        target: content
        property: "width"
        to: closedWidth
        duration: 250
        easing.type: Easing.InOutQuad
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }    
}
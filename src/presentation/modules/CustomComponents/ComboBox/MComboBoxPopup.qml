import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup
{
    id: root
    property string selectedContent : listView.currentItem.content
    property ListModel listContent
    property int maxHeight: 250
    property int radius : 5
    
    padding: 0
    focus: true
    implicitWidth: 300
    implicitHeight: container.implicitHeight
    background: Rectangle
    {
        color: "transparent"
    }
    
    
    Pane
    {
        id: container
        width: parent.width
        implicitHeight: mainLayout.height
        horizontalPadding: 8
        verticalPadding: 0
        focus: true
        background: Rectangle
        {
            color: properties.colorBackground
            border.width: 1
            border.color: properties.colorLightBorder
            radius: root.radius
            antialiasing: true
        }
        
        ColumnLayout
        {
            id: mainLayout
            width: parent.width
            
            
            Component
            {
                id: highlighter
                Rectangle
                {
                    width: 180; height: 40
                    color: "lightsteelblue"; radius: 5
                }
            }    
            
            ListView 
            {
                id: listView
                property int moveSpeed: 450
                
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                Layout.maximumHeight: root.maxHeight      
                
                highlight: highlighter
                highlightMoveDuration: 100
                highlightMoveVelocity: 100
                keyNavigationEnabled: true
                keyNavigationWraps: false                
                clip: true
                focus: true
                currentIndex: 0
                highlightFollowsCurrentItem: true
                boundsBehavior: Flickable.StopAtBounds
                
                model: root.listContent
                delegate: MComboBoxItem { } 
            }
        }
    }
}
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents

Popup
{
    id: root
    property string selectedContent : listView.currentItem.content
    property alias listContent : listView.model
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
        padding: 8
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
            
            
            ListView 
            {
                id: listView
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                Layout.maximumHeight: root.maxHeight      
                model: sortedModel
                delegate: MComboBoxItem { container: listView }                
                maximumFlickVelocity: 550
                currentIndex: 0
                keyNavigationEnabled: true
                clip: true
                focus: true
                boundsBehavior: Flickable.StopAtBounds
                highlightMoveDuration: 0
                highlightFollowsCurrentItem: true
                
                ScrollBar.vertical: ScrollBar { }
                highlight: Rectangle
                {
                    radius: 4
                    color: properties.colorLightPurple
                }
            }
        }
    }
    
    
    
    function closeComboBox()
    {
        root.close();
    }
    
    function comobBoxIsOpened()
    {
        return root.opened;
    }
}
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup
{
    id: root
    property ListModel listContent
    property int maxHeight: 250
    property int radius : 5
    
    padding: 0
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
            
            ScrollView
            {
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                Layout.maximumHeight: root.maxHeight
                
                
                ListView
                {
                    id: listview
                    property int moveSpeed: 450
                    
                    anchors.fill: parent
                    anchors.rightMargin: 10
                    clip: true
                    
                    model: root.listContent
                    delegate: MComboBoxItem { }
                    
                    MouseArea
                    {
                        anchors.fill: parent
                        
                        onWheel: (wheel) =>
                        {
                            listview.moveContent(wheel.angleDelta.y > 0)
                        }
                    }
                    
                    function moveContent(up)
                    {
                        listview.flick(0, up ? listview.moveSpeed : -listview.moveSpeed)
                    }
                }
            }
        }
    }
}
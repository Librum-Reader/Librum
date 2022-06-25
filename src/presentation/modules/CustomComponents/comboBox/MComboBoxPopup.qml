import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import CustomComponents


Popup
{
    id: root
    property string selectedContent: listView.currentItem.content
    property color backgroundColor
    property alias listContent: listView.model
    property int maxHeight: 200
    property int radius: 5
    property int defaultIndex: 0
    
    property int fontSize: 11
    property color fontColor: Style.colorBaseText
    property int fontWeight: Font.Normal
    property string fontFamily: Style.defaultFontFamily
    property string highlightColor: Style.colorLightPurple
    
    padding: 0
    implicitWidth: 300
    implicitHeight: container.implicitHeight
    focus: true
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
            color: root.backgroundColor
            border.width: 1
            border.color: Style.colorLightBorder
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
                maximumFlickVelocity: 550
                currentIndex: root.defaultIndex
                keyNavigationEnabled: true
                clip: true
                focus: true
                boundsBehavior: Flickable.StopAtBounds
                highlightMoveDuration: 0
                highlightFollowsCurrentItem: true
                
                delegate: MComboBoxItem
                {
                    container: listView
                    fontSize: root.fontSize
                    fontColor: root.fontColor
                    fontWeight: root.fontWeight
                    fontFamily: root.fontFamily
                    radius: root.radius - 1
                }
                
                ScrollBar.vertical: ScrollBar { }
                highlight: Rectangle
                {
                    radius: 4
                    color: root.highlightColor
                }
            }
        }
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
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
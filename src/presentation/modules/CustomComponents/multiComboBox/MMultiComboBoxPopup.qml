import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import CustomComponents


Popup
{
    id: root
    property var selectedItems: []
    property alias model: listView.model
    property int itemHeight: 30
    property color backgroundColor
    property int maxHeight: 208
    property int radius: 5
    property int defaultIndex: 0
    signal itemsChanged
    
    property int checkBoxSize: 18
    property int checkBoxImageSize: 18
    
    property double fontSize: 11
    property color fontColor: Style.colorLightText3
    property int fontWeight: Font.Normal
    property string fontFamily: Style.defaultFontFamily
    property string highlightColor: Style.colorLightPurple
    
    implicitWidth: 300
    padding: 8
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
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
            
            delegate: MMultiComboBoxItem
            {
                height: root.itemHeight
                container: listView
                fontSize: root.fontSize
                fontColor: root.fontColor
                fontWeight: root.fontWeight
                fontFamily: root.fontFamily
                radius: root.radius - 1
                
                onItemSelected:
                    (index) =>
                    {
                        let content = listView.itemAtIndex(index).content;
                        root.selectedItems.push(content);
                        root.itemsChanged();
                    }
                
                onItemDeselected:
                    (index) =>
                    {
                        let content = listView.itemAtIndex(index).content;
                        root.selectedItems = removeFromArray(root.selectedItems, content);
                        root.itemsChanged();
                    }
                
                
                function removeFromArray(arr, value)
                { 
                    return arr.filter(function(ele){ 
                        return ele !== value; 
                    });
                }
            }
            
            ScrollBar.vertical: ScrollBar { }
        }
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}
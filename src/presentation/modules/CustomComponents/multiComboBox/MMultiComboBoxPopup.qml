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
    property int checkBoxImageSize: 10
    
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
            property MBaseListItem currentSelected
            
            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            Layout.maximumHeight: root.maxHeight
            maximumFlickVelocity: 550
            currentIndex: root.defaultIndex
            keyNavigationEnabled: true
            clip: true
            focus: true
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar { }
            
            delegate: MBaseListItem
            {
                width: listView.width
                height: root.itemHeight
                containingListview: listView
                fontSize: root.fontSize
                fontColor: root.fontColor
                checkBoxImageSize: root.checkBoxImageSize
                checkBoxSize: root.checkBoxSize
                
                onClicked:
                    (index) =>
                    {
                        listView.addItemToList(index);
                        listView.changeSelected(index);
                    }
            }
            
            Keys.onReturnPressed:
            {
                listView.addItemToList(listView.currentIndex);
                listView.changeSelected(listView.currentIndex);
            }
            
        
            function addItemToList(index)
            {
                if(root.selectedItems.includes(listView.itemAtIndex(index).text))
                {
                    let text = listView.itemAtIndex(index).text;
                    root.selectedItems = removeFromArray(root.selectedItems, text);
                    root.itemsChanged();
                }
                else
                {
                    let text = listView.itemAtIndex(index).text;
                    root.selectedItems.push(text);
                    root.itemsChanged();
                }
            }
            
            function removeFromArray(arr, value)
            { 
                return arr.filter(function(ele){ 
                    return ele !== value; 
                });
            }
            
            function changeSelected(index)
            {
                listView.currentIndex = index;
                
                listView.itemAtIndex(index).selected = !listView.itemAtIndex(index).selected;
            }
        }
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import CustomComponents


Popup
{
    id: root
    property string selectedContents
    property alias model: listView.model
    property int itemHeight: 28
    property color backgroundColor
    property int maxHeight: 208
    property int radius: 5
    property int defaultIndex: -1
    property bool checkBoxStyle: true
    property int checkBoxSize: 18
    property int checkBoxImageSize: 9
    property bool multiSelect: false    
    property double fontSize: 11
    property color fontColor: Style.colorLightText3
    property int fontWeight: Font.Normal
    property string fontFamily: Style.defaultFontFamily
    property string highlightColor: Style.colorLightPurple
    
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
            property var currentSelectedItems: []
            
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
                checkBoxStyle: root.checkBoxStyle
                checkBoxImageSize: root.checkBoxImageSize
                checkBoxSize: root.checkBoxSize
                
                onClicked:
                    (index) =>
                    {
                        if(root.multiSelect)
                        {
                            listView.addItemToList(index);
                            listView.changeSelectedForMultiSelect(index);
                            return;
                        }
                        
                        listView.changeSelectedForSingleSelect(index);
                    }
            }
            
            Keys.onReturnPressed:
            {
                if(listView.currentIndex !== -1)
                {
                    if(root.multiSelect)
                    {
                        addItemToList(listView.currentIndex);
                        changeSelectedForMultiSelect(listView.currentIndex);
                        return;
                    }
                    
                    listView.changeSelectedForSingleSelect(listView.currentIndex);
                }
            }
            
            function addItemToList(index)
            {
                if(listView.currentSelectedItems.includes(listView.itemAtIndex(index).text))
                {
                    let text = listView.itemAtIndex(index).text;
                    listView.currentSelectedItems = removeFromArray(listView.currentSelectedItems, text);
                }
                else
                {
                    let text = listView.itemAtIndex(index).text;
                    listView.currentSelectedItems.push(text);
                }
                
                let temp = "";
                for(let i = 0; i < listView.currentSelectedItems.length; i++)
                {
                    temp += listView.currentSelectedItems[i];
                    
                    if(i < listView.currentSelectedItems.length - 1)
                        temp += ", "
                }
                
                root.selectedContents = temp;
            }
            
            
            
            function removeFromArray(arr, value)
            { 
                return arr.filter(function(ele){ 
                    return ele !== value; 
                });
            }
            
            
            
            function changeSelectedForMultiSelect(index)
            {
                listView.currentIndex = index;
                
                listView.itemAtIndex(index).selected = !listView.itemAtIndex(index).selected;
            }
            
            
            function changeSelectedForSingleSelect(index)
            {
                listView.currentIndex = index;
                
                if(listView.itemAtIndex(index) === listView.currentSelected)
                {
                    listView.currentSelected.selected = false;
                    root.selectedContents = "";
                    return;
                }
                
                
                if(listView.currentSelected != null)
                    listView.currentSelected.selected = false;
                
                listView.itemAtIndex(index).selected = true;
                
                listView.currentSelected = listView.itemAtIndex(index);
                
                root.selectedContents = listView.currentSelected.text;
            }
        }
        
        Component.onCompleted:
        {
            
            if(root.defaultIndex != -1)
            {
                if(root.multiSelect)
                {
                    listView.addItemToList(listView.currentIndex);
                    listView.changeSelectedForMultiSelect(listView.currentIndex);
                    return;
                }
                
                listView.changeSelectedForSingleSelect(listView.currentIndex);
            }
        }
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}
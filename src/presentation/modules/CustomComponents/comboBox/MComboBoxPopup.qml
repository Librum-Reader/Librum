import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import CustomComponents


Popup
{
    id: root
    property var selectedItem
    property alias model: listView.model
    property int itemHeight: 28
    property color backgroundColor
    property int maxHeight: 208
    property int radius: 5
    property int defaultIndex: -1
    property bool checkBoxStyle: true
    property int checkBoxSize: 18
    property int checkBoxImageSize: 9
    
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
                        listView.changeSelected(index);
                    }
            }
            
            Keys.onReturnPressed:
            {
                if(listView.currentIndex !== -1)
                    listView.changeSelected(listView.currentIndex)
            }
            
            
            function changeSelected(index)
            {
                listView.currentIndex = index;
                
                if(listView.itemAtIndex(index) === listView.currentSelected)
                {
                    listView.currentSelected.selected = false;
                    root.selectedItem = null;
                    return;
                }
                    
                
                if(listView.currentSelected != null)
                    listView.currentSelected.selected = false;
                
                listView.itemAtIndex(index).selected = true;
                
                listView.currentSelected = listView.itemAtIndex(index);
                
                root.selectedItem = listView.currentSelected;
            }
        }
        
        Component.onCompleted: if(root.defaultIndex != -1) listView.changeSelected(root.defaultIndex)
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0
import CustomComponents 1.0
import "ComboBoxLogic.js" as Logic


Popup
{
    id: root
    property string selectedContents: ""
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
    property string highlightColor: Style.colorLightPurple
    signal itemChanged
    
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
            property var selectedItemsStore: []
            
            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            Layout.maximumHeight: root.maxHeight
            maximumFlickVelocity: 550
            keyNavigationEnabled: true
            currentIndex: root.defaultIndex
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
                
                onClicked: (mouse, index) => root.selectItem(index);
            }
            
            
            Keys.onReturnPressed:
            {
                if(listView.currentIndex !== -1)
                    root.selectItem(listView.currentIndex);
            }
            
            Component.onCompleted:
            {
                if(root.defaultIndex != -1)
                    root.selectItem(listView.currentIndex);
            }
        }
    }
    
    
    function selectItem(index)
    {
        if(root.multiSelect)
            Logic.addItemToResult(index);
        
        Logic.changeSelectionMarker(index);
        root.itemChanged();
    }
    
    function deselectCurrenItem()
    {
        if(listView.currentItem)
            listView.currentItem.selected = false;
        
        listView.currentIndex = -1;
        root.selectedContents = "";
    }
}
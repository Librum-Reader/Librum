import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons


Popup
{
    id: root
    focus: true
    padding: 0
    implicitWidth: 168
    background: Rectangle
    {
        color: "transparent"
    }
    
    onOpenedChanged: if(opened) listView.forceActiveFocus()
    
    
    ColumnLayout
    {
        id: mainLayout
        anchors.fill: parent
        spacing: 0
        
        
        Image
        {
            Layout.leftMargin: 14
            Layout.bottomMargin: -1
            source: Icons.dropupLightGray
        }
        
        Pane
        {
            id: container
            Layout.fillHeight: true
            Layout.fillWidth: true
            padding: 6
            background: Rectangle
            {
                color: Style.colorBackground
                border.width: 1
                border.color: Style.colorLightBorder
                radius: 6
                antialiasing: true
            }
            
            
            ColumnLayout
            {
                id: itemLayout
                width: parent.width
                
                ListView
                {
                    id: listView
                    Layout.fillWidth: true
                    Layout.preferredHeight: contentHeight
                    Layout.maximumHeight: 200
                    maximumFlickVelocity: 550
                    currentIndex: -1
                    keyNavigationEnabled: true
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds
                    ScrollBar.vertical: ScrollBar { }
                    
                    model: ListModel
                    {
                        ListElement { text: "Technology" }
                        ListElement { text: "Favourite" }
                        ListElement { text: "Romance" }
                        ListElement { text: "Comedy" }
                        ListElement { text: "Sports" }
                        ListElement { text: "Physics" }
                        ListElement { text: "Blockchain" }
                        ListElement { text: "Psychology" }
                    }
                    
                    delegate: MBaseListItem
                    {
                        width: parent.width
                        containingListview: listView
                        
                        
                        onClicked:
                            (mouse, index) =>
                            {
                                listView.currentIndex = index;
                                listView.currentItem.selected = !listView.currentItem.selected;
                            }
                        
                        onRightClicked:
                            (mouse, index) =>
                            {
                                let absoluteMousePosition = mapToItem(container, mouse.x, mouse.y);
                                
                                tagOptionsPopup.x = absoluteMousePosition.x + 2;
                                tagOptionsPopup.y = absoluteMousePosition.y + 6;
                                tagOptionsPopup.open();
                            }
                    }
                    
                    Keys.onReturnPressed:
                    {
                        if(listView.currentItem != null)
                            listView.currentItem.selected = !listView.currentItem.selected;
                    }
                }
            }
        }
    }
    
    
    MRightClickMenu
    {
        id: tagOptionsPopup
        visible: false
        
        
        objectModel: ObjectModel
        {
            MRightClickMenuItem
            {
                width: tagOptionsPopup.width
                imagePath: Icons.edit
                imageSize: 17
                text: "Rename"
                
                onClicked:
                {
                    tagOptionsPopup.close();
                }
            }
            
            MRightClickMenuItem
            {
                width: tagOptionsPopup.width
                imagePath: Icons.trash_gray
                imageSize: 16
                text: "Delete"
                
                onClicked:
                {
                    tagOptionsPopup.close();
                }
            }
        }
    }
    
    
    function hasAtLeastOneTagSelected()
    {
        for(let i = 0; i < listView.count; i++)
        {
            if(listView.itemAtIndex(i).selected === true)
                return true;
        }
        
        return false;
    }
}
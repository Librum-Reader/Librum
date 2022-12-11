import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import QtQml.Models 2.15
import Librum.style 1.0
import Librum.icons 1.0
import Librum.controllers 1.0


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
                    
                    model: UserController.tagsModel
                    
                    delegate: MBaseListItem
                    {
                        width: parent.width
                        containingListview: listView
                        
                        function getContent()
                        {
                            return model.name;
                        }
                        
                        onClicked:
                            (mouse, index) => 
                            {
                                listView.selectItem(index);
                                if(listView.itemAtIndex(index).selected)
                                {
                                    BookController.libraryModel.addFilterTag(getContent());
                                }
                                else
                                {
                                    BookController.libraryModel.removeFilterTag(getContent());
                                }
                            }
                        
                        onRightClicked:
                            (mouse, index) =>
                            {
                                let absoluteMousePosition = mapToItem(container, mouse.x, mouse.y);
                                
                                tagOptionsPopup.x = absoluteMousePosition.x + 2;
                                tagOptionsPopup.y = absoluteMousePosition.y + 6;
                                
                                tagOptionsPopup.index = index;
                                tagOptionsPopup.open();
                            }
                    }
                    
                    Keys.onReturnPressed:
                    {
                        if(listView.currentItem != null)
                            listView.currentItem.selected = !listView.currentItem.selected;
                    }
                    
                    
                    function selectItem(index)
                    {
                        listView.currentIndex = index;
                        listView.currentItem.selected = !listView.currentItem.selected;
                    }
                }
            }
        }
    }
    
    
    MRightClickMenu
    {
        id: tagOptionsPopup
        property int index
        
        visible: false
        
        
        objectModel: ObjectModel
        {
            MRightClickMenuItem
            {
                width: tagOptionsPopup.width
                imagePath: Icons.checkCircle
                imageSize: 17
                text: "Select"
                
                onClicked:
                {
                    
                    listView.selectItem(tagOptionsPopup.index);
                    tagOptionsPopup.close();
                }
            }
            
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
                imagePath: Icons.trashGray
                imageSize: 16
                text: "Delete"
                
                onClicked:
                {
                    tagOptionsPopup.close();
                }
            }
        }
    }
    
    function clearSelections()
    {
        for(let i = 0; i < listView.count; i++)
        {
            listView.itemAtIndex(i).selected = false;
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
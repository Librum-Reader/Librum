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
    closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    background: Rectangle
    {
        color: "transparent"
    }
    
    onAboutToHide:
    {
        // Prevent closing when tagOptionsPopup is still opened
        if(tagOptionsPopup.opened)
            root.open();
        
        // Make sure to stop renaming (if something is currently being renamed) when closing
        let currentItem = listView.itemAtIndex(tagOptionsPopup.index);
        if(currentItem !== null && currentItem.renameable)
        {
            currentItem.stopRenaming(false);
        }
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
                        width: listView.width
                        containingListview: listView
                        
                        function getContent()
                        {
                            return model.name;
                        }
                        
                        onClicked:
                            (mouse, index) => 
                            {
                                if(!listView.itemAtIndex(index).renameable)
                                {
                                    listView.selectItem(index);
                                }
                            }
                        
                        onRightClicked:
                            (mouse, index) =>
                            {
                                // Stop editing for previous item
                                let currItem = listView.itemAtIndex(index);
                                if(currItem.renameable)
                                {
                                    currItem.stopRenaming();
                                }
                                
                                let absoluteMousePosition = mapToItem(container, mouse.x, mouse.y);
                                tagOptionsPopup.x = absoluteMousePosition.x + 2;
                                tagOptionsPopup.y = absoluteMousePosition.y + 6;
                                
                                tagOptionsPopup.index = index;
                                tagOptionsPopup.open();
                            }
                        
                        onRenamed: (index, text) =>
                                   {
                                       let currentItem = listView.itemAtIndex(tagOptionsPopup.index);
                                       let tagName = currentItem.getContent();
                                       let uuid = UserController.getTagUuidForName(tagName);
                                       
                                       let success = UserController.renameTag(uuid, text);
                                       if(success)
                                       {
                                           let oldText = tagOptionsPopup.originalTextOfLastEdited;
                                           BookController.renameTags(oldText, text);
                                       }
                                   }
                    }
                    
                    function selectItem(index)
                    {
                        // Stop the renaming of the currentItem
                        if(listView.itemAtIndex(tagOptionsPopup.index).renameable)
                            listView.itemAtIndex(tagOptionsPopup.index).stopRenaming();
                        
                        listView.currentIndex = index;
                        listView.currentItem.selected = !listView.currentItem.selected;
                        
                        if(listView.itemAtIndex(index).selected)
                        {
                            BookController.libraryModel.addFilterTag(listView.currentItem.getContent());
                        }
                        else
                        {
                            BookController.libraryModel.removeFilterTag(listView.currentItem.getContent());
                        }
                    }
                }
            }
        }
    }
    
    
    MRightClickMenu
    {
        id: tagOptionsPopup
        property int index
        property string originalTextOfLastEdited
        
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
                imagePath: Icons.editGray
                imageSize: 17
                text: "Rename"
                
                onClicked:
                {
                    let currentItem = listView.itemAtIndex(tagOptionsPopup.index);
                    tagOptionsPopup.originalTextOfLastEdited = currentItem.getContent();
                    
                    currentItem.startRenaming();
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
                    let tagName = listView.itemAtIndex(tagOptionsPopup.index).getContent();
                    let uuid = UserController.getTagUuidForName(tagName);
                    
                    let success = UserController.deleteTag(uuid);
                    if(success)
                    {
                        BookController.deleteAllTagsWithUuid(uuid);
                    }
                    
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
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
    closePolicy: Popup.CloseOnReleaseOutsideParent | Popup.CloseOnEscape
    background: Rectangle { color: "transparent" }
    
    onAboutToHide:
    {
        // Prevent closing when tagOptionsPopup is still opened
        if(tagOptionsPopup.opened)
            root.open();
        
        // Make sure to stop renaming any item before closing
        let currentItem = listView.itemAtIndex(tagOptionsPopup.index);
        if(currentItem !== null && currentItem.renameable)
        {
            currentItem.stopRenaming(false);
        }
    }
    
    onOpenedChanged: if(opened) listView.forceActiveFocus()
    
    
    ColumnLayout
    {
        id: layout
        anchors.fill: parent
        spacing: 0
        
        
        Image
        {
            id: triangleDecoration
            Layout.leftMargin: 14
            Layout.bottomMargin: -1
            source: Icons.popupDroplet
        }
        
        Pane
        {
            id: container
            Layout.fillHeight: true
            Layout.fillWidth: true
            padding: 6
            background: Rectangle
            {
                color: Style.colorPopupBackground
                border.width: 1
                border.color: Style.colorContainerBorder
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
                        
                        onClicked: (mouse, index) => 
                                   {
                                       if(!listView.itemAtIndex(index).renameable)
                                       {
                                           internal.selectTag(index);
                                       }
                                   }
                        
                        onRightClicked: (mouse, index) => internal.openTagOptionsPopup(mouse, index)
                        
                        onRenamed: (index, text) => internal.renameTag(index, text)
                    }
                }
            }
        }
    }
    
    
    MRightClickMenu
    {
        id: tagOptionsPopup
        property int index: -1
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
                    internal.selectTag(tagOptionsPopup.index);
                    tagOptionsPopup.close();
                }
            }
            
            MRightClickMenuItem
            {
                width: tagOptionsPopup.width
                imagePath: Icons.edit
                imageSize: 17
                text: "Rename"
                
                onClicked: internal.startRenamingTag(tagOptionsPopup.index)
            }
            
            MRightClickMenuItem
            {
                width: tagOptionsPopup.width
                imagePath: Icons.trashGray
                imageSize: 16
                text: "Delete"
                
                onClicked: internal.deleteTag(tagOptionsPopup.index)
            }
        }
    }
    
    QtObject
    {
        id: internal
        
        /*
          Only innitiate the renaming proccess of tag at @index
          */
        function startRenamingTag(index)
        {
            let currentItem = listView.itemAtIndex(index);
            tagOptionsPopup.originalTextOfLastEdited = currentItem.getContent();
            
            currentItem.startRenaming();
            tagOptionsPopup.close();
        }
        
        function renameTag(index, text)
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
        
        function deleteTag(index)
        {
            // Get tag uuid
            let tagName = listView.itemAtIndex(index).getContent();
            let uuid = UserController.getTagUuidForName(tagName);
            
            // Delete tag
            let success = UserController.deleteTag(uuid);
            if(success)
            {
                BookController.removeAllTagsWithUuid(uuid);
            }
            
            tagOptionsPopup.close();
        }
        
        function selectTag(index)
        {
            // Stop the renaming the currentItem
            if(tagOptionsPopup.index != -1 && listView.itemAtIndex(tagOptionsPopup.index).renameable)
                listView.itemAtIndex(tagOptionsPopup.index).stopRenaming();
            
            // Set ListView properties
            listView.currentIndex = index;
            listView.currentItem.selected = !listView.currentItem.selected;
            
            if(listView.itemAtIndex(index).selected)
                BookController.libraryModel.addFilterTag(listView.currentItem.getContent());
            else
                BookController.libraryModel.removeFilterTag(listView.currentItem.getContent());
        }
        
        function openTagOptionsPopup(mouse, index)
        {
            // Stop renaming current tag
            let currItem = listView.itemAtIndex(index);
            if(currItem.renameable)
                currItem.stopRenaming();
            
            // Calculate tagOptionsPopup position
            let absoluteMousePosition = mapToItem(container, mouse.x, mouse.y);
            tagOptionsPopup.x = absoluteMousePosition.x + 2;
            tagOptionsPopup.y = absoluteMousePosition.y + 6;
            
            // Open tagOptionsPopup
            tagOptionsPopup.index = index;
            tagOptionsPopup.open();
        }
    }
    
    
    function clearSelections()
    {
        for(let i = 0; i < listView.count; i++)
        {
            if(listView.itemAtIndex(i) !== null)
                listView.itemAtIndex(i).selected = false;
        }
    }
    
    function hasAtLeastOneTagSelected()
    {
        for(let i = 0; i < listView.count; i++)
        {
            if(listView.itemAtIndex(i) !== null && listView.itemAtIndex(i).selected === true)
                return true;
        }
        
        return false;
    }
}
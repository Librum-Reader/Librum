import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CustomComponents 1.0
import Librum.controllers 1.0
import "filterByButton"
import "sortByButton"
import "tagSelector"

Item
{
    id: root
    signal searchRequested(string query)
    signal checkBoxClicked()
    
    implicitWidth:  1714
    implicitHeight: 36
    
    onWidthChanged:
    {
        if(searchButton.opened)
            searchButton.close();
    }
    
    
    RowLayout
    {
        id: mainLayout
        
        anchors.fill: parent
        spacing: 12
        
        MWrappedCheckBox
        {
            id: checkBox
            
            onChecked: checkBoxClicked();
        }
        
        MSortByButton
        {
            id: sortByButton
            
            onItemSelected: (role) => BookController.libraryModel.sortRole = role
        }
        
        MFilterByButton
        {
            id: filterByButton
            
            onFilterSelected: resetFiltersButton.visible = true;        
        }
        
        MTagSelectorButton
        {
            id: tagSelector
            
            onTagsSelected: resetTagsButton.visible = true
            onTagsRemoved: resetTagsButton.resetTags()
        }
        
        MRemoveOptionButton
        {
            id: resetFiltersButton
            visible: false
            text: "Remove Filters"
            
            onClicked:
            {
                // Reset filters
                visible = false;
            }
        }
        
        MRemoveOptionButton
        {
            id: resetTagsButton
            visible: false
            text: "Remove Tags"
            
            onClicked: resetTags()
            
            
            function resetTags()
            {
                visible = false;
            }
        }
        
        Item
        {
            id: spacer
            Layout.fillWidth: true
        }
        
        MSearchButton
        {
            id: searchButton
            onTriggered: (query) => searchRequested(query);
            expansionWidth: (spacer.width <= 445 ? spacer.width : 445)
            
            onOpenedChanged: if(!opened) searchRequested("")
        }
    }
}

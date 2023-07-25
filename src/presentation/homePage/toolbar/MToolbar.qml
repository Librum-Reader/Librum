import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.controllers
import Librum.models
import "filterByButton"
import "sortByButton"
import "tagSelector"

Item
{
    id: root
    signal searchRequested(string query)
    signal checkBoxClicked()
    
    implicitWidth: 1714
    implicitHeight: 36
    
    onWidthChanged: if(searchButton.opened) searchButton.close();
    
    
    RowLayout
    {
        id: layout
        anchors.fill: parent
        spacing: 12
        
        MWrappedCheckBox
        {
            id: selectBooksCheckBox
            
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
            
            onFilterSelected: (authors, format, date, onlyBooks, 
                               onlyFiles, read, unread) =>
                              {
                                  BookController.libraryModel.setFilterRequest(authors, format, date, onlyBooks,
                                                                               onlyFiles, read, unread)
                                  
                                  resetFiltersButton.visible = true;
                              }
        }
        
        MTagSelectorButton
        {
            id: tagSelectorButton
            
            onTagsSelected: resetTagsButton.visible = true
            onTagsRemoved: root.resetTags()
        }
        
        /* 
          Button which appears when a filter is applied.
          When clicked, it clears the filters and disappears.
          */
        MRemoveOptionButton
        {
            id: resetFiltersButton
            visible: false
            text: "Remove Filters"
            
            onClicked: root.resetFilters()
        }
        
        /* 
          Button which appears when tag filters are applied.
          When clicked, it clears the tag filters and disappears.
          */
        MRemoveOptionButton
        {
            id: resetTagsButton
            visible: false
            text: "Remove Tags"
            
            onClicked: root.resetTags()
        }
        
        Item
        {
            id: widthFiller
            Layout.fillWidth: true
        }
        
        MSearchButton
        {
            id: searchButton
            onTriggered: (query) => searchRequested(query);
            expansionWidth: (widthFiller.width <= 445 ? widthFiller.width : 445)
            
            onOpenedChanged: if(!opened) searchRequested("")
        }
    }
    
    
    function resetFilters()
    {
        BookController.libraryModel.setFilterRequest("", "", "", false,
                                                     false, false, false);
        filterByButton.resetFilter();
        resetFiltersButton.visible = false;
    }
    
    function resetTags()
    {
        BookController.libraryModel.clearFilterTags();
        tagSelectorButton.clearSelections();
        resetTagsButton.visible = false;
    }
}

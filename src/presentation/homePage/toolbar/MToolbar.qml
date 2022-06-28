import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "filterByButton"
import "sortByButton"
import "tagSelector"
import CustomComponents

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
        }
        
        MFilterByButton
        {
            id: filterByButton
            
            onFilterSelected: resetFiltersButton.visible = true;        
        }
        
        MTagSelectorButton
        {
            id: tagSelector
            
            onTagSelected: resetTagsButton.visible = true;
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
            
            onClicked:
            {
                // Reset tags
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
        }
    }
}

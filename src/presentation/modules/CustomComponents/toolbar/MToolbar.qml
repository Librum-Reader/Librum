import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import "filterByButton"
import "sortByButton"
import "tagSelector"

FocusScope
{
    id: root
    property bool showTagBox : true
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
        property int popUpTopSpacing : 6
        
        anchors.fill: parent
        spacing: 12
        
        MSelectCheckBox
        {
            id: checkBox
            
            onChecked: checkBoxClicked();
        }
        
        MSortByButton
        {
            id: sortByButton
            
            onClicked: (sortByPopup.opened) ? sortByPopup.close() : sortByPopup.open()
            
            MSortByPopup
            {
                id: sortByPopup
                y: sortByButton.y + sortByButton.height + mainLayout.popUpTopSpacing
                visible: false
                closePolicy: Popup.CloseOnReleaseOutsideParent | Popup.CloseOnEscape
            }
        }
        
        MFilterByButton
        {
            id: filterByButton
            
            onClicked: (filterByPopup.opened) ? filterByPopup.close() : filterByPopup.open()
            
            MFilterByPopup
            {
                id: filterByPopup
                y: filterByButton.y + filterByButton.height + mainLayout.popUpTopSpacing
                visible: false
                closePolicy: Popup.CloseOnReleaseOutsideParent | Popup.CloseOnEscape
                
                onFilterQuerySent:
                {
                    close();
                    resetFiltersButton.visible = true;
                }
            }
        }
        
        MTagSelectorButton
        {
            id: tagSelector
            visible: root.showTagBox
            
            onClicked: (tagSelectorPopup.opened) ? tagSelectorPopup.close() : tagSelectorPopup.open()
            
            MTagSelectorPopup
            {
                id: tagSelectorPopup
                y: tagSelector.y + tagSelector.height + mainLayout.popUpTopSpacing
                visible: false
                closePolicy: Popup.CloseOnReleaseOutsideParent | Popup.CloseOnEscape
                
                onTagSelctionMade:
                {
                    resetTagsButton.visible = true;
                }
            }
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
             expensionWidth: (spacer.width <= 445 ? spacer.width : 445)
        }
    }
}

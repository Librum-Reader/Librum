import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents

Item
{
    id: root
    signal checkBoxClicked
    
    implicitWidth:  1714
    implicitHeight: 36
    
    RowLayout
    {
        id: mainLayout
        property int popUpTopSpacing : 6
        
        anchors.fill: parent
        spacing: 12
        
        
        MWrappedCheckBox
        {
            id: checkBox
            
            onChecked: checkBoxClicked();
        }
        
        MExplorerFilterByButton
        {
            id: filterByButton
            onClicked: (filterByPopup.opened) ? filterByPopup.close() : filterByPopup.open()
            
            MExplorerFilterByPopup
            {
                id: filterByPopup
                y: filterByButton.y + filterByButton.height + mainLayout.popUpTopSpacing
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
                
                onFilterQuerySent:
                {
                    close();
                    resetFiltersButton.visible = true;
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
        
        Item
        {
            id: spacer
            Layout.fillWidth: true
        }
        
        MSearchButton
        {
             id: searchButton

             expansionWidth: (spacer.width <= 445 ? spacer.width : 445)
        }
    }
}
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0

Item
{
    id: root
    signal checkBoxClicked
    
    implicitWidth: 1714
    implicitHeight: 36
    
    
    RowLayout
    {
        id: layout
        anchors.fill: parent
        spacing: 12
        
        
        MWrappedCheckBox
        {
            id: checkBox
            
            onChecked: checkBoxClicked()
        }
        
        MExplorerFilterByButton
        {
            id: filterByButton
            onClicked: filterByPopup.opened ? filterByPopup.close() : filterByPopup.open()
            
            
            MExplorerFilterByPopup
            {
                id: filterByPopup
                y: filterByButton.y + filterByButton.height + internal.filterPopupTopSpacing
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
                
                onFilterQuerySent:
                {
                    filterByPopup.close();
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
            id: widthFiller
            Layout.fillWidth: true
        }
        
        MSearchButton
        {
             id: searchButton

             expansionWidth: (widthFiller.width <= 445 ? widthFiller.width : 445)
        }
    }
    
    QtObject
    {
        id: internal
        property int filterPopupTopSpacing: 6
    }
}
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
            }
        }
        
        MTagSelectorButton
        {
            id: tagSelector
            visible: root.showTagBox
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

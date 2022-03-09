import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../"

FocusScope
{
    id: root
    
    implicitWidth:  1714
    implicitHeight: 36
    
    signal searchRequested(string query)
    signal checkBoxClicked()
    
    RowLayout
    {
        id: mainLayout
        width: parent.width
        height: parent.height
        spacing: 12
        
        MSelectCheckBox
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
            id: filterBy
        }
        
        MTagSelectorButton
        {
            id: tagSelector
        }
        
        Item { Layout.fillWidth: true }
        
        MSearchButton
        {
             id: searchButton
             onTriggered: (query) => searchRequested(query);
        }
    }
}

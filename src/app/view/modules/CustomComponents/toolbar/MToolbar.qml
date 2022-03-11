import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents

FocusScope
{
    id: root
    signal searchRequested(string query)
    signal checkBoxClicked()
    
    implicitWidth:  1714
    implicitHeight: 36
    
    
    RowLayout
    {
        id: mainLayout
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

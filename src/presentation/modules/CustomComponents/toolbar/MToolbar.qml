import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents

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

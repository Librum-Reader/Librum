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
    
    RowLayout
    {
        id: mainLayout
        width: parent.width
        height: parent.height
        spacing: 12
        
        Rectangle
        {
            id: selectedCheckBox
            Layout.preferredWidth: 40
            Layout.preferredHeight: 36
            color: properties.colorBackground
            border.width: 1
            border.color: properties.colorLightBorder
            radius: 5
            
            MCheckBox
            {
                id: innerCheckBox
                width: 22
                height: 22
                anchors.centerIn: parent
                borderColor: properties.colorLightBorder2
                borderWidth: 1
            }
        }
        
        Rectangle
        {
            id: sortBy
            Layout.preferredWidth: 100
            Layout.preferredHeight: 36
            color: properties.colorBackground
            border.width: 1
            border.color: properties.colorLightBorder
            radius: 5
            
            RowLayout
            {
                anchors.centerIn: parent
                spacing: 6
                
                Label
                {
                    id: sortByLabel
                    color: properties.colorBaseText
                    text: "Sort by"
                    font.pointSize: 12
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Image
                {
                    id: sortByArrowIcon
                    sourceSize.height: 6
                    source: "/resources/images/arrow-filled-icon.svg"
                    fillMode: Image.PreserveAspectFit
                    antialiasing: false
                }
            }
        }
        
        Rectangle
        {
            id: filterBy
            Layout.preferredWidth: 100
            Layout.preferredHeight: 36
            color: properties.colorBackground
            border.width: 1
            border.color: properties.colorLightBorder
            radius: 5
            
            RowLayout
            {
                anchors.centerIn: parent
                spacing: 5
                
                Image
                {
                    id: filterByArrowIcon
                    sourceSize.height: 14
                    source: "/resources/images/filter-icon.svg"
                    fillMode: Image.PreserveAspectFit
                    antialiasing: false
                }
                
                Label
                {
                    id: filterByLabel
                    color: properties.colorBaseText
                    text: "Filters"
                    font.pointSize: 12
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                }
            }
        }
        
        Rectangle
        {
            id: tag
            Layout.preferredWidth: 100
            Layout.preferredHeight: 36
            color: properties.colorBackground
            border.width: 1
            border.color: properties.colorLightBorder
            radius: 5
            
            RowLayout
            {
                anchors.centerIn: parent
                spacing: 4
                
                Image
                {
                    id: tagIcon
                    sourceSize.height: 18
                    source: "/resources/images/tag-icon.svg"
                    fillMode: Image.PreserveAspectFit
                    antialiasing: false
                }
                
                Label
                {
                    id: tagLabel
                    color: properties.colorBaseText
                    text: "Tags"
                    font.pointSize: 12
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                }
            }
        }
        
        Item
        {
            Layout.fillWidth: true
        }
        
        MSearchButton
        {
             id: searchButton
             onTriggered: (query) => searchRequested(query);
        }
    }
}

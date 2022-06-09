import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Item 
{
    id: root
    property string titleText: "Title"
    property string descriptionText: "Description"
    property double titleSize: 28
    property double descriptionSize: 14
    
    implicitWidth: layout.implicitWidth
    implicitHeight: layout.implicitHeight
    
    
    ColumnLayout
    {
        id: layout
        spacing: 0
        
        Label
        {
            id: title
            text: root.titleText
            color: properties.colorBaseText
            font.weight: Font.Bold
            font.pointSize: root.titleSize
            font.family: properties.defaultFontFamily
        }
        
        Label
        {
            id: description
            text: root.descriptionText
            color: properties.colorLightText3
            font.pointSize: root.descriptionSize
            font.family: properties.defaultFontFamily
        }
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope 
{
    id: root
    property string titleText: "Title"
    property string descriptionText: "Description"
    
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
            font.pointSize: 28
            font.family: properties.defaultFontFamily
        }
        
        Label
        {
            id: description
            text: root.descriptionText
            color: properties.colorLightText3
            font.pointSize: 14
            font.family: properties.defaultFontFamily
        }
    }
}

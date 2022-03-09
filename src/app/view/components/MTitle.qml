import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope {
    id: root
    implicitWidth: titleColumn.implicitWidth
    implicitHeight: titleColumn.implicitHeight
    property string titleText: "Title"
    property string descriptionText: "Description"
    
    ColumnLayout
    {
        id: titleColumn
        spacing: 0
        
        Label
        {
            id: title
            text: root.titleText
            font.weight: Font.Bold
            color: properties.colorBaseText
            font.pointSize: 28
            font.family: properties.defaultFontFamily
        }
        
        Label
        {
            id: pageDescription
            text: root.descriptionText
            color: properties.colorLightText3
            font.pointSize: 14
            font.family: properties.defaultFontFamily
        }
    }
}

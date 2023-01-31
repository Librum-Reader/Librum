import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0


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
            color: Style.colorBaseText
            font.weight: Font.Bold
            font.pointSize: root.titleSize
        }
        
        Label
        {
            id: description
            text: root.descriptionText
            color: Style.colorLightText
            font.pointSize: root.descriptionSize
        }
    }
}

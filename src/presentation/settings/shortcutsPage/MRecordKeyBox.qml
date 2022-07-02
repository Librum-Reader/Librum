import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style


Item
{
    id: root    
    
    ColumnLayout
    {
        id: keyInputButton
        anchors.fill: parent
        spacing: 2
        
        Label
        {
            id: recordText
            text: "Key"
            font.weight: Font.DemiBold
            font.pointSize: 12
            font.family: Style.defaultFontFamily
            color: Style.colorBaseTitle
        }
        
        
        MButton
        {
            id: recordButton
            Layout.fillWidth: true
            Layout.preferredHeight: 38
            radius: 4
            borderColor: Style.colorLightBorder
            backgroundColor: "transparent"
            text: "Press to record"
            fontSize: 13
            fontColor: Style.colorBrightText
            fontWeight: Font.Bold
        }
    }
}

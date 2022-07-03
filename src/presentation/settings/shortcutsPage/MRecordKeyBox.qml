import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons


Item
{
    id: root
    
    
    ColumnLayout
    {
        id: mainLayout
        anchors.fill: parent
        spacing: 2
        
        Label
        {
            id: header
            text: "Key"
            font.weight: Font.DemiBold
            font.pointSize: 12
            font.family: Style.defaultFontFamily
            color: Style.colorBaseTitle
        }
        
        
        Rectangle
        {
            id: button
            Layout.fillWidth: true
            Layout.preferredHeight: 38
            radius: 4
            color: "transparent"
            border.color: Style.colorLightBorder
            opacity: mouseArea.pressed ? 0.8 : 1
            
            
            RowLayout
            {
                id: contentLayout
                anchors.fill: parent
                spacing: 4
                
                
                Label
                {
                    id: recordLabel
                    Layout.fillWidth: true
                    Layout.leftMargin: 12
                    text: "Press to record"
                    font.pointSize: 13
                    font.family: Style.defaultFontFamily
                    color: Style.colorLightText3
                }
                
                Image
                {
                    id: microphoneIcon
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin: 12
                    source: Icons.microphone
                    sourceSize.width: 18
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
    }
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
    }
}
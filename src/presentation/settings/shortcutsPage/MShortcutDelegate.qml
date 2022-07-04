import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import Librum.icons


Item
{
    id: root
    required property string action
    required property string shortcuts
    signal gapWidthChanged(int newWidth)
    
    height: 52
    width: listView.width
    
    
    ColumnLayout
    {
        anchors.fill: parent
        spacing: 0
        
        Rectangle
        {
            id: topBorder
            Layout.fillWidth: true
            Layout.preferredHeight: 2
            color: Style.colorLightGray
        }
        
        RowLayout
        {
            id: contentRow
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0
            
            Label
            {
                id: actionText
                Layout.leftMargin: 12
                Layout.preferredWidth: 150
                Layout.alignment: Qt.AlignVCenter
                text: root.action
                color: Style.colorBaseText
                font.pointSize: 12
                font.family: Style.defaultFontFamily
                font.weight: Font.DemiBold
            }
            
            Item
            {
                Layout.fillWidth: true
                Layout.maximumWidth: 247
                
                onWidthChanged: root.gapWidthChanged(width)
            }
            
            Label
            {
                id: shortcutsText
                Layout.preferredWidth: 172
                Layout.alignment: Qt.AlignVCenter
                text: root.shortcuts
                color: Style.colorBaseText
                font.pointSize: 12
                font.family: Style.defaultFontFamily
                font.weight: Font.DemiBold
            }
            
            Item { Layout.fillWidth: true }
            
            Image
            {
                id: editIcon
                Layout.alignment: Qt.AlignVCenter
                sourceSize.width: 23
                source: Icons.edit
                fillMode: Image.PreserveAspectFit
            }
            
            
            Image
            {
                id: deleteIcon
                Layout.leftMargin: 28
                Layout.rightMargin: 6
                Layout.alignment: Qt.AlignVCenter
                sourceSize.width: 21
                source: Icons.trash_gray
                fillMode: Image.PreserveAspectFit
            }
        }
    }
}
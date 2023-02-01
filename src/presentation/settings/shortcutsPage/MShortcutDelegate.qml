import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0


Item
{
    id: root
    required property int index
    required property string text
    required property string shortcut
    
    // The gap width between ACTION and SETTING. When the page gets smaller the gap decreases,
    // this needs to be signaled to the header because it also needs to reize its gap aswell
    signal gapWidthChanged(int spacing)
    signal editClicked(int index)
    signal deleteClicked(int index)
    
    height: 52
    width: listView.width
    
    
    ColumnLayout
    {
        id: layout
        anchors.fill: parent
        spacing: 0
        
        
        Rectangle
        {
            id: topBorder
            Layout.fillWidth: true
            Layout.preferredHeight: 2
            color: Style.colorLightSeparator
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
                text: root.text
                color: Style.colorText
                font.pointSize: 12
                font.weight: Font.DemiBold
            }
            
            Item
            {
                id: widthFiller
                Layout.fillWidth: true
                Layout.maximumWidth: 247
                
                onWidthChanged: root.gapWidthChanged(width)
            }
            
            Label
            {
                id: shortcutName
                Layout.preferredWidth: 172
                Layout.alignment: Qt.AlignVCenter
                text: root.shortcut
                color: Style.colorText
                font.pointSize: 12
                font.weight: Font.DemiBold
            }
            
            Item { Layout.fillWidth: true }
            
            MButton
            {
                id: editButton
                Layout.preferredHeight: 37
                Layout.preferredWidth: 37
                backgroundColor: "transparent"
                opacityOnPressed: 0.8
                borderColor: "transparent"
                radius: 6
                borderColorOnPressed: Style.colorLightBorder
                imagePath: Icons.editGray
                imageSize: 23
                
                onClicked: root.editClicked(root.index)
            }
            
            MButton
            {
                id: deleteButton
                Layout.preferredHeight: 37
                Layout.preferredWidth: 37
                Layout.leftMargin: 13
                backgroundColor: "transparent"
                opacityOnPressed: 0.8
                borderColor: "transparent"
                radius: 6
                borderColorOnPressed: Style.colorLightBorder
                imagePath: Icons.trashGray
                imageSize: 21
                
                onClicked: root.deleteClicked(root.index)
            }
        }
    }
}
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.elements 1.0
import Librum.models 1.0


Item
{
    id: root
    height: content.height
    
    
    Pane
    {
        id: content
        anchors.left: parent.left
        anchors.right: parent.right
        horizontalPadding: 40
        topPadding: 32
        bottomPadding: 32
        background: Rectangle
        {
            color: Style.colorBackground
            border.color: Style.colorLightBorder
            radius: 4
            antialiasing: true
        }
        
        
        ColumnLayout
        {
            id: layout
            width: parent.width
            spacing: 0
            
            Label
            {
                id: newUpdateTitle
                Layout.fillWidth: true
                text: "A new update is available!"
                wrapMode: Text.WordWrap
                color: Style.colorBaseText
                font.pointSize: 23
                font.weight: Font.Bold
            }
            
            Label
            {
                Layout.fillWidth: true
                Layout.topMargin: 7
                text: "Download it and get the great new improvements"
                wrapMode: Text.WordWrap
                color: Style.colorLightText
                font.pointSize: 15
            }
            
            Label
            {
                Layout.fillWidth: true
                Layout.topMargin: 26
                text: "The newest version is:"
                wrapMode: Text.WordWrap
                color: Style.colorLightText
                font.pointSize: 14.5
            }
            
            Label
            {
                Layout.fillWidth: true
                text: AppInformation.newestVersion
                wrapMode: Text.WordWrap
                color: Style.colorLightText
                font.pointSize: 14.2
                font.bold: true
            }
            
            MButton
            {
                id: downloadButton
                Layout.preferredWidth: 132
                Layout.preferredHeight: 36
                Layout.topMargin: 14
                borderWidth: 0
                backgroundColor: Style.colorBasePurple
                imagePath: Icons.downloadWhite
                imageSize: 16
                imageSpacing: 8
                text: "Download"
                fontSize: 12
                fontWeight: Font.Bold
                fontColor: Style.colorFocusedButtonText
                opacityOnPressed: 0.8
            }
            
            Label
            {
                Layout.fillWidth: true
                Layout.topMargin: 42
                text: "See the exact changes on our website at:"
                wrapMode: Text.WordWrap
                color: Style.colorLightText
                font.pointSize: 13.5
            }
            
            Label
            {
                Layout.fillWidth: true
                text: "www.etovex.com/new_updates"
                wrapMode: Text.WordWrap
                font.underline: true
                color: Style.colorBasePurple
                font.pointSize: 13.5
            }
        }
    }
}
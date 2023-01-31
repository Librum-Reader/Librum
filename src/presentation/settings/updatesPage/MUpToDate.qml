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
        bottomPadding: 42
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
                text: "You are up to date!"
                wrapMode: Text.WordWrap
                color: Style.colorBaseText
                font.pointSize: 23
                font.weight: Font.Bold
            }
            
            Label
            {
                Layout.fillWidth: true
                Layout.topMargin: 7
                text: "Make sure to check for updates regularly, so you dont miss out on any great features."
                wrapMode: Text.WordWrap
                color: Style.colorLightText
                font.pointSize: 15
            }
            
            Label
            {
                Layout.fillWidth: true
                Layout.topMargin: 24
                text: "Your current version is:"
                wrapMode: Text.WordWrap
                color: Style.colorLightText
                font.pointSize: 14.5
            }
            
            Label
            {
                Layout.fillWidth: true
                text: AppInformation.newestVersion
                wrapMode: Text.WordWrap
                color: Style.colorBasePurple
                font.pointSize: 14.2
                font.bold: true
            }
            
            Image
            {
                id: checkIllustration
                z: -1
                Layout.topMargin: 8
                Layout.leftMargin: -20
                source: Icons.checkBubbleIllustration
                sourceSize.width: 320
                fillMode: Image.PreserveAspectFit
            }
            
            
            Label
            {
                Layout.fillWidth: true
                Layout.topMargin: 32
                text: "See our latest changes at:"
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
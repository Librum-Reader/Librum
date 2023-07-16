import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.elements
import Librum.models


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
            color: Style.colorContainerBackground
            border.color: Style.colorContainerBorder
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
                color: Style.colorText
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
                Layout.preferredWidth: implicitWidth
                Layout.minimumWidth: implicitWidth
                text: AppInformation.newsWebsite
                wrapMode: Text.WordWrap
                font.underline: true
                color: Style.colorBasePurple
                font.pointSize: 13.5
                opacity: newsWebsiteLinkArea.pressed ? 0.8 : 1
                
                MouseArea
                {
                    id: newsWebsiteLinkArea
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    
                    onClicked: Qt.openUrlExternally(AppInformation.newsWebsite)
                }
            }
        }
    }
}
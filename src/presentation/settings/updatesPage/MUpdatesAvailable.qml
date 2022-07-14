import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.Elements


Item
{
    id: root
    height: layout.height
    
    ColumnLayout
    {
        id: layout        
        width: parent.width
        spacing: 0
        
        Pane
        {
            id: container
            Layout.fillWidth: true
            Layout.topMargin: 32
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
                id: inDetailsLayout
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
                    font.family: Style.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 7
                    text: "Download it and get the great new improvements"
                    wrapMode: Text.WordWrap
                    color: Style.colorLightText3
                    font.pointSize: 15
                    font.family: Style.defaultFontFamily
                }
                
                Label
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 26
                    text: "The newest version is:"
                    wrapMode: Text.WordWrap
                    color: Style.colorLightText3
                    font.pointSize: 14.5
                    font.family: Style.defaultFontFamily
                }
                
                Label
                {
                    Layout.fillWidth: true
                    text: AppInformation.newestVersion
                    wrapMode: Text.WordWrap
                    color: Style.colorLightText3
                    font.pointSize: 14.2
                    font.bold: true
                    font.family: Style.defaultFontFamily
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
                    fontColor: Style.colorBrightText
                    opacityOnPressed: 0.8
                }
                
                Label
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 42
                    text: "See the exact changes on our website at:"
                    wrapMode: Text.WordWrap
                    color: Style.colorLightText3
                    font.pointSize: 13.5
                    font.family: Style.defaultFontFamily
                }
                
                Label
                {
                    Layout.fillWidth: true
                    text: "www.etovex.com/new_updates"
                    wrapMode: Text.WordWrap
                    font.underline: true
                    color: Style.colorBasePurple
                    font.pointSize: 13.5
                    font.family: Style.defaultFontFamily
                }
            }
        }
    }
}
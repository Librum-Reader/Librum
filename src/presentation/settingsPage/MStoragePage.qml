import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons


Page
{
    id: root
    horizontalPadding: 48
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.pagesBackground
    }
    
    
    ColumnLayout
    {
        id: layout        
        width: parent.width
        spacing: 0
        
        
        RowLayout
        {
            id: titleRow
            Layout.fillWidth: true
            spacing: 0
            
            
            MTitle
            {
                id: title
                Layout.topMargin: 64
                titleText: "Storage"
                descriptionText: "Your storage"
                titleSize: 25
                descriptionSize: 13.25
            }
            
            Item { Layout.fillWidth: true }
            
            MButton
            {
                id: upgradeButton
                Layout.preferredWidth: 118
                Layout.preferredHeight: 38
                Layout.alignment: Qt.AlignBottom
                borderWidth: 0
                backgroundColor: Style.colorBasePurple
                text: "Upgrade"
                fontSize: 12
                fontWeight: Font.Bold
                fontColor: Style.colorBackground
                imagePath: Icons.heartWhiteHallow
                imageSize: 18
            }
        }
        
        Pane
        {
            id: container
            Layout.fillWidth: true
            Layout.topMargin: 32
            horizontalPadding: 40
            topPadding: 0
            bottomPadding: 55
            background: Rectangle
            {
                color: Style.colorBackground
                border.color: Style.colorLightBorder
                radius: 4
                antialiasing: true
            }
            
            
            ColumnLayout
            {
                id: inContainerLayout
                anchors.fill: parent
                spacing: 0
                
                
                Label
                {
                    id: storageTitle
                    Layout.fillWidth: true
                    Layout.topMargin: 24
                    text: "Total storage"
                    wrapMode: Text.WordWrap
                    color: Style.colorBaseText
                    font.pointSize: 21
                    font.family: Style.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                Label
                {
                    id: maxStorageText
                    Layout.fillWidth: true
                    Layout.leftMargin: 15
                    Layout.topMargin: 10
                    text: "2GB"
                    wrapMode: Text.WordWrap
                    color: Style.colorBaseText
                    font.pointSize: 42
                    font.family: Style.defaultFontFamily
                    font.bold: true
                }
                
                Label
                {
                    id: approximateBooksText
                    Layout.fillWidth: true
                    Layout.leftMargin: 40
                    text: "This are approximately <font size=4 bold><b>1000</b></font> books."
                    wrapMode: Text.WordWrap
                    color: Style.colorLightText3
                    font.pointSize: 15
                    textFormat: Text.RichText
                    font.family: Style.defaultFontFamily
                }
                
                Label
                {
                    id: availableStorageTitle
                    Layout.fillWidth: true
                    Layout.topMargin: 50
                    text: "Available storage"
                    wrapMode: Text.WordWrap
                    color: Style.colorBaseText
                    font.pointSize: 21
                    font.family: Style.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                Image
                {
                    id: usedStorageChart
                    Layout.leftMargin: 15
                    Layout.topMargin: 20
                    source: Icons.pieChart
                    sourceSize.width: 100
                    fillMode: Image.PreserveAspectFit
                }
                
                Label
                {
                    id: availableStorageText
                    Layout.fillWidth: true
                    Layout.leftMargin: 20
                    Layout.topMargin: 15
                    text: "You currently have <font size=4 color=" + Style.colorBasePurple + " bold><b>24</b></font> files stored " +
                          "and have <font size=4 color=" + Style.colorBasePurple + " bold><b>1.8GB</b></font> free!"
                    wrapMode: Text.WordWrap
                    color: Style.colorLightText3
                    font.pointSize: 15
                    textFormat: Text.RichText
                    font.family: Style.defaultFontFamily
                }
                
                Label
                {
                    id: upgradeText
                    Layout.fillWidth: true
                    Layout.leftMargin: 20
                    Layout.topMargin: 50
                    text: "If you want to have more storage space available <font color=" + 
                          Style.colorBasePurple + ">upgrade your tier.</font><br>" +
                          "To know why we offer different tiers <font color=" + Style.colorBasePurple + " bold>click here.</font>"
                    wrapMode: Text.WordWrap
                    color: Style.colorLightText3
                    font.pointSize: 15
                    textFormat: Text.RichText
                    font.family: Style.defaultFontFamily
                }
            }
        }
    }
}
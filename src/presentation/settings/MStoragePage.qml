import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.controllers 1.0


MFlickWrapper
{
    id: root
    contentHeight: page.implicitHeight
    
    
    Page
    {
        id: page
        width: parent.width
        horizontalPadding: 48
        bottomPadding: 22
        background: Rectangle { anchors.fill: parent; color: Style.colorPageBackground }
        
        
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
                    id: pageTitle
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
                    textColor: Style.colorFocusedButtonText
                    imagePath: Icons.heartHallow
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
                    color: Style.colorContainerBackground
                    border.color: Style.colorContainerBorder
                    radius: 4
                    antialiasing: true
                }
                
                
                ColumnLayout
                {
                    id: contentLayout
                    anchors.fill: parent
                    spacing: 0
                    
                    
                    Label
                    {
                        id: storageTitle
                        Layout.fillWidth: true
                        Layout.topMargin: 24
                        text: "Total storage"
                        wrapMode: Text.WordWrap
                        color: Style.colorText
                        font.pointSize: 21
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
                        color: Style.colorText
                        font.pointSize: 42
                        font.bold: true
                    }
                    
                    Label
                    {
                        id: approximateBooksText
                        Layout.fillWidth: true
                        Layout.leftMargin: 40
                        text: "This are approximately <font size=4 bold><b>1000</b></font> books."
                        wrapMode: Text.WordWrap
                        color: Style.colorLightText
                        font.pointSize: 15
                        textFormat: Text.RichText
                    }
                    
                    Label
                    {
                        id: availableStorageTitle
                        Layout.fillWidth: true
                        Layout.topMargin: 50
                        text: "Available storage"
                        wrapMode: Text.WordWrap
                        color: Style.colorText
                        font.pointSize: 21
                        font.weight: Font.DemiBold
                    }
                    
                    Label
                    {
                        id: availableStorageText
                        property int usedMB: (UserController.usedBookStorage / (1024 * 1024))
                        property int storageLimitMB: (UserController.bookStorageLimit / (1024 * 1024))
                        
                        Layout.fillWidth: true
                        Layout.leftMargin: 20
                        Layout.topMargin: 15
                        text: "You are currently using <font size=4 color=" + Style.colorBasePurple + " bold><b>" + 
                              (usedMB <= storageLimitMB ? usedMB : storageLimitMB) + "</b></font> MB " + 
                              "out of <font size=4 color=" + Style.colorBasePurple + " bold><b> " + storageLimitMB + " </b></font> MB!"
                        wrapMode: Text.WordWrap
                        color: Style.colorLightText
                        font.pointSize: 15
                        textFormat: Text.RichText
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
                        color: Style.colorLightText
                        font.pointSize: 15
                        textFormat: Text.RichText
                    }
                }
            }
        }
    }
}
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0


MFlickWrapper
{
    id: root
    contentHeight: page.implicitHeight
    flickDeceleration: 3500
    
    
    Page
    {
        id: page
        width: parent.width
        horizontalPadding: 48
        bottomPadding: 22
        background: Rectangle
        {
            anchors.fill: parent
            color: Style.pagesBackground
        }
        
        
        ColumnLayout
        {
            id: layout
            property int insideMargin : 40
            
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
                    titleText: "General settings"
                    descriptionText: "Change how Librum works"
                    titleSize: 25
                    descriptionSize: 13.25
                }
                
                Item { Layout.fillWidth: true }
                
                MButton
                {
                    id: resetButton
                    Layout.preferredWidth: 82
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignBottom
                    borderWidth: 0
                    backgroundColor: Style.colorBasePurple
                    text: "Reset"
                    fontSize: 12
                    fontWeight: Font.Bold
                    fontColor: Style.colorBackground
                    
                    onClicked: resetSettingsPopup.open()
                }
            }
            
            Pane
            {
                id: downloadBlock
                Layout.fillWidth: true
                Layout.topMargin: 32
                verticalPadding: 24
                horizontalPadding: layout.insideMargin
                background: Rectangle
                {
                    color: Style.colorBackground
                    border.color: Style.colorLightBorder
                    radius: 4
                    antialiasing: true
                }
                
                
                ColumnLayout
                {
                    id: downloadColumn
                    anchors.fill: parent
                    spacing: 0
                    
                    Label
                    {
                        id: downloadTitle
                        Layout.fillWidth: true
                        text: "Download"
                        font.pointSize: 16.5
                        font.weight: Font.DemiBold
                        color: Style.colorBaseText
                    }
                    
                    
                    Label
                    {
                        id: forceEthernetTitle
                        Layout.fillWidth: true
                        Layout.topMargin: 24
                        text: "Enforce ethernet for big downloads"
                        font.pointSize: 13
                        font.weight: Font.DemiBold
                        color: Style.colorBaseText
                    }
                    
                    
                    MDualToggle
                    {
                        id: forceEthernetSwitch
                        Layout.topMargin: 4
                        leftProperty: "OFF"
                        rightProperty: "ON"
                        selected: leftProperty
                    }
                }
            }
        }
    }
    
    
    MResetSettingsPopup
    {
        id: resetSettingsPopup
        x: Math.round(root.width / 2 - implicitWidth / 2 - settingsSidebar.width / 2 - page.horizontalPadding)
        y: Math.round(root.height / 2 - implicitHeight / 2 - page.topPadding - 50)
        
        onKeepChoosed: close()
        onResetChoosed:
        {
            // Reset settings
            close();
        }
    }
}
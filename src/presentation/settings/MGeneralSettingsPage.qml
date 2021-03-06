import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons


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
                    
                    onClicked: root.resetAppearanceSettings()
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
                        font.family: Style.defaultFontFamily
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
                        font.family: Style.defaultFontFamily
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
    
    
    function resetAppearanceSettings()
    {
        
    }
}
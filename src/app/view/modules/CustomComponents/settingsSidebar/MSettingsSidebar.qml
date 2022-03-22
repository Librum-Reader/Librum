import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


FocusScope
{
    id: root
    implicitWidth: 238
    implicitHeight: Screen.height
    
    Rectangle
    {
        id: container
        width: parent.width
        height: parent.height
        color: "transparent"
        border.color: properties.colorLightBorder
    }
    
    ColumnLayout
    {
        id: layout
        spacing: 0
        
        Label
        {
            Layout.topMargin: 28
            Layout.leftMargin: 25
            text: "Settings"
            font.pointSize: 19
            font.family: properties.defaultFontFamily
            font.bold: true
            color: properties.colorBaseText
        }
        
        Rectangle
        {
            id: titleBorder
            Layout.preferredWidth: 56
            Layout.preferredHeight: 2
            Layout.topMargin: 15
            Layout.leftMargin: 26
            color: properties.colorLightBorder
        }
        
        Label
        {
            Layout.topMargin: 38
            Layout.leftMargin: 25
            text: "GLOBAL SETTINGS"
            font.pointSize: 10.2
            font.family: properties.defaultFontFamily
            font.bold: true
            color: properties.colorLightText3
        }
        
        Rectangle
        {
            id: about
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 14
            Layout.leftMargin: 1
            color: "#F0F0FF"
            
            RowLayout
            {
                height: parent.height
                width: parent.width
                spacing: 0
                
                
                Image
                {
                    Layout.leftMargin: 26
                    source: properties.iconSettingsAboutPurple
                    fillMode: Image.PreserveAspectFit
                    sourceSize.width: 14
                }
                
                Label
                {
                    Layout.leftMargin: 12
                    verticalAlignment: Text.AlignVCenter
                    text: "About"
                    color: "#271FE0"
                    font.pointSize: 13.3
                    font.family: properties.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                Item { Layout.fillWidth: true }
                
                Rectangle
                {
                    Layout.preferredWidth: 2
                    Layout.preferredHeight: parent.height
                    color: "#271FE0"
                }
            }
        }
        
        Rectangle
        {
            id: appearance
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 5
            Layout.leftMargin: 1
            color: "transparent"
            
            RowLayout
            {
                height: parent.height
                width: parent.width
                spacing: 0
                
                
                Image
                {
                    Layout.leftMargin: 25
                    source: properties.iconSettingsAppearanceGray
                    fillMode: Image.PreserveAspectFit
                    sourceSize.width: 18
                }
                
                Label
                {
                    Layout.leftMargin: 8
                    verticalAlignment: Text.AlignVCenter
                    text: "Appearance"
                    color: properties.colorBaseText
                    font.pointSize: 13.3
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Normal
                }
                
                Item { Layout.fillWidth: true }
                
                Rectangle
                {
                    visible: false
                    Layout.preferredWidth: 2
                    Layout.preferredHeight: parent.height
                    color: "#271FE0"
                }
            }
        }
        
        
        Rectangle
        {
            id: shortcuts
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 5
            Layout.leftMargin: 1
            color: "transparent"
            
            RowLayout
            {
                height: parent.height
                width: parent.width
                spacing: 0
                
                
                Image
                {
                    Layout.leftMargin: 26
                    source: properties.iconSettingsShortcutsGray
                    fillMode: Image.PreserveAspectFit
                    sourceSize.width: 16
                }
                
                Label
                {
                    Layout.leftMargin: 9
                    verticalAlignment: Text.AlignVCenter
                    text: "Shortcuts"
                    color: properties.colorBaseText
                    font.pointSize: 13.3
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Normal
                }
                
                Item { Layout.fillWidth: true }
                
                Rectangle
                {
                    visible: false
                    Layout.preferredWidth: 2
                    Layout.preferredHeight: parent.height
                    color: "#271FE0"
                }
            }
        }
        
        
        Rectangle
        {
            id: updates
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 5
            Layout.leftMargin: 1
            color: "transparent"
            
            RowLayout
            {
                height: parent.height
                width: parent.width
                spacing: 0
                
                
                Image
                {
                    Layout.leftMargin: 25
                    source: properties.iconSettingsUpdatesGray
                    fillMode: Image.PreserveAspectFit
                    sourceSize.width: 16
                }
                
                Label
                {
                    Layout.leftMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    text: "Updates"
                    color: properties.colorBaseText
                    font.pointSize: 13.3
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Normal
                }
                
                Item { Layout.fillWidth: true }
                
                Rectangle
                {
                    visible: false
                    Layout.preferredWidth: 2
                    Layout.preferredHeight: parent.height
                    color: "#271FE0"
                }
            }
        }
        
        Rectangle
        {
            id: advancedSettings
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 5
            Layout.leftMargin: 1
            color: "transparent"
            
            RowLayout
            {
                height: parent.height
                width: parent.width
                spacing: 0
                
                
                Image
                {
                    Layout.leftMargin: 24
                    source: properties.iconSettingsSettingsGray
                    fillMode: Image.PreserveAspectFit
                    sourceSize.width: 19
                }
                
                Label
                {
                    Layout.leftMargin: 8
                    verticalAlignment: Text.AlignVCenter
                    text: "Advanced Settings"
                    color: properties.colorBaseText
                    font.pointSize: 13.3
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Normal
                }
                
                Item { Layout.fillWidth: true }
                
                Rectangle
                {
                    visible: false
                    Layout.preferredWidth: 2
                    Layout.preferredHeight: parent.height
                    color: "#271FE0"
                }
            }
        }
        
        
        Label
        {
            Layout.topMargin: 25
            Layout.leftMargin: 25
            text: "USER & ACCOUNT"
            font.pointSize: 10.2
            font.family: properties.defaultFontFamily
            font.bold: true
            color: properties.colorLightText3
        }
        
        Rectangle
        {
            id: account
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 14
            Layout.leftMargin: 1
            color: "transparent"
            
            RowLayout
            {
                height: parent.height
                width: parent.width
                spacing: 0
                
                
                Image
                {
                    Layout.leftMargin: 25
                    source: properties.iconSettingsAccountGray
                    fillMode: Image.PreserveAspectFit
                    sourceSize.width: 14
                }
                
                Label
                {
                    Layout.topMargin: 2
                    Layout.leftMargin: 12
                    verticalAlignment: Text.AlignBottom
                    text: "Account"
                    color: properties.colorBaseText
                    font.pointSize: 13.3
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Normal
                }
                
                Item { Layout.fillWidth: true }
                
                Rectangle
                {
                    visible: false
                    Layout.preferredWidth: 2
                    Layout.preferredHeight: parent.height
                    color: "#271FE0"
                }
            }
        }
        
        Rectangle
        {
            id: storage
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 5
            Layout.leftMargin: 1
            color: "transparent"
            
            RowLayout
            {
                height: parent.height
                width: parent.width
                spacing: 0
                
                
                Image
                {
                    Layout.leftMargin: 26
                    source: properties.iconSettingsStorageGray
                    fillMode: Image.PreserveAspectFit
                    sourceSize.width: 14
                }
                
                Label
                {
                    Layout.leftMargin: 11
                    verticalAlignment: Text.AlignBottom
                    text: "Storage"
                    color: properties.colorBaseText
                    font.pointSize: 13.3
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Normal
                }
                
                Item { Layout.fillWidth: true }
                
                Rectangle
                {
                    visible: false
                    Layout.preferredWidth: 2
                    Layout.preferredHeight: parent.height
                    color: "#271FE0"
                }
            }
        }
        
        Rectangle
        {
            id: supportUs
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 5
            Layout.leftMargin: 1
            color: "transparent"
            
            RowLayout
            {
                height: parent.height
                width: parent.width
                spacing: 0
                
                
                Image
                {
                    Layout.leftMargin: 25
                    source: properties.iconSettingsSupportUsGray
                    fillMode: Image.PreserveAspectFit
                    sourceSize.width: 18
                }
                
                Label
                {
                    Layout.leftMargin: 8
                    verticalAlignment: Text.AlignBottom
                    text: "Support us"
                    color: properties.colorBaseText
                    font.pointSize: 13.3
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Normal
                }
                
                Item { Layout.fillWidth: true }
                
                Rectangle
                {
                    visible: false
                    Layout.preferredWidth: 2
                    Layout.preferredHeight: parent.height
                    color: "#271FE0"
                }
            }
        }
    }
}
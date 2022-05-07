import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


FocusScope
{
    id: root
    implicitWidth: 238
    implicitHeight: Window.height
    
    Rectangle
    {
        id: container
        anchors.fill: parent
        color: "transparent"
        border.color: properties.colorLightBorder
    }
    
    ColumnLayout
    {
        id: layout
        property MSettingsSidebarItem current : aboutItem
        
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
            font.weight: Font.Bold
            color: properties.colorLightText3
        }
        
        
        MSettingsSidebarItem
        {
            id: aboutItem
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 14
            Layout.leftMargin: 1
            selected: true
            imageLeftMargin: 26
            imageWidth: 14
            labelLeftMargin: 12
            textContent: "About"
            defaultIcon: properties.iconSettingsAboutGray
            selectedIcon: properties.iconSettingsAboutPurple
            
            onClicked:
            {
                layout.changeSelected(this);
                loadSettingsPage("AboutPage");
            }
        }
        
        MSettingsSidebarItem
        {
            id: appearanceItem
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 5
            Layout.leftMargin: 1
            imageLeftMargin: 25
            imageWidth: 18
            labelLeftMargin: 8
            textContent: "Appearance"
            defaultIcon: properties.iconSettingsAppearanceGray
            selectedIcon: properties.iconSettingsAppearancePurple            
            
            onClicked:
            {
                layout.changeSelected(this);
                loadSettingsPage("AppearancePage");
            }
        }
        
        MSettingsSidebarItem
        {
            id: shortcutsItem
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 5
            Layout.leftMargin: 1
            imageLeftMargin: 26
            imageWidth: 16
            labelLeftMargin: 9
            textContent: "Shortcuts"
            defaultIcon: properties.iconSettingsShortcutsGray
            selectedIcon: properties.iconSettingsShortcutsPurple
            
            onClicked:
            {
                layout.changeSelected(this);
                loadSettingsPage("ShortcutsPage");
            }
        }
        
        MSettingsSidebarItem
        {
            id: updatesItem
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 5
            Layout.leftMargin: 1
            imageLeftMargin: 25
            imageWidth: 16
            labelLeftMargin: 10
            textContent: "Updates"
            defaultIcon: properties.iconSettingsUpdatesGray
            selectedIcon: properties.iconSettingsUpdatesPurple
            
            onClicked:
            {
                layout.changeSelected(this);
                loadSettingsPage("UpdatesPage");
            }
        }
        
        MSettingsSidebarItem
        {
            id: advancedSettingsItem
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 5
            Layout.leftMargin: 1
            imageLeftMargin: 24
            imageWidth: 19
            labelLeftMargin: 8
            textContent: "Advanced Settings"
            defaultIcon: properties.iconSettingsSettingsGray
            selectedIcon: properties.iconSettingsSettingsPurple
            
            onClicked:
            {
                layout.changeSelected(this);
                loadSettingsPage("AdvancedSettingsPage");
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
        
        MSettingsSidebarItem
        {
            id: accountItem
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 12
            Layout.leftMargin: 1
            imageLeftMargin: 25
            imageWidth: 13
            labelTopMargin: 2
            labelLeftMargin: 13
            textContent: "Account"
            defaultIcon: properties.iconSettingsAccountGray
            selectedIcon: properties.iconSettingsAccountPurple
            
            onClicked:
            {
                layout.changeSelected(this);
                loadSettingsPage("AccountPage");
            }
        }
        
        MSettingsSidebarItem
        {
            id: storageItem
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 5
            Layout.leftMargin: 1
            imageLeftMargin: 26
            imageWidth: 14
            labelLeftMargin: 11
            textContent: "Storage"
            defaultIcon: properties.iconSettingsStorageGray
            selectedIcon: properties.iconSettingsStoragePurple
            
            onClicked:
            {
                layout.changeSelected(this);
                loadSettingsPage("StoragePage");
            }
        }
        
        MSettingsSidebarItem
        {
            id: supportUsItem
            Layout.preferredHeight: 32
            Layout.preferredWidth: root.width - 2
            Layout.topMargin: 5
            Layout.leftMargin: 1
            imageLeftMargin: 25
            imageWidth: 18
            labelLeftMargin: 8
            textContent: "Support us"
            defaultIcon: properties.iconSettingsSupportUsGray
            selectedIcon: properties.iconSettingsSupportUsPurple
            
            onClicked:
            {
                layout.changeSelected(this);
                loadSettingsPage("SupportUsPage");
            }
        }
        
        
        function changeSelected(newItem)
        {
            current.selected = false;
            current = newItem;
            current.selected = true;
        }
    }
}

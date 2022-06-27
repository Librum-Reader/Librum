import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import Librum.icons


Item
{
    id: root
    implicitWidth: 238
    implicitHeight: Window.height
    
    Rectangle
    {
        id: container
        anchors.fill: parent
        color: "transparent"
        border.color: Style.colorLightBorder
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
            font.family: Style.defaultFontFamily
            font.bold: true
            color: Style.colorBaseText
        }
        
        Rectangle
        {
            id: titleBorder
            Layout.preferredWidth: 56
            Layout.preferredHeight: 2
            Layout.topMargin: 15
            Layout.leftMargin: 26
            color: Style.colorLightBorder
        }
        
        Label
        {
            Layout.topMargin: 38
            Layout.leftMargin: 25
            text: "GLOBAL SETTINGS"
            font.pointSize: 10.2
            font.family: Style.defaultFontFamily
            font.weight: Font.Bold
            color: Style.colorLightText3
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
            defaultIcon: Icons.settingsAboutGray
            selectedIcon: Icons.settingsAboutPurple
            
            onClicked:
            {
                if(loadSettingsPage("AboutPage"))
                    layout.changeSelected(this);
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
            defaultIcon: Icons.settingsAppearanceGray
            selectedIcon: Icons.settingsAppearancePurple            
            
            onClicked:
            {
                if(loadSettingsPage("AppearancePage"))
                    layout.changeSelected(this);
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
            defaultIcon: Icons.settingsShortcutsGray
            selectedIcon: Icons.settingsShortcutsPurple
            
            onClicked:
            {
                if(loadSettingsPage("ShortcutsPage"))
                    layout.changeSelected(this);
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
            defaultIcon: Icons.settingsUpdatesGray
            selectedIcon: Icons.settingsUpdatesPurple
            
            onClicked:
            {
                if(loadSettingsPage("UpdatesPage"))
                    layout.changeSelected(this);
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
            defaultIcon: Icons.settingsSettingsGray
            selectedIcon: Icons.settingsSettingsPurple
            
            onClicked:
            {
                if(loadSettingsPage("AdvancedSettingsPage"))
                    layout.changeSelected(this);
            }
        }
        
        
        Label
        {
            Layout.topMargin: 25
            Layout.leftMargin: 25
            text: "USER & ACCOUNT"
            font.pointSize: 10.2
            font.family: Style.defaultFontFamily
            font.bold: true
            color: Style.colorLightText3
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
            defaultIcon: Icons.settingsAccountGray
            selectedIcon: Icons.settingsAccountPurple
            
            onClicked:
            {
                if(loadSettingsPage("AccountPage"))
                    layout.changeSelected(this);
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
            defaultIcon: Icons.settingsStorageGray
            selectedIcon: Icons.settingsStoragePurple
            
            onClicked:
            {
                if(loadSettingsPage("StoragePage"))
                    layout.changeSelected(this);
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
            defaultIcon: Icons.settingsSupportUsGray
            selectedIcon: Icons.settingsSupportUsPurple
            
            onClicked:
            {
                if(loadSettingsPage("SupportUsPage"))
                    layout.changeSelected(this);
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
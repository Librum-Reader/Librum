import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons


Item
{
    id: root
    property MSettingsSidebarItem aboutItem: aboutItem
    property MSettingsSidebarItem appearanceItem: appearanceItem
    property MSettingsSidebarItem shortcutsItem: shortcutsItem
    property MSettingsSidebarItem updatesItem: updatesItem
    property MSettingsSidebarItem generalSettingsItem: generalSettingsItem
    property MSettingsSidebarItem accountItem: accountItem
    property MSettingsSidebarItem storageItem: storageItem
    property MSettingsSidebarItem supportUsItem: supportUsItem
    
    property MSettingsSidebarItem currentItem : aboutItem
    
    implicitWidth: 238
    implicitHeight: Window.height
    
    Rectangle
    {
        id: container
        anchors.fill: parent
        color: "transparent"
        border.color: Style.colorLightBorder
    }
    
    MFlickWrapper
    {
        anchors.fill: parent
        contentHeight: layout.implicitHeight
        
        
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
                
                onClicked: loadSettingsPage(aboutPage, root.aboutItem);
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
                
                onClicked: loadSettingsPage(appearancePage, root.appearanceItem)
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
                
                onClicked: loadSettingsPage(shortcutsPage, root.shortcutsItem)
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
                
                onClicked: loadSettingsPage(updatesPage, root.updatesItem)
            }
            
            MSettingsSidebarItem
            {
                id: generalSettingsItem
                Layout.preferredHeight: 32
                Layout.preferredWidth: root.width - 2
                Layout.topMargin: 5
                Layout.leftMargin: 1
                imageLeftMargin: 24
                imageWidth: 19
                labelLeftMargin: 8
                textContent: "General Settings"
                defaultIcon: Icons.settingsSettingsGray
                selectedIcon: Icons.settingsSettingsPurple
                
                onClicked: loadSettingsPage(generalSettingsPage, root.generalSettingsItem)
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
                    loadSettingsPage(accountPage, root.accountItem)
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
                
                onClicked: loadSettingsPage(storagePage, root.storageItem)
            }
            
            MSettingsSidebarItem
            {
                id: supportUsItem
                Layout.preferredHeight: 32
                Layout.preferredWidth: root.width - 2
                Layout.topMargin: 5
                Layout.bottomMargin: 12
                Layout.leftMargin: 1
                imageLeftMargin: 25
                imageWidth: 18
                labelLeftMargin: 8
                textContent: "Support us"
                defaultIcon: Icons.settingsSupportUsGray
                selectedIcon: Icons.settingsSupportUsPurple
                
                onClicked: loadSettingsPage(supportUsPage, root.supportUsItem)
            }
        }
    }
    
    function changeSelectedSettingsItem(newItem)
    {
        root.currentItem.selected = false;
        root.currentItem = newItem;
        root.currentItem.selected = true;
    }
}
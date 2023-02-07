import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0


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
    
    
    /*
      Adds a border to the whole settings sidebar
      */
    Rectangle
    {
        id: borderRect
        anchors.fill: parent
        color: "transparent"
        border.color: Style.colorContainerBorder
    }
    
    MFlickWrapper
    {
        id: flickWrapper
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
                font.bold: true
                color: Style.colorTitle
            }
            
            Rectangle
            {
                id: titleSeparator
                Layout.preferredWidth: 56
                Layout.preferredHeight: 2
                Layout.topMargin: 15
                Layout.leftMargin: 26
                color: Style.colorDarkSeparator
            }
            
            Label
            {
                Layout.topMargin: 38
                Layout.leftMargin: 25
                text: "GLOBAL SETTINGS"
                font.pointSize: 10.2
                font.weight: Font.Bold
                color: Style.colorLightText
            }
            
            
            MSettingsSidebarItem
            {
                id: aboutItem
                Layout.preferredHeight: 32
                Layout.preferredWidth: internal.sidebarItemWidth
                Layout.topMargin: 14
                Layout.leftMargin: 1
                selected: true
                imageLeftMargin: 26
                imageWidth: 14
                labelLeftMargin: 12
                textContent: "About"
                defaultIcon: Icons.settingsSidebarAbout
                selectedIcon: Icons.settingsSidebarAboutSelected
                
                onClicked: loadSettingsPage(aboutPage, root.aboutItem);
            }
            
            MSettingsSidebarItem
            {
                id: appearanceItem
                Layout.preferredHeight: 32
                Layout.preferredWidth: internal.sidebarItemWidth
                Layout.topMargin: 5
                Layout.leftMargin: 1
                imageLeftMargin: 25
                imageWidth: 18
                labelLeftMargin: 8
                textContent: "Appearance"
                defaultIcon: Icons.settingsSidebarAppearance
                selectedIcon: Icons.settingsSidebarAppearanceSelected           
                
                onClicked: loadSettingsPage(appearancePage, root.appearanceItem)
            }
            
            MSettingsSidebarItem
            {
                id: shortcutsItem
                Layout.preferredHeight: 32
                Layout.preferredWidth: internal.sidebarItemWidth
                Layout.topMargin: 5
                Layout.leftMargin: 1
                imageLeftMargin: 26
                imageWidth: 16
                labelLeftMargin: 9
                textContent: "Shortcuts"
                defaultIcon: Icons.settingsSidebarShortcuts
                selectedIcon: Icons.settingsSidebarShortcutsSelected
                
                onClicked: loadSettingsPage(shortcutsPage, root.shortcutsItem)
            }
            
            MSettingsSidebarItem
            {
                id: updatesItem
                Layout.preferredHeight: 32
                Layout.preferredWidth: internal.sidebarItemWidth
                Layout.topMargin: 5
                Layout.leftMargin: 1
                imageLeftMargin: 25
                imageWidth: 16
                labelLeftMargin: 10
                textContent: "Updates"
                defaultIcon: Icons.settingsSidebarUpdates
                selectedIcon: Icons.settingsSidebarUpdatesSelected
                
                onClicked: loadSettingsPage(updatesPage, root.updatesItem)
            }
            
            MSettingsSidebarItem
            {
                id: generalSettingsItem
                Layout.preferredHeight: 32
                Layout.preferredWidth: internal.sidebarItemWidth
                Layout.topMargin: 5
                Layout.leftMargin: 1
                imageLeftMargin: 24
                imageWidth: 19
                labelLeftMargin: 8
                textContent: "General Settings"
                defaultIcon: Icons.settingsSidebarSettings
                selectedIcon: Icons.settingsSidebarSettingsSelected
                
                onClicked: loadSettingsPage(generalSettingsPage, root.generalSettingsItem)
            }
            
            
            Label
            {
                Layout.topMargin: 25
                Layout.leftMargin: 25
                text: "USER & ACCOUNT"
                font.pointSize: 10.2
                font.bold: true
                color: Style.colorLightText
            }
            
            MSettingsSidebarItem
            {
                id: accountItem
                Layout.preferredHeight: 32
                Layout.preferredWidth: internal.sidebarItemWidth
                Layout.topMargin: 12
                Layout.leftMargin: 1
                imageLeftMargin: 25
                imageWidth: 13
                labelTopMargin: 2
                labelLeftMargin: 13
                textContent: "Account"
                defaultIcon: Icons.settingsSidebarAccount
                selectedIcon: Icons.settingsSidebarAccountSelected
                
                onClicked:
                    loadSettingsPage(accountPage, root.accountItem)
            }
            
            MSettingsSidebarItem
            {
                id: storageItem
                Layout.preferredHeight: 32
                Layout.preferredWidth: internal.sidebarItemWidth
                Layout.topMargin: 5
                Layout.leftMargin: 1
                imageLeftMargin: 26
                imageWidth: 14
                labelLeftMargin: 11
                textContent: "Storage"
                defaultIcon: Icons.settingsSidebarStorage
                selectedIcon: Icons.settingsSidebarStorageSelected
                
                onClicked: loadSettingsPage(storagePage, root.storageItem)
            }
            
            MSettingsSidebarItem
            {
                id: supportUsItem
                Layout.preferredHeight: 32
                Layout.preferredWidth: internal.sidebarItemWidth
                Layout.topMargin: 5
                Layout.bottomMargin: 12
                Layout.leftMargin: 1
                imageLeftMargin: 25
                imageWidth: 18
                labelLeftMargin: 8
                textContent: "Support us"
                defaultIcon: Icons.settingsSidebarSupportUs
                selectedIcon: Icons.settingsSidebarSupportUsSelected
                
                onClicked: loadSettingsPage(supportUsPage, root.supportUsItem)
            }
        }
    }
    
    QtObject
    {
        id: internal
        property int sidebarItemWidth: root.width - 2
    }
    
    
    function changeSelectedSettingsItem(newItem)
    {
        root.currentItem.selected = false;
        root.currentItem = newItem;
        root.currentItem.selected = true;
    }
}
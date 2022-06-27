import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import CustomComponents
import "settingsSidebar"
import "shortcutsPage"


Page
{
    id: settingsManager
    
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.pagesBackground
    }
    
    
    RowLayout
    {
        id: layout
        anchors.fill: parent
        spacing: 0
        
        
        MSettingsSidebar
        {
            id: settingsSidebar
            height: parent.height
        }
        
        StackView
        {
            id: settingsPageManager
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            initialItem: aboutPage
            
            popEnter: null
            popExit: null
            pushEnter: null
            pushExit: null
            replaceEnter: null
            replaceExit: null
        }
    }
    
    
    Component { id: aboutPage; MAboutPage{} }
    Component { id: appearancePage; MAppearancePage{} }
    Component { id: shortcutsPage; MShortcutsPage{} }
    Component { id: updatesPage; MUpdatesPage{} }
    Component { id: advancedSettingsPage; MAdvancedSettingsPage{} }
    Component { id: accountPage; MAccountPage {} }
    Component { id: storagePage; MStoragePage{} }
    Component { id: supportUsPage; MSupportUsPage{} }
    
    function loadSettingsPage(page, sidebarItem)
    {
        if(!ensureSettingPageIsSaved(switchSettingsPage, page))
            return;
        
        switchSettingsPage(page);
    }
    
    function ensureSettingPageIsSaved(switchPageFunction, page)
    {
        if(settingsPageManager.currentItem instanceof MAccountPage)
        {
            return settingsPageManager.currentItem.saveSettingsBeforePageSwitch(switchPageFunction, page);
        }
        return true;
    }
    
    function switchSettingsPage(page)
    {
        switch (page)
        {
        case 'AboutPage':
            if(settingsPageManager.currentItem instanceof MAboutPage)
                break;
            
            settingsPageManager.replace(aboutPage);
            settingsSidebar.changeSelectedSettingsPage(settingsSidebar.aboutItem);
            break;
        case 'AppearancePage':
            if(settingsPageManager.currentItem instanceof MAppearancePage)
                break;

            settingsPageManager.replace(appearancePage);
            settingsSidebar.changeSelectedSettingsPage(settingsSidebar.appearanceItem);
            break;
        case 'ShortcutsPage':
            if(settingsPageManager.currentItem instanceof MShortcutsPage)
                break;
            
            settingsPageManager.replace(shortcutsPage);
            settingsSidebar.changeSelectedSettingsPage(settingsSidebar.shortcutsItem);
            break;
        case 'UpdatesPage':
            if(settingsPageManager.currentItem instanceof MUpdatesPage)
                break;
            
            settingsPageManager.replace(updatesPage);
            settingsSidebar.changeSelectedSettingsPage(settingsSidebar.updatesItem);
            break;
        case 'AdvancedSettingsPage':
            if(settingsPageManager.currentItem instanceof MAdvancedSettingsPage)
                break;
            
            settingsPageManager.replace(advancedSettingsPage);
            settingsSidebar.changeSelectedSettingsPage(settingsSidebar.advancedSettingsItem);
            break;
        case 'AccountPage':
            if(settingsPageManager.currentItem instanceof MAccountPage)
                break;
            
            settingsPageManager.replace(accountPage);
            settingsSidebar.changeSelectedSettingsPage(settingsSidebar.accountItem);
            break;
        case 'StoragePage':
            if(settingsPageManager.currentItem instanceof MStoragePage)
                break;
            
            settingsPageManager.replace(storagePage);
            settingsSidebar.changeSelectedSettingsPage(settingsSidebar.storageItem);
            break;
        case 'SupportUsPage':
            if(settingsPageManager.currentItem instanceof MSupportUsPage)
                break;
            
            settingsPageManager.replace(supportUsPage);
            settingsSidebar.changeSelectedSettingsPage(settingsSidebar.supportUsItem);
            break;
        default:
            console.log("ERROR: You tried instantiating a not existing settings page");
        }
    }
}
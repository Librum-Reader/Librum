import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import CustomComponents
import "settingsSidebar"
import "shortcutsPage"
import "updatesPage"
import "accountPage"


Page
{
    id: settingsManager
    property alias settingsSidebar: settingsSidebar
    property alias aboutPage: aboutPage
    property alias appearancePage: appearancePage
    property alias shortcutsPage: shortcutsPage
    property alias updatesPage: updatesPage
    property alias advancedSettingsPage: advancedSettingsPage
    property alias accountPage: accountPage
    property alias storagePage: storagePage
    property alias supportUsPage: supportUsPage
    
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
        if(settingsSidebar.currentItem === sidebarItem)
            return;
        
        if(!ensureSettingsPageIsSaved(switchSettingsPage, page, sidebarItem))
            return;
        
        switchSettingsPage(page, sidebarItem);
    }
    
    function ensureSettingsPageIsSaved(switchPageFunction, page, sidebarItem)
    {
        if(settingsPageManager.currentItem.hasCleanup)
        {
            settingsPageManager.currentItem.pageCleanup.callbackMethod = () => switchPageFunction(page, sidebarItem);
            return settingsPageManager.currentItem.pageCleanup.cleanUp();
        }
        
        return true;
    }
    
    function switchSettingsPage(page, sidebarItem)
    {
        settingsPageManager.replace(page);
        settingsSidebar.changeSelectedSettingsItem(sidebarItem);
    }
}
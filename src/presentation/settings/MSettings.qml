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
        if(!ensureSettingPageIsSaved(switchSettingsPage, page, sidebarItem))
            return;
        
        switchSettingsPage(page, sidebarItem);
    }
    
    function ensureSettingPageIsSaved(switchPageFunction, page, sidebarItem)
    {
        if(settingsPageManager.currentItem instanceof MAccountPage)
        {
            if(!settingsPageManager.currentItem.saveSettingsBeforePageSwitch(switchPageFunction, page, sidebarItem))
                return false;
        }
        
        return true;
    }
    
    function switchSettingsPage(page, sidebarItem)
    {
        if(settingsPageManager.currentItem === page)
            return;

        settingsPageManager.replace(page);
        settingsSidebar.changeSelectedSettingsItem(sidebarItem);
    }
}
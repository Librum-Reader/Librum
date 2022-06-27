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
    
    function loadSettingsPage(page)
    {
        if(!ensureAccountSettingsAreSaved())
            return false;
        
        switchPage(page);
        return true;
    }
    
    function ensureAccountSettingsAreSaved()
    {
        // Opens the "Forgot to save" popup, when switching from Account settings to
        // another page without having saved (if there is anything to save)
        if(settingsPageManager.currentItem instanceof MAccountPage && settingsPageManager.currentItem.unsavedChanges)
        {
            settingsPageManager.currentItem.forgotToSaveChangesDialog.open();
            return false;
        }
        return true;
    }
    
    function switchPage(page)
    {
        switch (page)
        {
        case 'AboutPage':
            if(!(settingsPageManager.currentItem instanceof MAboutPage))
                settingsPageManager.replace(aboutPage);
            break;
        case 'AppearancePage':
            if(!(settingsPageManager.currentItem instanceof MAppearancePage))
                settingsPageManager.replace(appearancePage);
            break;
        case 'ShortcutsPage':
            if(!(settingsPageManager.currentItem instanceof MShortcutsPage))
                settingsPageManager.replace(shortcutsPage);
            break;
        case 'UpdatesPage':
            if(!(settingsPageManager.currentItem instanceof MUpdatesPage))
                settingsPageManager.replace(updatesPage);
            break;
        case 'AdvancedSettingsPage':
            if(!(settingsPageManager.currentItem instanceof MAdvancedSettingsPage))
                settingsPageManager.replace(advancedSettingsPage);
            break;
        case 'AccountPage':
            if(!(settingsPageManager.currentItem instanceof MAccountPage))
                settingsPageManager.replace(accountPage);
            break;
        case 'StoragePage':
            if(!(settingsPageManager.currentItem instanceof MStoragePage))
                settingsPageManager.replace(storagePage);
            break;
        case 'SupportUsPage':
            if(!(settingsPageManager.currentItem instanceof MSupportUsPage))
                settingsPageManager.replace(supportUsPage);
            break;
        default:
            console.log("ERROR: You tried instantiating a not existing settings page");
        }
    }
}
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import CustomComponents
import "settingsSidebar"
import "shortcutsPage"


Page
{
    id: root
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
    Component { id: accountPage; MAccountPage{} }
    Component { id: storagePage; MStoragePage{} }
    Component { id: supportUsPage; MSupportUsPage{} }
    
    function loadSettingsPage(page)
    {
        switch (page)
        {
        case 'AboutPage':
            settingsPageManager.replace(aboutPage);
            break;
        case 'AppearancePage':
            settingsPageManager.replace(appearancePage);
            break;
        case 'ShortcutsPage':
            settingsPageManager.replace(shortcutsPage);
            break;
        case 'UpdatesPage':
            settingsPageManager.replace(updatesPage);
            break;
        case 'AdvancedSettingsPage':
            settingsPageManager.replace(advancedSettingsPage);
            break;
        case 'AccountPage':
            settingsPageManager.replace(accountPage);
            break;
        case 'StoragePage':
            settingsPageManager.replace(storagePage);
            break;
        case 'SupportUsPage':
            settingsPageManager.replace(supportUsPage);
            break;
        default:
            console.log("ERROR: You tried instantiating a not existing settings page");
        }
    }
}
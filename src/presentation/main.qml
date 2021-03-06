import QtQuick 
import QtQuick.Controls
import QtQuick.Layouts

import "sidebar"
import "homePage"
import "loginPage"
import "registerPage"
import "freeBooksPage"
import "settings"
import "statisticsPage"
import "addOnsPage"
import "forgotPasswordPage"
import "readingPage"


ApplicationWindow
{
    id: baseRoot
    property alias pageManager: pageManager
    property int defaultMinimumWidth: 904
    
    minimumHeight: 400
    minimumWidth: defaultMinimumWidth
    visible: true
    visibility: Window.Maximized
    title: qsTr("Librum - Your ebook reader")
    
    RowLayout
    {
        id: mainlayout
        anchors.fill: parent
        spacing: 0
        
        MSidebar
        {
            id: sidebar
            z: 1
            Layout.alignment: Qt.AlignTop
            visible: pageManager.pageHasSidebar
        }
        
        
        StackView
        {
            id: pageManager
            property bool pageHasSidebar : false
            
            Layout.fillWidth: true
            Layout.fillHeight: true
            initialItem: loginPage
            
            popEnter: null
            popExit: null
            pushEnter: null
            pushExit: null
            replaceEnter: null
            replaceExit: null
        }
    }
    
    
    Component { id: loginPage; MLoginPage {} }
    Component { id: forgotPasswordPage; MForgotPasswordPage {} }
    Component { id: registerPage; MRegisterPage {} }
    Component { id: homePage; MHomePage {} }
    Component { id: freeBooksPage; MFreeBooksPage {} }
    Component { id: settings; MSettings {} }
    Component { id: addOnsPage; MAddOnsPage {} }
    Component { id: statisticsPage; MStatisticsPage {} }
    Component { id: readingPage; MReadingPage {} }
    
    
    function loadPage(page, sidebarItem, doSamePageCheck = true)
    {
        if(newPageIsTheSameAsOldPage(sidebarItem) && doSamePageCheck)
            return;
        
        if(!terminateActionOfPreviousPage(page, sidebarItem))
            return;
        
        switchPage(page, sidebarItem);
    }
    
    function newPageIsTheSameAsOldPage(sidebarItem)
    {
        return sidebar.currentItem === sidebarItem;
    }
    
    function terminateActionOfPreviousPage(page, sidebarItem)
    {
        if(pageManager.currentItem instanceof MSettings)
        {
            if(!pageManager.currentItem.ensureSettingsPageIsSaved(switchPage, page, sidebarItem))
                return false;
        }
        
        return true;
    }
    
    function switchPage(page, sidebarItem)
    {
        pageManager.replace(page);
        pageManager.pageHasSidebar = sidebarItem === undefined ? false : true;
        if(sidebarItem !== undefined) sidebar.changeSelectedItem(sidebarItem);
        
        if(page === readingPage)
            baseRoot.minimumWidth = 550;
        else
            baseRoot.minimumWidth = baseRoot.defaultMinimumWidth;
            
    }
    
    
    
    // Nested settings navigation
    
    function loadSettingsAccountPage()
    {
        loadPage(settings, sidebar.settingsItem, false);
        
        let page = pageManager.currentItem;
        page.loadSettingsPage(page.accountPage, page.settingsSidebar.accountItem);
    }
    
    function loadSettingsAppearancePage()
    {
        loadPage(settings, sidebar.settingsItem, false);
        
        let page = pageManager.currentItem;
        page.loadSettingsPage(page.appearancePage, page.settingsSidebar.appearanceItem);
    }
}
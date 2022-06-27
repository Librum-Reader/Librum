import QtQuick 
import QtQuick.Controls
import QtQuick.Layouts

import "sidebar"
import "home"
import "login"
import "register"
import "freeBooks"
import "settings"
import "statistics"
import "addOns"
import "tools"
import "forgotPassword"
import "readingView"


ApplicationWindow
{
    id: root
    minimumHeight: 590
    minimumWidth: 542
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
    Component { id: homePage; MHome {} }
    Component { id: freeBooksPage; MFreeBooks {} }
    Component { id: settingsPage; MSettings {} }
    Component { id: addOnsPage; MAddOns {} }
    Component { id: toolsPage; MTools {} }
    Component { id: statisticsPage; MStatistics {} }
    Component { id: readingViewPage; MReadingView {} }
    
    
    function loadPage(page, sidebarItem)
    {
        if(!terminateActionOfPreviousPage(page, sidebarItem))
            return;

        switchPage(page, sidebarItem);
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
        if(sidebar.currentItem === sidebarItem && sidebar.hasChangedAtLeastOnce)
            return;
        
        pageManager.replace(page);
        pageManager.pageHasSidebar = sidebarItem === undefined ? false : true;
        if(sidebarItem !== undefined) sidebar.changeSelectedItem(sidebarItem);
    }
}
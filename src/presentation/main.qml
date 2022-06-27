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

    
    function loadPage(page)
    {
        if(pageManager.currentItem instanceof MSettings)
        {
            if(!pageManager.currentItem.ensureSettingPageIsSaved(switchPage, page))
                return;
        }

        switchPage(page);
    }
    
    function switchPage(page)
    {
        switch (page)
        {
        case 'LoginPage':
            if(pageManager.currentItem instanceof MLoginPage)
                break;
            
            root.minimumWidth = 542
            root.minimumHeight = 590
            pageManager.replace(loginPage);
            pageManager.pageHasSidebar = false;
            break;
            
        case 'ForgotPasswordPage':
            if(pageManager.currentItem instanceof MForgotPasswordPage)
                break;
            
            root.minimumWidth = 542
            root.minimumHeight = 590
            pageManager.replace(forgotPasswordPage);
            pageManager.pageHasSidebar = false;
            break;
            
        case 'RegisterPage':
            if(pageManager.currentItem instanceof MRegisterPage)
                break;
            
            root.minimumHeight = 820
            root.minimumWidth = 542
            pageManager.replace(registerPage);
            pageManager.pageHasSidebar = false;
            break;
            
        case 'HomePage':
            if(pageManager.currentItem instanceof MHome)
                break;
            
            root.minimumWidth = 904
            pageManager.replace(homePage);
            pageManager.pageHasSidebar = true;
            sidebar.changeSelectedItem(sidebar.homeItem);
            break;
            
        case 'FreeBooksPage':
            if(pageManager.currentItem instanceof MFreeBooks)
                break;
            
            pageManager.replace(freeBooksPage);
            pageManager.pageHasSidebar = true;
            sidebar.changeSelectedItem(sidebar.freeBooksItem);
            break;
            
        case 'SettingsPage':
            if(pageManager.currentItem instanceof MSettings)
                break;
            
            pageManager.replace(settingsPage);
            pageManager.pageHasSidebar = true;
            sidebar.changeSelectedItem(sidebar.settingsItem);
            break;
            
        case 'AddOnsPage':
            if(pageManager.currentItem instanceof MAddOns)
                break;
            
            pageManager.replace(addOnsPage);
            pageManager.pageHasSidebar = true;
            sidebar.changeSelectedItem(sidebar.addOnsItem);
            break;
            
        case 'ToolsPage':
            if(pageManager.currentItem instanceof MTools)
                break;
            
            pageManager.replace(toolsPage);
            pageManager.pageHasSidebar = true;
            sidebar.changeSelectedItem(sidebar.toolsItem);
            break;
            
        case 'StatisticsPage':
            if(pageManager.currentItem instanceof MStatistics)
                break;
            
            pageManager.replace(statisticsPage);
            pageManager.pageHasSidebar = true;
            sidebar.changeSelectedItem(sidebar.statisticsItem);
            break;
            
        case 'ReadingViewPage':
            if(pageManager.currentItem instanceof MReadingView)
                break;
            
            pageManager.replace(readingViewPage);
            pageManager.pageHasSidebar = false;
            break;
        default:
            console.log("ERROR: You tried instantiating a not existing page");
        }
    }
}
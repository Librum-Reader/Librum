import QtQuick 
import QtQuick.Controls
import QtQuick.Layouts

import "components/sidebar"
import "home"
import "login"
import "register"
import "freeBooks"
import "settings"
import "statistics"
import "addOns"
import "tools"


ApplicationWindow
{
    id: root
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    minimumHeight: 590
    minimumWidth: 542
    visible: true
    title: qsTr("Librum - Your ebook reader")


    RowLayout
    {
        id: mainlayout
        anchors.fill: parent
        spacing: 0
        
        MSidebar
        {
            id: sidebar
            visible: pageManager.pageHasSidebar
            z: 1
            Layout.alignment: Qt.AlignTop
        }
        
        StackView
        {
            id: pageManager
            Layout.preferredHeight: root.height
            Layout.preferredWidth: root.width - (pageHasSidebar ? sidebar.width : 0)
            initialItem: loginPage
            
            property bool pageHasSidebar : false
            
            popEnter: null
            popExit: null
            pushEnter: null
            pushExit: null
            replaceEnter: null
            replaceExit: null
        }
    }
    
    Component { id: loginPage; MLoginPage {} }
    Component { id: registerPage; MRegisterPage {} }
    Component { id: homePage; MHome {} }
    Component { id: freeBooksPage; MFreeBooks {} }
    Component { id: settingsPage; MSettings {} }
    Component { id: addOnsPage; MAddOns {} }
    Component { id: toolsPage; MTools {} }
    Component { id: statisticsPage; MStatistics {} }
    
    
    
    function loadPage(page)
    {
        switch (page)
        {
        case 'LoginPage':
            pageManager.replace(loginPage);
            root.minimumHeight = 590
            root.minimumWidth = 542
            pageManager.pageHasSidebar = false;
            break;
        case 'RegisterPage':
            root.minimumHeight = 800
            root.minimumWidth = 542
            pageManager.pageHasSidebar = false;
            pageManager.replace(registerPage);
            break;
        case 'HomePage':
            pageManager.pageHasSidebar = true;
            pageManager.replace(homePage);
            break;
        case 'FreeBooksPage':
            pageManager.pageHasSidebar = true;
            pageManager.replace(freeBooksPage);
            break;
        case 'SettingsPage':
            pageManager.pageHasSidebar = true;
            pageManager.replace(settingsPage);
            break;
        case 'AddOnsPage':
            pageManager.pageHasSidebar = true;
            pageManager.replace(addOnsPage);
            break;
        case 'ToolsPage':
            pageManager.pageHasSidebar = true;
            pageManager.replace(toolsPage);
            break;
        case 'StatisticsPage':
            pageManager.pageHasSidebar = true;
            pageManager.replace(statisticsPage);
            break;
        default:
            console.log("ERROR: You tried instantiating a not existing page");
        }
    }
    
    
    MGlobalProperties
    {
        id: properties
    }
}
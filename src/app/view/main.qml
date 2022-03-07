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
    width: Screen.width
    height: Screen.height
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
            Layout.fillWidth: true
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
    
    Component { id: loginPage; MLoginPage {}}
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
            root.minimumWidth = 542
            root.minimumHeight = 590
            pageManager.replace(loginPage);
            pageManager.pageHasSidebar = false;
            break;
        case 'RegisterPage':
            root.minimumHeight = 800
            root.minimumWidth = 542
            pageManager.replace(registerPage);
            pageManager.pageHasSidebar = false;
            break;
        case 'HomePage':
            pageManager.replace(homePage);
            pageManager.pageHasSidebar = true;
            break;
        case 'FreeBooksPage':
            pageManager.replace(freeBooksPage);
            pageManager.pageHasSidebar = true;
            break;
        case 'SettingsPage':
            pageManager.replace(settingsPage);
            pageManager.pageHasSidebar = true;
            break;
        case 'AddOnsPage':
            pageManager.replace(addOnsPage);
            pageManager.pageHasSidebar = true;
            break;
        case 'ToolsPage':
            pageManager.replace(toolsPage);
            pageManager.pageHasSidebar = true;
            break;
        case 'StatisticsPage':
            pageManager.replace(statisticsPage);
            pageManager.pageHasSidebar = true;
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
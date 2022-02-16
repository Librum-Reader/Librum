import QtQuick 
import QtQuick.Controls
import QtQuick.Layouts

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
    


    StackView
    {
        id: mainStackView
        width: root.width
        height: root.height
        
        initialItem: loginPage
        
        // Transitions
        popEnter: null
        popExit: null
        pushEnter: null
        pushExit: null
        replaceEnter: null
        replaceExit: null
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
            mainStackView.replace(loginPage);
            root.minimumHeight = 590
            root.minimumWidth = 542
            break;
        case 'RegisterPage':
            root.minimumHeight = 800
            root.minimumWidth = 542
            mainStackView.replace(registerPage);
            break;
        case 'HomePage':
            root.minimumHeight = 0
            root.minimumWidth = 0
            mainStackView.replace(homePage);
            break;
        case 'FreeBooksPage':
            root.minimumHeight = 0
            root.minimumWidth = 0
            mainStackView.replace(freeBooksPage);
            break;
        case 'SettingsPage':
            root.minimumHeight = 0
            root.minimumWidth = 0
            mainStackView.replace(settingsPage);
            break;
        case 'AddOnsPage':
            root.minimumHeight = 0
            root.minimumWidth = 0
            mainStackView.replace(addOnsPage);
            break;
        case 'ToolsPage':
            root.minimumHeight = 0
            root.minimumWidth = 0
            mainStackView.replace(toolsPage);
            break;
        case 'StatisticsPage':
            root.minimumHeight = 0
            root.minimumWidth = 0
            mainStackView.replace(statisticsPage);
            break;
        default:
            console.log("A not existing page was called");
        }
    }
    
    
    MGlobalProperties
    {
        id: properties
    }
}
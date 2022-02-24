import QtQuick 
import QtQuick.Controls
import QtQuick.Layouts
import librum.extensions.sidebar

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
            z: 1
            Layout.alignment: Qt.AlignTop
        }
        
        StackView
        {
            id: mainStackView
            Layout.preferredHeight: root.height
            Layout.preferredWidth: root.width - sidebar.width
            
            initialItem: loginPage
            
            // Transitions
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
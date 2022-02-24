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
            visible: stackView.pageHasSidebar
            z: 1
            Layout.alignment: Qt.AlignTop
        }
        
        StackView
        {
            id: stackView
            Layout.preferredHeight: root.height
            Layout.preferredWidth: root.width - sidebar.width
            initialItem: loginPage
            
            property bool pageHasSidebar : false
            
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
            stackView.replace(loginPage);
            root.minimumHeight = 590
            root.minimumWidth = 542
            stackView.pageHasSidebar = false;
            break;
        case 'RegisterPage':
            root.minimumHeight = 800
            root.minimumWidth = 542
            stackView.pageHasSidebar = false;
            stackView.replace(registerPage);
            break;
        case 'HomePage':
            stackView.pageHasSidebar = true;
            stackView.replace(homePage);
            break;
        case 'FreeBooksPage':
            stackView.pageHasSidebar = true;
            stackView.replace(freeBooksPage);
            break;
        case 'SettingsPage':
            stackView.pageHasSidebar = true;
            stackView.replace(settingsPage);
            break;
        case 'AddOnsPage':
            stackView.pageHasSidebar = true;
            stackView.replace(addOnsPage);
            break;
        case 'ToolsPage':
            stackView.pageHasSidebar = true;
            stackView.replace(toolsPage);
            break;
        case 'StatisticsPage':
            stackView.pageHasSidebar = true;
            stackView.replace(statisticsPage);
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
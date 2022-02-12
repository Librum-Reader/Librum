import QtQuick 
import QtQuick.Controls
import QtQuick.Layouts

import "login"
import "register"
import "library"


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

    Component
    {
        id: loginPage
        MLoginPage {}
    }
    Component
    {
        id: registerPage
        MRegisterPage {}
    }
    Component
    {
        id: libraryPage
        MLibrary {}
    }
    
    
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
        case 'LibraryPage':
            root.minimumHeight = 0
            root.minimumWidth = 0
            mainStackView.replace(libraryPage);
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
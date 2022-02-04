import QtQuick 
import QtQuick.Controls
import QtQuick.Layouts

import "login"
import "register"


ApplicationWindow
{
    id: root
    width: Screen.width
    height: Screen.height
    minimumHeight: 550
    minimumWidth: 542
    visible: true
    title: qsTr("Hello World")
    


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
        MLoginPage
        {
            height: root.height
            width: root.width
        }
    }
    
    Component
    {
        id: registerPage
        MRegisterPage
        {
            height: root.height
            width: root.width
        }
    }
    
    
    function loadPage(page)
    {
        switch (page)
        {
        case 'LoginPage':
            mainStackView.replace(loginPage);
            root.minimumHeight = 550
            root.minimumWidth = 542
            break;
        case 'RegisterPage':
            root.minimumHeight = 800
            root.minimumWidth = 542
            mainStackView.replace(registerPage);
            break;
        }
    }
    
    
    MGlobalProperties
    {
        id: properties
    }
}
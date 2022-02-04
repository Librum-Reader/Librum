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
        MLoginPage{}
    }
    
    Component
    {
        id: registerPage
        MRegisterPage{}
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
            mainStackView.replace(registerPage);
            break;
        }
    }
    
    
    MGlobalProperties
    {
        id: properties
    }
}
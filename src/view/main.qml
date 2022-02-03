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
    visible: true
    title: qsTr("Hello World")
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.brightBackground
    }
    


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
        LoginPage
        {
            anchors.centerIn: parent
        }
    }
    
    Component
    {
        id: registerPage
        RegisterPage
        {
            anchors.centerIn: parent
        }
    }
    
    
    function loadPage(page)
    {
        switch (page)
        {
        case 'LoginPage':
            mainStackView.replace(loginPage);
            break;
        case 'RegisterPage':
            mainStackView.replace(registerPage);
            console.log(root.color);
            break;
        }
    }
    
    
    GlobalProperties
    {
        id: properties
    }
}
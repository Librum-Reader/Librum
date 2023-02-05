import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Librum.elements 1.0
import Librum.icons 1.0
import Librum.controllers 1.0


Popup
{
    id: root
    implicitWidth: 170
    implicitHeight: 130
    padding: 0
    background: Rectangle
    {
        anchors.fill: parent
        color: "#FDFDFD"
        radius: 5
        border.color: "#E0E0DE"
        antialiasing: true
    }
    
    
    MouseArea
    {
        id: closeManagement
        anchors.fill: parent
        hoverEnabled: true
        
        onContainsMouseChanged: if(!containsMouse) root.close();
    }
    
    ColumnLayout
    {
        id: layout
        width: parent.width
        
        
        MProfilePopupItem
        {
            id: syncItem
            Layout.fillWidth: true
            Layout.topMargin: 15
            Layout.leftMargin: 18
            image: Icons.sidebarSync
            text: "Sync"
            imageWidth: 16
            textSpacing: 9
        }
        
        
        MProfilePopupItem
        {
            id: manageProfileItem
            Layout.fillWidth: true
            Layout.topMargin: 15
            Layout.leftMargin: 16
            image: Icons.sidebarEmptyProfile
            text: "Manage Profile"
            imageWidth: 19
            textSpacing: 8
            
            onClicked:
            {
                loadSettingsAccountPage();
                root.close();
            }
        }
        
        
        MProfilePopupItem
        {
            id: logoutItem
            Layout.fillWidth: true
            Layout.topMargin: 15
            Layout.leftMargin: 15
            image: Icons.sidebarLogout
            text: "Logout"
            imageWidth: 20
            textSpacing: 8
            
            onClicked: root.logout()
        }
    }
    
    function logout()
    {
        loadPage(loginPage);
        root.close();
        AuthController.logoutUser();
        resetSidebar();
    }
}
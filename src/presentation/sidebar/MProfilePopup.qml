import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.elements
import Librum.icons
import Librum.controllers
import Librum.style


Popup
{
    id: root
    implicitWidth: 170
    implicitHeight: 130
    padding: 0
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.colorContainerBackground
        radius: 5
        border.color: Style.colorContainerBorder
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
            text: qsTr("Sync")
            imageWidth: 16
            textSpacing: 12
            
            onClicked: 
            {
                LibraryController.syncWithServer();
                UserController.syncWithServer();
                root.close();
            }
        }
        
        
        MProfilePopupItem
        {
            id: manageProfileItem
            Layout.fillWidth: true
            Layout.topMargin: 15
            Layout.leftMargin: 16
            image: Icons.sidebarEmptyProfile
            text: qsTr("Manage Profile")
            imageWidth: 19
            textSpacing: 11
            
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
            text: qsTr("Logout")
            imageWidth: 20
            textSpacing: 11
            
            onClicked: root.logout()
        }
    }
    
    function logout()
    {
        AuthController.logoutUser();
        loadPage(loginPage);
        root.close();
        resetSidebar();
    }
}

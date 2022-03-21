import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


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
        
        onContainsMouseChanged:
        {
            if(!containsMouse)
                root.close();
        }
    }
    
    
    ColumnLayout
    {
        id: layout
        width: parent.width
        
        
        MProfilePopupItem
        {
            Layout.fillWidth: true
            Layout.topMargin: 15
            Layout.leftMargin: 18
            imagePath: properties.iconSync
            textContent: "Sync"
            imageWidth: 16
            textSpacing: 9
        }
        
        
        MProfilePopupItem
        {
            Layout.fillWidth: true
            Layout.topMargin: 15
            Layout.leftMargin: 16
            imagePath: properties.iconEmptyProfile
            textContent: "Manage Profile"
            imageWidth: 19
            textSpacing: 8
        }
        
        
        MProfilePopupItem
        {
            Layout.fillWidth: true
            Layout.topMargin: 15
            Layout.leftMargin: 15
            imagePath: properties.iconLogout
            textContent: "Logout"
            imageWidth: 20
            textSpacing: 8
            
            onClicked:
            {
                root.close();
                loadPage("LoginPage");
            }
        }
    }
}
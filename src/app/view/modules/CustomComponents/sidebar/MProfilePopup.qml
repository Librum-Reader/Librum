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
    
    ColumnLayout
    {
        id: layout
        
        RowLayout
        {
            id: syncColumn
            Layout.topMargin: 15
            Layout.leftMargin: 18
            
            Image
            {
                id: syncIcon
                source: properties.iconSync
                fillMode: Image.PreserveAspectFit
                sourceSize.width: 16
            }
            
            Label
            {
                Layout.leftMargin: 9
                text: "Sync"
                color: properties.colorLightText3
                font.family: properties.defaultFontFamily
                font.pointSize: 10.5
                font.weight: Font.Medium
            }
        }
        
        RowLayout
        {
            id: profileColumn
            Layout.topMargin: 15
            Layout.leftMargin: 16
            
            Image
            {
                id: profileIcon
                source: properties.iconEmptyProfile
                fillMode: Image.PreserveAspectFit
                sourceSize.width: 19
            }
            
            Label
            {
                Layout.leftMargin: 8
                text: "Manage Profile"
                color: properties.colorLightText3
                font.family: properties.defaultFontFamily
                font.pointSize: 10.5
                font.weight: Font.Medium
            }
        }
        
        RowLayout
        {
            id: logoutColumn
            Layout.topMargin: 15
            Layout.leftMargin: 15
            
            Image
            {
                id: logoutIcon
                source: properties.iconLogout
                fillMode: Image.PreserveAspectFit
                sourceSize.width: 20
            }
            
            Label
            {
                Layout.leftMargin: 8
                text: "Logout"
                color: properties.colorLightText3
                font.family: properties.defaultFontFamily
                font.pointSize: 10.5
                font.weight: Font.Medium
            }
        }
    }
}

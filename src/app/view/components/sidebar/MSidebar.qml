import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../"

FocusScope
{
    id: root
    implicitWidth: 72
    implicitHeight: Window.height
    
    property bool opened : true
    
    Rectangle
    {
        id: container
        color: properties.colorBackground
        height: parent.height
        width: parent.width
        
        ColumnLayout
        {
            id: mainLayout
            width: parent.width
            height: parent.height
            spacing: 0
            
            MLogo
            {
                id: logo
                Layout.preferredWidth: 44
                Layout.preferredHeight: 44
                Layout.topMargin: 14
                Layout.leftMargin: 14
            }
            
            Rectangle
            {
                id: topSeparator
                Layout.preferredWidth: root.width
                Layout.preferredHeight: 2
                Layout.topMargin: 16
                color: properties.colorSidebarSeparator
            }
            
            MSidebarItem
            {
                id: downloadButton
                Layout.topMargin: 16
                Layout.leftMargin: 10
                imageWidth: 45
                imageHeight: 45
                imageSource: "/resources/images/download-icon.svg"
            }
            
            Rectangle
            {
                id: smallSeperator1
                Layout.preferredWidth: (root.opened ? 44 : 44)
                Layout.preferredHeight: 2
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 16
                color: properties.colorSidebarSeparator
            }
            
            MSidebarItem
            {
                id: homeButton
                Layout.topMargin: 15
                Layout.leftMargin: 10
                imageWidth: 30
                imageHeight: 28
                imageSource: "/resources/images/home-icon.svg"
            }
            
            MSidebarItem
            {
                id: statisticsButton
                Layout.topMargin: 13
                Layout.leftMargin: 10
                imageWidth: 28
                imageHeight: 28
                imageSource: "/resources/images/pieChart-icon.svg"
            }
            
            Rectangle
            {
                id: smallSeperator2
                Layout.preferredWidth: (root.opened ? 44 : 44)
                Layout.preferredHeight: 2
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 15
                color: properties.colorSidebarSeparator
            }
            
            MSidebarItem
            {
                id: toolsButton
                Layout.topMargin: 15
                Layout.leftMargin: 10
                imageWidth: 30
                imageHeight: 30
                imageSource: "/resources/images/wrench-icon.svg"
            }
            
            MSidebarItem
            {
                id: extensionsButton
                Layout.topMargin: 13
                Layout.leftMargin: 10
                imageWidth: 31
                imageHeight: 31
                imageSource: "/resources/images/puzzle-icon.svg"
            }
            
            MSidebarItem
            {
                id: settingsButton
                Layout.topMargin: 13
                Layout.leftMargin: 10
                imageWidth: 36
                imageHeight: 36
                imageSource: "/resources/images/settings-icon.svg"
            }
            
            Item
            {
                id: emptySpace
                Layout.preferredWidth: parent.width
                Layout.fillHeight: true
                Layout.minimumHeight: 30
                Layout.alignment: Qt.AlignTop
            }
            
            Rectangle
            {
                id: bottomSeparator
                Layout.preferredWidth: root.width
                Layout.preferredHeight: 2
                Layout.alignment: Qt.AlignBottom
                color: properties.colorSidebarSeparator
            }
            
            Rectangle
            {
                id: profileBox
                Layout.preferredWidth: root.width
                Layout.preferredHeight: 70
                Layout.alignment: Qt.AlignBottom
                color: "transparent"
                
                RowLayout
                {
                    anchors.fill: parent
                    spacing: 0
                    
                    Rectangle
                    {
                        id: profilePicture
                        Layout.preferredWidth: 36
                        Layout.preferredHeight: 36
                        Layout.alignment: Qt.AlignCenter
                        Layout.leftMargin: parent.width / 2 - width / 2
                        radius: width
                        antialiasing: true
                        color: "#DBCE5F"
                        
                        Label
                        {
                            id: initials
                            anchors.centerIn: parent
                            text: "DL"
                            font.pointSize: 12
                            font.bold: true
                            font.family: properties.defaultFontFamily
                            color: properties.colorBrightText
                        }
                    }
                    
                    Rectangle
                    {
                        id: expandButton
                        Layout.preferredWidth: 24
                        Layout.preferredHeight: 24
                        Layout.alignment: Qt.AlignVCenter
                        Layout.leftMargin: 4
                        border.width: 1
                        border.color: properties.colorLightBorder
                        
                        Image
                        {
                            sourceSize.width: 18
                            sourceSize.height: 18
                            anchors.centerIn: parent
                            source: "/resources/images/right_icon.svg"
                        }
                    }
                }
            }
        }
    }
}
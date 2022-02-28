import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import librum.extensions.sidebar
import "../components"
import "../components/toolbar"



Page
{
    id: root
    width: Window.width
    height: Window.height
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.pagesBackground
    }
    
    ListModel
    {
        id: model
        
        ListElement { color: "red"; text: "First" }
        ListElement { color: "gray"; text: "Second" }
        ListElement { color: "green"; text: "Third" }
        ListElement { color: "cyan"; text: "Fourth" }
        ListElement { color: "yellow"; text: "Fift" }
        ListElement { color: "gray"; text: "Sixt" }
        ListElement { color: "purple"; text: "Seventh" }
        ListElement { color: "black"; text: "Eitht" }
        ListElement { color: "red"; text: "Nineth" }
        ListElement { color: "gray"; text: "Tenth" }
    }
    
    RowLayout
    {
        id: verticalLayout
        anchors.fill: parent
        spacing: 0
        
        Rectangle
        {
            id: leftSpacer
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: (SidebarState.currentState === SidebarState.Opened ?
                                        openedSidebarMargin : closedSidebarMargin)
            Layout.alignment: Qt.AlignLeft
            color: "transparent"
            
            property int closedSidebarMargin : 64
            property int openedSidebarMargin : 85
        }
        
        ColumnLayout
        {
            id: mainLayout
            Layout.preferredWidth: parent.width - leftSpacer.width
            Layout.preferredHeight: parent.height
            Layout.alignment: Qt.AlignLeft
            spacing: 0
            
            RowLayout
            {
                id: headerRow
                spacing: 0
                Layout.preferredWidth: parent.width
                
                ColumnLayout
                {
                    id: titleColumn
                    spacing: 0
                    
                    Label
                    {
                        id: title
                        Layout.topMargin: 40
                        Layout.alignment: Qt.AlignTop
                        text: "Home"
                        font.weight: Font.Bold
                        color: properties.colorBaseText
                        font.pointSize: 28
                        font.family: properties.defaultFontFamily
                    }
                    
                    Label
                    {
                        id: pageDescription
                        Layout.alignment: Qt.AlignTop
                        text: "You have 10 books"
                        color: properties.colorLightText3
                        font.pointSize: 14
                        font.family: properties.defaultFontFamily
                    }
                }
                
                MButton
                {
                    id: addBooksButton
                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 40
                    Layout.topMargin: 22
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin: 71
                    backgroundColor: properties.colorBasePurple
                    textContent: "Add books"
                    fontColor: properties.colorBackground
                    fontBold: true
                    fontSize: 11.8
                    imageSource: "/resources/images/plus.svg"
                }
            }
            
            MToolbar
            {
                id: toolbar
                Layout.alignment: Qt.AlignTop
                Layout.topMargin: 45
            }
            
            GridView
            {
                id: bookGrid
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.topMargin: 30
                cellWidth: bookWidth + (SidebarState.currentState === SidebarState.Opened ? 
                                            sidebarOpenedBookSpacing : sidebarClosedBookSpacing)
                cellHeight: bookHeight + bookTopSpacing
                clip: true
                model: model
                delegate: MBook {}
                
                property int bookWidth : 190
                property int bookHeight : 320
                property int bookTopSpacing : 48
                property int sidebarClosedBookSpacing : 64
                property int sidebarOpenedBookSpacing : 68
            }
        }
    }
}
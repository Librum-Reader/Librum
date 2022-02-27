import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import librum.extensions.sidebar
import "../components"



Page
{
    id: root
    width: Window.width
    height: Window.height
    
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
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
            spacing: 0
            
            RowLayout
            {
                spacing: 0
                Layout.bottomMargin: 150
                Layout.preferredWidth: parent.width
                
                Label
                {
                    id: title
                    Layout.topMargin: 35
                    Layout.alignment: Qt.AlignTop
                    text: "Home"
                    font.weight: Font.Bold
                    color: properties.colorBaseText
                    font.pointSize: 25
                    font.family: properties.defaultFontFamily
                }
                
                MButton
                {
                    id: addBooksButton
                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 40
                    Layout.topMargin: 35
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin: 71
                    backgroundColor: properties.colorBasePurple
                    textContent: "+ Add Books"
                    fontColor: properties.colorBackground
                    fontBold: true
                }
            }
            
            GridView
            {
                id: bookGrid
                Layout.fillWidth: true
                Layout.fillHeight: true
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
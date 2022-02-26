import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import librum.extensions.sidebar
import "../components"



Page
{
    id: root
    
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
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft
            Layout.topMargin: 216
            spacing: 0
            
            property int bookWidth : 190
            property int bookHeight : 320
            property int bookTopSpacing : 48
            property int sidebarClosedBookSpacing : 64
            property int sidebarOpenedBookSpacing : 68
            
            GridView
            {
                
                id: bookGrid
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: parent.height
                cellWidth: mainLayout.bookWidth + (SidebarState.currentState === SidebarState.Opened ? 
                                            mainLayout.sidebarOpenedBookSpacing : mainLayout.sidebarClosedBookSpacing)
                cellHeight: mainLayout.bookHeight + mainLayout.bookTopSpacing
                clip: true
                model: model
                delegate: MBook {}
            }
        }
    }
}
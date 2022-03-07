import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import librum.extensions.sidebar
import "../components"
import "../components/toolbar"



Page
{
    id: root
    width: parent.width
    height: parent.height
    
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
                                        sidebarMargin : sidebarMargin)
            Layout.alignment: Qt.AlignLeft
            color: "transparent"
            
            property int sidebarMargin : 64
        }
        
        ColumnLayout
        {
            id: mainLayout
            Layout.preferredWidth: parent.width - leftSpacer.width
            Layout.preferredHeight: parent.height
            spacing: 0
            
            property int rightMargin : 70
            
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
                        text: "Home"
                        font.weight: Font.Bold
                        color: properties.colorBaseText
                        font.pointSize: 28
                        font.family: properties.defaultFontFamily
                    }
                    
                    Label
                    {
                        id: pageDescription
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
                    Layout.rightMargin: mainLayout.rightMargin
                    Layout.topMargin: 22
                    Layout.alignment: Qt.AlignRight
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
                Layout.preferredWidth: parent.width - mainLayout.rightMargin
                Layout.alignment: Qt.AlignLeft
                Layout.topMargin: 45
            }
            
            Grid
            {
                id: bookGrid
                width: parent.width - mainLayout.rightMargin
                height: parent.height
                columnSpacing: 64
                rowSpacing: 48
                columns: 0
                clip: true
                
                Repeater
                {
                    model: model
                    delegate: MBook { }
                }
                
                property int bookWidth : 190
                onWidthChanged: columns = (width+columnSpacing) / (columnSpacing + bookWidth);     // Formula to calculate the column numbers
            }
        }
    }
}
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
        
        Item
        {
            id: leftSpacer
            Layout.fillHeight: true
            Layout.preferredWidth: 64
        }
        
        ColumnLayout
        {
            id: mainLayout
            Layout.preferredWidth: parent.width - leftSpacer.width
            Layout.alignment: Qt.AlignTop
            spacing: 0
            
            property int rightMargin : 70
            
            RowLayout
            {
                id: headerRow
                spacing: 0
                Layout.preferredWidth: parent.width
                
                MTitle
                {
                    id: title
                    Layout.topMargin: 44
                    titleText: "Home"
                    descriptionText: "You have 10 books"
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
                
                onSearchRequested: (query) => 
                {
                    console.log(query);
                }
            }
            
            Grid
            {
                id: bookGrid
                Layout.preferredWidth: parent.width - mainLayout.rightMargin
                Layout.topMargin: 30
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
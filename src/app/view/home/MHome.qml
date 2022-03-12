import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import librum.extensions.sidebar
import CustomComponents


Page
{
    id: root
    
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.pagesBackground
    }
    
    ListModel
    {
        id: model
        
        ListElement { spaceHolder: "" }
        ListElement { spaceHolder: "" }
        ListElement { spaceHolder: "" }
        ListElement { spaceHolder: "" }
        ListElement { spaceHolder: "" }
        ListElement { spaceHolder: "" }
        ListElement { spaceHolder: "" }
        ListElement { spaceHolder: "" }
        ListElement { spaceHolder: "" }
        ListElement { spaceHolder: "" }
    }
    
    RowLayout
    {
        id: verticalLayout
        property int leftSpacing : 64
        
        width: parent.width
        spacing: 0
        
        Item
        {
            id: leftSpacer
            Layout.fillHeight: true
            Layout.preferredWidth: verticalLayout.leftSpacing
        }
        
        ColumnLayout
        {
            id: contentLayout
            property int rightMargin : 70
            
            Layout.preferredWidth: parent.width - verticalLayout.leftSpacing
            Layout.alignment: Qt.AlignLeft
            spacing: 0
            
            
            RowLayout
            {
                id: headerRow
                Layout.preferredWidth: parent.width
                spacing: 0
                
                MTitle
                {
                    id: title
                    Layout.topMargin: 44
                    titleText: "Home"
                    descriptionText: "You have 10 books"
                }
                
                Item { Layout.fillWidth: true }
                
                MButton
                {
                    id: addBooksButton
                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 40
                    Layout.rightMargin: contentLayout.rightMargin
                    Layout.topMargin: 22
                    backgroundColor: properties.colorBasePurple
                    textContent: "Add books"
                    fontColor: properties.colorBackground
                    fontBold: true
                    fontSize: 13
                    imagePath: "/resources/images/plus.svg"
                }
            }
            
            MToolbar
            {
                id: toolbar
                Layout.preferredWidth: parent.width - contentLayout.rightMargin
                Layout.alignment: Qt.AlignLeft
                Layout.topMargin: 45
            }
            
            Grid
            {
                id: bookGrid
                property int bookWidth : 190
                
                Layout.preferredWidth: parent.width - contentLayout.rightMargin
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
                
                // Calculate the amount of columns which can be displayed
                onWidthChanged: columns = (width+columnSpacing) / (columnSpacing + bookWidth);
            }
            
            MIndexBar
            {
                Layout.topMargin: 50
            }
        }
    }
}
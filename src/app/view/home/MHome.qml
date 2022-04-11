import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import librum.extensions.sidebar
import CustomComponents


Page
{
    id: root
    property bool empty : true
    
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
        ListElement { spaceHolder: "" }
        ListElement { spaceHolder: "" }
    }
    
    RowLayout
    {
        id: verticalLayout
        property int outsideSpacing : 64
        
        width: parent.width
        spacing: 0
        
        Item { Layout.preferredWidth: verticalLayout.outsideSpacing }
        
        ColumnLayout
        {
            id: contentLayout
            property int rightMargin : 70
            
            Layout.fillWidth: true
            Layout.rightMargin: contentLayout.rightMargin
            spacing: 0
            
            
            RowLayout
            {
                id: headerRow
                Layout.fillWidth: true
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
                    Layout.topMargin: 22
                    Layout.alignment: Qt.AlignBottom
                    backgroundColor: properties.colorBasePurple
                    textContent: "Add books"
                    fontColor: properties.colorBackground
                    fontBold: true
                    fontSize: 13
                    imagePath: properties.iconPlusWhite
                }
            }
            
            MToolbar
            {
                id: toolbar
                visible: !root.empty
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                Layout.topMargin: 45
            }
            
            Grid
            {
                id: bookGrid
                property int bookWidth : 190
                
                Layout.fillWidth: true
                Layout.topMargin: 30
                visible: !root.empty
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
            
            MEmptyScreenContent
            {
                id: emptyScreenContent
                visible: root.empty
                Layout.fillWidth: true
                Layout.topMargin: 32
                
                onClicked: root.empty = false
            }
            
            MIndexBar
            {
                id: indexBar
                visible: !root.empty
                Layout.fillWidth: true
                Layout.topMargin: 50
            }
        }
    }
}
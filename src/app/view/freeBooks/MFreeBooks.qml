import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
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
            Layout.rightMargin: rightMargin
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
                    titleText: "Free books"
                    descriptionText: "Choose from over 60,000 books"
                }
            }
            
            Item { Layout.fillWidth: true }
            
            MToolbar
            {
                id: toolbar
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                Layout.topMargin: 45
                showTagBox: false
            }
            
            Grid
            {
                id: bookGrid
                property int bookWidth : 190
                
                Layout.fillWidth: true
                Layout.fillHeight: true
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
        }
    }
}
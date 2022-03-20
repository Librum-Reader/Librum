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
                    imagePath: properties.iconPlusWhite
                }
            }
            
            MToolbar
            {
                id: toolbar
                visible: !root.empty
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
            
            Item
            {
                id: emptyBackground
                Layout.preferredWidth: parent.width - contentLayout.rightMargin
                Layout.preferredHeight: emptyBackgroundImage.implicitHeight
                Layout.topMargin: 32
                
                
                Image
                {
                    id: emptyBackgroundImage
                    visible: root.empty
                    source: properties.imageEmptyHomeBackground
                    sourceSize.width: parent.width
                    fillMode: Image.PreserveAspectFit
                    
                    ColumnLayout
                    {
                        id: inEmptyBackgroundLayout
                        width: parent.width
                        
                        Image
                        {
                            id: fileSwiftImage
                            Layout.preferredWidth: 250
                            Layout.preferredHeight: 135
                            Layout.alignment: Qt.AlignHCenter
                            Layout.topMargin: 225
                            source: properties.imageFileSwift
                        }
                        
                        Label
                        {
                            id: addBooksQuestion
                            Layout.preferredWidth: 250
                            Layout.alignment: Qt.AlignHCenter
                            text: "Quiet empty here, what about importing your first book?"
                            horizontalAlignment: Text.AlignHCenter
                            font.pointSize: 14
                            font.family: properties.defaultFontFamily
                            font.weight: Font.DemiBold
                            color: properties.colorLightText3
                            wrapMode: Text.WordWrap
                        }
                        
                        MButton
                        {
                            id: emptyAddBooksButton
                            Layout.preferredWidth: 134
                            Layout.preferredHeight: 42
                            Layout.alignment: Qt.AlignHCenter
                            Layout.topMargin: 20
                            backgroundColor: properties.colorLightPurple
                            borderColor: properties.colorMediumPurple
                            textContent: "Add book"
                            fontColor: properties.colorNeonBlue
                            fontBold: true
                            fontSize: 13.5
                            imagePath: properties.iconPlusBlack
                            imageSize: 16
                        }
                    }
                }
            }
            
            MIndexBar
            {
                id: indexBar
                visible: !root.empty
                Layout.preferredWidth: parent.width - contentLayout.rightMargin
                Layout.topMargin: 50
            }
        }
    }
}
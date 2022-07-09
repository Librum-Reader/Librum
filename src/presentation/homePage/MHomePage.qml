import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import Librum.Elements
import CustomComponents
import Librum.style
import Librum.icons
import "toolbar"
import "indexbar"
import "bookOptionsPopup"


Page
{
    id: root
    property bool empty : true
    
    horizontalPadding: 64
    rightPadding: 70
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.pagesBackground
    }
    
    
    Shortcut
    {
        sequence: StandardKey.New
        onActivated: fileDialog.open()
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
    
    
    ColumnLayout
    {
        id: contentLayout
        
        width: parent.width
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
                borderWidth: 0
                backgroundColor: Style.colorBasePurple
                text: "Add books"
                fontColor: Style.colorBackground
                fontWeight: Font.Bold
                fontSize: 13
                imagePath: Icons.plusWhite
                
                onClicked: fileDialog.open()
            }
        }
        
        MToolbar
        {
            id: toolbar
            visible: !root.empty
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft
            Layout.topMargin: 45
            z: 2
        }
        
        Pane
        {
            id: bookGridContainer
            Layout.fillWidth: true
            Layout.preferredHeight: 695
            Layout.topMargin: 30
            visible: !root.empty
            padding: 0
            background: Rectangle
            {
                color: "transparent"
            }
            
            
            GridView
            {
                id: bookGrid
                property int bookWidth: 190
                property int bookHeight: 300
                property int horizontalSpacing: 64
                property int verticalSpacing: 48
                
                anchors.fill: parent
                cellWidth: bookWidth + horizontalSpacing
                cellHeight: bookHeight + verticalSpacing
                rightMargin: -horizontalSpacing
                interactive: true
                boundsBehavior: Flickable.StopAtBounds
                flickDeceleration: 10000
                clip: true
                
                
                model: indexBar.selectedAmountOfBooks
                delegate: MBook
                { 
                    onLeftButtonClicked: loadPage(readingPage);
                    
                    onRightButtonClicked:
                        (index, mouse) =>
                        {
                            let currentMousePosition = mapToItem(bookGridContainer, mouse.x, mouse.y);
                            let absoluteMousePosition = mapToItem(root, mouse.x, mouse.y);
                            
                            bookOptionsPopup.x = bookOptionsPopup.getBookOptionsPopupXCoord(currentMousePosition.x, absoluteMousePosition.x) + 2;
                            bookOptionsPopup.y = bookOptionsPopup.getBookOptionsPopupYCoord(currentMousePosition.y, absoluteMousePosition.y) + 2;
                            bookOptionsPopup.visible = !bookOptionsPopup.visible;
                        }
                    
                    onMoreOptionClicked:
                        (index, mouse) =>
                        {
                            let currentMousePosition = mapToItem(bookGridContainer, mouse.x, mouse.y);
                            
                            bookOptionsPopup.x = currentMousePosition.x - bookOptionsPopup.implicitWidth / 2;
                            bookOptionsPopup.y = currentMousePosition.y - bookOptionsPopup.implicitHeight - 6;
                            bookOptionsPopup.visible = !bookOptionsPopup.visible;
                        }
                }
                
                
                MBookOptionsPopup
                {
                    id: bookOptionsPopup
                    visible: false
                    
                    function getBookOptionsPopupXCoord(currentXPosition, absoluteXPosition)
                    {
                        if(spaceToRootWidth(absoluteXPosition) <= 0)
                            return currentXPosition + spaceToRootWidth(absoluteXPosition);
                        
                        return currentXPosition;
                    }
                    
                    function spaceToRootWidth(xCoord)
                    {
                        return root.width - (xCoord + implicitWidth);
                    }
                    
                    
                    function getBookOptionsPopupYCoord(currentYPosition, absoluteYPosition)
                    {
                        if(spaceToRootHeight(absoluteYPosition) <= 0)
                            return currentYPosition + spaceToRootHeight(absoluteYPosition);
                        
                        return currentYPosition;
                    }
                    
                    function spaceToRootHeight(yCoord)
                    {
                        return root.height - (yCoord + implicitHeight);
                    }
                }
            }
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
            Layout.topMargin: 46
        }
    }
    
    
    MAcceptDeletionPopup
    {
        id: acceptDeletionPopup
        x: Math.round(root.width / 2 - implicitWidth / 2 - sidebar.width / 2 - root.horizontalPadding)
        y: Math.round(root.height / 2 - implicitHeight / 2 - root.topPadding - 50)
    }
    
    
    MBookDetailsPopup
    {
        id: bookDetailsPopup
        
        x: Math.round(root.width / 2 - implicitWidth / 2 - sidebar.width / 2 - root.horizontalPadding)
        y: Math.round(root.height / 2 - implicitHeight / 2 - root.topPadding - 30)
    }
    
    
    FileDialog
    {
        id: fileDialog
        acceptLabel: "Import"
        fileMode: FileDialog.OpenFiles
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        nameFilters: ["Text files (*.txt)", "PDF files (*.pdf)", "MOBI files (*.pdf)",
            "WOLF files (*.wol)", "RTF files (*.rtf)", "PDB files (*.pdb)",
            "HTML files (*.html *.htm)", "EPUB files (*.epub)", "MOBI files (*mobi)",
            "DJVU files (*.djvu)"]
    
        onAccepted: root.empty = false
        onRejected: root.empty = false
    }
}
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.0
import QtQml.Models 2.15
import CustomComponents 1.0
import Librum.elements 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.controllers 1.0
import Librum.globals 1.0
import "toolbar"
import "indexbar"
import "tags"


Page
{
    id: root
    property bool empty : BookController.libraryModel.rowCount() === 0
    
    horizontalPadding: 64
    rightPadding: 70
    bottomPadding: 15
    background: Rectangle { anchors.fill: parent; color: Style.pagesBackground }
    
    
    Connections
    {
        id: emptyLibraryUpdater
        target: BookController.libraryModel
        
        
        function onRowsRemoved(index, first, last)
        {
            root.empty = BookController.libraryModel.rowCount() === 0
        }
    }
    
    
    Shortcut
    {
        sequence: StandardKey.New
        onActivated: fileDialog.open()
    }
    
    ListModel { id: bookList }
    
    
    ColumnLayout
    {
        id: contentLayout
        anchors.fill: parent
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
        
        Item { Layout.fillHeight: true; Layout.maximumHeight: 45; Layout.minimumHeight: 8 }
        
        MToolbar
        {
            id: toolbar
            visible: !root.empty
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft
            z: 2
        }
        
        Pane
        {
            id: bookGridContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 700
            Layout.minimumHeight: 100
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
                property var xl : [1,2]
                
                anchors.fill: parent
                cellWidth: bookWidth + horizontalSpacing
                cellHeight: bookHeight + verticalSpacing
                rightMargin: -horizontalSpacing
                interactive: true
                boundsBehavior: Flickable.StopAtBounds
                flickDeceleration: 3500
                maximumFlickVelocity: 3000
                clip: true
                model: BookController.libraryModel
                delegate: MBook
                {
                    id: bookDel
                    
                    onLeftButtonClicked:
                    {
                        Globals.selectedBook = BookController.getBook(model.title);
                        bookGrid.openBook();
                    }
                        
                    onRightButtonClicked:
                        (index, mouse) =>
                        {
                            let currentMousePosition = mapToItem(bookGridContainer, mouse.x, mouse.y);
                            let absoluteMousePosition = mapToItem(root, mouse.x, mouse.y);
                            bookOptionsPopup.setSpawnPosition(currentMousePosition, absoluteMousePosition, root);
                            bookDel.openBookOptions();
                        }
                    
                    onMoreOptionClicked:
                        (index, mouse) =>
                        {
                            let currentMousePosition = mapToItem(bookGridContainer, mouse.x, mouse.y);
                            bookOptionsPopup.x = currentMousePosition.x - bookOptionsPopup.implicitWidth / 2;
                            bookOptionsPopup.y = currentMousePosition.y - bookOptionsPopup.implicitHeight - 6;
                            bookDel.openBookOptions();
                        }
                    
                    function openBookOptions()
                    {
                        Globals.selectedBook = BookController.getBook(model.title);
                        Globals.bookTags = Qt.binding(function () { return model.tags; });
                        bookOptionsPopup.open();
                    }
                }
                
                
                MRightClickMenu
                {
                    id: bookOptionsPopup
                    implicitHeight: 213
                    visible: false
                    
                    
                    objectModel: ObjectModel
                    {
                        MRightClickMenuItem
                        {
                            width: bookOptionsPopup.width
                            imagePath: Icons.bookOpen
                            imageSize: 17
                            text: "Read book"
                            
                            onClicked: bookGrid.openBook()
                        }
                        
                        MRightClickMenuItem
                        {
                            width: bookOptionsPopup.width
                            imagePath: Icons.book
                            imageSize: 14
                            text: "Book details"
                            
                            onClicked: {
                                bookDetailsPopup.open();
                                bookOptionsPopup.close();
                            }
                        }
                        
                        MRightClickMenuItem
                        {
                            width: bookOptionsPopup.width
                            Layout.bottomMargin: 4
                            imagePath: Icons.addFile
                            imageSize: 14
                            text: "Save to files"
                            
                            onClicked:
                            {
                                downloadFileDialog.open();
                                bookOptionsPopup.close();
                            }
                        }
                        
                        Item { id: rectMargin1; height: 4; width: bookOptionsPopup.width }
                        
                        Rectangle { width: bookOptionsPopup.width; height: 1; color: Style.colorLightBorder }
                        
                        Item { id: rectMargin2; height: 4; width: bookOptionsPopup.width }
                        
                        MRightClickMenuItem
                        {
                            width: bookOptionsPopup.width
                            Layout.topMargin: 4
                            imagePath: Icons.tagGray
                            imageSize: 16
                            text: "Manage tags"
                            
                            onClicked:
                            {
                                manageTagsPopup.open();
                                bookOptionsPopup.close();
                            }
                        }
                        
                        MRightClickMenuItem
                        {
                            width: bookOptionsPopup.width
                            imagePath: Icons.checkCircle
                            imageSize: 17
                            text: "Mark as read"
                            
                            onClicked:
                            {
                                bookOptionsPopup.close();
                            }
                        }
                        
                        MRightClickMenuItem
                        {
                            width: bookOptionsPopup.width
                            imagePath: Icons.trashGray
                            imageSize: 16
                            text: "Delete book"
                            
                            onClicked:
                            {
                                acceptDeletionPopup.open();
                                bookOptionsPopup.close();
                            }
                        }
                    }
                }
            
            
                function openBook(title)
                {
                    if(bookOptionsPopup.opened)
                        bookOptionsPopup.close();
                    
                    BookController.refreshLastOpenedFlag(Globals.selectedBook.title);
                    loadPage(readingPage);
                }
            }
        }
        
        MEmptyScreenContent
        {
            id: emptyScreenContent
            visible: root.empty
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 32
            
            onClicked:
            {
                fileDialog.open();
            }
        }
        
        Item { Layout.fillHeight: true }
        
        MIndexBar
        {
            id: indexBar
            visible: !root.empty
            Layout.fillWidth: true
            
            onSelectedAmountOfBooksChanged: bookGrid.model = selectedAmountOfBooks
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
    
    MManageTagsPopup
    {
        id: manageTagsPopup
        x: Math.round(root.width / 2 - implicitWidth / 2 - sidebar.width / 2 - root.horizontalPadding)
        y: Math.round(root.height / 2 - implicitHeight / 2 - root.topPadding - 30)
    }
    
    FileDialog
    {
        id: downloadFileDialog
        acceptLabel: "Save"
        fileMode: FileDialog.SaveFile
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
    }
    
    FileDialog
    {
        id: fileDialog
        acceptLabel: "Import"
        fileMode: FileDialog.OpenFiles
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        nameFilters: ["All files (*)", "Text files (*.txt)", "PDF files (*.pdf)", "MOBI files (*.pdf)",
            "WOLF files (*.wol)", "RTF files (*.rtf)", "PDB files (*.pdb)",
            "HTML files (*.html *.htm)", "EPUB files (*.epub)", "MOBI files (*mobi)",
            "DJVU files (*.djvu)"]
        
        onAccepted:
        {
            if(BookController.addBook(file) === BookOperationStatus.Success)
            {
                root.empty = false;
            }
            else
            {
                console.log("Error loading file!");
            }
        }
    }
}
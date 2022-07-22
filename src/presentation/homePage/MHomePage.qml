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
import "tags"


Page
{
    id: root
    property bool empty : true
    
    horizontalPadding: 64
    rightPadding: 70
    bottomPadding: 15
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
                
                anchors.fill: parent
                cellWidth: bookWidth + horizontalSpacing
                cellHeight: bookHeight + verticalSpacing
                rightMargin: -horizontalSpacing
                interactive: true
                boundsBehavior: Flickable.StopAtBounds
                flickDeceleration: 10000
                clip: true
                
                
                model: 12
                delegate: MBook
                {
                    onLeftButtonClicked: loadPage(readingPage);
                    
                    onRightButtonClicked:
                        (index, mouse) =>
                        {
                            let currentMousePosition = mapToItem(bookGridContainer, mouse.x, mouse.y);
                            let absoluteMousePosition = mapToItem(root, mouse.x, mouse.y);
                            
                            bookOptionsPopup.setSpawnPosition(currentMousePosition, absoluteMousePosition, root);
                            bookOptionsPopup.open();
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
                
                
                MRightClickMenu
                {
                    id: bookOptionsPopup
                    visible: false
                    
                    
                    objectModel: ObjectModel
                    {
                        MRightClickMenuItem
                        {
                            width: bookOptionsPopup.width
                            imagePath: Icons.bookOpen
                            imageSize: 17
                            text: "Read book"
                            
                            onClicked:
                            {
                                bookOptionsPopup.close();
                                loadPage(readingPage);
                            }
                        }
                        
                        MRightClickMenuItem
                        {
                            width: bookOptionsPopup.width
                            imagePath: Icons.book
                            imageSize: 14
                            text: "Book details"
                            
                            onClicked:
                            {
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
            }
        }
        
        MEmptyScreenContent
        {
            id: emptyScreenContent
            visible: root.empty
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 32
            
            onClicked: root.empty = false
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
        nameFilters: ["Text files (*.txt)", "PDF files (*.pdf)", "MOBI files (*.pdf)",
            "WOLF files (*.wol)", "RTF files (*.rtf)", "PDB files (*.pdb)",
            "HTML files (*.html *.htm)", "EPUB files (*.epub)", "MOBI files (*mobi)",
            "DJVU files (*.djvu)"]
        
        onAccepted: root.empty = false
        onRejected: root.empty = false
    }
}
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.elements 1.0
import Librum.controllers 1.0
import Librum.globals 1.0
import "readingToolbar"
import "readingSearchbar"


Page
{
    id: root
    background: Rectangle { anchors.fill: parent; color: Style.colorPageBackground }
    
    Component.onCompleted: root.forceActiveFocus()
    
    
    Shortcut
    {
        id: zoomIn
        sequences: [StandardKey.ZoomIn]
        onActivated: documentView.zoom(1.2)
    }
    
    Shortcut
    {
        id: zoomOut
        sequences: [StandardKey.ZoomOut]
        onActivated: documentView.zoom(0.8)
    }
    
    Shortcut
    {
        id: flickUp
        sequences: ["Up"]
        onActivated: documentView.flick("up")
    }
    
    Shortcut
    {
        id: flickDown
        sequences: ["Down"]
        onActivated: documentView.flick("down")
    }
    
    Shortcut
    {
        id: nextPage
        sequences: [StandardKey.MoveToNextPage, "Right"]
        onActivated: documentView.nextPage()
    }
    
    Shortcut
    {
        id: previousPage
        sequences: [StandardKey.MoveToPreviousPage, "Left"]
        onActivated: documentView.previousPage()
    }
    
    Shortcut
    {
        id: stopFullScreenMode
        sequence: "ESC"
        onActivated: internal.stopFullScreenMode();
    }
    
    
    DocumentItem
    {
        id: documentItem
        
        onUrlChanged: currentPage = 0
        onOpenedChanged:
        {
            if(!opened)
                return;
            
            toolbar.currentPageButton.maxPages = pageCount;
            toolbar.bookTitle = windowTitleForDocument;
        }
        
        Component.onCompleted: documentItem.url = Globals.selectedBook.filePath;
    }
    
    /*
      An invisible component at the top of the screen, when hovering over it
      while the fullscreen mode is enabled, the fullscreen mode will be disabled.
      */
    Item
    {
        id: toolbarReactivationContainer
        width: parent.width
        height: toolbar.height
        z: 1
        
        HoverHandler
        {
            id: toolbarReactivationArea
            enabled: true
            onHoveredChanged: if(hovered) internal.stopFullScreenMode();
        }
    }
    
    
    ColumnLayout
    {
        id: layout
        anchors.fill: parent
        spacing: 0
        
        
        MReadingToolBar
        {
            id: toolbar            
            Layout.fillWidth: true
            currentPage: documentView.document.currentPage
            
            onBackButtonClicked:
            {
                // Save current page before leaving
                internal.saveCurrentPage();
                loadPage(homePage, sidebar.homeItem, false);
            }
            
            onChapterButtonClicked:
            {
                if(chapterSidebar.active)
                {
                    chapterSidebar.close();
                    return;
                }
                
                if(bookmarksSidebar.active)
                    bookmarksSidebar.close();
                
                chapterSidebar.open();
            }
            
            onBookMarkButtonClicked:
            {
                if(bookmarksSidebar.active)
                {
                    bookmarksSidebar.close();
                    return;
                }
                
                if(chapterSidebar.active)
                    chapterSidebar.close();
                
                bookmarksSidebar.open();
            }
            
            onCurrentPageButtonClicked:
            {
                currentPageButton.active = !currentPageButton.active;
            }
            
            onFullScreenButtonClicked:
            {
                if(internal.fullScreen)
                    internal.stopFullScreenMode();
                else
                    internal.startFullScreenMode();
            }
            
            onSearchButtonClicked:
            {
                searchbar.visible = !searchbar.visible;
            }
            
            onOptionsPopupVisibileChanged:
            {
                optionsButton.active = !optionsButton.active
            }
            
            
            PropertyAnimation
            {
                id: hideToolbar
                target: toolbar
                property: "opacity"
                to: 0
                duration: 100
                
                onFinished: toolbar.visible = false
            }
            
            PropertyAnimation
            {
                id: showToolbar
                target: toolbar
                property: "opacity"
                to: 1
                duration: 200
                
                onStarted: toolbar.visible = true
            }
        }
        
        
        /*
          Contains the reizeable sidebars, e.g. chapter or book-sidebar
          */
        SplitView
        {
            id: sidebarSplitView
            Layout.fillHeight: true
            Layout.fillWidth: true
            orientation: Qt.Horizontal
            padding: 0
            spacing: 10
            handle: Rectangle { implicitWidth: 8; color: "transparent" }
            smooth: true
            
            /*
              Need to combine sidebar items into one item, else rezising doesn't
              work properly, since the SplitView thinks they are 3 different items
              */
            Item
            {
                id: sidebarItem
                visible: chapterSidebar.active || bookmarksSidebar.active
                // Load in the last width of the correct sidebar
                SplitView.preferredWidth: chapterSidebar.visible ? chapterSidebar.lastWidth 
                                                                 : bookmarksSidebar.visible ? bookmarksSidebar.lastWidth : 0
                SplitView.minimumWidth: chapterSidebar.visible || bookmarksSidebar.visible ? 140 : 0
                SplitView.maximumWidth: 480
                
                
                MChapterSidebar
                {
                    id: chapterSidebar
                    property int lastWidth: 370
                    property bool active: false
                    chapterModel: documentItem.tableOfContents
                    anchors.fill: parent
                    visible: false
                    
                    // Save the last width to restore it if re-enabled
                    onVisibleChanged: if(!visible) lastWidth = width
                    onSwitchPage: (pageNumber) => documentView.setPage(pageNumber - 1)
                     
                    
                    Rectangle
                    {
                        id: rightChaptersBorder
                        width: 1
                        height: parent.height
                        color: Style.colorDarkSeparator
                        anchors.right: parent.right
                    }
                    
                    
                    function open()
                    {
                        chapterSidebar.active = true;
                        chapterSidebar.visible = true;
                        toolbar.chapterButton.active = true;
                    }
                    
                    function close()
                    {
                        chapterSidebar.active = false;
                        chapterSidebar.visible = false;
                        toolbar.chapterButton.active = false;
                    }
                }
                
                MBookmarksSidebar
                {
                    id: bookmarksSidebar
                    property int lastWidth: 300
                    property bool active: false
                    
                    anchors.fill: parent
                    visible: false
                    
                    // Save the last width to restore it if re-enabled
                    onVisibleChanged: if(!visible) lastWidth = width
                    
                    
                    Rectangle
                    {
                        id: rightBookmarksBorder
                        width: 1
                        height: parent.height
                        color: Style.colorDarkSeparator
                        anchors.right: parent.right
                    }
                    
                    
                    function open()
                    {
                        bookmarksSidebar.active = true;
                        bookmarksSidebar.visible = true;
                        toolbar.bookmarksButton.active = true;
                    }
                    
                    function close()
                    {
                        bookmarksSidebar.active = false;
                        bookmarksSidebar.visible = false;
                        toolbar.bookmarksButton.active = false;
                    }
                }
            }
            
            
            RowLayout
            {
                id: documentLayout
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                spacing: 0
                clip: true
                
                
                MDocumentView
                {
                    id: documentView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    visible: documentItem.opened
                    document: documentItem
                    
                    onPageWidthChanged: (width) => toolbar.pageWidth = width
                }
            }
        }
        
        MReadingSearchbar
        {
            id: searchbar
            visible: false
            Layout.fillWidth: true
            
            onVisibleChanged: toolbar.searchButton.active = visible;
        }
    }
    
    QtObject
    {
        id: internal
        property bool fullScreen: false
        
        
        function startFullScreenMode()
        {
            if(internal.fullScreen)
                return;
            
            internal.fullScreen = true;
            hideToolbar.start();
        }
        
        function stopFullScreenMode()
        {
            if(!internal.fullScreen)
                return;
            
            internal.fullScreen = false;
            showToolbar.start();
        }
        
        function saveCurrentPage()
        {
            var operationsMap = {};
            operationsMap[BookController.MetaProperty.CurrentPage] = documentItem.currentPage + 1;
            BookController.updateBook(Globals.selectedBook.uuid, operationsMap);
        }
    }
}
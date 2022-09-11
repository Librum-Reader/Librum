import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.elements 1.0
import "readingToolbar"
import "readingSearchbar"


Page
{
    id: root
    property bool fullScreen: false
    
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.pagesBackground
    }
    
    
    Shortcut
    {
        sequences: [ StandardKey.ZoomIn ]
        onActivated: view.renderScale += 0.1
    }
    Shortcut
    {
        sequence: StandardKey.ZoomOut
        onActivated: view.renderScale -= 0.1
    }
    Shortcut
    {
        sequence: "ESC"
        onActivated: if(root.fullScreen) root.exitFullScreen();
    }
    
    
    DocumentItem
    {
        id: documentItem
        onUrlChanged: currentPage = 0
        
        Component.onCompleted: documentItem.url = "file:/home/creapermann/Me/resources/books/linux/Linux_Bible.pdf";
        
        onOpenedChanged:
        {
            if(opened)
            {
                toolbar.currentPageButton.maxPages = pageCount;
                toolbar.bookTitle = windowTitleForDocument;
            }
        }
    }
    
    
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
            onHoveredChanged:
            {
                if(hovered)
                    root.exitFullScreen();
            }
        }
    }
    
    
    ColumnLayout
    {
        id: mainLayout
        anchors.fill: parent
        spacing: 0
        
        
        MReadingToolBar
        {
            id: toolbar            
            Layout.fillWidth: true
            
            currentPage: pageArea.document.currentPage
            
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
                if(root.fullScreen) root.exitFullScreen();
                else root.enterFullScreen();
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
        
        SplitView
        {
            Layout.fillHeight: true
            Layout.fillWidth: true
            orientation: Qt.Horizontal
            padding: 0
            spacing: 10
            handle: Rectangle
            {
                implicitWidth: 8
                color: "transparent"
            }
            smooth: true
            
            // Need to combine sidebars into one item, else rezising doesnt work properly
            Item
            {
                id: sidebarItem
                SplitView.preferredWidth: chapterSidebar.visible ? chapterSidebar.lastWidth 
                                                                 : bookmarksSidebar.visible ? bookmarksSidebar.lastWidth : 0
                SplitView.minimumWidth: chapterSidebar.visible || bookmarksSidebar.visible ? 140 : 0
                SplitView.maximumWidth: 480
                
                visible: chapterSidebar.active || bookmarksSidebar.active
                
                
                MChapterSidebar
                {
                    id: chapterSidebar
                    property int lastWidth: 300
                    property bool active: false
                    
                    onVisibleChanged: if(!visible) lastWidth = width
                    
                    anchors.fill: parent
                    visible: false
                    
                    
                    Rectangle
                    {
                        width: 1
                        height: parent.height
                        color: Style.colorLightBorder
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
                    
                    onVisibleChanged: if(!visible) lastWidth = width
                    
                    anchors.fill: parent
                    visible: false
                    
                    
                    Rectangle
                    {
                        width: 1
                        height: parent.height
                        color: Style.colorLightBorder
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
            
            
            Pane
            {
                id: readingSpace
                padding: 0
                SplitView.fillWidth: true
                SplitView.fillHeight: true
                background: Rectangle
                {
                    color: "transparent"
                }
                
                Rectangle
                {
                    id: page
                    height: parent.height
                    width: pageArea.contentWidth == 0 ? 1020 : pageArea.contentWidth >= parent.width ? parent.width : pageArea.contentWidth
                    anchors.centerIn: parent
                    Layout.alignment: Qt.AlignCenter
                    color: Style.colorBackground
                    radius: 2
                    
                    onWidthChanged: toolbar.pageWidth = width
                    
                    
                    RowLayout
                    {
                        id: displayLayout
                        anchors.fill: parent
                        spacing: 0
                        clip: true
                        
                        
                        Rectangle
                        {
                            Layout.preferredWidth: 1
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignLeft
                            color: Style.colorLightBorder
                        }
                        
                        DocumentView
                        {
                            id: pageArea
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            visible: documentItem.opened
                            document: documentItem
                        }
                        
                        
                        Rectangle
                        {
                            Layout.preferredWidth: 1
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignRight
                            color: Style.colorLightBorder
                        }
                    }
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
    
    
    Component.onCompleted: root.forceActiveFocus()
     
    
    function enterFullScreen()
    {
        fullScreen = true;
        
        if(toolbar.visible)
            hideToolbar.start();
    }
    
    function exitFullScreen()
    {
        fullScreen = false;
        
        if(!toolbar.visible)
            showToolbar.start();
    }
}
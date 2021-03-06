import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Pdf
import CustomComponents
import Librum.style
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
        sequence: StandardKey.ZoomIn
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
            
            onChapterButtonClicked:
            {
                // close
                if(chapterButton.active)
                {
                    chapterButton.active = false;
                    chapterSidebar.visible = false;
                    return;
                }
                
                // close bookmarks sidebar and open
                if(bookmarkButton.active)
                {
                    bookmarkButton.active = false;
                    bookmarksSidebar.visible = false;
                    
                    chapterButton.active = true;
                    chapterSidebar.visible = true;
                    return;
                }
                
                // open
                chapterButton.active = true;
                chapterSidebar.visible = true;
            }
            
            onBookMarkButtonClicked:
            {
                // close
                if(bookmarkButton.active)
                {
                    bookmarkButton.active = false;
                    bookmarksSidebar.visible = false;
                    return;
                }
                
                // close chapters sidebar and open
                if(chapterButton.active)
                {
                    chapterButton.active = false;
                    chapterSidebar.visible = false;
                    
                    bookmarkButton.active = true;
                    bookmarksSidebar.visible = true;
                    return;
                }
                
                // open
                bookmarkButton.active = true;
                bookmarksSidebar.visible = true;
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
                SplitView.preferredWidth: chapterSidebar.visible ? chapterSidebar.lastWidth 
                                                                 : bookmarksSidebar.visible ? bookmarksSidebar.lastWidth 
                                                                                            : 0
                SplitView.minimumWidth: 140
                SplitView.maximumWidth: 480
                
                MChapterSidebar
                {
                    id: chapterSidebar
                    property int lastWidth: 300
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
                }
                
                MBookmarksSidebar
                {
                    id: bookmarksSidebar
                    property int lastWidth: 300
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
                    width: 1020
                    anchors.centerIn: parent
                    Layout.alignment: Qt.AlignCenter
                    color: Style.colorBackground
                    radius: 2
                    
                    
                    Rectangle
                    {
                        width: 1
                        height: parent.height
                        color: Style.colorLightBorder
                        anchors.left: parent.left
                    }
                    
                    Rectangle
                    {
                        width: 1
                        height: parent.height
                        color: Style.colorLightBorder
                        anchors.right: parent.right
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
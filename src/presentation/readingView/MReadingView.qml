import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import "readingToolbar"
import "readingSearchbar"


Page
{
    id: root    
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.pagesBackground
    }
    
    Keys.onPressed:
        (event) =>
        {
            if((event.key === Qt.Key_F) && (event.modifiers & Qt.ControlModifier && (event.modifiers & Qt.AltModifier)))
            {
                // @disable-check M127
                toolbar.visible ? fullScreenAnimation.start() : partScreenAnimation.start();
                toolbarReactivationDetector.notFirstMouseExit = true;
            }
            else if((event.key === Qt.Key_F) && (event.modifiers & Qt.ControlModifier))
            {
                searchbar.visible = !searchbar.visible;
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
                if(root.fullScreen)
                    partScreenAnimation.start();
                else
                    fullScreenAnimation.start();
            }
            
            onSearchButtonClicked:
            {
                searchbar.visible = !searchbar.visible;
            }
            
            onOptionsPopupVisibileChanged:
            {
                optionsButton.active = !optionsButton.active
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
            
            
            MChapterSidebar
            {
                id: chapterSidebar
                SplitView.preferredWidth: 280
                SplitView.minimumWidth: 140
                SplitView.maximumWidth: 480
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
                SplitView.preferredWidth: 280
                SplitView.minimumWidth: 140
                SplitView.maximumWidth: 480
                visible: false
                
                
                Rectangle
                {
                    width: 1
                    height: parent.height
                    color: Style.colorLightBorder
                    anchors.right: parent.right
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
                
                
                MouseArea
                {
                    id: toolbarReactivationDetector
                    
                    // When closing the toolbar, the mouse ends up in this mouseArea, this check makes sure that
                    // the toolbar doesnt get opened directly again, because "onEntered" will be triggered directly
                    property bool notFirstMouseExit: false
                    
                    width: parent.width
                    height: 40
                    visible: !toolbar.visible
                    z: 5
                    hoverEnabled: true
                    
                    onExited: notFirstMouseExit = true
                    onEntered: if(notFirstMouseExit) partScreenAnimation.start()
                    onVisibleChanged: if(!visible) notFirstMouseExit = false
                }
                
                Rectangle
                {
                    id: page
                    height: parent.height
                    width: 800
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
        
        
        PropertyAnimation
        {
            id: fullScreenAnimation
            target: toolbar
            property: "opacity"
            to: 0
            duration: 0
            
            onStarted:toolbar.fullScreenButton.active = true
            onFinished: toolbar.visible = false
        }
        
        PropertyAnimation
        {
            id: partScreenAnimation
            target: toolbar
            property: "opacity"
            to: 1
            duration: 200
            
            onStarted:
            {
                onStarted: toolbar.fullScreenButton.active = false;
                toolbar.visible = true;
            }
        }
        
        
        Component.onCompleted: root.forceActiveFocus()
    }
}
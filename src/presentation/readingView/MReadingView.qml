import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import "readingToolbar"


Page
{
    id: root
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.pagesBackground
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
                fullScreenButton.active = !fullScreenButton.active;
            }
            
            onSearchButtonClicked:
            {
                searchButton.active = !searchButton.active;
            }
            
            onOptionsButtonClicked:
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
                background: Rectangle
                {
                    color: "transparent"
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
            Layout.fillWidth: true
        }
    }
}
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
                if(!chapterSidebar.visible && !bookmarksSidebar.visible)
                {
                    chapterSidebar.visible = true;
                    return;
                }
                chapterSidebar.visible = false;
            }
            
            onBookMarkButtonClicked:
            {
                if(!bookmarksSidebar.visible && !chapterSidebar.visible)
                {
                    bookmarksSidebar.visible = true;
                    return;
                }
                bookmarksSidebar.visible = false;
            }
            
            onFullScreenButtonClicked:
            {
                
            }
            
            onSearchButtonClicked:
            {
                
            }
            
            onOptionsButtonClicked:
            {
                
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
            
            MChapterSidebar
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
    }
}
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import CustomComponents
import "explorerToolbar"


Page
{
    id: root
    
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.pagesBackground
    }
    
    onWidthChanged:
    {
        if(getBookPopup.opened)
        {
            getBookPopup.close();
        }
    }
    
    
    RowLayout
    {
        id: verticalLayout
        property int outsideSpacing : 64
        
        width: parent.width
        spacing: 0
        
        Item { Layout.preferredWidth: verticalLayout.outsideSpacing }
        
        ColumnLayout
        {
            id: contentLayout
            property int rightMargin : 70
            
            Layout.fillWidth: true
            Layout.rightMargin: rightMargin
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
                    titleText: "Free books"
                    descriptionText: "Choose from over 60,000 books"
                }
            }
            
            Item { Layout.fillWidth: true }
            
            MExplorerToolbar
            {
                id: toolbar
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                Layout.topMargin: 45
            }
            
            Pane
            {
                id: bookGridContainer
                Layout.fillWidth: true
                Layout.preferredHeight: 759
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
                    
                    model: 500
                    delegate: MMinimalBook
                    {
                        MouseArea   
                        {
                            anchors.fill: parent
                            
                            onClicked: getBookPopup.open();
                        }
                    }
                }
            }
        }
    }
    
    
    MDownloadBookPopup
    {
        id: getBookPopup
        
        x: Math.trunc(root.width / 2) - Math.trunc(width / 2) - 30
        y: Math.trunc(root.height / 2) - Math.trunc(height / 2) - 15
    }
}
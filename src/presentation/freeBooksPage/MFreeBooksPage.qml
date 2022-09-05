import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0
import CustomComponents 1.0
import "explorerToolbar"


Page
{
    id: root
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.pagesBackground
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
                    flickDeceleration: 3500
                    maximumFlickVelocity: 3000
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
        
        x: Math.round(root.width / 2 - implicitWidth / 2 - sidebar.width / 2 - root.horizontalPadding)
        y: Math.round(root.height / 2 - implicitHeight / 2 - root.topPadding - 30)
    }
}
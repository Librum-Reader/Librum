import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons


Popup
{
    id: root
    property int maxHeight: 200
    signal selected
    
    focus: true
    padding: 0
    implicitWidth: 151
    background: Rectangle
    {
        color: "transparent"
    }
    
    onOpenedChanged: if(opened) listView.forceActiveFocus();
    
    
    ColumnLayout
    {
        id: mainLayout
        anchors.fill: parent
        spacing: 0
        
        
        Image
        {
            Layout.leftMargin: 14
            Layout.bottomMargin: -1
            source: Icons.dropupLightGray
        }
        
        Pane
        {
            id: container
            Layout.fillHeight: true
            Layout.fillWidth: true
            verticalPadding: 8
            horizontalPadding: 4
            background: Rectangle
            {
                color: Style.colorBackground
                border.width: 1
                border.color: Style.colorLightBorder
                radius: 6
                antialiasing: true
            }
            
            
            ColumnLayout
            {
                width: parent.width
                
                
                ListView
                {
                    id: listView
                    Layout.fillWidth: true
                    Layout.preferredHeight: contentHeight
                    Layout.maximumHeight: 200
                    maximumFlickVelocity: 550
                    currentIndex: 0
                    keyNavigationEnabled: true
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds
                    ScrollBar.vertical: ScrollBar { }
                    
                    model: ListModel
                    {
                        ListElement { text: "Recently read" }
                        ListElement { text: "Recently added" }
                        ListElement { text: "By Percentage" }
                        ListElement { text: "Book (A-Z)" }
                        ListElement { text: "Author (A-Z)" }
                    }
                    
                    delegate: MBaseListItem
                    {
                        width: parent.width
                        containingListview: listView
                        
                        onClicked:
                            (index) =>
                            {
                                listView.currentIndex = index;
                                root.selected();
                            }
                    }
                    
                    Keys.onPressed: 
                        (event) =>
                        {
                            if(event.key === Qt.Key_Return)
                            {
                                root.selected();
                            }
                        }
                }
            }
        }
    }
}
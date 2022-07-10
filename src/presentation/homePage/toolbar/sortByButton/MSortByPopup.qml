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
    
    focus: true
    padding: 0
    implicitWidth: 176
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
                    property MBaseListItem currentSelected: null
                    
                    Layout.fillWidth: true
                    Layout.preferredHeight: contentHeight
                    Layout.maximumHeight: 200
                    maximumFlickVelocity: 550
                    currentIndex: -1
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
                                listView.changeSelected(index);
                            }
                    }
                    
                    Keys.onReturnPressed:
                    {
                        if(listView.currentIndex !== -1)
                            listView.changeSelected(listView.currentIndex)
                    }
                    
                    
                    function changeSelected(index)
                    {
                        listView.currentIndex = index;
                        
                        if(listView.currentSelected == listView.currentItem)
                        {
                            listView.currentItem.selected = !listView.currentItem.selected;
                            return;
                        }
                        
                        if(listView.currentSelected != null)
                            listView.currentSelected.selected = false;
                        
                        listView.currentItem.selected = true;
                        listView.currentSelected = listView.currentItem;
                    }
                }
            }
        }
    }
}
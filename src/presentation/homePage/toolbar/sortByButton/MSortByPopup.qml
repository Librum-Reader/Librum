import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.models 1.0


Popup
{
    id: root
    signal itemSelected(int role)
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
            padding: 6
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
                        ListElement { text: "Recently added"; role: LibraryProxyModel.RecentlyAdded }
                        ListElement { text: "Recently read"; role: LibraryProxyModel.LastOpened }
                        ListElement { text: "Percentage"; role: LibraryProxyModel.Title }
                        ListElement { text: "Book (A-Z)"; role: LibraryProxyModel.Title }
                        ListElement { text: "Authors (A-Z)"; role: LibraryProxyModel.Authors }
                    }
                    
                    delegate: MBaseListItem
                    {
                        width: parent.width
                        containingListview: listView
                        
                        onClicked:
                            (mouse, index) =>
                            {
                                if(listView.itemAtIndex(index).selected)
                                    return;
                                    
                                listView.changeSelected(index);
                                let role = listView.itemAtIndex(index).getRole();
                                root.itemSelected(role);
                            }
                    }
                    
                    Keys.onReturnPressed:
                    {
                        if(listView.currentIndex !== -1)
                            listView.changeSelected(listView.currentIndex)
                    }
                    
                    Component.onCompleted: listView.changeSelected(0);
                    
                    
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
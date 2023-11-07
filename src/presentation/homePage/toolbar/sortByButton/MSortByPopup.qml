import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.models


Popup
{
    id: root
    signal itemSelected(int role)
    property int maxHeight: 200
    
    padding: 0
    implicitWidth: 176
    background: Rectangle { color: "transparent" }
    
    onOpenedChanged: if(opened) listView.forceActiveFocus();
    
    
    ColumnLayout
    {
        id: layout
        anchors.fill: parent
        spacing: 0
        
        
        Image
        {
            id: triangleDecorator
            Layout.leftMargin: 14
            Layout.bottomMargin: -1
            source: Icons.popupDroplet
        }
        
        Pane
        {
            id: container
            Layout.fillHeight: true
            Layout.fillWidth: true
            padding: 6
            background: Rectangle
            {
                color: Style.colorPopupBackground
                border.width: 1
                border.color: Style.colorContainerBorder
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
                        ListElement { text: qsTr("Recently added"); role: LibraryProxyModel.RecentlyAdded }
                        ListElement { text: qsTr("Recently read"); role: LibraryProxyModel.LastOpened }
                        ListElement { text: qsTr("Progress"); role: LibraryProxyModel.Progress }
                        ListElement { text: qsTr("Book (A-Z)"); role: LibraryProxyModel.Title }
                        ListElement { text: qsTr("Authors (A-Z)"); role: LibraryProxyModel.Authors }
                    }
                    
                    delegate: MBaseListItem
                    {
                        width: parent.width
                        containingListview: listView
                        
                        
                        onClicked: (mouse, index) =>
                                   {
                                       // Disable unselecting items
                                       if(listView.itemAtIndex(index).selected)
                                       {
                                           return;
                                       }
                                       
                                       internal.changeSelectedItem(index);
                                       let role = listView.itemAtIndex(index).getRole();
                                       root.itemSelected(role);
                                   }
                    }
                    
                    Keys.onReturnPressed: internal.changeSelectedItem(listView.currentIndex)
                    Component.onCompleted: internal.changeSelectedItem(0);
                }
            }
        }
    }
    
    QtObject
    {
        id: internal
        
        
        function changeSelectedItem(index)
        {
            listView.currentItem.selected = false;
            
            listView.currentIndex = index;
            listView.currentItem.selected = true;
        }
    }
}

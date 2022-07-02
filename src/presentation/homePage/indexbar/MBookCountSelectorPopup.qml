import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style


Popup
{
    id: root
    signal selected(int content)
    
    focus: true
    padding: 0
    background: Rectangle
    {
        color: "transparent"
    }
    
    onOpenedChanged: if(opened) listView.forceActiveFocus()
    
    
    ColumnLayout
    {
        id: mainLayout
        width: parent.width
        spacing: 0
        
        
        Pane
        {
            id: container
            Layout.fillWidth: true
            verticalPadding: 8
            horizontalPadding: 8
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
                    Layout.preferredWidth: parent.width
                    Layout.preferredHeight: contentHeight
                    maximumFlickVelocity: 550
                    currentIndex: 0
                    keyNavigationEnabled: true
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds
                    ScrollBar.vertical: ScrollBar { }
                    
                    model: ListModel
                    {
                        ListElement { text: "2" }
                        ListElement { text: "4" }
                        ListElement { text: "6" }
                        ListElement { text: "8" }
                        ListElement { text: "10" }
                        ListElement { text: "12" }
                        ListElement { text: "14" }
                    }
                    
                    delegate: MBookCountSelectorItem
                    {
                        width: parent.width
                        containingListview: listView
                        
                        onClicked:
                            (index) =>
                            {
                                listView.currentIndex = index;
                                root.selected(listView.currentItem.text);
                            }
                    }
                    
                    Keys.onPressed: 
                        (event) =>
                        {
                            if(event.key === Qt.Key_Return)
                            {
                                root.selected(listView.currentItem.text);
                            }
                        }
                }
            }
        }
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}
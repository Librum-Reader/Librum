import QtQuick
import QtQuick.Layouts
import QtQuick.Controls


Item
{
    id: root
    property int bookCount : 10
    
    implicitWidth: 1714
    implicitHeight: 32
    
    
    RowLayout
    {
        id: layout
        property MIndexBarItem current : contentRect1
        
        anchors.fill: parent
        spacing: 0
        
        
        MBookCountSelector
        {
            id: bookCountSelector
            Layout.alignment: Qt.AlignVCenter
            onSelected: (bookCountSelectorPopup.opened) ? bookCountSelectorPopup.close() : bookCountSelectorPopup.open()
            
            MBookCountSelectorPopup
            {
                id: bookCountSelectorPopup
                x: parent.x
                y: parent.y - bookCountSelectorPopup.height - 8
                closePolicy: Popup.CloseOnReleaseOutsideParent | Popup.CloseOnEscape
                
                onBookCountSelctionMade: content =>
                                         {
                                             parent.amountOfDisplayedBooks = content;
                                             close();
                                         }
            }
        }
        
        Label
        {
            id: descriptionLabel
            Layout.alignment: Qt.AlignVCenter
            Layout.leftMargin: 8
            text: "Entries per page"
            color: properties.colorLightText
            font.pointSize: 11
            font.family: properties.defaultFontFamily            
        }
        
        Item { Layout.fillWidth: true }
        
        
        MNavigationArrow
        {
            id: leftNavigationArrow
            toLeft: true
            Layout.rightMargin: 11
            onClicked: layout.moveToLeft();
        }
        
        MIndexBarItem
        {
            id: contentRect1
            pageNumber: 1
            selected: true
            
            onClicked: layout.changeSelected(this)
        }
        
        MIndexBarItem
        {
            id: contentRect2
            pageNumber: 2
            
            onClicked: layout.changeSelected(this)
        }
        
        MIndexBarItem
        {
            id: contentRect3
            pageNumber: 3
            
            onClicked: layout.changeSelected(this)
        }
        
        Rectangle
        {
            id: dotsRect
            Layout.preferredWidth: 36
            Layout.preferredHeight: 36
            color: "transparent"
            border.color: "transparent"               
            radius: 5
            
            Label
            {
                anchors.centerIn: parent
                text: "..."
                font.pointSize: 14
                font.family: properties.defaultFontFamily
                font.bold: false
                color: properties.colorBaseText
            }
        }
        
        MIndexBarItem
        {
            id: contentRect4
            pageNumber: root.bookCount
            
            onClicked: layout.changeSelected(this)
        }
        
        MNavigationArrow
        {
            id: rightNavigationArrow
            Layout.leftMargin: 6
            
            onClicked: layout.moveToRight();
        }
        
        
        
        function giveFocus()
        {
            root.forceActiveFocus();
        }
        
        function changeSelected(newItem)
        {
            if(newItem === layout.current)
                return;
            
            layout.current.selected = false;
            layout.current = newItem;
            layout.current.selected = true
        }
        
        function moveToLeft()
        {
            switch(layout.current)
            {
            case contentRect1:
                break;
            case contentRect2:
                changeSelected(contentRect1);
                break;
            case contentRect3:
                changeSelected(contentRect2);
                break;
            case contentRect4:
                changeSelected(contentRect3);
                break;
            }
        }
        
        function moveToRight()
        {
            switch(layout.current)
            {
            case contentRect1:
                changeSelected(contentRect2);
                break;
            case contentRect2:
                changeSelected(contentRect3);
                break;
            case contentRect3:
                changeSelected(contentRect4);
                break;
            case contentRect4:
                break;
            }
        }
    }
}
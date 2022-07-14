import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.Elements
import CustomComponents
import Librum.style
import Librum.icons

Item
{
    id: root
    property int  closedWidth: 72
    property int  openedWidth: 232
    property int  currentWidth: width
    property bool isOpened: false
    
    property alias freeBooksItem: freeBooksItem
    property alias homeItem: homeItem
    property alias statisticsItem: statisticsItem
    property alias addOnsItem: addOnButton
    property alias settingsItem: settingsItem
    
    property MSidebarItem defaultItem: homeItem
    property MSidebarItem currentItem: defaultItem
    
    implicitWidth: closedWidth
    implicitHeight: Window.height
    
    
    Shortcut
    {
        sequence: "Ctrl+O"
        onActivated: SidebarState.currentState === SidebarState.Opened ? root.closeSidebar() : root.openSidebar()
    }
    Shortcut
    {
        sequence: "Ctrl+1"
        onActivated: loadPage(freeBooksPage, root.freeBooksItem)
    }
    Shortcut
    {
        sequence: "Ctrl+2"
        onActivated: loadPage(homePage, root.homeItem)
    }
    Shortcut
    {
        sequence: "Ctrl+3"
        onActivated: loadPage(statisticsPage, root.statisticsItem)
    }
    Shortcut
    {
        sequence: "Ctrl+4"
        onActivated: loadPage(addOnsPage, root.addOnsItem)
    }
    Shortcut
    {
        sequence: "Ctrl+5"
        onActivated: loadPage(settings, root.settingsItem)
    }
    
    
    Pane
    {
        id: container
        anchors.fill: parent
        padding: 0
        background: Rectangle
        {
            color: Style.colorBackground
        }
        
        
        MSidebarAnimations
        {
            id: animations
        }
        
        
        MFlickWrapper
        {
            anchors.fill: parent
            contentHeight: mainLayout.implicitHeight
            
            
            ColumnLayout
            {
                id: mainLayout
                width: parent.width
                height: container.height
                spacing: 0
                
                RowLayout
                {
                    id: logo
                    Layout.topMargin: 14
                    Layout.leftMargin: 14
                    spacing: 20
                    
                    MLogo
                    {
                        id: logoIcon
                        Layout.preferredWidth: 44
                        Layout.preferredHeight: 44
                    }
                    
                    Label
                    {
                        id: logoLabel
                        visible: false
                        text: "Librum"
                        font.bold: true
                        font.family: Style.defaultFontFamily
                        font.pointSize: 16
                        color: Style.colorBaseTitle
                    }
                }
                
                Rectangle
                {
                    id: topSeparator
                    Layout.fillWidth: true
                    Layout.preferredHeight: 2
                    Layout.topMargin: 15
                    color: Style.colorSeparator
                }
                
                MSidebarItem
                {
                    id: freeBooksItem
                    Layout.topMargin: 16
                    Layout.leftMargin: 10
                    imageWidth: 31
                    imageSource: Icons.downloadGray
                    labelContent: "Free books"
                    
                    onClicked: loadPage(freeBooksPage, root.freeBooksItem)
                }            
                
                Rectangle
                {
                    id: smallSeperator1
                    Layout.preferredWidth: parent.width * 0.61
                    Layout.preferredHeight: 2
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 16
                    color: Style.colorSeparator
                }
                
                MSidebarItem
                {
                    id: homeItem
                    Layout.topMargin: 15
                    Layout.leftMargin: 10
                    imageWidth: 30
                    preserveImageFit: false
                    imageHeight: 28
                    imageSource: Icons.home
                    labelContent: "Home"
                    
                    onClicked: loadPage(homePage, root.homeItem)
                }
                
                MSidebarItem
                {
                    id: statisticsItem
                    Layout.topMargin: 13
                    Layout.leftMargin: 10
                    imageWidth: 28
                    imageSource: Icons.pieChart
                    labelContent: "Statistics"
                    
                    onClicked: loadPage(statisticsPage, root.statisticsItem)
                }
                
                Rectangle
                {
                    id: smallSeperator2
                    Layout.preferredWidth: parent.width * 0.61
                    Layout.preferredHeight: 2
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 15
                    color: Style.colorSeparator
                }
                
                MSidebarItem
                {
                    id: addOnButton
                    Layout.topMargin: 13
                    Layout.leftMargin: 10
                    imageWidth: 30
                    imageSource: Icons.addOns
                    labelContent: "Add-ons"
                    
                    onClicked: loadPage(addOnsPage, root.addOnsItem)
                }
                
                MSidebarItem
                {
                    id: settingsItem
                    Layout.topMargin: 13
                    Layout.leftMargin: 10
                    imageWidth: 36
                    imageSource: Icons.settings
                    labelContent: "Settings"
                    
                    onClicked: loadPage(settings, root.settingsItem)
                }
                
                Item
                {
                    Layout.fillHeight: true
                    Layout.minimumHeight: 30
                }
                
                Rectangle
                {
                    id: bottomSeparator
                    Layout.fillWidth: true
                    Layout.preferredHeight: 2
                    color: Style.colorSeparator
                }
                
                MProfileBox
                {
                    id: profileBox
                    Layout.alignment: Qt.AlignBottom
                    Layout.topMargin: 2
                    
                    onClicked:
                    {
                        if(profilePopup.opened)
                            profilePopup.close();
                        else
                            profilePopup.open();
                    }
                }
                
                
                MProfilePopup
                {
                    id: profilePopup
                    x: 12
                    y: profileBox.y - implicitHeight + 6
                }
            }
            
        }
    }
    
    function changeSelectedItem(newItem)
    {
        root.currentItem.selected = false;
        root.currentItem = newItem;
        root.currentItem.selected = true;
    }
    
    function openSidebar()
    {
        animations.openAnimation.start();
        currentItem.openAnimation.start();
    }
    
    function closeSidebar()
    {
        animations.closeAnimation.start();
        currentItem.closeAnimation.start();
    }
    
    function resetSidebar()
    {
        closeSidebar();
        changeSelectedItem(root.defaultItem);
    }
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }    
}
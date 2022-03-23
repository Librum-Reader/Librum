import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import librum.extensions.sidebar
import CustomComponents


FocusScope
{
    id: root
    property int  closedWidth : 72
    property int  openedWidth : 232
    property int  currentWidth : width
    property bool isOpened    : false
    property MSidebarItem defaultTab : homeButton
    property MSidebarItem selectedTab : homeButton
    
    implicitWidth: closedWidth
    implicitHeight: Window.height
    
    
    Rectangle
    {
        id: container
        anchors.fill: parent
        color: properties.colorBackground
        
        MSidebarAnimations
        {
            id: animations
        }
        
        ColumnLayout
        {
            id: mainLayout
            anchors.fill: parent            
            spacing: 0
            
            RowLayout
            {
                id: logo
                Layout.topMargin: 14
                Layout.leftMargin: 14
                spacing: 0
                
                MLogo
                {
                    id: logoIcon
                    Layout.preferredWidth: 44
                    Layout.preferredHeight: 44
                }
                
                Label
                {
                    id: logoLabel
                    Layout.leftMargin: 20
                    visible: false
                    text: "Librum"
                    font.bold: true
                    font.family: properties.defaultFontFamily
                    font.pointSize: 16
                    color: properties.colorBaseTitle
                }
            }
            
            Rectangle
            {
                id: topSeparator
                Layout.preferredWidth: root.width
                Layout.preferredHeight: 2
                Layout.topMargin: 15
                color: properties.colorSidebarSeparator
            }
            
            MSidebarItem
            {
                id: freeBooksButton
                Layout.topMargin: 16
                Layout.leftMargin: 10
                imageWidth: 45
                imageHeight: 45
                imageSource: properties.iconDownload
                labelContent: "Free books"
                selected: root.selectedTab == freeBooksButton
                
                onClicked: {
                    root.selectedTab = freeBooksButton;
                    if(true)
                        loadPage("FreeBooksPage");
                }
            }            
            
            Rectangle
            {
                id: smallSeperator1
                Layout.preferredWidth: parent.width * 0.61
                Layout.preferredHeight: 2
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 16
                color: properties.colorSidebarSeparator
            }
            
            MSidebarItem
            {
                id: homeButton
                Layout.topMargin: 15
                Layout.leftMargin: 10
                imageWidth: 30
                imageHeight: 28
                imageSource: properties.iconHome
                labelContent: "Home"
                selected: root.selectedTab == homeButton
                
                onClicked: {
                    root.selectedTab = homeButton;
                    if(true)
                        loadPage("HomePage");
                }
            }
            
            MSidebarItem
            {
                id: statisticsButton
                Layout.topMargin: 13
                Layout.leftMargin: 10
                imageWidth: 28
                imageHeight: 28
                imageSource: properties.iconPieChart
                labelContent: "Statistics"
                selected: root.selectedTab == statisticsButton
                
                onClicked: {
                    root.selectedTab = statisticsButton;
                    if(true)
                        loadPage("StatisticsPage");
                }
            }
            
            Rectangle
            {
                id: smallSeperator2
                Layout.preferredWidth: parent.width * 0.61
                Layout.preferredHeight: 2
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 15
                color: properties.colorSidebarSeparator
            }
            
            MSidebarItem
            {
                id: toolsButton
                Layout.topMargin: 15
                Layout.leftMargin: 10
                imageWidth: 30
                imageHeight: 30
                imageSource: properties.iconTool
                labelContent: "Tools"
                selected: root.selectedTab == toolsButton
                
                onClicked: {
                    root.selectedTab = toolsButton;
                    if(true)
                        loadPage("ToolsPage");
                }
            }
            
            MSidebarItem
            {
                id: addOnButton
                Layout.topMargin: 13
                Layout.leftMargin: 10
                imageWidth: 31
                imageHeight: 31
                imageSource: properties.iconAddOns
                labelContent: "Add-ons"
                selected: root.selectedTab == addOnButton
                
                onClicked: {
                    root.selectedTab = addOnButton;
                    if(true)
                        loadPage("AddOnsPage");
                }
            }
            
            MSidebarItem
            {
                id: settingsButton
                Layout.topMargin: 13
                Layout.leftMargin: 10
                imageWidth: 36
                imageHeight: 36
                imageSource: properties.iconSettings
                labelContent: "Settings"
                selected: root.selectedTab == settingsButton
                
                onClicked: {
                    root.selectedTab = settingsButton;
                    if(true)
                        loadPage("SettingsPage");
                }
            }
            
            Item
            {
                Layout.fillHeight: true
                Layout.minimumHeight: 30
            }
            
            Rectangle
            {
                id: bottomSeparator
                Layout.preferredWidth: root.width
                Layout.preferredHeight: 2
                color: properties.colorSidebarSeparator
            }
            
            MProfilePopup
            {
                id: profilePopup
                x: 12
                y: root.height - 54 - implicitHeight
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
        }
    }
    
    function openSidebar()
    {
        animations.openAnimation.start();
        selectedTab.openAnimation.start();
        SidebarState.currentState = SidebarState.Opened;
    }
    
    function closeSidebar()
    {
        animations.closeAnimation.start();
        selectedTab.closeAnimation.start();
        SidebarState.currentState = SidebarState.Closed;
    }
    
    function resetSidebar()
    {
        closeSidebar();
        root.selectedTab = root.defaultTab;
    }
}
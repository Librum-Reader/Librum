import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0


Pane
{
    id: root
    property bool fullScreenMode: false
    property string bookTitle: "Unknown name"
    property int pageWidth: 0
    property int currentPage: 0
    property alias chapterButton: chapterButton
    property alias bookmarksButton: bookmarksButton
    property alias searchButton: searchButton
    property alias currentPageButton: currentPageButton
    property alias fullScreenButton: fullScreenButton
    property alias optionsButton: optionsButton
    signal chapterButtonClicked
    signal bookMarkButtonClicked
    signal searchButtonClicked
    signal currentPageButtonClicked
    signal fullScreenButtonClicked
    signal optionsPopupVisibileChanged
    
    implicitHeight: 48
    padding: 8
    background: Rectangle
    {
        color: Style.colorLightGray2
        radius: 4
    }
    
    onVisibleChanged: if(optionsPopup.opened) optionsPopup.close()
    
    
    Shortcut
    {
        sequence: Qt.Key_Tab
        onActivated: chapterButton.buttonTriggeredAction()
    }
    Shortcut
    {
        sequence: "Ctrl+B"
        onActivated: bookmarksButton.buttonTriggeredAction()
    }
    Shortcut
    {
        sequence: "Ctrl+F"
        onActivated: searchButton.buttonTriggeredAction()
    }
    Shortcut
    {
        sequences: ["Ctrl+Alt+F"]
        onActivated: fullScreenButton.buttonTriggeredAction()
    }
    Shortcut
    {
        sequence: "Ctrl+Alt+O"
        onActivated: optionsButton.buttonTriggeredAction()
    }
    
    
    RowLayout
    {
        id: mainLayout
        anchors.fill: parent
        spacing: 8
        
        
        MButton
        {
            id: backButton
            Layout.preferredWidth: 40
            Layout.preferredHeight: 32
            backgroundColor: Style.colorBaseGray
            borderWidth: 0
            imagePath: Icons.arrowtopBlackLeft
            imageSize: 11
            opacityOnPressed: 0.7
            
            onClicked: buttonTriggeredAction()
            
            
            function buttonTriggeredAction()
            {
                loadPage(homePage, sidebar.homeItem, false);
            }
        }
        
        MButton
        {
            id: chapterButton
            Layout.preferredWidth: 40
            Layout.preferredHeight: 32
            backgroundColor: Style.colorBaseGray
            borderWidth: 0
            imagePath: active ? Icons.readingViewSidebarPurple : Icons.readingViewSidebarBlack
            imageSize: 18
            opacityOnPressed: 0.7
            
            onClicked: buttonTriggeredAction()
            
            
            function buttonTriggeredAction()
            {
                root.chapterButtonClicked();
            }
        }
        
        MButton
        {
            id: bookmarksButton
            Layout.preferredWidth: 40
            Layout.preferredHeight: 32
            backgroundColor: Style.colorBaseGray
            borderWidth: 0
            imagePath: active ? Icons.readingViewBookmarkPurple : Icons.readingViewBookmarkBlack
            imageSize: 14
            opacityOnPressed: 0.7
            
            onClicked: buttonTriggeredAction()
            
            
            function buttonTriggeredAction()
            {
                root.bookMarkButtonClicked();
            }
        }
        
        MButton
        {
            id: currentPageButton
            property int maxPages
            
            Layout.preferredWidth: 105
            Layout.preferredHeight: 32
            backgroundColor: Style.colorBaseGray
            borderWidth: 0
            text: "(" + (root.currentPage + 1).toString() + " of " + maxPages.toString() + ")"
            fontColor: active ? Style.colorBasePurple : Style.colorBaseText
            fontWeight: Font.Medium
            fontSize: 10.5
            opacityOnPressed: 0.7
            
            onClicked: buttonTriggeredAction()
            
            
            function buttonTriggeredAction()
            {
                root.currentPageButtonClicked();
            }
        }
        
        Label
        {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
            // To be positioned at the horizontal the center
            property int paddingToCenter: - backButton.width - chapterButton.width - bookmarksButton.width 
                                          - currentPageButton.width - mainLayout.spacing*5 + Window.width / 2 - contentWidth / 2
            leftPadding: paddingToCenter
            text: root.bookTitle
            color: Style.colorBaseTitle
            font.weight: Font.DemiBold
            font.pointSize: 12
            elide: Text.ElideRight
        }
        
        
        MComboBox
        {
            id: zoomComboBox
            Layout.preferredHeight: 32
            Layout.preferredWidth: 92
            headerText: ""
            titleFontColor: Style.colorBaseTitle
            boxBackgroundColor: Style.colorBaseGray
            defaultIndex: 6
            image: Icons.dropdownBlack
            imageSize: 9
            
            checkBoxStyle: false
            model: ListModel
            {
                ListElement { text: "12%" }
                ListElement { text: "25%" }
                ListElement { text: "33%" }
                ListElement { text: "50%" }
                ListElement { text: "66%" }
                ListElement { text: "75%" }
                ListElement { text: "100%" }
                ListElement { text: "125%" }
                ListElement { text: "150%" }
                ListElement { text: "175%" }
                ListElement { text: "200%" }
                ListElement { text: "400%" }
                ListElement { text: "800%" }
                ListElement { text: "1000%" }
                ListElement { text: "2000%" }
                ListElement { text: "4000%" }
            }
        }
        
        MButton
        {
            id: fullScreenButton
            Layout.preferredWidth: 40
            Layout.preferredHeight: 32
            backgroundColor: Style.colorBaseGray
            borderWidth: 0
            imagePath: active ? Icons.readingViewMaximizePurple : Icons.readingViewMaximizeBlack
            
            imageSize: 20
            opacityOnPressed: 0.7
            
            onClicked: buttonTriggeredAction()
            
            
            function buttonTriggeredAction()
            {
                root.fullScreenButtonClicked();
            }
        }
        
        MButton
        {
            id: searchButton
            Layout.preferredWidth: 40
            Layout.preferredHeight: 32
            backgroundColor: Style.colorBaseGray
            borderWidth: 0
            imagePath: active ? Icons.readingViewSearchPurpleThin : Icons.readingViewSearchBlackThin
            imageSize: 18
            opacityOnPressed: 0.7
            
            onClicked: buttonTriggeredAction()
            
            
            function buttonTriggeredAction()
            {
                root.searchButtonClicked();
            }
        }
        
        MButton
        {
            id: optionsButton
            Layout.preferredWidth: 40
            Layout.preferredHeight: 32
            backgroundColor: Style.colorBaseGray
            borderWidth: 0
            imagePath: active ? Icons.readingViewMenuPurple : Icons.readingViewMenuBlack
            imageSize: 20
            opacityOnPressed: 0.7
            
            onClicked: buttonTriggeredAction()
            
            
            function buttonTriggeredAction()
            {
                // @disable-check M127
                optionsPopup.opened ? optionsPopup.close() : optionsPopup.open();
            }
        }
    }
    
    MReadingOptionsPopup
    {
        id: optionsPopup
        x: optionsButton.x - width + optionsButton.width
        y: optionsButton.height + 12
        
        onOpenedChanged: root.optionsPopupVisibileChanged()
    }
}
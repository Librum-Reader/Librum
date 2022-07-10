import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons


Pane
{
    id: root
    property bool fullScreenMode: false
    property alias chapterButton: chapterButton
    property alias bookmarkButton: bookmarkButton
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
        sequence: "Ctrl+Backspace"
        onActivated: backButton.buttonTriggeredAction()
    }
    Shortcut
    {
        sequence: "Tab"
        onActivated: chapterButton.buttonTriggeredAction()
    }
    Shortcut
    {
        sequence: "Ctrl+B"
        onActivated: bookmarkButton.buttonTriggeredAction()
    }
    Shortcut
    {
        sequence: "Ctrl+F"
        onActivated: searchButton.buttonTriggeredAction()
    }
    Shortcut
    {
        sequence: "Ctrl+Alt+F"
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
            id: bookmarkButton
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
            Layout.preferredWidth: 105
            Layout.preferredHeight: 32
            backgroundColor: Style.colorBaseGray
            borderWidth: 0
            text: "(312 of 1486)"
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
        
        
        Item { Layout.fillWidth: true }
        
        Label
        {
            Layout.alignment: Qt.AlignVCenter
            text: "C-Sharp_In_Depth: A Practical Guide"
            color: Style.colorBaseTitle
            font
            {
                family: Style.defaultFontFamily
                weight: Font.DemiBold
                pointSize: 12
            }
        }
        
        Item { Layout.fillWidth: true }
        
        
        MComboBox
        {
            id: zoomComboBox
            Layout.preferredHeight: 32
            Layout.preferredWidth: 92
            headerText: ""
            titleFontColor: Style.colorBaseTitle
            boxBackgroundColor: Style.colorBaseGray
            selectionPopup.highlightColor: Style.colorLightGray3
            selectionPopup.defaultIndex: 6
            imagePath: Icons.dropdownBlack
            imageSize: 9
            
            selectionPopup.checkBoxStyle: false
            selectionPopup.model: ListModel
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
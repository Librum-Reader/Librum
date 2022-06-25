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
            
            onClicked: loadPage("HomePage")
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
            
            onClicked: root.chapterButtonClicked()
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
            
            onClicked: root.bookMarkButtonClicked()
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
            
            onClicked: root.currentPageButtonClicked()
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
            Layout.preferredWidth: 84
            headerText: ""
            backgroundColor: Style.colorBaseGray
            highlightColor: Style.colorLightGray3
            defaultIndex: 6
            imagePath: Icons.dropdownBlack
            imageSize: 9
            maxPopupHeight: 208
            
            listContent: listModel
            
            ListModel
            {
                id: listModel
                ListElement { content: "12%" }
                ListElement { content: "25%" }
                ListElement { content: "33%" }
                ListElement { content: "50%" }
                ListElement { content: "66%" }
                ListElement { content: "75%" }
                ListElement { content: "100%" }
                ListElement { content: "125%" }
                ListElement { content: "150%" }
                ListElement { content: "175%" }
                ListElement { content: "200%" }
                ListElement { content: "400%" }
                ListElement { content: "800%" }
                ListElement { content: "1000%" }
                ListElement { content: "2000%" }
                ListElement { content: "4000%" }
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
            
            onClicked: root.fullScreenButtonClicked()
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
            
            onClicked: root.searchButtonClicked()
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
            
            onClicked:
            {
                optionsPopup.opened ? optionsPopup.close() : optionsPopup.open();
            }
        }
    }
    
    MOptionsPopup
    {
        id: optionsPopup
        x: optionsButton.x - width + optionsButton.width
        y: optionsButton.height + 12
        
        onOpenedChanged: root.optionsPopupVisibileChanged()
    }
}
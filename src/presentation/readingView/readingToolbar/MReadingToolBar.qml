import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import CustomComponents


Pane
{
    id: root
    implicitHeight: 48
    padding: 8
    background: Rectangle
    {
        color: Style.colorLightGray2
        radius: 4
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
            imagePath: properties.iconArrowBlackHallowThin
            imageSize: 11
            opacityOnPressed: 0.7
            
            onClicked: loadPage("HomePage")
        }
        
        MButton
        {
            id: sidebarButton
            Layout.preferredWidth: 40
            Layout.preferredHeight: 32
            backgroundColor: Style.colorBaseGray
            borderWidth: 0
            imagePath: properties.iconReadingViewSidebarBlack
            imageSize: 18
            opacityOnPressed: 0.7
        }
        
        MButton
        {
            id: bookmarkButton
            Layout.preferredWidth: 40
            Layout.preferredHeight: 32
            backgroundColor: Style.colorBaseGray
            borderWidth: 0
            imagePath: properties.iconReadingViewBookmarkBlack
            imageSize: 14
            opacityOnPressed: 0.7
        }
        
        MButton
        {
            id: currentPageButton
            Layout.preferredWidth: 105
            Layout.preferredHeight: 32
            backgroundColor: Style.colorBaseGray
            borderWidth: 0
            text: "(312 of 1486)"
            fontWeight: Font.Medium
            fontSize: 10.5
            opacityOnPressed: 0.7
        }
        
        
        Item { Layout.fillWidth: true }
        
        Label
        {
            Layout.alignment: Qt.AlignVCenter
            text: "C-Sharp_In_Depth: A Practical Guide - Librum"
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
            id: languagesComboBox
            Layout.preferredHeight: 32
            Layout.preferredWidth: 84
            headerText: ""
            backgroundColor: Style.colorBaseGray
            highlightColor: Style.colorLightGray3
            defaultIndex: 6
            imagePath: properties.iconDropdownBlack
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
            imagePath: properties.iconReadingViewMaximizeBlack
            imageSize: 20
            opacityOnPressed: 0.7
        }
        
        MButton
        {
            id: searchButton
            Layout.preferredWidth: 40
            Layout.preferredHeight: 32
            backgroundColor: Style.colorBaseGray
            borderWidth: 0
            imagePath: properties.iconReadingViewSearchBlackThin
            imageSize: 18
            opacityOnPressed: 0.7
        }
        
        MButton
        {
            id: optionsButton
            Layout.preferredWidth: 40
            Layout.preferredHeight: 32
            backgroundColor: Style.colorBaseGray
            borderWidth: 0
            imagePath: properties.iconReadingViewMenuBlack
            imageSize: 20
            opacityOnPressed: 0.7
        }
    }
}
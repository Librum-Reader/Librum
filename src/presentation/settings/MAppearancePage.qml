import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons


Page
{
    id: root
    horizontalPadding: 48
    bottomPadding: 22
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.pagesBackground
    }
    
    
    ColumnLayout    
    {
        id: layout
        property int insideMargin : 40
        
        width: parent.width
        spacing: 26
        
        RowLayout
        {
            id: titleRow
            Layout.fillWidth: true
            spacing: 0
            
            
            MTitle
            {
                id: title
                Layout.topMargin: 64
                titleText: "Appearance"
                descriptionText: "Make your own experience"
                titleSize: 25
                descriptionSize: 13.25
            }
            
            Item { Layout.fillWidth: true }
            
            MButton
            {
                id: saveButton
                Layout.preferredWidth: 90
                Layout.preferredHeight: 38
                Layout.alignment: Qt.AlignBottom
                borderWidth: 0
                backgroundColor: Style.colorBasePurple
                text: "Save"
                fontSize: 12
                fontWeight: Font.Bold
                fontColor: Style.colorBackground
                imagePath: Icons.checkWhite
                imageSize: 14
                
                onClicked: root.saveAppearanceSettings()
            }
        }
        
        Pane
        {
            id: details
            Layout.fillWidth: true
            topPadding: 30
            horizontalPadding: layout.insideMargin
            bottomPadding: 21
            background: Rectangle
            {
                color: Style.colorBackground
                border.color: Style.colorLightBorder
                radius: 4
                antialiasing: true
            }
            
            
            ColumnLayout
            {
                width: parent.width
                spacing: 0
                
                
                Label
                {
                    id: displayTitle
                    Layout.fillWidth: true
                    text: "Display"
                    font.family: Style.defaultFontFamily
                    font.pointSize: 18
                    font.weight: Font.Bold
                    color: Style.colorBaseText
                }
                
                
                Label
                {
                    id: themeTitle
                    Layout.fillWidth: true
                    Layout.topMargin: 16
                    text: "Theme"
                    font.family: Style.defaultFontFamily
                    font.pointSize: 13
                    font.weight: Font.DemiBold
                    color: Style.colorBaseText
                }
                
                MDualToggle
                {
                    id: themeSwitch
                    Layout.topMargin: 6
                    leftProperty: "Dark"
                    rightProperty: "Light"
                    selected: rightProperty
                }
                
                Label
                {
                    id: fontFamilyTitle
                    Layout.fillWidth: true
                    Layout.topMargin: 14
                    text: "Font family"
                    font.family: Style.defaultFontFamily
                    font.pointSize: 13
                    font.weight: Font.DemiBold
                    color: Style.colorBaseText
                }
                
                MComboBox
                {
                    id: fontFamilyComboBox
                    Layout.topMargin: 6
                    Layout.preferredHeight: 40
                    Layout.fillWidth: true
                    Layout.maximumWidth: 380
                    titleEmptyText: "None"
                    titleFontSize: 12
                    titleSpacing: 4
                    imagePath: Icons.dropdownGray
                    imageSize: 9
                    selectionPopup.itemHeight: 36
                    selectionPopup.fontSize: 12
                    selectionPopup.checkBoxStyle: false
                    selectionPopup.maxHeight: 200
                    selectionPopup.defaultIndex: 0
                    selectionPopup.model: ListModel
                    {
                        ListElement { text: "Arial" }
                        ListElement { text: "Helvetica" }
                        ListElement { text: "Baskerville" }
                        ListElement { text: "Akzidenz Grotesk" }
                        ListElement { text: "Gotham" }
                        ListElement { text: "Bodoni" }
                        ListElement { text: "Didot" }
                        ListElement { text: "Museo Slab" }
                    }
                }
                
                Label
                {
                    id: fontSizeTitle
                    Layout.fillWidth: true
                    Layout.topMargin: 16
                    text: "Font size"
                    font.family: Style.defaultFontFamily
                    font.pointSize: 13
                    font.weight: Font.DemiBold
                    color: Style.colorBaseText
                }
                
                MSpinbox
                {
                    id: fontSizeSpinBox
                    value: 18
                    Layout.preferredWidth: 76
                    Layout.preferredHeight: 34
                    Layout.topMargin: 6
                }
                
                
                Label
                {
                    id: pagesTitle
                    Layout.fillWidth: true
                    Layout.topMargin: 36
                    text: "Pages"
                    font.family: Style.defaultFontFamily
                    font.pointSize: 18
                    font.weight: Font.Bold
                    color: Style.colorBaseText
                }
                
                Label
                {
                    id: pageSpacingTitle
                    Layout.fillWidth: true
                    Layout.topMargin: 14
                    text: "Page spacing"
                    font.family: Style.defaultFontFamily
                    font.pointSize: 13
                    font.weight: Font.DemiBold
                    color: Style.colorBaseText
                }
                
                MSpinbox
                {
                    id: pageSpacingSpingBox
                    value: 20
                    Layout.preferredWidth: 76
                    Layout.preferredHeight: 34
                    Layout.topMargin: 6
                }
            }
        }
    }
    
    function saveAppearanceSettings()
    {
        
    }
}
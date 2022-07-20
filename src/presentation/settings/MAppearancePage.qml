import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons


MFlickWrapper
{
    id: root
    contentHeight: page.implicitHeight
    flickDeceleration: 3500
    
    
    Page
    {
        id: page
        width: parent.width
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
            spacing: 0
            
            
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
                    id: resetButton
                    Layout.preferredWidth: 82
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignBottom
                    borderWidth: 0
                    backgroundColor: Style.colorBasePurple
                    text: "Reset"
                    fontSize: 12
                    fontWeight: Font.Bold
                    fontColor: Style.colorBackground
                    
                    onClicked: root.resetAppearanceSettings()
                }
            }
            
            Pane
            {
                id: displayBlock
                Layout.fillWidth: true
                Layout.topMargin: 32
                verticalPadding: 24
                horizontalPadding: layout.insideMargin
                background: Rectangle
                {
                    color: Style.colorBackground
                    border.color: Style.colorLightBorder
                    radius: 4
                    antialiasing: true
                }
                
                
                ColumnLayout
                {
                    id: displayColumn
                    anchors.fill: parent
                    spacing: 0
                    
                    Label
                    {
                        id: displayTitle
                        Layout.fillWidth: true
                        text: "Display"
                        font.family: Style.defaultFontFamily
                        font.pointSize: 16.5
                        font.weight: Font.DemiBold
                        color: Style.colorBaseText
                    }
                    
                    
                    Label
                    {
                        id: themeTitle
                        Layout.fillWidth: true
                        Layout.topMargin: 24
                        text: "Theme"
                        font.family: Style.defaultFontFamily
                        font.pointSize: 13
                        font.weight: Font.DemiBold
                        color: Style.colorBaseText
                    }
                    
                    
                    MDualToggle
                    {
                        id: themeSwitch
                        Layout.topMargin: 4
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
                        Layout.topMargin: 4
                        Layout.preferredHeight: 36
                        Layout.fillWidth: true
                        Layout.maximumWidth: 380
                        titleEmptyText: "None"
                        titleFontSize: 12
                        titleSpacing: 4
                        imagePath: Icons.dropdownGray
                        imageSize: 9
                        selectionPopup.itemHeight: 32
                        selectionPopup.fontSize: 11.5
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
                        Layout.preferredWidth: 76
                        Layout.topMargin: 4
                        value: 18
                    }
                }   
            }
            
            Pane
            {
                id: pagesBlock
                Layout.fillWidth: true
                Layout.topMargin: 24
                verticalPadding: 24
                horizontalPadding: layout.insideMargin
                background: Rectangle
                {
                    color: Style.colorBackground
                    border.color: Style.colorLightBorder
                    radius: 4
                    antialiasing: true
                }
                
                
                ColumnLayout
                {
                    id: pagesColumn
                    anchors.fill: parent
                    spacing: 0
                    
                    
                    Label
                    {
                        id: pagesTitle
                        Layout.fillWidth: true
                        text: "Pages"
                        font.family: Style.defaultFontFamily
                        font.pointSize: 16.5
                        font.weight: Font.DemiBold
                        color: Style.colorBaseText
                    }
                    
                    Label
                    {
                        id: pageSpacingTitle
                        Layout.fillWidth: true
                        Layout.topMargin: 24
                        text: "Page spacing"
                        font.family: Style.defaultFontFamily
                        font.pointSize: 13
                        font.weight: Font.DemiBold
                        color: Style.colorBaseText
                    }
                    
                    MSpinbox
                    {
                        id: pageSpacingSpingBox
                        Layout.preferredWidth: 76
                        Layout.topMargin: 4
                        value: 20
                    }
                    
                    Label
                    {
                        id: displayModeTitle
                        Layout.fillWidth: true
                        Layout.topMargin: 16
                        text: "Display mode"
                        font.family: Style.defaultFontFamily
                        font.pointSize: 13
                        font.weight: Font.DemiBold
                        color: Style.colorBaseText
                    }
                    
                    MRadioButtonSelector
                    {
                        id: displayModeSelector
                        Layout.fillWidth: true
                        Layout.topMargin: 8
                        options: ["Single Page", "Double Page"]
                        currentSelected: options[0]
                    }
                    
                    Label
                    {
                        id: layoutDirectionTitle
                        Layout.fillWidth: true
                        Layout.topMargin: 16
                        text: "Layout direction"
                        font.family: Style.defaultFontFamily
                        font.pointSize: 13
                        font.weight: Font.DemiBold
                        color: Style.colorBaseText
                    }
                    
                    MRadioButtonSelector
                    {
                        id: layoutDirectionSelector
                        Layout.fillWidth: true
                        Layout.topMargin: 8
                        options: ["Vertical", "Horizontal"]
                        currentSelected: options[0]
                    }
                    
                    Label
                    {
                        id: pageTransitionTitle
                        Layout.fillWidth: true
                        Layout.topMargin: 16
                        text: "Page transition"
                        font.family: Style.defaultFontFamily
                        font.pointSize: 13
                        font.weight: Font.DemiBold
                        color: Style.colorBaseText
                    }
                    
                    MComboBox
                    {
                        id: pageTransitionComboBox
                        Layout.topMargin: 4
                        Layout.preferredHeight: 36
                        Layout.fillWidth: true
                        Layout.maximumWidth: 380
                        titleFontSize: 12
                        titleSpacing: 4
                        imagePath: Icons.dropdownGray
                        imageSize: 9
                        selectionPopup.defaultIndex: 0
                        selectionPopup.itemHeight: 32
                        selectionPopup.fontSize: 12
                        selectionPopup.checkBoxStyle: false
                        selectionPopup.maxHeight: 200
                        selectionPopup.model: ListModel
                        {
                            ListElement { text: "Instant" }
                            ListElement { text: "Fading" }
                            ListElement { text: "Swipe" }
                            ListElement { text: "Swap" }
                        }
                    }
                }
            }
            
            Pane
            {
                id: behaviorBlock
                Layout.fillWidth: true
                Layout.topMargin: 24
                verticalPadding: 24
                horizontalPadding: layout.insideMargin
                background: Rectangle
                {
                    color: Style.colorBackground
                    border.color: Style.colorLightBorder
                    radius: 4
                    antialiasing: true
                }
                
                
                ColumnLayout
                {
                    id: behaviorColumn
                    anchors.fill: parent
                    spacing: 0
                    
                    Label
                    {
                        id: behaviorTitle
                        Layout.fillWidth: true
                        text: "Behavior"
                        font.family: Style.defaultFontFamily
                        font.pointSize: 16.5
                        font.weight: Font.DemiBold
                        color: Style.colorBaseText
                    }
                    
                    
                    Label
                    {
                        id: smoothScrollingTitle
                        Layout.fillWidth: true
                        Layout.topMargin: 24
                        text: "Smooth scrolling"
                        font.family: Style.defaultFontFamily
                        font.pointSize: 13
                        font.weight: Font.DemiBold
                        color: Style.colorBaseText
                    }
                    
                    
                    MDualToggle
                    {
                        id: smoothScrollingSwitch
                        Layout.topMargin: 4
                        leftProperty: "OFF"
                        rightProperty: "ON"
                        selected: leftProperty
                    }
                    
                    Label
                    {
                        id: cursorModeTitle
                        Layout.fillWidth: true
                        Layout.topMargin: 16
                        text: "Cursor mode"
                        font.family: Style.defaultFontFamily
                        font.pointSize: 13
                        font.weight: Font.DemiBold
                        color: Style.colorBaseText
                    }
                    
                    MRadioButtonSelector
                    {
                        id: cursorModeSelector
                        Layout.fillWidth: true
                        Layout.topMargin: 8
                        options: ["Hidden after delay", "Always visible", "Never visible"]
                        currentSelected: options[0]
                    }
                    
                    Label
                    {
                        id: defaultZoomTitle
                        Layout.fillWidth: true
                        Layout.topMargin: 16
                        text: "Default zoom"
                        font.family: Style.defaultFontFamily
                        font.pointSize: 13
                        font.weight: Font.DemiBold
                        color: Style.colorBaseText
                    }
                    
                    MSpinbox
                    {
                        id: defaultZoomBox
                        Layout.preferredWidth: 92
                        Layout.topMargin: 4
                        value: 100
                        minVal: 10
                        maxVal: 4000
                    }
                }
            }
        }
    }
    
    
    function resetAppearanceSettings()
    {
    }
}
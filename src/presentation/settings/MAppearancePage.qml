import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.controllers 1.0



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
        property int insideMargin: 40
        
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
                
                onClicked: resetSettingsPopup.open()
            }
        }
        
        
        MFlickWrapper
        {
            id: flickWrapper
            property int scrollbarOffset: 22
            
            Layout.fillWidth: true
            Layout.preferredHeight: root.height - titleRow.implicitHeight - root.bottomPadding * 2
            contentHeight: displayLayout.implicitHeight
            Layout.topMargin: 32
            Layout.rightMargin: -scrollbarOffset
            maximumFlickVelocity: 1000
            flickDeceleration: 4000
            clip: true
            
            ScrollBar.vertical: ScrollBar { width: 10; policy: "AlwaysOn" }
            
            
            ColumnLayout
            {
                id: displayLayout
                width: parent.width
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: flickWrapper.scrollbarOffset
                spacing: 0
                
                
                Pane
                {
                    id: displayBlock
                    Layout.fillWidth: true
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
                            font.pointSize: 17
                            font.weight: Font.DemiBold
                            color: Style.colorBaseText
                        }
                        
                        Label
                        {
                            id: themeTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 24
                            text: "Theme"
                            font.pointSize: 13
                            font.weight: Font.DemiBold
                            color: Style.colorBaseText
                        }
                        
                        MDualToggle
                        {
                            id: themeSwitch
                            property string savedSetting: layout.getSavedSetting("theme")
                            
                            Layout.topMargin: 4
                            leftProperty: "Dark"
                            rightProperty: "Light"
                            leftSelected: savedSetting == "dark"
                            rightSelected: savedSetting == "light"
                            
                            onSelectedChanged: (newSelected) => layout.saveSetting("theme", newSelected)
                        }
                    }   
                }
                
                Pane
                {
                    id: readingBlock
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
                        id: readingColumn
                        anchors.fill: parent
                        spacing: 0
                        
                        
                        Label
                        {
                            id: readingTitle
                            Layout.fillWidth: true
                            text: "Reading"
                            font.pointSize: 17
                            font.weight: Font.DemiBold
                            color: Style.colorBaseText
                        }
                        
                        Label
                        {
                            id: pageSpacingTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 24
                            text: "Page spacing"
                            font.pointSize: 13
                            font.weight: Font.DemiBold
                            color: Style.colorBaseText
                        }
                        
                        MSpinbox
                        {
                            id: pageSpacingSpinBox
                            Layout.preferredWidth: 76
                            Layout.topMargin: 4
                            value: parseInt(layout.getSavedSetting("page-spacing"))
                            
                            onNewValueSelected: layout.saveSetting("page-spacing", value)
                        }
                        
                        Label
                        {
                            id: docTitleDisplayTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: "Display book title in titlebar"
                            font.pointSize: 13
                            font.weight: Font.DemiBold
                            color: Style.colorBaseText
                        }
                        
                        MDualToggle
                        {
                            id: docTitleDisplaySwitch
                            property string savedValue: layout.getSavedSetting("display-book-title-in-titlebar")
                            
                            Layout.topMargin: 4
                            leftProperty: "OFF"
                            rightProperty: "ON"
                            rightSelected: savedValue == "true"
                            leftSelected: savedValue == "false"
                            
                            onSelectedChanged:
                            {
                                let status = true;
                                if(docTitleDisplaySwitch.leftSelected) // "OFF"
                                {
                                    status = false;
                                }
                                
                                layout.saveSetting("display-book-title-in-titlebar", status.toString())
                            }
                        }
                        
                        Label
                        {
                            id: layoutDirectionTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: "Layout direction"
                            font.pointSize: 13
                            font.weight: Font.DemiBold
                            color: Style.colorBaseText
                        }
                        
                        MRadioButtonSelector
                        {
                            id: layoutDirectionSelector
                            property string savedValue: layout.getSavedSetting("layout-direction")
                            
                            Layout.fillWidth: true
                            Layout.topMargin: 6
                            options: ["Vertical", "Horizontal"]
                            currentSelected: savedValue == "vertical" ? options[0] : options[1]
                            
                            onNewCurrentSelected: layout.saveSetting("layout-direction", currentSelected)
                        }
                        
                        Label
                        {
                            id: displayModeTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: "Display mode"
                            font.pointSize: 13
                            font.weight: Font.DemiBold
                            color: Style.colorBaseText
                        }
                        
                        MRadioButtonSelector
                        {
                            id: displayModeSelector
                            property string savedValue: layout.getSavedSetting("display-mode")
                            
                            Layout.fillWidth: true
                            Layout.topMargin: 6
                            options: ["Single Page", "Double Page"]
                            currentSelected: savedValue == "single page" ? options[0] : options[1]
                            
                            onNewCurrentSelected: layout.saveSetting("display-mode", currentSelected)
                        }
                        
                        Label
                        {
                            id: pageTransitionTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: "Page transition"
                            font.pointSize: 13
                            font.weight: Font.DemiBold
                            color: Style.colorBaseText
                        }
                        
                        MComboBox
                        {
                            id: pageTransitionComboBox
                            property string savedValue: layout.getSavedSetting("page-transition")
                            
                            defaultIndex:
                            {
                                if(savedValue == "instant")
                                    return 0;
                                else if(savedValue == "swipe")
                                    return 1;
                                else if(savedValue == "fading")
                                    return 2;
                                else
                                    return 3;
                            }
                            
                            Layout.topMargin: 4
                            Layout.preferredHeight: 36
                            Layout.fillWidth: true
                            Layout.maximumWidth: 380
                            titleFontSize: 12
                            titleSpacing: 4
                            image: Icons.dropdownGray
                            imageSize: 9
                            itemHeight: 32
                            fontSize: 12
                            checkBoxStyle: false
                            maxHeight: 200
                            model: ListModel
                            {
                                ListElement { text: "Instant" }
                                ListElement { text: "Fading" }
                                ListElement { text: "Swipe" }
                                ListElement { text: "Swap" }
                            }
                            
                            onItemChanged: layout.saveSetting("page-transition", text)
                        }
                        
                        Label
                        {
                            id: defaultZoomTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: "Default Zoom"
                            font.pointSize: 13
                            font.weight: Font.DemiBold
                            color: Style.colorBaseText
                        }
                        
                        MSpinbox
                        {
                            id: defaultZoomSpinBox
                            Layout.preferredWidth: 76
                            Layout.topMargin: 4
                            value: layout.getSavedSetting("default-zoom")
                            
                            onNewValueSelected: layout.saveSetting("default-zoom", value)
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
                            font.pointSize: 17
                            font.weight: Font.DemiBold
                            color: Style.colorBaseText
                        }
                        
                        
                        Label
                        {
                            id: smoothScrollingTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 24
                            text: "Smooth scrolling"
                            font.pointSize: 13
                            font.weight: Font.DemiBold
                            color: Style.colorBaseText
                        }
                        
                        
                        MDualToggle
                        {
                            id: smoothScrollingSwitch
                            property string savedValue: layout.getSavedSetting("smooth-scrolling")
                            Layout.topMargin: 4
                            leftProperty: "OFF"
                            rightProperty: "ON"
                            leftSelected: savedValue == "false"
                            rightSelected: savedValue == "true"
                            
                            onSelectedChanged:
                            {
                                let status = true;
                                if(leftSelected) // "OFF"
                                {
                                    status = false;
                                }
                                
                                layout.saveSetting("smooth-scrolling", status.toString())
                            }
                        }
                        
                        Label
                        {
                            id: loopAfterLastTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: "Loop after last page"
                            font.pointSize: 13
                            font.weight: Font.DemiBold
                            color: Style.colorBaseText
                        }
                        
                        
                        MDualToggle
                        {
                            id: loopAfterLastSwitch
                            property string savedValue: layout.getSavedSetting("loop-after-last-page")
                            
                            Layout.topMargin: 4
                            leftProperty: "OFF"
                            rightProperty: "ON"
                            leftSelected: true
                            rightSelected: false
                            
                            onSelectedChanged:
                            {
                                let status = true;
                                if(leftSelected) // "OFF"
                                {
                                    status = false;
                                }
                                
                                layout.saveSetting("loop-after-last-page", status.toString())
                            }
                        }
                        
                        Label
                        {
                            id: cursorModeTitle
                            Layout.fillWidth: true
                            Layout.topMargin: 18
                            text: "Cursor mode"
                            font.pointSize: 13
                            font.weight: Font.DemiBold
                            color: Style.colorBaseText
                        }
                        
                        MRadioButtonSelector
                        {
                            id: cursorModeSelector
                            property string savedValue: layout.getSavedSetting("cursor-mode")
                            
                            Layout.fillWidth: true
                            Layout.topMargin: 6
                            options: ["Hidden after delay", "Always visible"]
                            currentSelected: savedValue == "hidden after delay" ? options[0] : options[1]
                                                        
                            onNewCurrentSelected: layout.saveSetting("cursor-mode", currentSelected)
                        }
                    }
                }
            }
        }
        
        // comfortability methods => eases the syntax
        function saveSetting(name, value)
        {
            console.log("Setting: " + name + " to: " + value);
            SettingsController.setSetting(name, value, "Appearance");
        }
        
        function getSavedSetting(name)
        {
            return SettingsController.getSetting(name, "Appearance");
        }
    }
    
    
    MResetSettingsPopup
    {
        id: resetSettingsPopup
        x: Math.round(root.width / 2 - implicitWidth / 2 - settingsSidebar.width / 2 - root.horizontalPadding)
        y: Math.round(root.height / 2 - implicitHeight / 2 - root.topPadding - 50)
        
        onKeepChoosed: close()
        onResetChoosed:
        {
            // Reset settings
            close();
        }
    }
    
    
    function resetAppearanceSettings()
    {
    }
}
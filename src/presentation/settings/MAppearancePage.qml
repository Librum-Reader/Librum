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
        property var settings: []
        
        width: parent.width
        spacing: 0
        
        function registerSetting(setting)
        {
            settings.push(setting);
        }
        
        
        Connections
        {
            target: SettingsController
            function onReload()
            {
                for (var i = 0; i < layout.settings.length; i++)
                {
                    layout.settings[i].reset();
                }
            }
        }
        
        
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
                Layout.preferredWidth: 160
                Layout.preferredHeight: 38
                Layout.alignment: Qt.AlignBottom
                borderWidth: 0
                backgroundColor: Style.colorBasePurple
                text: "Restore Defaults"
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
                            property string savedValue: layout.getSavedSetting(SettingKeys.Theme)
                            
                            Layout.topMargin: 4
                            leftProperty: "Dark"
                            rightProperty: "Light"
                            leftSelected: savedValue == leftProperty
                            rightSelected: savedValue == rightProperty
                            
                            onSelectedChanged: (newSelected) => layout.saveSetting(SettingKeys.Theme,
                                                                                   newSelected)
                            
                            function reset()
                            {
                                savedValue = layout.getSavedSetting(SettingKeys.Theme);
                                if(savedValue === leftProperty)
                                    selectLeft();
                                else
                                    selectRight();
                            }
                            
                            Component.onCompleted: layout.registerSetting(this);
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
                            value: layout.getSavedSetting(SettingKeys.PageSpacing)
                            
                            onNewValueSelected: layout.saveSetting(SettingKeys.PageSpacing,
                                                                   value)
                            
                            function reset()
                            {
                                value = layout.getSavedSetting(SettingKeys.PageSpacing);
                            }
                            
                            Component.onCompleted: layout.registerSetting(this);
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
                        
                        MOnOffToggle
                        {
                            id: displayBookTitleInTitlebarToggle
                            property string savedValue: layout.getSavedSetting(SettingKeys.DisplayBookTitleInTitlebar)
                            
                            Layout.topMargin: 4
                            onByDefault: savedValue === onText
                            
                            onToggled:
                            {
                                layout.saveSetting(SettingKeys.DisplayBookTitleInTitlebar,
                                                   currentlyOn === true ? onText : offText)
                            }
                            
                            function reset()
                            {
                                savedValue = layout.getSavedSetting(SettingKeys.DisplayBookTitleInTitlebar);
                                if(savedValue === onText)
                                    setOn();
                                else
                                    setOff();
                            }
                            
                            Component.onCompleted: layout.registerSetting(this);
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
                            property string savedValue: layout.getSavedSetting(SettingKeys.LayoutDirection)
                            
                            Layout.fillWidth: true
                            Layout.topMargin: 6
                            options: ["Vertical", "Horizontal"]
                            currentSelected: savedValue === options[0] ? options[0] : options[1]
                            
                            onNewCurrentSelected: layout.saveSetting(SettingKeys.LayoutDirection,
                                                                     currentSelected)
                            
                            function reset()
                            {
                                savedValue = layout.getSavedSetting(SettingKeys.LayoutDirection);
                                currentSelected = (savedValue === options[0] ? options[0] : options[1]);
                            }
                            
                            Component.onCompleted: layout.registerSetting(this);
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
                            property string savedValue: layout.getSavedSetting(SettingKeys.DisplayMode)
                            
                            Layout.fillWidth: true
                            Layout.topMargin: 6
                            options: ["Single Page", "Double Page"]
                            currentSelected: getCurrentSelected()
                            
                            onNewCurrentSelected: layout.saveSetting(SettingKeys.DisplayMode,
                                                                     currentSelected)
                            
                            function getCurrentSelected()
                            {
                                return savedValue === options[0] ? options[0] : options[1];
                            }
                            
                            function reset()
                            {
                                savedValue = layout.getSavedSetting(SettingKeys.DisplayMode);
                                currentSelected = getCurrentSelected();
                            }
                            
                            Component.onCompleted: layout.registerSetting(this);
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
                            property string savedValue: layout.getSavedSetting(SettingKeys.PageTransition)
                            
                            Layout.topMargin: 4
                            Layout.preferredHeight: 36
                            Layout.fillWidth: true
                            Layout.maximumWidth: 380
                            titleFontSize: 12
                            titleSpacing: 4
                            image: Icons.dropdownGray
                            defaultIndex: calculateDefaultIndex()
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
                            
                            onItemChanged: layout.saveSetting(SettingKeys.PageTransition,
                                                              text)
                            
                            function calculateDefaultIndex()
                            {
                                for(let i = 0; i < model.count; ++i)
                                {
                                    if(model.get(i).text === savedValue)
                                        return i;
                                }
                                return -1;
                            }
                            
                            function reset()
                            {
                                savedValue = layout.getSavedSetting(SettingKeys.PageTransition);
                                deselectCurrenItem();
                                selectItem(calculateDefaultIndex());
                            }
                            
                            Component.onCompleted: layout.registerSetting(this);
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
                            property int savedValue: layout.getSavedSetting(SettingKeys.DefaultZoom)
                            
                            Layout.preferredWidth: 76
                            Layout.topMargin: 4
                            value: savedValue
                            
                            onNewValueSelected: layout.saveSetting(SettingKeys.DefaultZoom,
                                                                   value)
                            
                            function reset()
                            {
                                savedValue = layout.getSavedSetting(SettingKeys.DefaultZoom);
                                value = savedValue;
                            }
                            
                            Component.onCompleted: layout.registerSetting(this);
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
                        
                        
                        MOnOffToggle
                        {
                            id: smoothScrollingToggle
                            property string savedValue: layout.getSavedSetting(SettingKeys.SmoothScrolling)
                            Layout.topMargin: 4
                            onByDefault: savedValue === onText
                            
                            onToggled:
                            {
                                layout.saveSetting(SettingKeys.SmoothScrolling,
                                                   currentlyOn === true ? onText : offText)
                            }
                            
                            function reset()
                            {
                                savedValue = layout.getSavedSetting(SettingKeys.SmoothScrolling);
                                if(savedValue === onText)
                                    setOn();
                                else
                                    setOff();
                            }
                            
                            Component.onCompleted: layout.registerSetting(this);
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
                        
                        
                        MOnOffToggle
                        {
                            id: loopAfterLastToggle
                            property string savedValue: layout.getSavedSetting(SettingKeys.LoopAfterLastPage)
                            
                            Layout.topMargin: 4
                            onByDefault: savedValue === onText
                            
                            onToggled:
                            {
                                layout.saveSetting(SettingKeys.LoopAfterLastPage,
                                                   currentlyOn === true ? onText : offText)
                            }
                            
                            function reset()
                            {
                                savedValue = layout.getSavedSetting(SettingKeys.LoopAfterLastPage);
                                if(savedValue === onText)
                                    setOn();
                                else
                                    setOff();
                            }
                            
                            Component.onCompleted: layout.registerSetting(this);
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
                            property string savedValue: layout.getSavedSetting(SettingKeys.CursorMode)
                            
                            Layout.fillWidth: true
                            Layout.topMargin: 6
                            options: ["Hidden after delay", "Always visible"]
                            currentSelected: savedValue === options[0] ? options[0] : options[1]
                            
                            onNewCurrentSelected: layout.saveSetting(SettingKeys.CursorMode,
                                                                     currentSelected)
                            
                            function reset()
                            {
                                savedValue = layout.getSavedSetting(SettingKeys.CursorMode);
                                currentSelected = (savedValue === options[0] ? options[0] : options[1]);
                            }
                            
                            Component.onCompleted: layout.registerSetting(this);
                        }
                    }
                }
            }
        }
        
        // higher order functions -> simplify syntax
        function saveSetting(key, value)
        {
            SettingsController.setSetting(key, value, SettingGroups.Appearance);
        }
        
        function getSavedSetting(key)
        {
            return SettingsController.getSetting(key, SettingGroups.Appearance);
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
            SettingsController.resetSettingGroup(SettingGroups.Appearance);
            close();
        }
    }
    
    
    function resetAppearanceSettings()
    {
    }
}
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
    background: Rectangle { anchors.fill: parent; color: Style.pagesBackground }
    
    
    /*
      This connection is triggered after the settings are reset, upon which
      the SettingsController sends a reload() signal.
      */
    Connections
    {
        id: reloadSettings
        target: SettingsController
        function onReload() { internal.resetSettings() }
    }
    
    
    ColumnLayout
    {
        id: layout
        width: parent.width
        spacing: 0
        
        
        RowLayout
        {
            id: titleRow
            Layout.fillWidth: true
            spacing: 0
            
            
            MTitle
            {
                id: pageTitle
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
            Layout.fillWidth: true
            // Calculate the scrollable height (Scrollable only if the page is bigger than the screen)
            Layout.preferredHeight: root.height - titleRow.implicitHeight - root.bottomPadding * 2
            contentHeight: contentLayout.implicitHeight
            Layout.topMargin: 32
            Layout.rightMargin: -internal.scrollbarOffset
            maximumFlickVelocity: 1000
            flickDeceleration: 4000
            clip: true
            
            ScrollBar.vertical: ScrollBar { width: 10; policy: "AlwaysOn" }
            
            
            ColumnLayout
            {
                id: contentLayout
                width: parent.width
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: internal.scrollbarOffset
                spacing: 0
                
                
                Pane
                {
                    id: displayBlock
                    Layout.fillWidth: true
                    verticalPadding: 24
                    horizontalPadding: internal.pagePadding
                    background: Rectangle
                    {
                        color: Style.colorBackground
                        border.color: Style.colorLightBorder
                        radius: 4
                        antialiasing: true
                    }
                    
                    
                    ColumnLayout
                    {
                        id: displayLayout
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
                            property string savedValue: internal.getSavedSetting(SettingKeys.Theme)
                            
                            Layout.topMargin: 4
                            leftText: "Dark"
                            rightText: "Light"
                            leftSelected: savedValue == leftText
                            rightSelected: savedValue == rightText
                            
                            onToggled: (newSelected) => internal.saveSetting(SettingKeys.Theme,
                                                                           newSelected)
                            
                            function reset()
                            {
                                savedValue = internal.getSavedSetting(SettingKeys.Theme);
                                if(savedValue === leftText)
                                    selectLeft();
                                else
                                    selectRight();
                            }
                            
                            Component.onCompleted: internal.registerSetting(this);
                        }
                    }   
                }
                
                Pane
                {
                    id: readingBlock
                    Layout.fillWidth: true
                    Layout.topMargin: 24
                    verticalPadding: 24
                    horizontalPadding: internal.pagePadding
                    background: Rectangle
                    {
                        color: Style.colorBackground
                        border.color: Style.colorLightBorder
                        radius: 4
                        antialiasing: true
                    }
                    
                    
                    ColumnLayout
                    {
                        id: readingLayout
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
                            value: internal.getSavedSetting(SettingKeys.PageSpacing)
                            
                            onNewValueSelected: internal.saveSetting(SettingKeys.PageSpacing,
                                                                   value)
                            
                            function reset()
                            {
                                value = internal.getSavedSetting(SettingKeys.PageSpacing);
                            }
                            
                            Component.onCompleted: internal.registerSetting(this);
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
                            property string savedValue: internal.getSavedSetting(SettingKeys.DisplayBookTitleInTitlebar)
                            
                            Layout.topMargin: 4
                            onByDefault: savedValue === onText
                            
                            onToggled:
                            {
                                internal.saveSetting(SettingKeys.DisplayBookTitleInTitlebar,
                                                   currentlyOn === true ? onText : offText)
                            }
                            
                            function reset()
                            {
                                savedValue = internal.getSavedSetting(SettingKeys.DisplayBookTitleInTitlebar);
                                if(savedValue === onText)
                                    setOn();
                                else
                                    setOff();
                            }
                            
                            Component.onCompleted: internal.registerSetting(this);
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
                            property string savedValue: internal.getSavedSetting(SettingKeys.LayoutDirection)
                            
                            Layout.fillWidth: true
                            Layout.topMargin: 6
                            options: ["Vertical", "Horizontal"]
                            currentSelected: savedValue === options[0] ? options[0] : options[1]
                            
                            onNewCurrentSelected: internal.saveSetting(SettingKeys.LayoutDirection,
                                                                     currentSelected)
                            
                            function reset()
                            {
                                savedValue = internal.getSavedSetting(SettingKeys.LayoutDirection);
                                currentSelected = (savedValue === options[0] ? options[0] : options[1]);
                            }
                            
                            Component.onCompleted: internal.registerSetting(this);
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
                            property string savedValue: internal.getSavedSetting(SettingKeys.DisplayMode)
                            
                            Layout.fillWidth: true
                            Layout.topMargin: 6
                            options: ["Single Page", "Double Page"]
                            currentSelected: getCurrentSelected()
                            
                            onNewCurrentSelected: internal.saveSetting(SettingKeys.DisplayMode,
                                                                     currentSelected)
                            
                            function getCurrentSelected()
                            {
                                return savedValue === options[0] ? options[0] : options[1];
                            }
                            
                            function reset()
                            {
                                savedValue = internal.getSavedSetting(SettingKeys.DisplayMode);
                                currentSelected = getCurrentSelected();
                            }
                            
                            Component.onCompleted: internal.registerSetting(this);
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
                            property string savedValue: internal.getSavedSetting(SettingKeys.PageTransition)
                            
                            Layout.topMargin: 4
                            Layout.preferredHeight: 36
                            Layout.fillWidth: true
                            Layout.maximumWidth: 380
                            selectedItemFontSize: 12
                            selectedItemPadding: 4
                            defaultIndex: calculateDefaultIndex()
                            dropdownIconSize: 9
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
                            
                            onItemChanged: internal.saveSetting(SettingKeys.PageTransition,
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
                                savedValue = internal.getSavedSetting(SettingKeys.PageTransition);
                                deselectCurrenItem();
                                selectItem(calculateDefaultIndex());
                            }
                            
                            Component.onCompleted: internal.registerSetting(this);
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
                            property int savedValue: internal.getSavedSetting(SettingKeys.DefaultZoom)
                            
                            Layout.preferredWidth: 76
                            Layout.topMargin: 4
                            value: savedValue
                            
                            onNewValueSelected: internal.saveSetting(SettingKeys.DefaultZoom,
                                                                   value)
                            
                            function reset()
                            {
                                savedValue = internal.getSavedSetting(SettingKeys.DefaultZoom);
                                value = savedValue;
                            }
                            
                            Component.onCompleted: internal.registerSetting(this);
                        }
                    }
                }
                
                Pane
                {
                    id: behaviorBlock
                    Layout.fillWidth: true
                    Layout.topMargin: 24
                    verticalPadding: 24
                    horizontalPadding: internal.pagePadding
                    background: Rectangle
                    {
                        color: Style.colorBackground
                        border.color: Style.colorLightBorder
                        radius: 4
                        antialiasing: true
                    }
                    
                    
                    ColumnLayout
                    {
                        id: behaviorLayout
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
                            property string savedValue: internal.getSavedSetting(SettingKeys.SmoothScrolling)
                            Layout.topMargin: 4
                            onByDefault: savedValue === onText
                            
                            onToggled:
                            {
                                internal.saveSetting(SettingKeys.SmoothScrolling,
                                                   currentlyOn === true ? onText : offText)
                            }
                            
                            function reset()
                            {
                                savedValue = internal.getSavedSetting(SettingKeys.SmoothScrolling);
                                if(savedValue === onText)
                                    setOn();
                                else
                                    setOff();
                            }
                            
                            Component.onCompleted: internal.registerSetting(this);
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
                            property string savedValue: internal.getSavedSetting(SettingKeys.LoopAfterLastPage)
                            
                            Layout.topMargin: 4
                            onByDefault: savedValue === onText
                            
                            onToggled:
                            {
                                internal.saveSetting(SettingKeys.LoopAfterLastPage,
                                                   currentlyOn === true ? onText : offText)
                            }
                            
                            function reset()
                            {
                                savedValue = internal.getSavedSetting(SettingKeys.LoopAfterLastPage);
                                if(savedValue === onText)
                                    setOn();
                                else
                                    setOff();
                            }
                            
                            Component.onCompleted: internal.registerSetting(this);
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
                            property string savedValue: internal.getSavedSetting(SettingKeys.CursorMode)
                            
                            Layout.fillWidth: true
                            Layout.topMargin: 6
                            options: ["Hidden after delay", "Always visible"]
                            currentSelected: savedValue === options[0] ? options[0] : options[1]
                            
                            onNewCurrentSelected: internal.saveSetting(SettingKeys.CursorMode,
                                                                     currentSelected)
                            
                            function reset()
                            {
                                savedValue = internal.getSavedSetting(SettingKeys.CursorMode);
                                currentSelected = (savedValue === options[0] ? options[0] : options[1]);
                            }
                            
                            Component.onCompleted: internal.registerSetting(this);
                        }
                    }
                }
            }
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
    
    QtObject
    {
        id: internal
        property int pagePadding: 40
        property int scrollbarOffset: 22
        property var registeredSettings: []
        
        
        /*
          Every setting needs to register it self and define some specific methods
          e.g. reset(), so that certain operations can be executed on all settings dynamically
          */
        function registerSetting(setting)
        {
            internal.registeredSettings.push(setting);
        }
        
        /*
          Reset the values of all settings, by calling their reset() method
          */
        function resetSettings()
        {
            for (var i = 0; i < internal.registeredSettings.length; i++)
            {
                internal.registeredSettings[i].reset();
            }
        }
        
        
        /*
          Higher order functions to simplify the call on the SettingsController
          */
        function saveSetting(key, value)
        {
            SettingsController.setSetting(key, value, SettingGroups.Appearance);
        }
        
        function getSavedSetting(key)
        {
            return SettingsController.getSetting(key, SettingGroups.Appearance);
        }
    }
}
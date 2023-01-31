pragma Singleton
import QtQuick 2.15
import Librum.controllers 1.0

Item
{
    id: styleSheet
    property color colorFocusedButtonText: "#FFFFFF"
    property color colorPlaceholderText: "#8E8EA9"
    property color colorSubtitle: "#8A8AA0"
    property color colorLightText3   : "#666687"
    property color colorBasePurple   : "#946BDE"
    property color colorMediumPurple : "#D9D8FF"
    property color colorPinkPurple   : "#E3D6FF"
    property color colorLightPurple  : "#F0F0FF"
    property color colorLightPurple2 : "#EEE9F1"
    property color colorBaseGray     : "#D2D2D2"
    property color colorDarkGray     : "#737373"
    property color colorLightGray    : "#F6F6F9"
    property color colorLightGray2   : "#EDEDED"
    property color colorLightGray3   : "#E2E2E2"
    property color colorNeonBlue     : "#4945FF"
    property color colorLightGreen   : "#EAFBE7"
    property color colorLightGreen2  : "#C6F0C2"
    property color colorMediumGreen  : "#5CB176"
    property color colorBaseRed      : "#D84B4D"
    property color colorBaseText     : "#32324D"
    property color colorBaseTitle    : "#4A4A6A"
    property color colorMediumText   : "#505057"
    property color colorLightBorder  : "#DCDCE4"
    property color colorLightBorder2 : "#C0C0CF"
    property color colorLightBorder3 : "#E0E0DE"
    property color colorDarkBorder   : "#ADADB4"
    property color colorErrorText    : "#F25E42"
    property color colorErrorBorder  : "#E87964"
    property color colorErrorLight   : "#FDEDE8"
    property color colorBackground   : "#FFFFFF"
    property color darkBackground    : "#CCCCCC"
    property color colorSidebarMark  : "#F0F0FF"
    property color colorSeparator    : "#EAEAEF"
    property color containerBackground : "#FFFFFF"
    property color colorLightGrayBackground : "#EAEAEF"
    property color pagesBackground: "#F1F1F1"
    
    
    // Temporary solution for switching themes
    Connections
    {
        id: theme
        target: SettingsController
        function onSettingChanged()
        {
            let newTheme = SettingsController.getSetting(SettingKeys.Theme, SettingGroups.Appearance);
            if(newTheme !== state)
                state = newTheme;
        }
    }
    
    state: /*"Dark"*/SettingsController.getSetting(SettingKeys.Theme, SettingGroups.Appearance)
    states: [
        State {
            name: "Light"
            PropertyChanges { target: styleSheet; pagesBackground: "#F1F1F1" }
            PropertyChanges { target: styleSheet; containerBackground: "#FFFFFF" }
            PropertyChanges { target: styleSheet; colorFocusedButtonText: "#FFFFFF" }
        },
        State {
            name: "Dark"
            PropertyChanges { target: styleSheet; pagesBackground: "#24242D" }
            PropertyChanges { target: styleSheet; containerBackground: "#2D2D38" }
            PropertyChanges { target: styleSheet; colorFocusedButtonText: "#FFFFFF" }
        }
    ]
}
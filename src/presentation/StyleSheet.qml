pragma Singleton
import QtQuick 2.15
import Librum.controllers 1.0

Item
{
    id: styleSheet
    // Buttons
    property color colorFocusedButtonText: "#FFFFFF"
    property color colorUnfocusedButtonText: "#666687"
    property color colorButtonBackground: "transparent"
    property color colorBorderlessButtonBackground: "#D2D2D2"
    property color colorLightBorderlessButtonBackground: "#EDEDED"
    
    // Texts
    property color colorPlaceholderText: "#8E8EA9"
    property color colorLightText: "#666687"
    property color colorNoImageLabel: "#737373"
    
    // Subtitless
    property color colorSubtitle: "#8A8AA0"
    property color colorPageSubtitle: "#666687"
    
    // Input
    property color colorReadOnlyInputText: "#666687"
    property color colorBaseInputText: "#32324D"
    property color colorLightInputText: "#666687"
    
    // Purple tones
    property color colorBasePurple: "#946BDE"
    property color colorLightPurple: "#D9D8FF"
    
    // Highlight
    property color colorLightHighlight: "#F0F0FF"
    property color colorHighlight: "#EEE9F1"
    property color colorHoverHighlight: "#F6F6F9"
    
    // Separators
    property color colorSeparator: "#EAEAEF"
    property color colorDarkSeparator: "#D2D2D2"
    property color colorLightSeparator: "#F6F6F9"
    
    // Backgrounds
    property color colorReadingToolbarBackground: "#EDEDED"
    
    // Other
    property color colorDropArea: "#F6F6F9"
    
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
    property color colorSidebarMark  : "#F0F0FF"
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
            PropertyChanges { target: styleSheet; pagesBackground: "#212121" }
            PropertyChanges { target: styleSheet; containerBackground: "#2C2C2C" }
            PropertyChanges { target: styleSheet; colorFocusedButtonText: "#FFFFFF" }
        }
    ]
}
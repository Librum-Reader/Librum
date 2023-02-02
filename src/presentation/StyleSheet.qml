pragma Singleton
import QtQuick 2.15
import Librum.controllers 1.0

Item
{
    id: styleSheet
    
    // Buttons
    property color colorFocusedButtonText: "#FFFFFF"
    property color colorUnfocusedButtonText: "#666687"
    property color colorControlBackground: "#FFFFFF"
    property color colorReadingViewButtonBackground: "#D2D2D2"
    property color colorLightReadingViewButtonBackground: "#EDEDED"
    
    // Texts
    property color colorPlaceholderText: "#8E8EA9"
    property color colorLightText: "#666687"
    property color colorNoImageLabel: "#737373"
    property color colorGreenText: "#5CB176"
    property color colorText: "#32324D"
    property color colorErrorText: "#F25E42"
    property color colorTitle: "#32324D"
    
    // Subtitless
    property color colorSubtitle: "#8A8AA0"
    property color colorPageSubtitle: "#666687"
    
    // Input
    property color colorReadOnlyInputText: "#666687"
    property color colorBaseInputText: "#32324D"
    property color colorLightInputText: "#666687"
    
    // Color tones
    property color colorBasePurple: "#946BDE"
    property color colorLightPurple: "#D9D8FF"
    property color colorRed: "#D84B4D"
    
    // Highlight
    property color colorLightHighlight: "#F0F0FF"
    property color colorHighlight: "#EEE9F1"
    property color colorHoverHighlight: "#F6F6F9"
    
    // Separators
    property color colorSeparator: "#EAEAEF"
    property color colorDarkSeparator: "#DCDCE4"
    property color colorLightSeparator: "#F6F6F9"
    
    // Backgrounds
    property color colorReadingToolbarBackground: "#EDEDED"
    property color colorGreenBackground: "#EAFBE7"
    property color colorBookImageBackground: "#DCDCE4"
    property color colorErrorBackground: "#FDEDE8"
    property color colorContainerBackground: "#FFFFFF"
    property color colorPopupBackground: "#FFFFFF"
    property color colorBookBackground: "#FFFFFF"
    property color colorReadingViewBackground: "#FFFFFF"
    property color colorPageBackground: "#F1F1F1"
    
    // Borders
    property color colorGreenBorder: "#C6F0C2"
    property color colorBookBorder: "#E0E0DE"
    property color colorButtonBorder: "#DCDCE4"
    property color colorCheckboxBorder: "#C0C0CF"
    property color colorContainerBorder: "#DCDCE4"
    property color colorErrorBorder: "#E87964"
    
    // Other
    property color colorDropArea: "#F6F6F9"
    
    
    
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
            PropertyChanges { target: styleSheet; colorPageBackground: "#F1F1F1" }
            PropertyChanges { target: styleSheet; colorContainerBackground: "#FFFFFF" }
            PropertyChanges { target: styleSheet; colorFocusedButtonText: "#FFFFFF" }
        },
        State {
            name: "Dark"
            PropertyChanges { target: styleSheet; colorPageBackground: "#212121" }
            PropertyChanges { target: styleSheet; colorContainerBackground: "#2C2C2C" }
            PropertyChanges { target: styleSheet; colorFocusedButtonText: "#FFFFFF" }
        }
    ]
}
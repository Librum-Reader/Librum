pragma Singleton
import QtQuick 2.15
import Librum.controllers 1.0
import Librum.icons 1.0

Item
{
    id: styleSheet
    
    // Buttons
    property color colorFocusedButtonText: "#FFFFFF"
    property color colorUnfocusedButtonText: "#666687"
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
    property color colorAuthenticationPageBackground: "#EFEFEF"
    property color colorPageBackground: "#F6F6F9"
    property color colorControlBackground: "#FFFFFF"
    
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
    
    state: "Dark"/*SettingsController.getSetting(SettingKeys.Theme, SettingGroups.Appearance)*/
    states: [
        State
        {
            name: "Light"
            
            // Buttons
            PropertyChanges { target: styleSheet; colorFocusedButtonText: "#FFFFFF" }
            PropertyChanges { target: styleSheet; colorUnfocusedButtonText: "#666687" }
            PropertyChanges { target: styleSheet; colorReadingViewButtonBackground: "#D2D2D2" }
            PropertyChanges { target: styleSheet; colorLightReadingViewButtonBackground: "#EDEDED" }
            
            // Texts
            PropertyChanges { target: styleSheet; colorPlaceholderText: "#8E8EA9" }
            PropertyChanges { target: styleSheet; colorLightText: "#666687" }
            PropertyChanges { target: styleSheet; colorNoImageLabel: "#737373" }
            PropertyChanges { target: styleSheet; colorGreenText: "#5CB176" }
            PropertyChanges { target: styleSheet; colorText: "#32324D" }
            PropertyChanges { target: styleSheet; colorErrorText: "#F25E42" }
            PropertyChanges { target: styleSheet; colorTitle: "#32324D" }
            
            // Subtitless
            PropertyChanges { target: styleSheet; colorSubtitle: "#8A8AA0" }
            PropertyChanges { target: styleSheet; colorPageSubtitle: "#666687" }
            
            // Input
            PropertyChanges { target: styleSheet; colorReadOnlyInputText: "#666687" }
            PropertyChanges { target: styleSheet; colorBaseInputText: "#32324D" }
            PropertyChanges { target: styleSheet; colorLightInputText: "#666687" }
            
            // Color tones
            PropertyChanges { target: styleSheet; colorBasePurple: "#946BDE" }
            PropertyChanges { target: styleSheet; colorLightPurple: "#D9D8FF" }
            PropertyChanges { target: styleSheet; colorRed: "#D84B4D" }
            
            // Highlight
            PropertyChanges { target: styleSheet; colorLightHighlight: "#F0F0FF" }
            PropertyChanges { target: styleSheet; colorHighlight: "#EEE9F1" }
            PropertyChanges { target: styleSheet; colorHoverHighlight: "#F6F6F9" }
            
            // Separators
            PropertyChanges { target: styleSheet; colorSeparator: "#EAEAEF" }
            PropertyChanges { target: styleSheet; colorDarkSeparator: "#DCDCE4" }
            PropertyChanges { target: styleSheet; colorLightSeparator: "#F6F6F9" }
            
            // Backgrounds
            PropertyChanges { target: styleSheet; colorReadingToolbarBackground: "#EDEDED" }
            PropertyChanges { target: styleSheet; colorGreenBackground: "#EAFBE7" }
            PropertyChanges { target: styleSheet; colorBookImageBackground: "#DCDCE4" }
            PropertyChanges { target: styleSheet; colorErrorBackground: "#FDEDE8" }
            PropertyChanges { target: styleSheet; colorContainerBackground: "#FFFFFF" }
            PropertyChanges { target: styleSheet; colorPopupBackground: "#FFFFFF" }
            PropertyChanges { target: styleSheet; colorBookBackground: "#FFFFFF" }
            PropertyChanges { target: styleSheet; colorReadingViewBackground: "#FFFFFF" }
            PropertyChanges { target: styleSheet; colorAuthenticationPageBackground: "#EFEFEF" }
            PropertyChanges { target: styleSheet; colorPageBackground: "#F6F6F9" }
            PropertyChanges { target: styleSheet; colorControlBackground: "#FFFFFF" }
            
            // Borders
            PropertyChanges { target: styleSheet; colorGreenBorder: "#C6F0C2" }
            PropertyChanges { target: styleSheet; colorBookBorder: "#E0E0DE" }
            PropertyChanges { target: styleSheet; colorButtonBorder: "#DCDCE4" }
            PropertyChanges { target: styleSheet; colorCheckboxBorder: "#C0C0CF" }
            PropertyChanges { target: styleSheet; colorContainerBorder: "#DCDCE4" }
            PropertyChanges { target: styleSheet; colorErrorBorder: "#E87964" }
            
            // Other
            PropertyChanges { target: styleSheet; colorDropArea: "#F6F6F9" }
        },
        State
        {
            name: "Dark"
            
            // Buttons
            PropertyChanges { target: styleSheet; colorFocusedButtonText: "#FFFFFF" }
            PropertyChanges { target: styleSheet; colorUnfocusedButtonText: "#C8C8D8" }
            PropertyChanges { target: styleSheet; colorReadingViewButtonBackground: "#D2D2D2" }
            PropertyChanges { target: styleSheet; colorLightReadingViewButtonBackground: "#EDEDED" }
            
            // Texts
            PropertyChanges { target: styleSheet; colorPlaceholderText: "#C8C8C8" }
            PropertyChanges { target: styleSheet; colorLightText: "#CECEDD" }
            PropertyChanges { target: styleSheet; colorNoImageLabel: "#737373" }
            PropertyChanges { target: styleSheet; colorGreenText: "#5CB176" }
            PropertyChanges { target: styleSheet; colorText: "#D5D5D5" }
            PropertyChanges { target: styleSheet; colorErrorText: "#F25E42" }
            PropertyChanges { target: styleSheet; colorTitle: "#CBCBCB" }
            
            // Subtitless
            PropertyChanges { target: styleSheet; colorSubtitle: "#E5E5EF" }
            PropertyChanges { target: styleSheet; colorPageSubtitle: "#C4C4C4" }
            
            // Input
            PropertyChanges { target: styleSheet; colorReadOnlyInputText: "#666687" }
            PropertyChanges { target: styleSheet; colorBaseInputText: "#FFFFFF" }
            PropertyChanges { target: styleSheet; colorLightInputText: "#666687" }
            
            // Color tones
            PropertyChanges { target: styleSheet; colorBasePurple: "#946BDE" }
            PropertyChanges { target: styleSheet; colorLightPurple: "#625669" }
            PropertyChanges { target: styleSheet; colorRed: "#D84B4D" }
            
            // Highlight
            PropertyChanges { target: styleSheet; colorLightHighlight: "#3F3F49" }
            PropertyChanges { target: styleSheet; colorHighlight: "#3E3E48" }
            PropertyChanges { target: styleSheet; colorHoverHighlight: "#3C3C3C" }
            
            // Separators
            PropertyChanges { target: styleSheet; colorSeparator: "#434344" }
            PropertyChanges { target: styleSheet; colorDarkSeparator: "#454547" }
            PropertyChanges { target: styleSheet; colorLightSeparator: "#F6F6F9" }
            
            // Backgrounds
            PropertyChanges { target: styleSheet; colorReadingToolbarBackground: "#EDEDED" }
            PropertyChanges { target: styleSheet; colorGreenBackground: "#EAFBE7" }
            PropertyChanges { target: styleSheet; colorBookImageBackground: "#454545" }
            PropertyChanges { target: styleSheet; colorErrorBackground: "#FDEDE8" }
            PropertyChanges { target: styleSheet; colorContainerBackground: "#2C2C2C" }
            PropertyChanges { target: styleSheet; colorPopupBackground: "#2C2C2C" }
            PropertyChanges { target: styleSheet; colorBookBackground: "#2C2C2C" }
            PropertyChanges { target: styleSheet; colorReadingViewBackground: "#FFFFFF" }
            PropertyChanges { target: styleSheet; colorAuthenticationPageBackground: "#212121" }
            PropertyChanges { target: styleSheet; colorPageBackground: "#212121" }
            PropertyChanges { target: styleSheet; colorControlBackground: "#2E2E2E" }
            
            // Borders
            PropertyChanges { target: styleSheet; colorGreenBorder: "#C6F0C2" }
            PropertyChanges { target: styleSheet; colorBookBorder: "#3C3C40" }
            PropertyChanges { target: styleSheet; colorButtonBorder: "#55555A" }
            PropertyChanges { target: styleSheet; colorCheckboxBorder: "#6C6C73" }
            PropertyChanges { target: styleSheet; colorContainerBorder: "#404044" }
            PropertyChanges { target: styleSheet; colorErrorBorder: "#E87964" }
            
            // Other
            PropertyChanges { target: styleSheet; colorDropArea: "#F6F6F9" }
        }
    ]
    
    // Change icons
    onStateChanged: Icons.setState(state)
}
pragma Singleton
import QtQuick 2.15
import Qt.labs.settings 1.1
import Librum.controllers 1.0
import Librum.icons 1.0

Item
{
    id: styleSheet
    
    // Buttons
    property color colorFocusedButtonText
    property color colorUnfocusedButtonText
    property color colorReadingViewButtonBackground
    property color colorLightReadingViewButtonBackground
    
    // Texts
    property color colorPlaceholderText
    property color colorLightText
    property color colorNoImageLabel
    property color colorGreenText
    property color colorText
    property color colorErrorText
    property color colorTitle
    
    // Subtitless
    property color colorSubtitle
    property color colorPageSubtitle
    
    // Input
    property color colorReadOnlyInputText
    property color colorBaseInputText
    property color colorLightInputText
    
    // Color tones
    property color colorBasePurple
    property color colorLightPurple
    property color colorRed
    
    // Highlight
    property color colorLightHighlight
    property color colorHighlight
    property color colorHoverHighlight
    
    // Separators
    property color colorSeparator
    property color colorDarkSeparator
    property color colorLightSeparator
    
    // Backgrounds
    property color colorReadingToolbarBackground
    property color colorGreenBackground
    property color colorBookImageBackground
    property color colorErrorBackground
    property color colorContainerBackground
    property color colorPopupBackground
    property color colorBookBackground
    property color colorReadingViewBackground
    property color colorAuthenticationPageBackground
    property color colorPageBackground
    property color colorControlBackground
    property color colorSettingsSidebarBackground
    
    // Borders
    property color colorGreenBorder
    property color colorRecordActiveBorder
    property color colorBookBorder
    property color colorButtonBorder
    property color colorCheckboxBorder
    property color colorContainerBorder
    property color colorErrorBorder
    
    // Other
    property color colorDropArea
    property color colorPopupDim
    property color colorRecordActiveGlow
    property color colorBookCoverDim
    property color colorDefaultProfilePicture
    
    
    
    state: (SettingsController.appearanceSettings.Theme === undefined 
            ? lastRunSettings.theme // default
            : SettingsController.appearanceSettings.Theme)
    
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
            PropertyChanges { target: styleSheet; colorSettingsSidebarBackground: "#F6F6F9" }
            
            // Borders
            PropertyChanges { target: styleSheet; colorGreenBorder: "#C6F0C2" }
            PropertyChanges { target: styleSheet; colorRecordActiveBorder: "#E8B9BA" }
            PropertyChanges { target: styleSheet; colorBookBorder: "#E0E0DE" }
            PropertyChanges { target: styleSheet; colorButtonBorder: "#DCDCE4" }
            PropertyChanges { target: styleSheet; colorCheckboxBorder: "#C0C0CF" }
            PropertyChanges { target: styleSheet; colorContainerBorder: "#DCDCE4" }
            PropertyChanges { target: styleSheet; colorErrorBorder: "#E87964" }
            
            // Other
            PropertyChanges { target: styleSheet; colorDropArea: "#F6F6F9" }
            PropertyChanges { target: styleSheet; colorPopupDim: "#aa32324D" }
            PropertyChanges { target: styleSheet; colorRecordActiveGlow: "#aaA92224" }
            PropertyChanges { target: styleSheet; colorBookCoverDim: "#32324D" }
            PropertyChanges { target: styleSheet; colorDefaultProfilePicture: "#946BDE" }
        },
        State
        {
            name: "Dark"
            
            // Buttons
            PropertyChanges { target: styleSheet; colorFocusedButtonText: "#FFFFFF" }
            PropertyChanges { target: styleSheet; colorUnfocusedButtonText: "#C8C8D8" }
            PropertyChanges { target: styleSheet; colorReadingViewButtonBackground: "#3A3A3A" }
            PropertyChanges { target: styleSheet; colorLightReadingViewButtonBackground: "#3D3D3D" }
            
            // Texts
            PropertyChanges { target: styleSheet; colorPlaceholderText: "#C8C8C8" }
            PropertyChanges { target: styleSheet; colorLightText: "#CACACF" }
            PropertyChanges { target: styleSheet; colorNoImageLabel: "#737373" }
            PropertyChanges { target: styleSheet; colorGreenText: "#5CB176" }
            PropertyChanges { target: styleSheet; colorText: "#D5D5D5" }
            PropertyChanges { target: styleSheet; colorErrorText: "#DC4C31" }
            PropertyChanges { target: styleSheet; colorTitle: "#CBCBCB" }
            
            // Subtitless
            PropertyChanges { target: styleSheet; colorSubtitle: "#E5E5EF" }
            PropertyChanges { target: styleSheet; colorPageSubtitle: "#C4C4C4" }
            
            // Input
            PropertyChanges { target: styleSheet; colorReadOnlyInputText: "#C1C1C1" }
            PropertyChanges { target: styleSheet; colorBaseInputText: "#FFFFFF" }
            PropertyChanges { target: styleSheet; colorLightInputText: "#E2E2E2" }
            
            // Color tones
            PropertyChanges { target: styleSheet; colorBasePurple: "#946BDE" }
            PropertyChanges { target: styleSheet; colorLightPurple: "#29232C" }
            PropertyChanges { target: styleSheet; colorRed: "#CD4D34" }
            
            // Highlight
            PropertyChanges { target: styleSheet; colorLightHighlight: "#3F3F49" }
            PropertyChanges { target: styleSheet; colorHighlight: "#3E3E48" }
            PropertyChanges { target: styleSheet; colorHoverHighlight: "#3C3C3C" }
            
            // Separators
            PropertyChanges { target: styleSheet; colorSeparator: "#434344" }
            PropertyChanges { target: styleSheet; colorDarkSeparator: "#454547" }
            PropertyChanges { target: styleSheet; colorLightSeparator: "#3B3B3B" }
            
            // Backgrounds
            PropertyChanges { target: styleSheet; colorReadingToolbarBackground: "#2D2D2D" }
            PropertyChanges { target: styleSheet; colorGreenBackground: "#204619" }
            PropertyChanges { target: styleSheet; colorBookImageBackground: "#454545" }
            PropertyChanges { target: styleSheet; colorErrorBackground: "#382E2C" }
            PropertyChanges { target: styleSheet; colorContainerBackground: "#2C2C2C" }
            PropertyChanges { target: styleSheet; colorPopupBackground: "#2C2C2C" }
            PropertyChanges { target: styleSheet; colorBookBackground: "#2C2C2C" }
            PropertyChanges { target: styleSheet; colorReadingViewBackground: "#FFFFFF" }
            PropertyChanges { target: styleSheet; colorAuthenticationPageBackground: "#212121" }
            PropertyChanges { target: styleSheet; colorPageBackground: "#212121" }
            PropertyChanges { target: styleSheet; colorControlBackground: "#2E2E2E" }
            PropertyChanges { target: styleSheet; colorSettingsSidebarBackground: "#262626" }
            
            // Borders
            PropertyChanges { target: styleSheet; colorGreenBorder: "#4B7947" }
            PropertyChanges { target: styleSheet; colorRecordActiveBorder: "#45302E" }
            PropertyChanges { target: styleSheet; colorBookBorder: "#3C3C40" }
            PropertyChanges { target: styleSheet; colorButtonBorder: "#55555A" }
            PropertyChanges { target: styleSheet; colorCheckboxBorder: "#6C6C73" }
            PropertyChanges { target: styleSheet; colorContainerBorder: "#48484D" }
            PropertyChanges { target: styleSheet; colorErrorBorder: "#CB4C34" }
            
            // Other
            PropertyChanges { target: styleSheet; colorDropArea: "#313131" }
            PropertyChanges { target: styleSheet; colorPopupDim: "#aa3F3F52" }
            PropertyChanges { target: styleSheet; colorRecordActiveGlow: "#aaC0452C" }
            PropertyChanges { target: styleSheet; colorBookCoverDim: "#0C0C0F" }
            PropertyChanges { target: styleSheet; colorDefaultProfilePicture: "#946BDE" }
        }
    ]
    
    // Change icons
    onStateChanged: Icons.setState(state)
    
    
    // Settings which capture the application theme the last time the application ran.
    // This way the correct theme is also chosen before the user is authenticated.
    Settings
    {
        id: lastRunSettings
        fileName: "last_run_settings"
        
        property string theme: "Light"
    }
    Component.onDestruction: lastRunSettings.theme = styleSheet.state
}
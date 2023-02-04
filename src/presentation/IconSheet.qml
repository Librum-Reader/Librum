pragma Singleton
import QtQuick 2.15

Item
{
    id: iconSheet
    
    // Arrows-icons
    property string arrowheadBackIcon: "/resources/images/light_mode/arrows/arrowhead_back_icon.svg"
    property string arrowheadNextIcon: "/resources/images/light_mode/arrows/arrowhead_next_icon.svg"
    
    // Combobox
    property string comboboxDropdownDark: "/resources/images/light_mode/combobox/combobox_dropdown_dark.svg"
    property string comboboxDropdownLight: "/resources/images/light_mode/combobox/combobox_dropdown_light.svg"
    
    // Decision-icons
    property string checkGreen: "/resources/images/light_mode/decision/check_green_icon.svg"
    property string checkWhite: "/resources/images/light_mode/decision/check_white_icon.svg"
    property string closePopup: "/resources/images/light_mode/decision/close_popup_icon.svg"
    property string closeDarkgray: "/resources/images/light_mode/decision/close_darkgray_icon.svg"
    property string closeGray: "/resources/images/light_mode/decision/close_gray_icon.svg"
    property string closePurple: "/resources/images/light_mode/decision/close_purple_icon.svg"
    property string plusBlack: "/resources/images/light_mode/decision/plus_black_icon.svg"
    property string plusWhite: "/resources/images/light_mode/decision/plus_white_icon.svg"
    property string plusSquare: "/resources/images/light_mode/decision/plus_square_icon.svg"
    property string checkCircle: "/resources/images/light_mode/decision/check_circle_icon.svg"
    
    // Decoration-icons
    property string heartGray: "/resources/images/light_mode/decoration/heart_gray_icon.svg"
    property string heartWhiteHallow: "/resources/images/light_mode/decoration/heart_white_hallow_icon.svg"
    property string seed: "/resources/images/light_mode/decoration/seed_icon.svg"
    property string popupDroplet: "/resources/images/light_mode/decoration/popup_droplet_icon.svg"
    
    // Illustrations
    property string addImage: "/resources/images/light_mode/illustrations/add_image_illustration.svg"
    property string bookCover: "/resources/images/light_mode/illustrations/book_cover.png"
    property string emptyHomeBackground: "/resources/images/light_mode/illustrations/empty_home_background.svg"
    property string fileSwift: "/resources/images/light_mode/illustrations/file_swift_illustration.svg"
    property string lockProtected: "/resources/images/light_mode/illustrations/lock_protected_illustration.svg"
    property string attentionPurple: "/resources/images/light_mode/illustrations/attention_purple_illustration.svg"
    property string checkBubbleIllustration: "/resources/images/light_mode/illustrations/check_bubble_illustration.svg"
    property string addFileIllustration: "/resources/images/light_mode/illustrations/add_file_illustration.svg"
    
    
    // ReadingView-icons
    property string arrowtopBlackLeft: "/resources/images/light_mode/readingView/arrowtop_black_left_icon.svg"
    property string arrowtopPurpleDown: "/resources/images/light_mode/readingView/arrowtop_purple_down_icon.svg"
    property string arrowtopPurpleUp: "/resources/images/light_mode/readingView/arrowtop_purple_up_icon.svg"
    property string readingViewBookmarkBlack: "/resources/images/light_mode/readingView/bookmark_black_icon.svg"
    property string readingViewBookmarkPurple: "/resources/images/light_mode/readingView/bookmark_purple_icon.svg"
    property string readingViewMaximizeBlack: "/resources/images/light_mode/readingView/maximize_black_icon.svg"
    property string readingViewMaximizePurple: "/resources/images/light_mode/readingView/maximize_purple_icon.svg"
    property string readingViewMenuBlack: "/resources/images/light_mode/readingView/menu_black_icon.svg"
    property string readingViewMenuPurple: "/resources/images/light_mode/readingView/menu_purple_icon.svg"
    property string readingViewSearchBlackThin: "/resources/images/light_mode/readingView/search_black_thin_icon.svg"
    property string readingViewSearchPurpleThin: "/resources/images/light_mode/readingView/search_purple_thin_icon.svg"
    property string readingViewSidebarBlack: "/resources/images/light_mode/readingView/sidebar_black_icon.svg"
    property string readingViewSidebarPurple: "/resources/images/light_mode/readingView/sidebar_purple_icon.svg"
    
    // Setting-icons
    property string settingsAboutGray: "/resources/images/light_mode/settings/about_gray_icon.svg"
    property string settingsAboutPurple: "/resources/images/light_mode/settings/about_purple_icon.svg"
    property string settingsAccountGray: "/resources/images/light_mode/settings/account_gray_icon.svg"
    property string settingsAccountPurple: "/resources/images/light_mode/settings/account_purple_icon.svg"
    property string settingsAppearanceGray: "/resources/images/light_mode/settings/appearance_gray_icon.svg"
    property string settingsAppearancePurple: "/resources/images/light_mode/settings/appearance_purple_icon.svg"
    property string settingsSettingsGray: "/resources/images/light_mode/settings/settings_gray_icon.svg"
    property string settingsSettingsPurple: "/resources/images/light_mode/settings/settings_purple_icon.svg"
    property string settingsShortcutsGray: "/resources/images/light_mode/settings/shortcuts_gray_icon.svg"
    property string settingsShortcutsPurple: "/resources/images/light_mode/settings/shortcuts_purple_icon.svg"
    property string settingsStorageGray: "/resources/images/light_mode/settings/storage_gray_icon.svg"
    property string settingsStoragePurple: "/resources/images/light_mode/settings/storage_purple_icon.svg"
    property string settingsSupportUsGray: "/resources/images/light_mode/settings/support_us_gray_icon.svg"
    property string settingsSupportUsPurple: "/resources/images/light_mode/settings/support_us_purple_icon.svg"
    property string settingsUpdatesGray: "/resources/images/light_mode/settings/updates_gray_icon.svg"
    property string settingsUpdatesPurple: "/resources/images/light_mode/settings/updates_purple_icon.svg"
    
    // Sidebar-icons
    property string emptyProfile : "/resources/images/light_mode/sidebar/sidebar_empty_profile_icon.svg"
    property string home: "/resources/images/light_mode/sidebar/sidebar_home_icon.svg"
    property string pieChart: "/resources/images/light_mode/sidebar/sidebar_pieChart_icon.svg"
    property string tool: "/resources/images/light_mode/sidebar/sidebar_wrench_icon.svg"
    property string settings: "/resources/images/light_mode/sidebar/sidebar_settings_icon.svg"
    property string addOns: "/resources/images/light_mode/sidebar/sidebar_puzzle_icon.svg"
    
    // Utility-icons
    property string dots: "/resources/images/light_mode/utility/dots_icon.svg"
    property string downloadBlack: "/resources/images/light_mode/utility/download_black_icon.svg"
    property string downloadGray: "/resources/images/light_mode/utility/download_gray_icon.svg"
    property string downloadDarkGray: "/resources/images/light_mode/utility/download_darkgray_icon.svg"
    property string downloadWhite: "/resources/images/light_mode/utility/download_white_icon.svg"
    property string editGray: "/resources/images/light_mode/utility/edit_gray_icon.svg"
    property string editPurple: "/resources/images/light_mode/utility/edit_purple_icon.svg"
    property string editLightGray: "/resources/images/light_mode/utility/edit_light_gray_icon.svg"
    property string eyeOff: "/resources/images/light_mode/utility/eye_off_icon.svg"
    property string eyeOn: "/resources/images/light_mode/utility/eye_on_icon.svg"
    property string filter: "/resources/images/light_mode/utility/filter_icon.svg"
    property string logout: "/resources/images/light_mode/utility/log_out_icon.svg"
    property string searchBlack: "/resources/images/light_mode/utility/search_black_icon.svg"
    property string searchGray: "/resources/images/light_mode/utility/search_gray_icon.svg"
    property string sync: "/resources/images/light_mode/utility/sync_icon.svg"
    property string tag: "/resources/images/light_mode/utility/tag_icon.svg"
    property string tagGray: "/resources/images/light_mode/utility/tag_gray_icon.svg"
    property string printer: "/resources/images/light_mode/utility/printer_black_icon.svg"
    property string settingsGrayHallow: "/resources/images/light_mode/utility/settings_gray_hallow_icon.svg"
    property string share: "/resources/images/light_mode/utility/share_black_icon.svg"
    property string sun: "/resources/images/light_mode/utility/sun_gray_icon.svg"
    property string microphone: "/resources/images/light_mode/utility/microphone_icon.svg"
    property string activeMicrophone: "/resources/images/light_mode/utility/active_microphone_icon.svg"
    property string trashGray: "/resources/images/light_mode/utility/trash_gray_icon.svg"
    property string trashWhite: "/resources/images/light_mode/utility/trash_white_icon.svg"
    property string bookOpen: "/resources/images/light_mode/utility/book_open_icon.svg"
    property string book: "/resources/images/light_mode/utility/book_icon.svg"
    property string addFile: "/resources/images/light_mode/utility/add_file_icon.svg"
    property string patreon: "/resources/images/light_mode/utility/patreon_icon.svg"
    
    
    
    state: "Light"
    states: [
        State
        {
            name: "Light"
            
            
        },
        State
        {
            name: "Dark"
            
        }
    ]
    
    
    function setState(name)
    {
        state = name;
        console.log("Setting to: " + name)
    }
}
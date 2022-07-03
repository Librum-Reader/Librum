pragma Singleton
import QtQuick

Item
{
    // Arrows-icons
    property string arrowDownBlack: "/resources/images/arrows/arrow_down_black_icon.svg"
    property string arrowtopBlackRight: "/resources/images/arrows/arrowtop_black_right_icon.svg"
    property string arrowtopGrayRight: "/resources/images/arrows/arrowtop_gray_right_icon.svg"
    property string dropdownBlack: "/resources/images/arrows/dropdown_black_icon.svg"
    property string dropdownGray: "/resources/images/arrows/dropdown_gray_icon.svg"
    property string dropupLightGray: "/resources/images/arrows/dropup_lightgray_icon.svg"
    
    // Decision-icons
    property string checkBlackThin: "/resources/images/decision/check_black_thin_icon.svg"
    property string checkGreen: "/resources/images/decision/check_green_icon.svg"
    property string checkWhite: "/resources/images/decision/check_white_icon.svg"
    property string checkPurple: "/resources/images/decision/check_purple_icon.svg"
    property string closeBlack: "/resources/images/decision/close_black_icon.svg"
    property string closePurple: "/resources/images/decision/close_purple_icon.svg"
    property string plusBlack: "/resources/images/decision/plus_black_icon.svg"
    property string plusPurple: "/resources/images/decision/plus_purple_icon.svg"
    property string plusWhite: "/resources/images/decision/plus_white_icon.svg"
    
    // Decoration-icons
    property string coffee: "/resources/images/decoration/coffee_icon.svg"
    property string heartGray: "/resources/images/decoration/heart_gray_icon.svg"
    property string heartWhiteHallow: "/resources/images/decoration/heart_white_hallow_icon.svg"
    property string seed: "/resources/images/decoration/seed_icon.svg"
    
    // Illustrations
    property string addImage: "/resources/images/illustrations/add_image_illustration.svg"
    property string bookCover: "/resources/images/illustrations/book_cover.png"
    property string emptyHomeBackground: "/resources/images/illustrations/empty_home_background.svg"
    property string eyeCrossedSwift: "/resources/images/illustrations/eye_crossed_illustration.png"
    property string fileSwift: "/resources/images/illustrations/file_swift_illustration.svg"
    property string lockProtected: "/resources/images/illustrations/lock_protected_illustration.svg"
    property string attention: "/resources/images/illustrations/attention_illustration.svg"
    property string checkBubbleIllustration: "/resources/images/illustrations/check_bubble_illustration.svg"
    
    
    // ReadingView-icons
    property string arrowtopBlackLeft: "/resources/images/readingView/arrowtop_black_left_icon.svg"
    property string arrowtopPurpleDown: "/resources/images/readingView/arrowtop_purple_down_icon.svg"
    property string arrowtopPurpleUp: "/resources/images/readingView/arrowtop_purple_up_icon.svg"
    property string readingViewBookmarkBlack: "/resources/images/readingView/bookmark_black_icon.svg"
    property string readingViewBookmarkPurple: "/resources/images/readingView/bookmark_purple_icon.svg"
    property string readingViewMaximizeBlack: "/resources/images/readingView/maximize_black_icon.svg"
    property string readingViewMaximizePurple: "/resources/images/readingView/maximize_purple_icon.svg"
    property string readingViewMenuBlack: "/resources/images/readingView/menu_black_icon.svg"
    property string readingViewMenuPurple: "/resources/images/readingView/menu_purple_icon.svg"
    property string readingViewSearchBlackThin: "/resources/images/readingView/search_black_thin_icon.svg"
    property string readingViewSearchPurpleThin: "/resources/images/readingView/search_purple_thin_icon.svg"
    property string readingViewSidebarBlack: "/resources/images/readingView/sidebar_black_icon.svg"
    property string readingViewSidebarPurple: "/resources/images/readingView/sidebar_purple_icon.svg"
    
    // Setting-icons
    property string settingsAboutGray: "/resources/images/settings/about_gray_icon.svg"
    property string settingsAboutPurple: "/resources/images/settings/about_purple_icon.svg"
    property string settingsAccountGray: "/resources/images/settings/account_gray_icon.svg"
    property string settingsAccountPurple: "/resources/images/settings/account_purple_icon.svg"
    property string settingsAppearanceGray: "/resources/images/settings/appearance_gray_icon.svg"
    property string settingsAppearancePurple: "/resources/images/settings/appearance_purple_icon.svg"
    property string settingsSettingsGray: "/resources/images/settings/settings_gray_icon.svg"
    property string settingsSettingsPurple: "/resources/images/settings/settings_purple_icon.svg"
    property string settingsShortcutsGray: "/resources/images/settings/shortcuts_gray_icon.svg"
    property string settingsShortcutsPurple: "/resources/images/settings/shortcuts_purple_icon.svg"
    property string settingsStorageGray: "/resources/images/settings/storage_gray_icon.svg"
    property string settingsStoragePurple: "/resources/images/settings/storage_purple_icon.svg"
    property string settingsSupportUsGray: "/resources/images/settings/support_us_gray_icon.svg"
    property string settingsSupportUsPurple: "/resources/images/settings/support_us_purple_icon.svg"
    property string settingsUpdatesGray: "/resources/images/settings/updates_gray_icon.svg"
    property string settingsUpdatesPurple: "/resources/images/settings/updates_purple_icon.svg"
    
    // Sidebar-icons
    property string emptyProfile : "/resources/images/sidebar/empty_profile_icon.svg"
    property string home: "/resources/images/sidebar/home_icon.svg"
    property string pieChart: "/resources/images/sidebar/pieChart_icon.svg"
    property string tool: "/resources/images/sidebar/wrench_icon.svg"
    property string settings: "/resources/images/sidebar/settings_icon.svg"
    property string addOns: "/resources/images/sidebar/puzzle_icon.svg"
    
    // Utility-icons
    property string dots: "/resources/images/utility/dots_icon.svg"
    property string downloadBlack: "/resources/images/utility/download_black_icon.svg"
    property string downloadGray: "/resources/images/utility/download_gray_icon.svg"
    property string downloadWhite: "/resources/images/utility/download_white_icon.svg"
    property string edit: "/resources/images/utility/edit_icon.svg"
    property string eyeOff: "/resources/images/utility/eye_off_icon.svg"
    property string eyeOn: "/resources/images/utility/eye_on_icon.svg"
    property string filter: "/resources/images/utility/filter_icon.svg"
    property string logout: "/resources/images/utility/log_out_icon.svg"
    property string searchBlack: "/resources/images/utility/search_black_icon.svg"
    property string searchGray: "/resources/images/utility/search_gray_icon.svg"
    property string sync: "/resources/images/utility/sync_icon.svg"
    property string tag: "/resources/images/utility/tag_icon.svg"
    property string trash: "/resources/images/utility/trash_icon.svg"
    property string printer: "/resources/images/utility/printer_black_icon.svg"
    property string settingsGrayHallow: "/resources/images/utility/settings_gray_hallow_icon.svg"
    property string share: "/resources/images/utility/share_black_icon.svg"
    property string sun: "/resources/images/utility/sun_gray_icon.svg"
    property string microphone: "/resources/images/utility/microphone_icon.svg"
}
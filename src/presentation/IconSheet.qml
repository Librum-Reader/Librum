pragma Singleton
import QtQuick 2.15

Item
{
    id: iconSheet
    
    // Arrows-icons
    property string arrowheadBackIcon: "/resources/images/light_mode/arrows/arrowhead_back_icon.svg"
    property string arrowheadNextIcon: "/resources/images/light_mode/arrows/arrowhead_next_icon.svg"
    property string dropdownDark: "/resources/images/light_mode/arrows/dropdown_dark.svg"
    property string dropdownLight: "/resources/images/light_mode/arrows/dropdown_light.svg"
    
    // Decision-icons
    property string checkGreen: "/resources/images/light_mode/decision/check_green_icon.svg"
    property string checkWhite: "/resources/images/light_mode/decision/check_white_icon.svg"
    property string closePopup: "/resources/images/light_mode/decision/close_popup_icon.svg"
    property string removeItem: "/resources/images/light_mode/decision/remove_item_icon.svg"
    property string removeItemFocused: "/resources/images/light_mode/decision/remove_item_focused_icon.svg"
    property string cancelPurple: "/resources/images/light_mode/decision/cancel_purple_icon.svg"
    property string addFirstBookPlus: "/resources/images/light_mode/decision/add_first_book_plus_icon.svg"
    property string addWhite: "/resources/images/light_mode/decision/add_white_icon.svg"
    property string plusSquare: "/resources/images/light_mode/decision/plus_square_icon.svg"
    property string checkCircle: "/resources/images/light_mode/decision/check_circle_icon.svg"
    
    // Book popup
    property string bookPopupUninstall: "/resources/images/light_mode/book_popup/uninstall_icon.svg"
    property string bookPopupRead: "/resources/images/light_mode/book_popup/read_icon.svg"
    property string bookPopupDetails: "/resources/images/light_mode/book_popup/details_icon.svg"
    property string bookPopupSave: "/resources/images/light_mode/book_popup/save_icon.svg"
    property string bookPopupTags: "/resources/images/light_mode/book_popup/tags_icon.svg"
    property string bookPopupMarkAsRead: "/resources/images/light_mode/book_popup/mark_as_read_icon.svg"
    property string bookPopupDelete: "/resources/images/light_mode/book_popup/delete_icon.svg"
    
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
    
    // ReadingSearchbar-icons
    property string readingSearchbarCancel: "/resources/images/light_mode/reading_searchbar/cancel_icon.svg"
    property string readingSearchbarNext: "/resources/images/light_mode/reading_searchbar/next_icon.svg"
    property string readingSearchbarPrevious: "/resources/images/light_mode/reading_searchbar/previous_icon.svg"
    
    // ReadingView-icons
    property string readingViewBack: "/resources/images/light_mode/reading_view/back_icon.svg"
    property string readingViewBookmark: "/resources/images/light_mode/reading_view/bookmark_icon.svg"
    property string readingViewBookmarkSelected: "/resources/images/light_mode/reading_view/bookmark_selected_icon.svg"
    property string readingViewChapters: "/resources/images/light_mode/reading_view/chapters_icon.svg"
    property string readingViewChaptersSelected: "/resources/images/light_mode/reading_view/chapters_selected_icon.svg"
    property string readingViewMaximize: "/resources/images/light_mode/reading_view/maximize_icon.svg"
    property string readingViewMaximizeSelected: "/resources/images/light_mode/reading_view/maximize_selected_icon.svg"
    property string readingViewOptions: "/resources/images/light_mode/reading_view/options_icon.svg"
    property string readingViewOptionsPurple: "/resources/images/light_mode/reading_view/options_selected_icon.svg"
    property string readingViewSearch: "/resources/images/light_mode/reading_view/search_icon.svg"
    property string readingViewSearchSelected: "/resources/images/light_mode/reading_view/search_selected_icon.svg"
    
    // Setting-icons
    property string settingsSidebarAbout: "/resources/images/light_mode/settings_sidebar/about_icon.svg"
    property string settingsSidebarAboutSelected: "/resources/images/light_mode/settings_sidebar/about_selected_icon.svg"
    property string settingsSidebarAccount: "/resources/images/light_mode/settings_sidebar/account_icon.svg"
    property string settingsSidebarAccountSelected: "/resources/images/light_mode/settings_sidebar/account_selected_icon.svg"
    property string settingsSidebarAppearance: "/resources/images/light_mode/settings_sidebar/appearance_icon.svg"
    property string settingsSidebarAppearanceSelected: "/resources/images/light_mode/settings_sidebar/appearance_selected_icon.svg"
    property string settingsSidebarSettings: "/resources/images/light_mode/settings_sidebar/settings_icon.svg"
    property string settingsSidebarSettingsSelected: "/resources/images/light_mode/settings_sidebar/settings_selected_icon.svg"
    property string settingsSidebarShortcuts: "/resources/images/light_mode/settings_sidebar/shortcuts_icon.svg"
    property string settingsSidebarShortcutsSelected: "/resources/images/light_mode/settings_sidebar/shortcuts_selected_icon.svg"
    property string settingsSidebarStorage: "/resources/images/light_mode/settings_sidebar/storage_icon.svg"
    property string settingsSidebarStorageSelected: "/resources/images/light_mode/settings_sidebar/storage_selected_icon.svg"
    property string settingsSidebarSupportUs: "/resources/images/light_mode/settings_sidebar/support_us_icon.svg"
    property string settingsSidebarSupportUsSelected: "/resources/images/light_mode/settings_sidebar/support_us_selected_icon.svg"
    property string settingsSidebarUpdates: "/resources/images/light_mode/settings_sidebar/updates_icon.svg"
    property string settingsSidebarUpdatesSelected: "/resources/images/light_mode/settings_sidebar/updates_selected_icon.svg"
    
    // Sidebar-icons
    property string sidebarDownload: "/resources/images/light_mode/sidebar/download_icon.svg"
    property string sidebarEmptyProfile : "/resources/images/light_mode/sidebar/empty_profile_icon.svg"
    property string sidebarHome: "/resources/images/light_mode/sidebar/home_icon.svg"
    property string sidebarPieChart: "/resources/images/light_mode/sidebar/pieChart_icon.svg"
    property string sidebarSettings: "/resources/images/light_mode/sidebar/settings_icon.svg"
    property string sidebarAddOns: "/resources/images/light_mode/sidebar/puzzle_icon.svg"
    
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
    property string printer: "/resources/images/light_mode/utility/printer_black_icon.svg"
    property string settingsGrayHallow: "/resources/images/light_mode/utility/settings_gray_hallow_icon.svg"
    property string share: "/resources/images/light_mode/utility/share_black_icon.svg"
    property string sun: "/resources/images/light_mode/utility/sun_gray_icon.svg"
    property string microphone: "/resources/images/light_mode/utility/microphone_icon.svg"
    property string activeMicrophone: "/resources/images/light_mode/utility/active_microphone_icon.svg"
    property string trashGray: "/resources/images/light_mode/utility/trash_gray_icon.svg"
    property string trashWhite: "/resources/images/light_mode/utility/trash_white_icon.svg"
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
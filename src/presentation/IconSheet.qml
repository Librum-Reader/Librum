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
    property string cancelPurple: "/resources/images/light_mode/decision/cancel_purple_icon.svg"
    property string addFirstBookPlus: "/resources/images/light_mode/decision/add_first_book_plus_icon.svg"
    property string addWhite: "/resources/images/light_mode/decision/add_white_icon.svg"
    property string plusSquare: "/resources/images/light_mode/decision/plus_square_icon.svg"
    property string checkCircle: "/resources/images/light_mode/decision/check_circle_icon.svg"
    
    // Tags
    property string tagsRemove: "/resources/images/light_mode/tags/remove_icon.svg"
    property string tagsRemoveSelected: "/resources/images/light_mode/tags/remove_selected_icon.svg"
    property string tagsEdit: "/resources/images/light_mode/tags/edit_icon.svg"
    property string tagsEditSelected: "/resources/images/light_mode/tags/edit_selected_icon.svg"
    
    // Book popup
    property string bookPopupDownload: "/resources/images/light_mode/book_popup/download_icon.svg"
    property string bookPopupUninstall: "/resources/images/light_mode/book_popup/uninstall_icon.svg"
    property string bookPopupRead: "/resources/images/light_mode/book_popup/read_icon.svg"
    property string bookPopupDetails: "/resources/images/light_mode/book_popup/details_icon.svg"
    property string bookPopupSave: "/resources/images/light_mode/book_popup/save_icon.svg"
    property string bookPopupTags: "/resources/images/light_mode/book_popup/tags_icon.svg"
    property string bookPopupMarkAsRead: "/resources/images/light_mode/book_popup/mark_as_read_icon.svg"
    property string bookPopupDelete: "/resources/images/light_mode/book_popup/delete_icon.svg"
    
    // Decoration
    property string heart: "/resources/images/light_mode/decoration/heart_icon.svg"
    property string heartHallow: "/resources/images/light_mode/decoration/heart_hallow_icon.svg"
    property string seed: "/resources/images/light_mode/decoration/seed_icon.svg"
    property string popupDroplet: "/resources/images/light_mode/decoration/popup_droplet_icon.svg"
    property string patreon: "/resources/images/light_mode/decoration/patreon_icon.svg"
    
    // Illustrations
    property string addImage: "/resources/images/light_mode/illustrations/add_image_illustration.svg"
    property string bookCover: "/resources/images/light_mode/illustrations/book_cover.png"
    property string emptyHomeBackground: "/resources/images/light_mode/illustrations/empty_home_background.svg"
    property string fileSwift: "/resources/images/light_mode/illustrations/file_swift_illustration.svg"
    property string lockProtected: "/resources/images/light_mode/illustrations/lock_protected_illustration.svg"
    property string attentionPurple: "/resources/images/light_mode/illustrations/attention_purple_illustration.svg"
    property string checkBubbleIllustration: "/resources/images/light_mode/illustrations/check_bubble_illustration.svg"
    property string addFileIllustration: "/resources/images/light_mode/illustrations/add_file_illustration.svg"
    
    // Reading searchbar
    property string readingSearchbarCancel: "/resources/images/light_mode/reading_searchbar/cancel_icon.svg"
    property string readingSearchbarNext: "/resources/images/light_mode/reading_searchbar/next_icon.svg"
    property string readingSearchbarPrevious: "/resources/images/light_mode/reading_searchbar/previous_icon.svg"
    property string readingSearchbarSearch: "/resources/images/light_mode/reading_searchbar/search_icon.svg"
    
    // ReadingView
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
    
    // Reading options popup
    property string readingOptionsPopupPrinter: "/resources/images/light_mode/reading_options_popup/printer_icon.svg"
    property string readingOptionsPopupDownload: "/resources/images/light_mode/reading_options_popup/download_icon.svg"
    property string readingOptionsPopupShare: "/resources/images/light_mode/reading_options_popup/share_icon.svg"
    property string readingOptionsPopupSun: "/resources/images/light_mode/reading_options_popup/sun_icon.svg"
    property string readingOptionsPopupSync: "/resources/images/light_mode/reading_options_popup/sync_icon.svg"
    property string readingOptionsSettings: "/resources/images/light_mode/reading_options_popup/settings_icon.svg"
    
    // Setting
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
    
    // Sidebar
    property string sidebarDownload: "/resources/images/light_mode/sidebar/download_icon.svg"
    property string sidebarHome: "/resources/images/light_mode/sidebar/home_icon.svg"
    property string sidebarPieChart: "/resources/images/light_mode/sidebar/pieChart_icon.svg"
    property string sidebarSettings: "/resources/images/light_mode/sidebar/settings_icon.svg"
    property string sidebarAddOns: "/resources/images/light_mode/sidebar/puzzle_icon.svg"
    property string sidebarSync: "/resources/images/light_mode/sidebar/sync_icon.svg"
    property string sidebarEmptyProfile : "/resources/images/light_mode/sidebar/empty_profile_icon.svg"
    property string sidebarLogout: "/resources/images/light_mode/sidebar/log_out_icon.svg"
    
    // Utility
    property string dots: "/resources/images/light_mode/utility/dots_icon.svg"
    property string download: "/resources/images/light_mode/utility/download_icon.svg"
    property string downloadSelected: "/resources/images/light_mode/utility/download_selected_icon.svg"
    property string edit: "/resources/images/light_mode/utility/edit_icon.svg"
    property string eyeOff: "/resources/images/light_mode/utility/eye_off_icon.svg"
    property string eyeOn: "/resources/images/light_mode/utility/eye_on_icon.svg"
    property string filter: "/resources/images/light_mode/utility/filter_icon.svg"
    property string search: "/resources/images/light_mode/utility/search_icon.svg"
    property string tag: "/resources/images/light_mode/utility/tag_icon.svg"
    property string microphone: "/resources/images/light_mode/utility/microphone_icon.svg"
    property string activeMicrophone: "/resources/images/light_mode/utility/active_microphone_icon.svg"
    property string trash: "/resources/images/light_mode/utility/trash_icon.svg"
    property string trashHighlighted: "/resources/images/light_mode/utility/trash_highlighted_icon.svg"
    
    
    
    state: "Light"
    states: [
        State
        {
            name: "Light"
            
            // Arrows-icons
            PropertyChanges { target: iconSheet; arrowheadBackIcon: "/resources/images/light_mode/arrows/arrowhead_back_icon.svg" }
            PropertyChanges { target: iconSheet; arrowheadNextIcon: "/resources/images/light_mode/arrows/arrowhead_next_icon.svg" }
            PropertyChanges { target: iconSheet; dropdownDark: "/resources/images/light_mode/arrows/dropdown_dark.svg" }
            PropertyChanges { target: iconSheet; dropdownLight: "/resources/images/light_mode/arrows/dropdown_light.svg" }
            
            // Decision-icons
            PropertyChanges { target: iconSheet; checkGreen: "/resources/images/light_mode/decision/check_green_icon.svg" }
            PropertyChanges { target: iconSheet; checkWhite: "/resources/images/light_mode/decision/check_white_icon.svg" }
            PropertyChanges { target: iconSheet; closePopup: "/resources/images/light_mode/decision/close_popup_icon.svg" }
            PropertyChanges { target: iconSheet; cancelPurple: "/resources/images/light_mode/decision/cancel_purple_icon.svg" }
            PropertyChanges { target: iconSheet; addFirstBookPlus: "/resources/images/light_mode/decision/add_first_book_plus_icon.svg" }
            PropertyChanges { target: iconSheet; addWhite: "/resources/images/light_mode/decision/add_white_icon.svg" }
            PropertyChanges { target: iconSheet; plusSquare: "/resources/images/light_mode/decision/plus_square_icon.svg" }
            PropertyChanges { target: iconSheet; checkCircle: "/resources/images/light_mode/decision/check_circle_icon.svg" }
            
            // Tags
            PropertyChanges { target: iconSheet; tagsRemove: "/resources/images/light_mode/tags/remove_icon.svg" }
            PropertyChanges { target: iconSheet; tagsRemoveSelected: "/resources/images/light_mode/tags/remove_selected_icon.svg" }
            PropertyChanges { target: iconSheet; tagsEdit: "/resources/images/light_mode/tags/edit_icon.svg" }
            PropertyChanges { target: iconSheet; tagsEditSelected: "/resources/images/light_mode/tags/edit_selected_icon.svg" }
            
            // Book popup
            PropertyChanges { target: iconSheet; bookPopupDownload: "/resources/images/light_mode/book_popup/download_icon.svg" }
            PropertyChanges { target: iconSheet; bookPopupUninstall: "/resources/images/light_mode/book_popup/uninstall_icon.svg" }
            PropertyChanges { target: iconSheet; bookPopupRead: "/resources/images/light_mode/book_popup/read_icon.svg" }
            PropertyChanges { target: iconSheet; bookPopupDetails: "/resources/images/light_mode/book_popup/details_icon.svg" }
            PropertyChanges { target: iconSheet; bookPopupSave: "/resources/images/light_mode/book_popup/save_icon.svg" }
            PropertyChanges { target: iconSheet; bookPopupTags: "/resources/images/light_mode/book_popup/tags_icon.svg" }
            PropertyChanges { target: iconSheet; bookPopupMarkAsRead: "/resources/images/light_mode/book_popup/mark_as_read_icon.svg" }
            PropertyChanges { target: iconSheet; bookPopupDelete: "/resources/images/light_mode/book_popup/delete_icon.svg" }
            
            // Decoration
            PropertyChanges { target: iconSheet; heart: "/resources/images/light_mode/decoration/heart_icon.svg" }
            PropertyChanges { target: iconSheet; heartHallow: "/resources/images/light_mode/decoration/heart_hallow_icon.svg" }
            PropertyChanges { target: iconSheet; seed: "/resources/images/light_mode/decoration/seed_icon.svg" }
            PropertyChanges { target: iconSheet; popupDroplet: "/resources/images/light_mode/decoration/popup_droplet_icon.svg" }
            PropertyChanges { target: iconSheet; patreon: "/resources/images/light_mode/decoration/patreon_icon.svg" }
            
            // Illustrations
            PropertyChanges { target: iconSheet; addImage: "/resources/images/light_mode/illustrations/add_image_illustration.svg" }
            PropertyChanges { target: iconSheet; bookCover: "/resources/images/light_mode/illustrations/book_cover.png" }
            PropertyChanges { target: iconSheet; emptyHomeBackground: "/resources/images/light_mode/illustrations/empty_home_background.svg" }
            PropertyChanges { target: iconSheet; fileSwift: "/resources/images/light_mode/illustrations/file_swift_illustration.svg" }
            PropertyChanges { target: iconSheet; lockProtected: "/resources/images/light_mode/illustrations/lock_protected_illustration.svg" }
            PropertyChanges { target: iconSheet; attentionPurple: "/resources/images/light_mode/illustrations/attention_purple_illustration.svg" }
            PropertyChanges { target: iconSheet; checkBubbleIllustration: "/resources/images/light_mode/illustrations/check_bubble_illustration.svg" }
            PropertyChanges { target: iconSheet; addFileIllustration: "/resources/images/light_mode/illustrations/add_file_illustration.svg" }
            
            // Reading searchbar
            PropertyChanges { target: iconSheet; readingSearchbarCancel: "/resources/images/light_mode/reading_searchbar/cancel_icon.svg" }
            PropertyChanges { target: iconSheet; readingSearchbarNext: "/resources/images/light_mode/reading_searchbar/next_icon.svg" }
            PropertyChanges { target: iconSheet; readingSearchbarPrevious: "/resources/images/light_mode/reading_searchbar/previous_icon.svg" }
            PropertyChanges { target: iconSheet; readingSearchbarSearch: "/resources/images/light_mode/reading_searchbar/search_icon.svg" }
            
            // ReadingView
            PropertyChanges { target: iconSheet; readingViewBack: "/resources/images/light_mode/reading_view/back_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewBookmark: "/resources/images/light_mode/reading_view/bookmark_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewBookmarkSelected: "/resources/images/light_mode/reading_view/bookmark_selected_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewChapters: "/resources/images/light_mode/reading_view/chapters_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewChaptersSelected: "/resources/images/light_mode/reading_view/chapters_selected_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewMaximize: "/resources/images/light_mode/reading_view/maximize_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewMaximizeSelected: "/resources/images/light_mode/reading_view/maximize_selected_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewOptions: "/resources/images/light_mode/reading_view/options_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewOptionsPurple: "/resources/images/light_mode/reading_view/options_selected_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewSearch: "/resources/images/light_mode/reading_view/search_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewSearchSelected: "/resources/images/light_mode/reading_view/search_selected_icon.svg" }
            
            // Reading options popup
            PropertyChanges { target: iconSheet; readingOptionsPopupPrinter: "/resources/images/light_mode/reading_options_popup/printer_icon.svg" }
            PropertyChanges { target: iconSheet; readingOptionsPopupDownload: "/resources/images/light_mode/reading_options_popup/download_icon.svg" }
            PropertyChanges { target: iconSheet; readingOptionsPopupShare: "/resources/images/light_mode/reading_options_popup/share_icon.svg" }
            PropertyChanges { target: iconSheet; readingOptionsPopupSun: "/resources/images/light_mode/reading_options_popup/sun_icon.svg" }
            PropertyChanges { target: iconSheet; readingOptionsPopupSync: "/resources/images/light_mode/reading_options_popup/sync_icon.svg" }
            PropertyChanges { target: iconSheet; readingOptionsSettings: "/resources/images/light_mode/reading_options_popup/settings_icon.svg" }
            
            // Setting
            PropertyChanges { target: iconSheet; settingsSidebarAbout: "/resources/images/light_mode/settings_sidebar/about_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarAboutSelected: "/resources/images/light_mode/settings_sidebar/about_selected_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarAccount: "/resources/images/light_mode/settings_sidebar/account_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarAccountSelected: "/resources/images/light_mode/settings_sidebar/account_selected_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarAppearance: "/resources/images/light_mode/settings_sidebar/appearance_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarAppearanceSelected: "/resources/images/light_mode/settings_sidebar/appearance_selected_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarSettings: "/resources/images/light_mode/settings_sidebar/settings_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarSettingsSelected: "/resources/images/light_mode/settings_sidebar/settings_selected_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarShortcuts: "/resources/images/light_mode/settings_sidebar/shortcuts_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarShortcutsSelected: "/resources/images/light_mode/settings_sidebar/shortcuts_selected_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarStorage: "/resources/images/light_mode/settings_sidebar/storage_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarStorageSelected: "/resources/images/light_mode/settings_sidebar/storage_selected_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarSupportUs: "/resources/images/light_mode/settings_sidebar/support_us_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarSupportUsSelected: "/resources/images/light_mode/settings_sidebar/support_us_selected_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarUpdates: "/resources/images/light_mode/settings_sidebar/updates_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarUpdatesSelected: "/resources/images/light_mode/settings_sidebar/updates_selected_icon.svg" }
            
            // Sidebar
            PropertyChanges { target: iconSheet; sidebarDownload: "/resources/images/light_mode/sidebar/download_icon.svg" }
            PropertyChanges { target: iconSheet; sidebarHome: "/resources/images/light_mode/sidebar/home_icon.svg" }
            PropertyChanges { target: iconSheet; sidebarPieChart: "/resources/images/light_mode/sidebar/pieChart_icon.svg" }
            PropertyChanges { target: iconSheet; sidebarSettings: "/resources/images/light_mode/sidebar/settings_icon.svg" }
            PropertyChanges { target: iconSheet; sidebarAddOns: "/resources/images/light_mode/sidebar/puzzle_icon.svg" }
            PropertyChanges { target: iconSheet; sidebarSync: "/resources/images/light_mode/sidebar/sync_icon.svg" }
            PropertyChanges { target: iconSheet; sidebarEmptyProfile : "/resources/images/light_mode/sidebar/empty_profile_icon.svg" }
            PropertyChanges { target: iconSheet; sidebarLogout: "/resources/images/light_mode/sidebar/log_out_icon.svg" }
            
            // Utility
            PropertyChanges { target: iconSheet; dots: "/resources/images/light_mode/utility/dots_icon.svg" }
            PropertyChanges { target: iconSheet; download: "/resources/images/light_mode/utility/download_icon.svg" }
            PropertyChanges { target: iconSheet; downloadSelected: "/resources/images/light_mode/utility/download_selected_icon.svg" }
            PropertyChanges { target: iconSheet; edit: "/resources/images/light_mode/utility/edit_icon.svg" }
            PropertyChanges { target: iconSheet; eyeOff: "/resources/images/light_mode/utility/eye_off_icon.svg" }
            PropertyChanges { target: iconSheet; eyeOn: "/resources/images/light_mode/utility/eye_on_icon.svg" }
            PropertyChanges { target: iconSheet; filter: "/resources/images/light_mode/utility/filter_icon.svg" }
            PropertyChanges { target: iconSheet; search: "/resources/images/light_mode/utility/search_icon.svg" }
            PropertyChanges { target: iconSheet; tag: "/resources/images/light_mode/utility/tag_icon.svg" }
            PropertyChanges { target: iconSheet; microphone: "/resources/images/light_mode/utility/microphone_icon.svg" }
            PropertyChanges { target: iconSheet; activeMicrophone: "/resources/images/light_mode/utility/active_microphone_icon.svg" }
            PropertyChanges { target: iconSheet; trash: "/resources/images/light_mode/utility/trash_icon.svg" }
            PropertyChanges { target: iconSheet; trashHighlighted: "/resources/images/light_mode/utility/trash_highlighted_icon.svg" }
        },
        State
        {
            name: "Dark"
            
            // Arrows-icons
            PropertyChanges { target: iconSheet; arrowheadBackIcon: "/resources/images/dark_mode/arrows/arrowhead_back_icon.svg" }
            PropertyChanges { target: iconSheet; arrowheadNextIcon: "/resources/images/dark_mode/arrows/arrowhead_next_icon.svg" }
            PropertyChanges { target: iconSheet; dropdownDark: "/resources/images/dark_mode/arrows/dropdown_dark.svg" }
            PropertyChanges { target: iconSheet; dropdownLight: "/resources/images/dark_mode/arrows/dropdown_light.svg" }
            
            // Decision-icons
            PropertyChanges { target: iconSheet; checkGreen: "/resources/images/dark_mode/decision/check_green_icon.svg" }
            PropertyChanges { target: iconSheet; checkWhite: "/resources/images/dark_mode/decision/check_white_icon.svg" }
            PropertyChanges { target: iconSheet; closePopup: "/resources/images/dark_mode/decision/close_popup_icon.svg" }
            PropertyChanges { target: iconSheet; cancelPurple: "/resources/images/dark_mode/decision/cancel_purple_icon.svg" }
            PropertyChanges { target: iconSheet; addFirstBookPlus: "/resources/images/dark_mode/decision/add_first_book_plus_icon.svg" }
            PropertyChanges { target: iconSheet; addWhite: "/resources/images/dark_mode/decision/add_white_icon.svg" }
            PropertyChanges { target: iconSheet; plusSquare: "/resources/images/dark_mode/decision/plus_square_icon.svg" }
            PropertyChanges { target: iconSheet; checkCircle: "/resources/images/dark_mode/decision/check_circle_icon.svg" }
            
            // Tags
            PropertyChanges { target: iconSheet; tagsRemove: "/resources/images/dark_mode/tags/remove_icon.svg" }
            PropertyChanges { target: iconSheet; tagsRemoveSelected: "/resources/images/dark_mode/tags/remove_selected_icon.svg" }
            PropertyChanges { target: iconSheet; tagsEdit: "/resources/images/dark_mode/tags/edit_icon.svg" }
            PropertyChanges { target: iconSheet; tagsEditSelected: "/resources/images/dark_mode/tags/edit_selected_icon.svg" }
            
            // Book popup
            PropertyChanges { target: iconSheet; bookPopupDownload: "/resources/images/dark_mode/book_popup/download_icon.svg" }
            PropertyChanges { target: iconSheet; bookPopupUninstall: "/resources/images/dark_mode/book_popup/uninstall_icon.svg" }
            PropertyChanges { target: iconSheet; bookPopupRead: "/resources/images/dark_mode/book_popup/read_icon.svg" }
            PropertyChanges { target: iconSheet; bookPopupDetails: "/resources/images/dark_mode/book_popup/details_icon.svg" }
            PropertyChanges { target: iconSheet; bookPopupSave: "/resources/images/dark_mode/book_popup/save_icon.svg" }
            PropertyChanges { target: iconSheet; bookPopupTags: "/resources/images/dark_mode/book_popup/tags_icon.svg" }
            PropertyChanges { target: iconSheet; bookPopupMarkAsRead: "/resources/images/dark_mode/book_popup/mark_as_read_icon.svg" }
            PropertyChanges { target: iconSheet; bookPopupDelete: "/resources/images/dark_mode/book_popup/delete_icon.svg" }
            
            // Decoration
            PropertyChanges { target: iconSheet; heart: "/resources/images/dark_mode/decoration/heart_icon.svg" }
            PropertyChanges { target: iconSheet; heartHallow: "/resources/images/dark_mode/decoration/heart_hallow_icon.svg" }
            PropertyChanges { target: iconSheet; seed: "/resources/images/dark_mode/decoration/seed_icon.svg" }
            PropertyChanges { target: iconSheet; popupDroplet: "/resources/images/dark_mode/decoration/popup_droplet_icon.svg" }
            PropertyChanges { target: iconSheet; patreon: "/resources/images/dark_mode/decoration/patreon_icon.svg" }
            
            // Illustrations
            PropertyChanges { target: iconSheet; addImage: "/resources/images/dark_mode/illustrations/add_image_illustration.svg" }
            PropertyChanges { target: iconSheet; bookCover: "/resources/images/dark_mode/illustrations/book_cover.png" }
            PropertyChanges { target: iconSheet; emptyHomeBackground: "/resources/images/dark_mode/illustrations/empty_home_background.svg" }
            PropertyChanges { target: iconSheet; fileSwift: "/resources/images/dark_mode/illustrations/file_swift_illustration.svg" }
            PropertyChanges { target: iconSheet; lockProtected: "/resources/images/dark_mode/illustrations/lock_protected_illustration.svg" }
            PropertyChanges { target: iconSheet; attentionPurple: "/resources/images/dark_mode/illustrations/attention_purple_illustration.svg" }
            PropertyChanges { target: iconSheet; checkBubbleIllustration: "/resources/images/dark_mode/illustrations/check_bubble_illustration.svg" }
            PropertyChanges { target: iconSheet; addFileIllustration: "/resources/images/dark_mode/illustrations/add_file_illustration.svg" }
            
            // Reading searchbar
            PropertyChanges { target: iconSheet; readingSearchbarCancel: "/resources/images/dark_mode/reading_searchbar/cancel_icon.svg" }
            PropertyChanges { target: iconSheet; readingSearchbarNext: "/resources/images/dark_mode/reading_searchbar/next_icon.svg" }
            PropertyChanges { target: iconSheet; readingSearchbarPrevious: "/resources/images/dark_mode/reading_searchbar/previous_icon.svg" }
            PropertyChanges { target: iconSheet; readingSearchbarSearch: "/resources/images/dark_mode/reading_searchbar/search_icon.svg" }
            
            // ReadingView
            PropertyChanges { target: iconSheet; readingViewBack: "/resources/images/dark_mode/reading_view/back_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewBookmark: "/resources/images/dark_mode/reading_view/bookmark_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewBookmarkSelected: "/resources/images/dark_mode/reading_view/bookmark_selected_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewChapters: "/resources/images/dark_mode/reading_view/chapters_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewChaptersSelected: "/resources/images/dark_mode/reading_view/chapters_selected_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewMaximize: "/resources/images/dark_mode/reading_view/maximize_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewMaximizeSelected: "/resources/images/dark_mode/reading_view/maximize_selected_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewOptions: "/resources/images/dark_mode/reading_view/options_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewOptionsPurple: "/resources/images/dark_mode/reading_view/options_selected_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewSearch: "/resources/images/dark_mode/reading_view/search_icon.svg" }
            PropertyChanges { target: iconSheet; readingViewSearchSelected: "/resources/images/dark_mode/reading_view/search_selected_icon.svg" }
            
            // Reading options popup
            PropertyChanges { target: iconSheet; readingOptionsPopupPrinter: "/resources/images/dark_mode/reading_options_popup/printer_icon.svg" }
            PropertyChanges { target: iconSheet; readingOptionsPopupDownload: "/resources/images/dark_mode/reading_options_popup/download_icon.svg" }
            PropertyChanges { target: iconSheet; readingOptionsPopupShare: "/resources/images/dark_mode/reading_options_popup/share_icon.svg" }
            PropertyChanges { target: iconSheet; readingOptionsPopupSun: "/resources/images/dark_mode/reading_options_popup/sun_icon.svg" }
            PropertyChanges { target: iconSheet; readingOptionsPopupSync: "/resources/images/dark_mode/reading_options_popup/sync_icon.svg" }
            PropertyChanges { target: iconSheet; readingOptionsSettings: "/resources/images/dark_mode/reading_options_popup/settings_icon.svg" }
            
            // Setting
            PropertyChanges { target: iconSheet; settingsSidebarAbout: "/resources/images/dark_mode/settings_sidebar/about_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarAboutSelected: "/resources/images/dark_mode/settings_sidebar/about_selected_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarAccount: "/resources/images/dark_mode/settings_sidebar/account_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarAccountSelected: "/resources/images/dark_mode/settings_sidebar/account_selected_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarAppearance: "/resources/images/dark_mode/settings_sidebar/appearance_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarAppearanceSelected: "/resources/images/dark_mode/settings_sidebar/appearance_selected_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarSettings: "/resources/images/dark_mode/settings_sidebar/settings_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarSettingsSelected: "/resources/images/dark_mode/settings_sidebar/settings_selected_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarShortcuts: "/resources/images/dark_mode/settings_sidebar/shortcuts_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarShortcutsSelected: "/resources/images/dark_mode/settings_sidebar/shortcuts_selected_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarStorage: "/resources/images/dark_mode/settings_sidebar/storage_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarStorageSelected: "/resources/images/dark_mode/settings_sidebar/storage_selected_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarSupportUs: "/resources/images/dark_mode/settings_sidebar/support_us_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarSupportUsSelected: "/resources/images/dark_mode/settings_sidebar/support_us_selected_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarUpdates: "/resources/images/dark_mode/settings_sidebar/updates_icon.svg" }
            PropertyChanges { target: iconSheet; settingsSidebarUpdatesSelected: "/resources/images/dark_mode/settings_sidebar/updates_selected_icon.svg" }
            
            // Sidebar
            PropertyChanges { target: iconSheet; sidebarDownload: "/resources/images/dark_mode/sidebar/download_icon.svg" }
            PropertyChanges { target: iconSheet; sidebarHome: "/resources/images/dark_mode/sidebar/home_icon.svg" }
            PropertyChanges { target: iconSheet; sidebarPieChart: "/resources/images/dark_mode/sidebar/pieChart_icon.svg" }
            PropertyChanges { target: iconSheet; sidebarSettings: "/resources/images/dark_mode/sidebar/settings_icon.svg" }
            PropertyChanges { target: iconSheet; sidebarAddOns: "/resources/images/dark_mode/sidebar/puzzle_icon.svg" }
            PropertyChanges { target: iconSheet; sidebarSync: "/resources/images/dark_mode/sidebar/sync_icon.svg" }
            PropertyChanges { target: iconSheet; sidebarEmptyProfile : "/resources/images/dark_mode/sidebar/empty_profile_icon.svg" }
            PropertyChanges { target: iconSheet; sidebarLogout: "/resources/images/dark_mode/sidebar/log_out_icon.svg" }
            
            // Utility
            PropertyChanges { target: iconSheet; dots: "/resources/images/dark_mode/utility/dots_icon.svg" }
            PropertyChanges { target: iconSheet; download: "/resources/images/dark_mode/utility/download_icon.svg" }
            PropertyChanges { target: iconSheet; downloadSelected: "/resources/images/dark_mode/utility/download_selected_icon.svg" }
            PropertyChanges { target: iconSheet; edit: "/resources/images/dark_mode/utility/edit_icon.svg" }
            PropertyChanges { target: iconSheet; eyeOff: "/resources/images/dark_mode/utility/eye_off_icon.svg" }
            PropertyChanges { target: iconSheet; eyeOn: "/resources/images/dark_mode/utility/eye_on_icon.svg" }
            PropertyChanges { target: iconSheet; filter: "/resources/images/dark_mode/utility/filter_icon.svg" }
            PropertyChanges { target: iconSheet; search: "/resources/images/dark_mode/utility/search_icon.svg" }
            PropertyChanges { target: iconSheet; tag: "/resources/images/dark_mode/utility/tag_icon.svg" }
            PropertyChanges { target: iconSheet; microphone: "/resources/images/dark_mode/utility/microphone_icon.svg" }
            PropertyChanges { target: iconSheet; activeMicrophone: "/resources/images/dark_mode/utility/active_microphone_icon.svg" }
            PropertyChanges { target: iconSheet; trash: "/resources/images/dark_mode/utility/trash_icon.svg" }
            PropertyChanges { target: iconSheet; trashHighlighted: "/resources/images/dark_mode/utility/trash_highlighted_icon.svg" }
        }
    ]
    
    
    function setState(name)
    {
        state = name;
        console.log("Setting to: " + name)
    }
}
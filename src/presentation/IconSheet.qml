pragma Singleton

import QtQuick

Item {
    id: iconSheet

    // Animations
    property string loadingAnimation: "/resources/animations/loading_animation.gif"

    // Arrows-icons
    property string arrowheadBackIcon
    property string arrowheadNextIcon
    property string dropdownDark
    property string dropdownLight

    // Decision-icons
    property string checkGreen
    property string checkWhite
    property string closePopup
    property string closePopupWhite: "/resources/images/light_mode/decision/close_popup_white_icon.svg"
    property string cancelPurple
    property string addFirstBookPlus
    property string addWhite
    property string plusSquare
    property string checkCircle

    // Tags
    property string tagsRemove
    property string tagsRemoveSelected
    property string tagsEdit
    property string tagsEditSelected

    // Book popup
    property string bookPopupDownload
    property string bookPopupUninstall
    property string bookPopupRead
    property string bookPopupDetails
    property string bookPopupSave
    property string bookPopupTags
    property string bookPopupMarkAsRead
    property string bookPopupDelete

    // Decoration
    property string heart
    property string heartHallow
    property string seed
    property string popupDroplet
    property string patreon
    property string warningCircle

    // Illustrations
    property string addImage
    property string bookCover
    property string emptyHomeBackground
    property string fileSwift
    property string lockProtected
    property string attentionPurple
    property string checkBubbleIllustration
    property string addFileIllustration
    property string notFoundIllustration

    // Reading searchbar
    property string readingSearchbarCancel
    property string readingSearchbarNext
    property string readingSearchbarPrevious
    property string readingSearchbarSearch

    // ReadingView
    property string readingViewBack
    property string readingViewBookmark
    property string readingViewBookmarkSelected
    property string readingViewChapters
    property string readingViewChaptersSelected
    property string readingViewMaximize
    property string readingViewMaximizeSelected
    property string readingViewOptions
    property string readingViewOptionsPurple
    property string readingViewSearch
    property string readingViewSearchSelected

    // Reading options popup
    property string readingOptionsPopupPrinter
    property string readingOptionsPopupDownload
    property string readingOptionsPopupShare
    property string readingOptionsPopupSun
    property string readingOptionsPopupSync
    property string readingOptionsSettings
    property string readingOptionsInvertColor

    // Setting
    property string settingsSidebarAbout
    property string settingsSidebarAboutSelected
    property string settingsSidebarAccount
    property string settingsSidebarAccountSelected
    property string settingsSidebarAppearance
    property string settingsSidebarAppearanceSelected
    property string settingsSidebarSettings
    property string settingsSidebarSettingsSelected
    property string settingsSidebarShortcuts
    property string settingsSidebarShortcutsSelected
    property string settingsSidebarStorage
    property string settingsSidebarStorageSelected
    property string settingsSidebarSupportUs
    property string settingsSidebarSupportUsSelected
    property string settingsSidebarUpdates
    property string settingsSidebarUpdatesSelected

    // Sidebar
    property string sidebarDownload
    property string sidebarHome
    property string sidebarPieChart
    property string sidebarSettings
    property string sidebarAddOns
    property string sidebarSync
    property string sidebarEmptyProfile
    property string sidebarLogout

    // Utility
    property string dots
    property string downloadSelected
    property string edit
    property string eyeOff
    property string eyeOn
    property string filter
    property string search
    property string tag
    property string microphone
    property string activeMicrophone
    property string trash
    property string trashHighlighted
    property string trashRed
    property string cloudOff
    property string folder
    property string unsorted
    property string addFolderPlus
    property string bookClosed
    property string pen

    state: "Light"
    states: [
        State {
            name: "Light"

            // Arrows-icons
            PropertyChanges {
                target: iconSheet
                arrowheadBackIcon: "/resources/images/light_mode/arrows/arrowhead_back_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                arrowheadNextIcon: "/resources/images/light_mode/arrows/arrowhead_next_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                dropdownDark: "/resources/images/light_mode/arrows/dropdown_dark.svg"
            }
            PropertyChanges {
                target: iconSheet
                dropdownLight: "/resources/images/light_mode/arrows/dropdown_light.svg"
            }

            // Decision-icons
            PropertyChanges {
                target: iconSheet
                checkGreen: "/resources/images/light_mode/decision/check_green_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                checkWhite: "/resources/images/light_mode/decision/check_white_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                closePopup: "/resources/images/light_mode/decision/close_popup_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                cancelPurple: "/resources/images/light_mode/decision/cancel_purple_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                addFirstBookPlus: "/resources/images/light_mode/decision/add_first_book_plus_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                addWhite: "/resources/images/light_mode/decision/add_white_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                plusSquare: "/resources/images/light_mode/decision/plus_square_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                checkCircle: "/resources/images/light_mode/decision/check_circle_icon.svg"
            }

            // Tags
            PropertyChanges {
                target: iconSheet
                tagsRemove: "/resources/images/light_mode/tags/remove_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                tagsRemoveSelected: "/resources/images/light_mode/tags/remove_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                tagsEdit: "/resources/images/light_mode/tags/edit_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                tagsEditSelected: "/resources/images/light_mode/tags/edit_selected_icon.svg"
            }

            // Book popup
            PropertyChanges {
                target: iconSheet
                bookPopupDownload: "/resources/images/light_mode/book_popup/download_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookPopupUninstall: "/resources/images/light_mode/book_popup/uninstall_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookPopupRead: "/resources/images/light_mode/book_popup/read_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookPopupDetails: "/resources/images/light_mode/book_popup/details_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookPopupSave: "/resources/images/light_mode/book_popup/save_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookPopupTags: "/resources/images/light_mode/book_popup/tags_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookPopupMarkAsRead: "/resources/images/light_mode/book_popup/mark_as_read_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookPopupDelete: "/resources/images/light_mode/book_popup/delete_icon.svg"
            }

            // Decoration
            PropertyChanges {
                target: iconSheet
                heart: "/resources/images/light_mode/decoration/heart_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                heartHallow: "/resources/images/light_mode/decoration/heart_hallow_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                seed: "/resources/images/light_mode/decoration/seed_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                popupDroplet: "/resources/images/light_mode/decoration/popup_droplet_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                patreon: "/resources/images/light_mode/decoration/patreon_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                warningCircle: "/resources/images/light_mode/decoration/warning_circle_icon.svg"
            }

            // Illustrations
            PropertyChanges {
                target: iconSheet
                addImage: "/resources/images/light_mode/illustrations/add_image_illustration.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookCover: "/resources/images/light_mode/illustrations/book_cover.png"
            }
            PropertyChanges {
                target: iconSheet
                emptyHomeBackground: "/resources/images/light_mode/illustrations/empty_home_background.svg"
            }
            PropertyChanges {
                target: iconSheet
                fileSwift: "/resources/images/light_mode/illustrations/file_swift_illustration.svg"
            }
            PropertyChanges {
                target: iconSheet
                lockProtected: "/resources/images/light_mode/illustrations/lock_protected_illustration.svg"
            }
            PropertyChanges {
                target: iconSheet
                attentionPurple: "/resources/images/light_mode/illustrations/attention_purple_illustration.svg"
            }
            PropertyChanges {
                target: iconSheet
                checkBubbleIllustration: "/resources/images/light_mode/illustrations/check_bubble_illustration.svg"
            }
            PropertyChanges {
                target: iconSheet
                addFileIllustration: "/resources/images/light_mode/illustrations/add_file_illustration.svg"
            }
            PropertyChanges {
                target: iconSheet
                notFoundIllustration: "/resources/images/light_mode/illustrations/not_found.svg"
            }

            // Reading searchbar
            PropertyChanges {
                target: iconSheet
                readingSearchbarCancel: "/resources/images/light_mode/reading_searchbar/cancel_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingSearchbarNext: "/resources/images/light_mode/reading_searchbar/next_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingSearchbarPrevious: "/resources/images/light_mode/reading_searchbar/previous_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingSearchbarSearch: "/resources/images/light_mode/reading_searchbar/search_icon.svg"
            }

            // ReadingView
            PropertyChanges {
                target: iconSheet
                readingViewBack: "/resources/images/light_mode/reading_view/back_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewBookmark: "/resources/images/light_mode/reading_view/bookmark_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewBookmarkSelected: "/resources/images/light_mode/reading_view/bookmark_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewChapters: "/resources/images/light_mode/reading_view/chapters_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewChaptersSelected: "/resources/images/light_mode/reading_view/chapters_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewMaximize: "/resources/images/light_mode/reading_view/maximize_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewMaximizeSelected: "/resources/images/light_mode/reading_view/maximize_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewOptions: "/resources/images/light_mode/reading_view/options_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewOptionsPurple: "/resources/images/light_mode/reading_view/options_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewSearch: "/resources/images/light_mode/reading_view/search_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewSearchSelected: "/resources/images/light_mode/reading_view/search_selected_icon.svg"
            }

            // Reading options popup
            PropertyChanges {
                target: iconSheet
                readingOptionsPopupPrinter: "/resources/images/light_mode/reading_options_popup/printer_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingOptionsPopupDownload: "/resources/images/light_mode/reading_options_popup/download_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingOptionsPopupShare: "/resources/images/light_mode/reading_options_popup/share_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingOptionsPopupSun: "/resources/images/light_mode/reading_options_popup/sun_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingOptionsPopupSync: "/resources/images/light_mode/reading_options_popup/sync_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingOptionsSettings: "/resources/images/light_mode/reading_options_popup/settings_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingOptionsInvertColor: "/resources/images/light_mode/reading_options_popup/invert_color_icon.svg"
            }

            // Setting
            PropertyChanges {
                target: iconSheet
                settingsSidebarAbout: "/resources/images/light_mode/settings_sidebar/about_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarAboutSelected: "/resources/images/light_mode/settings_sidebar/about_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarAccount: "/resources/images/light_mode/settings_sidebar/account_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarAccountSelected: "/resources/images/light_mode/settings_sidebar/account_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarAppearance: "/resources/images/light_mode/settings_sidebar/appearance_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarAppearanceSelected: "/resources/images/light_mode/settings_sidebar/appearance_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarSettings: "/resources/images/light_mode/settings_sidebar/settings_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarSettingsSelected: "/resources/images/light_mode/settings_sidebar/settings_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarShortcuts: "/resources/images/light_mode/settings_sidebar/shortcuts_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarShortcutsSelected: "/resources/images/light_mode/settings_sidebar/shortcuts_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarStorage: "/resources/images/light_mode/settings_sidebar/storage_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarStorageSelected: "/resources/images/light_mode/settings_sidebar/storage_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarSupportUs: "/resources/images/light_mode/settings_sidebar/support_us_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarSupportUsSelected: "/resources/images/light_mode/settings_sidebar/support_us_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarUpdates: "/resources/images/light_mode/settings_sidebar/updates_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarUpdatesSelected: "/resources/images/light_mode/settings_sidebar/updates_selected_icon.svg"
            }

            // Sidebar
            PropertyChanges {
                target: iconSheet
                sidebarDownload: "/resources/images/light_mode/sidebar/download_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                sidebarHome: "/resources/images/light_mode/sidebar/home_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                sidebarPieChart: "/resources/images/light_mode/sidebar/pieChart_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                sidebarSettings: "/resources/images/light_mode/sidebar/settings_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                sidebarAddOns: "/resources/images/light_mode/sidebar/puzzle_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                sidebarSync: "/resources/images/light_mode/sidebar/sync_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                sidebarEmptyProfile: "/resources/images/light_mode/sidebar/empty_profile_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                sidebarLogout: "/resources/images/light_mode/sidebar/log_out_icon.svg"
            }

            // Utility
            PropertyChanges {
                target: iconSheet
                dots: "/resources/images/light_mode/utility/dots_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                downloadSelected: "/resources/images/light_mode/utility/download_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                edit: "/resources/images/light_mode/utility/edit_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                eyeOff: "/resources/images/light_mode/utility/eye_off_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                eyeOn: "/resources/images/light_mode/utility/eye_on_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                filter: "/resources/images/light_mode/utility/filter_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                search: "/resources/images/light_mode/utility/search_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                tag: "/resources/images/light_mode/utility/tag_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                microphone: "/resources/images/light_mode/utility/microphone_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                activeMicrophone: "/resources/images/light_mode/utility/active_microphone_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                trash: "/resources/images/light_mode/utility/trash_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                trashHighlighted: "/resources/images/light_mode/utility/trash_highlighted_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                trashRed: "/resources/images/light_mode/utility/trash_red_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                cloudOff: "/resources/images/light_mode/utility/cloud_off_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                folder: "/resources/images/light_mode/utility/folder_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                unsorted: "/resources/images/light_mode/utility/unsorted_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                addFolderPlus: "/resources/images/light_mode/utility/add_folder_plus_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookClosed: "/resources/images/light_mode/utility/book_closed_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                pen: "/resources/images/light_mode/utility/pen_icon.svg"
            }
        },
        State {
            name: "Dark"

            // Arrows-icons
            PropertyChanges {
                target: iconSheet
                arrowheadBackIcon: "/resources/images/dark_mode/arrows/arrowhead_back_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                arrowheadNextIcon: "/resources/images/dark_mode/arrows/arrowhead_next_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                dropdownDark: "/resources/images/dark_mode/arrows/dropdown_dark.svg"
            }
            PropertyChanges {
                target: iconSheet
                dropdownLight: "/resources/images/dark_mode/arrows/dropdown_light.svg"
            }

            // Decision-icons
            PropertyChanges {
                target: iconSheet
                checkGreen: "/resources/images/dark_mode/decision/check_green_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                checkWhite: "/resources/images/dark_mode/decision/check_white_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                closePopup: "/resources/images/dark_mode/decision/close_popup_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                cancelPurple: "/resources/images/dark_mode/decision/cancel_purple_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                addFirstBookPlus: "/resources/images/dark_mode/decision/add_first_book_plus_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                addWhite: "/resources/images/dark_mode/decision/add_white_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                plusSquare: "/resources/images/dark_mode/decision/plus_square_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                checkCircle: "/resources/images/dark_mode/decision/check_circle_icon.svg"
            }

            // Tags
            PropertyChanges {
                target: iconSheet
                tagsRemove: "/resources/images/dark_mode/tags/remove_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                tagsRemoveSelected: "/resources/images/dark_mode/tags/remove_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                tagsEdit: "/resources/images/dark_mode/tags/edit_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                tagsEditSelected: "/resources/images/dark_mode/tags/edit_selected_icon.svg"
            }

            // Book popup
            PropertyChanges {
                target: iconSheet
                bookPopupDownload: "/resources/images/dark_mode/book_popup/download_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookPopupUninstall: "/resources/images/dark_mode/book_popup/uninstall_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookPopupRead: "/resources/images/dark_mode/book_popup/read_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookPopupDetails: "/resources/images/dark_mode/book_popup/details_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookPopupSave: "/resources/images/dark_mode/book_popup/save_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookPopupTags: "/resources/images/dark_mode/book_popup/tags_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookPopupMarkAsRead: "/resources/images/dark_mode/book_popup/mark_as_read_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookPopupDelete: "/resources/images/dark_mode/book_popup/delete_icon.svg"
            }

            // Decoration
            PropertyChanges {
                target: iconSheet
                heart: "/resources/images/dark_mode/decoration/heart_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                heartHallow: "/resources/images/dark_mode/decoration/heart_hallow_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                seed: "/resources/images/dark_mode/decoration/seed_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                popupDroplet: "/resources/images/dark_mode/decoration/popup_droplet_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                patreon: "/resources/images/dark_mode/decoration/patreon_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                warningCircle: "/resources/images/dark_mode/decoration/warning_circle_icon.svg"
            }

            // Illustrations
            PropertyChanges {
                target: iconSheet
                addImage: "/resources/images/dark_mode/illustrations/add_image_illustration.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookCover: "/resources/images/dark_mode/illustrations/book_cover.png"
            }
            PropertyChanges {
                target: iconSheet
                emptyHomeBackground: "/resources/images/dark_mode/illustrations/empty_home_background.svg"
            }
            PropertyChanges {
                target: iconSheet
                fileSwift: "/resources/images/dark_mode/illustrations/file_swift_illustration.svg"
            }
            PropertyChanges {
                target: iconSheet
                lockProtected: "/resources/images/dark_mode/illustrations/lock_protected_illustration.svg"
            }
            PropertyChanges {
                target: iconSheet
                attentionPurple: "/resources/images/dark_mode/illustrations/attention_purple_illustration.svg"
            }
            PropertyChanges {
                target: iconSheet
                checkBubbleIllustration: "/resources/images/dark_mode/illustrations/check_bubble_illustration.svg"
            }
            PropertyChanges {
                target: iconSheet
                addFileIllustration: "/resources/images/dark_mode/illustrations/add_file_illustration.svg"
            }
            PropertyChanges {
                target: iconSheet
                notFoundIllustration: "/resources/images/dark_mode/illustrations/not_found.svg"
            }

            // Reading searchbar
            PropertyChanges {
                target: iconSheet
                readingSearchbarCancel: "/resources/images/dark_mode/reading_searchbar/cancel_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingSearchbarNext: "/resources/images/dark_mode/reading_searchbar/next_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingSearchbarPrevious: "/resources/images/dark_mode/reading_searchbar/previous_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingSearchbarSearch: "/resources/images/dark_mode/reading_searchbar/search_icon.svg"
            }

            // ReadingView
            PropertyChanges {
                target: iconSheet
                readingViewBack: "/resources/images/dark_mode/reading_view/back_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewBookmark: "/resources/images/dark_mode/reading_view/bookmark_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewBookmarkSelected: "/resources/images/dark_mode/reading_view/bookmark_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewChapters: "/resources/images/dark_mode/reading_view/chapters_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewChaptersSelected: "/resources/images/dark_mode/reading_view/chapters_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewMaximize: "/resources/images/dark_mode/reading_view/maximize_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewMaximizeSelected: "/resources/images/dark_mode/reading_view/maximize_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewOptions: "/resources/images/dark_mode/reading_view/options_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewOptionsPurple: "/resources/images/dark_mode/reading_view/options_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewSearch: "/resources/images/dark_mode/reading_view/search_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingViewSearchSelected: "/resources/images/dark_mode/reading_view/search_selected_icon.svg"
            }

            // Reading options popup
            PropertyChanges {
                target: iconSheet
                readingOptionsPopupPrinter: "/resources/images/dark_mode/reading_options_popup/printer_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingOptionsPopupDownload: "/resources/images/dark_mode/reading_options_popup/download_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingOptionsPopupShare: "/resources/images/dark_mode/reading_options_popup/share_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingOptionsPopupSun: "/resources/images/dark_mode/reading_options_popup/sun_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingOptionsPopupSync: "/resources/images/dark_mode/reading_options_popup/sync_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingOptionsSettings: "/resources/images/dark_mode/reading_options_popup/settings_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                readingOptionsInvertColor: "/resources/images/dark_mode/reading_options_popup/invert_color_icon.svg"
            }

            // Setting
            PropertyChanges {
                target: iconSheet
                settingsSidebarAbout: "/resources/images/dark_mode/settings_sidebar/about_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarAboutSelected: "/resources/images/dark_mode/settings_sidebar/about_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarAccount: "/resources/images/dark_mode/settings_sidebar/account_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarAccountSelected: "/resources/images/dark_mode/settings_sidebar/account_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarAppearance: "/resources/images/dark_mode/settings_sidebar/appearance_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarAppearanceSelected: "/resources/images/dark_mode/settings_sidebar/appearance_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarSettings: "/resources/images/dark_mode/settings_sidebar/settings_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarSettingsSelected: "/resources/images/dark_mode/settings_sidebar/settings_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarShortcuts: "/resources/images/dark_mode/settings_sidebar/shortcuts_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarShortcutsSelected: "/resources/images/dark_mode/settings_sidebar/shortcuts_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarStorage: "/resources/images/dark_mode/settings_sidebar/storage_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarStorageSelected: "/resources/images/dark_mode/settings_sidebar/storage_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarSupportUs: "/resources/images/dark_mode/settings_sidebar/support_us_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarSupportUsSelected: "/resources/images/dark_mode/settings_sidebar/support_us_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarUpdates: "/resources/images/dark_mode/settings_sidebar/updates_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                settingsSidebarUpdatesSelected: "/resources/images/dark_mode/settings_sidebar/updates_selected_icon.svg"
            }

            // Sidebar
            PropertyChanges {
                target: iconSheet
                sidebarDownload: "/resources/images/dark_mode/sidebar/download_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                sidebarHome: "/resources/images/dark_mode/sidebar/home_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                sidebarPieChart: "/resources/images/dark_mode/sidebar/pieChart_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                sidebarSettings: "/resources/images/dark_mode/sidebar/settings_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                sidebarAddOns: "/resources/images/dark_mode/sidebar/puzzle_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                sidebarSync: "/resources/images/dark_mode/sidebar/sync_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                sidebarEmptyProfile: "/resources/images/dark_mode/sidebar/empty_profile_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                sidebarLogout: "/resources/images/dark_mode/sidebar/log_out_icon.svg"
            }

            // Utility
            PropertyChanges {
                target: iconSheet
                dots: "/resources/images/dark_mode/utility/dots_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                downloadSelected: "/resources/images/dark_mode/utility/download_selected_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                edit: "/resources/images/dark_mode/utility/edit_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                eyeOff: "/resources/images/dark_mode/utility/eye_off_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                eyeOn: "/resources/images/dark_mode/utility/eye_on_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                filter: "/resources/images/dark_mode/utility/filter_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                search: "/resources/images/dark_mode/utility/search_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                tag: "/resources/images/dark_mode/utility/tag_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                microphone: "/resources/images/dark_mode/utility/microphone_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                activeMicrophone: "/resources/images/dark_mode/utility/active_microphone_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                trash: "/resources/images/dark_mode/utility/trash_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                trashRed: "/resources/images/dark_mode/utility/trash_red_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                trashHighlighted: "/resources/images/dark_mode/utility/trash_highlighted_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                cloudOff: "/resources/images/dark_mode/utility/cloud_off_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                folder: "/resources/images/dark_mode/utility/folder_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                unsorted: "/resources/images/dark_mode/utility/unsorted_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                addFolderPlus: "/resources/images/dark_mode/utility/add_folder_plus_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                bookClosed: "/resources/images/dark_mode/utility/book_closed_icon.svg"
            }
            PropertyChanges {
                target: iconSheet
                pen: "/resources/images/dark_mode/utility/pen_icon.svg"
            }
        }
    ]

    function setState(name) {
        state = name
    }
}

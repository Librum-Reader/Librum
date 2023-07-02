#pragma once
#include <QObject>

// Each enum needs to be in a separate namespace due to a Qt bug reported at:
// https://bugreports.qt.io/browse/QTBUG-81792
//
// This causes not being able to register multiple enums to the Qt type system
// and to QML (Q_NAMESPACE and Q_ENUM_NS), if they are in the same namespace.
// The work around is to create a separate namespace for each enum.
namespace application::setting_keys
{

Q_NAMESPACE

enum class SettingKeys
{
    // Appearance
    Theme = 0,
    PageSpacing,
    DisplayBookTitleInTitlebar,
    LayoutDirection,
    DisplayMode,
    PageTransition,
    DefaultZoom,
    SmoothScrolling,
    LoopAfterLastPage,
    CursorMode,

    // General
    OpenBooksAfterCreation,

    // Shortcuts
    MoveUp,
    MoveDown,
    NextPage,
    PreviousPage,
    Search,
    StartFullScreenMode,
    ExitFullScreenMode,
    GoToHome,
    ZoomIn,
    ZoomOut,
    OpenChapters,
    OpenBookmarks,
    StartOfDocument,
    EndOfDocument,
    PrintPage,
    CreateBookmarkHere,
    ToggleSpeaking,
    ReloadApplication,
    AddBook,

    SettingKeys_END
};

Q_ENUM_NS(SettingKeys)

}  // namespace application::setting_keys

// Because the enum shouldn't be in a separate namespace in the first place,
// make the namespace available to all of its users to avoid syntactic clutter.
using namespace application::setting_keys;
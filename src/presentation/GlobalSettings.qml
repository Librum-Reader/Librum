pragma Singleton

import QtCore

Settings {
    location: "last_run_settings"

    property string theme: "Dark"
    property string lastFeedbackQuery: new Date().toString()
}

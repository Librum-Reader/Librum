pragma Singleton

import QtQuick

ListModel {
    id: languageModel

    ListElement {
        text: "English"
        code: "en"
    }
    ListElement {
        text: "Deutsch"
        code: "de"
    }
    ListElement {
        text: "Italiano"
        code: "it"
    }
    ListElement {
        text: "Русский"
        code: "ru"
    }
    ListElement {
        text: "Українська"
        code: "uk"
    }
    ListElement {
        text: "中文"
        code: "zh"
    }
    ListElement {
        text: "Português"
        code: "pt"
    }
    ListElement {
        text: "Bahasa Indonesia"
        code: "id"
    }
}

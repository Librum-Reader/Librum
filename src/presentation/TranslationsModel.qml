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
        text: "العربية"
        code: "ar"
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
        text: "한국어"
        code: "ko"
    }
    ListElement {
        text: "简体中文"
        code: "zh_CN"
    }
    ListElement {
        text: "繁體中文"
        code: "zh_TW"
    }
    ListElement {
        text: "Português"
        code: "pt"
    }
    ListElement {
        text: "Bahasa Indonesia"
        code: "id"
    }
    ListElement {
        text: "বাংলা"
        code: "bn"
    }
}

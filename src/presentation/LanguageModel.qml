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
        text: "Русский"
        code: "ru"
    }
    ListElement {
        text: "한국어"
        code: "ko"
    }
    ListElement {
        text: "中文"
        code: "zh"
    }
}

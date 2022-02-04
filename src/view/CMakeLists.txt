cmake_minimum_required(VERSION 3.22)

set(QML_SOURCES
                src/view/main.qml
                src/view/MGlobalProperties.qml
                src/view/components/MCheckBox.qml
                src/view/components/MButton.qml
                src/view/login/MLabeledInputBox.qml
                src/view/login/MLoginPage.qml
                src/view/register/MRegisterPage.qml
    PARENT_SCOPE
    )
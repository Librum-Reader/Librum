cmake_minimum_required(VERSION 3.22)

set(QML_SOURCES
                src/app/view/main.qml
                src/app/view/MGlobalProperties.qml
                src/app/view/components/MCheckBox.qml
                src/app/view/components/MButton.qml
                src/app/view/components/MLogo.qml
                src/app/view/components/MLabeledInputBox.qml
                src/app/view/components/sidebar/MSidebar.qml
                src/app/view/components/sidebar/MSidebarItem.qml
                src/app/view/login/MLoginPage.qml
                src/app/view/register/MRegisterPage.qml
                src/app/view/library/MLibrary.qml
    PARENT_SCOPE
    )
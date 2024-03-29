import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Librum.models
import Librum.controllers
import CustomComponents
import "PageNavigationLogic.js" as PageNavLogic

import "sidebar"
import "homePage"
import "loginPage"
import "registerPage"
import "freeBooksPage"
import "settings"
import "statisticsPage"
import "toolsPage"
import "forgotPasswordPage"
import "readingPage"

ApplicationWindow {
    id: baseRoot
    property int defaultMinWidth: 650
    property int sidebarOpenedMinWidth: 810
    property int readingPageMinWidth: 550
    property bool notifyAboutUpdates: true
    property bool rightAlign: AppInfoController.language === "العربية"
    property bool externalBookMode: false
    property bool isExitingReadingPage: false

    // Only initalize once at the start
    Component.onCompleted: externalBookMode = externalBook

    minimumHeight: 400
    minimumWidth: 650
    visible: true
    visibility: Window.Maximized
    title: qsTr("Librum - Your ebook reader")

    Shortcut {
        sequence: SettingsController.shortcuts.ReloadApplication
        onActivated: internal.reloadApplication()
    }

    RowLayout {
        id: mainlayout
        anchors.fill: parent
        spacing: 0
        LayoutMirroring.enabled: false

        MSidebar {
            id: sidebar
            z: 1
            visible: pageManager.pageHasSidebar
        }


        /*
          The StackView is managing the switching of pages
          */
        StackView {
            id: pageManager
            property bool pageHasSidebar: false
            property var prevPage: null

            Layout.fillWidth: true
            Layout.fillHeight: true
            initialItem: loginPage

            popEnter: null
            popExit: null
            pushEnter: null
            pushExit: null
            replaceEnter: null
            replaceExit: null

            ListView {
                id: alertList
                z: 100
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 4
                width: 600
                spacing: 4
                height: contentHeight
                model: ListModel {}
                delegate: MAlertBox {
                    onDestroyAlert: index => alertList.model.remove(index)
                }

                add: Transition {
                    NumberAnimation {
                        property: "opacity"
                        from: 0
                        to: 1.0
                        duration: 400
                    }
                    NumberAnimation {
                        property: "scale"
                        from: 0
                        to: 1.0
                        duration: 400
                    }
                }

                remove: Transition {
                    ParallelAnimation {
                        NumberAnimation {
                            property: "opacity"
                            to: 0
                            duration: 250
                        }
                        NumberAnimation {
                            properties: "y"
                            to: -40
                            duration: 250
                        }
                        NumberAnimation {
                            properties: "scale"
                            to: 0.2
                            duration: 250
                        }
                    }
                }

                displaced: Transition {
                    NumberAnimation {
                        property: "opacity"
                        to: 1.0
                        duration: 400
                    }
                    NumberAnimation {
                        property: "scale"
                        to: 1.0
                    }
                }
            }
        }
    }

    // Pages
    Component {
        id: loginPage
        MLoginPage {}
    }
    Component {
        id: forgotPasswordPage
        MForgotPasswordPage {}
    }
    Component {
        id: registerPage
        MRegisterPage {}
    }
    Component {
        id: homePage
        MHomePage {}
    }
    Component {
        id: freeBooksPage
        MFreeBooksPage {}
    }
    Component {
        id: settings
        MSettings {}
    }
    Component {
        id: toolsPage
        MToolsPage {}
    }
    Component {
        id: statisticsPage
        MStatisticsPage {}
    }
    Component {
        id: readingPage
        MReadingPage {}
    }
    Component {
        id: externalReadingPage
        MExternalReadingPage {}
    }

    function showAlert(level, title, message) {
        if (alertList.model.count >= 3) {
            alertList.model.remove(2)
        }

        alertList.model.insert(0, {
                                   "level": level,
                                   "title": title,
                                   "message": message
                               })
    }


    /*
      loadPage() manages the page switching through out the application
      */
    function loadPage(page, sidebarItem, doSamePageCheck = true) {
        // Prevent switching to the same page that is currently active
        if (doSamePageCheck && PageNavLogic.checkIfNewPageIsTheSameAsOld(
                    sidebarItem))
            return

        // Terminate any pending operation on the previous page
        if (!PageNavLogic.terminateActionOfCurrentPage(page, sidebarItem))
            return

        // Switch the page
        PageNavLogic.switchPage(page, sidebarItem)
    }


    /*
      Nested setting navigation - Navigate to a specific sub-page in one command.
      E.g. go from "Home" to "Settings / Account page"
      */
    function loadSettingsAccountPage() {
        loadPage(settings, sidebar.settingsItem, false)

        let page = pageManager.currentItem
        page.loadSettingsPage(page.accountPage,
                              page.settingsSidebar.accountItem)
    }

    function loadSettingsAppearancePage() {
        loadPage(settings, sidebar.settingsItem, false)

        let page = pageManager.currentItem
        page.loadSettingsPage(page.appearancePage,
                              page.settingsSidebar.appearanceItem)
    }

    function loadSettingsUpdatesPage() {
        loadPage(settings, sidebar.settingsItem, false)

        let page = pageManager.currentItem
        page.loadSettingsPage(page.updatesPage,
                              page.settingsSidebar.updatesItem)
    }

    function exitReadingPage() {
        baseRoot.isExitingReadingPage = true
        loadPage(homePage, sidebar.homeItem, false);
    }

    QtObject {
        id: internal

        function reloadApplication() {
            if (spamStopper.available) {
                LibraryController.syncWithServer()
                UserController.syncWithServer()
                FolderController.syncWithServer()
                spamStopper.available = false
                spamStopper.start()
            }
        }
    }

    // Connect in main so that addBook() is called no matter what page is displayed
    Connections {
        target: FreeBooksController

        function onGettingBookFinished(path, projectGutenbergId) {
            LibraryController.addBook(path, false, projectGutenbergId)
        }
    }

    Timer {
        id: spamStopper
        property bool available: true

        interval: 1000
        onTriggered: spamStopper.available = true
    }
}

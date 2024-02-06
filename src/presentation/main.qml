import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Librum.models
import Librum.controllers
import "PageNavigationLogic.js" as Logic

import "sidebar"
import "homePage"
import "loginPage"
import "registerPage"
import "freeBooksPage"
import "settings"
import "statisticsPage"
import "addOnsPage"
import "forgotPasswordPage"
import "readingPage"

ApplicationWindow {
    id: baseRoot
    property int defaultMinWidth: 650
    property int sidebarOpenedMinWidth: 810
    property int readingPageMinWidth: 550
    property bool notifyAboutUpdates: true
    property bool rightAlign: AppInfoController.language === "العربية"

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
        LayoutMirroring.enabled: baseRoot.rightAlign
        LayoutMirroring.childrenInherit: true

        MSidebar {
            id: sidebar
            z: 1
            visible: pageManager.pageHasSidebar

            onOpenChanged: open ? baseRoot.minimumWidth
                                  = sidebarOpenedMinWidth : baseRoot.minimumWidth = defaultMinWidth
        }


        /*
          The StackView is managing the switching of pages
          */
        StackView {
            id: pageManager
            property bool pageHasSidebar: false

            Layout.fillWidth: true
            Layout.fillHeight: true
            initialItem: loginPage

            popEnter: null
            popExit: null
            pushEnter: null
            pushExit: null
            replaceEnter: null
            replaceExit: null
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
        id: addOnsPage
        MAddOnsPage {}
    }
    Component {
        id: statisticsPage
        MStatisticsPage {}
    }
    Component {
        id: readingPage
        MReadingPage {}
    }


    /*
      loadPage() manages the page switching through out the application
      */
    function loadPage(page, sidebarItem, doSamePageCheck = true) {
        // Prevent switching to the same page that is currently active
        if (doSamePageCheck && Logic.checkIfNewPageIsTheSameAsOld(sidebarItem))
            return

        // Terminate any pending operation on the previous page
        if (!Logic.terminateActionOfCurrentPage(page, sidebarItem))
            return

        // Switch the page
        Logic.switchPage(page, sidebarItem)
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

    QtObject {
        id: internal

        function reloadApplication() {
            if (spamStopper.available) {
                LibraryController.syncWithServer()
                UserController.syncWithServer()
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

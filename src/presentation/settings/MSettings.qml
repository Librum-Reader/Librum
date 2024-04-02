import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import CustomComponents
import "settingsSidebar"
import "shortcutsPage"
import "updatesPage"
import "accountPage"

Page {
    id: settingsManager
    property alias settingsSidebar: settingsSidebar
    property alias aboutPage: aboutPage
    property alias appearancePage: appearancePage
    property alias shortcutsPage: shortcutsPage
    property alias updatesPage: updatesPage
    property alias behaviorSettingsPage: behaviorSettingsPage
    property alias accountPage: accountPage
    property alias storagePage: storagePage
    property alias supportUsPage: supportUsPage

    LayoutMirroring.enabled: baseRoot.rightAlign
    LayoutMirroring.childrenInherit: true

    background: Rectangle {
        anchors.fill: parent
        color: Style.colorPageBackground
    }


    /*
      Settings navigation shortcuts
      */
    Shortcut {
        sequence: "Shift+1"
        onActivated: loadSettingsPage(aboutPage, settingsSidebar.aboutItem)
    }
    Shortcut {
        sequence: "Shift+2"
        onActivated: loadSettingsPage(appearancePage,
                                      settingsSidebar.appearanceItem)
    }
    Shortcut {
        sequence: "Shift+3"
        onActivated: loadSettingsPage(behaviorSettingsPage,
                                      settingsSidebar.behaviorSettingsItem)
    }
    Shortcut {
        sequence: "Shift+4"
        onActivated: loadSettingsPage(shortcutsPage, settingsSidebar.shortcutsItem)
    }
    Shortcut {
        sequence: "Shift+5"
        onActivated: loadSettingsPage(updatesPage,
                                      settingsSidebar.updatesItem)
    }
    Shortcut {
        sequence: "Shift+6"
        onActivated: loadSettingsPage(accountPage, settingsSidebar.accountItem)
    }
    Shortcut {
        sequence: "Shift+7"
        onActivated: loadSettingsPage(storagePage, settingsSidebar.storageItem)
    }
    Shortcut {
        sequence: "Shift+8"
        onActivated: loadSettingsPage(supportUsPage,
                                      settingsSidebar.supportUsItem)
    }

    RowLayout {
        id: layout
        anchors.fill: parent
        spacing: 0

        MSettingsSidebar {
            id: settingsSidebar
            height: parent.height
        }


        /*
          The StackView is managing the switching of setting pages
          */
        StackView {
            id: settingsPageManager
            Layout.fillWidth: true
            Layout.fillHeight: true
            initialItem: aboutPage

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
        id: aboutPage
        MAboutPage {}
    }
    Component {
        id: appearancePage
        MAppearancePage {}
    }
    Component {
        id: behaviorSettingsPage
        MBehaviorPage {}
    }
    Component {
        id: shortcutsPage
        MShortcutsPage {}
    }
    Component {
        id: updatesPage
        MUpdatesPage {}
    }
    Component {
        id: accountPage
        MAccountPage {}
    }
    Component {
        id: storagePage
        MStoragePage {}
    }
    Component {
        id: supportUsPage
        MSupportUsPage {}
    }


    /*
      loadPage() manages the page switching through out the settings
      */
    function loadSettingsPage(page, sidebarItem) {
        if (settingsSidebar.currentItem === sidebarItem)
            return

        if (!saveSettingsPage(internal.switchSettingsPage, page, sidebarItem))
            return

        internal.switchSettingsPage(page, sidebarItem)
    }


    /*
      saveSettingsPage() saves the given settings page

      @param "switchPageFunction" is a callback for continuing page switching
      after the page was saved successfully. This can be a switch between settings pages,
      but also a switch between main pages.
      */
    function saveSettingsPage(switchPageFunction, page, sidebarItem) {
        if (settingsPageManager.currentItem.pageCleanup) {
            settingsPageManager.currentItem.pageCleanup.callbackFunction = () => switchPageFunction(
                        page, sidebarItem)

            return settingsPageManager.currentItem.pageCleanup.cleanUp()
        }

        return true
    }

    QtObject {
        id: internal

        function switchSettingsPage(page, sidebarItem) {
            settingsPageManager.replace(page)
            settingsSidebar.changeSelectedSettingsItem(sidebarItem)
        }
    }
}

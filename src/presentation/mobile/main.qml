import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Window
import "startPage"
import "loginPage"
import "registerPage"
import "navbar"
import "libraryPage"
import "explorePage"
import "profilePage"
import "myAccountPage"

ApplicationWindow {
    id: baseRoot
    visible: true
    width: 640
    height: 480
    visibility: Window.Maximized
    title: qsTr("Librum")

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        StackView {
            id: stackView
            property bool pageHasNavbar: false

            Layout.fillWidth: true
            Layout.fillHeight: true
            initialItem: startPage

            popEnter: null
            popExit: null
            pushEnter: null
            pushExit: null
            replaceEnter: null
            replaceExit: null
        }

        MNavbar {
            id: navbar
            visible: stackView.pageHasNavbar
            Layout.preferredHeight: 66
            Layout.fillWidth: true
        }
    }

    Component {
        id: startPage
        MStartPage {}
    }
    Component {
        id: loginPage
        MLoginPage {}
    }
    Component {
        id: registerPage
        MRegisterPage {}
    }
    Component {
        id: homePage
        MLibraryPage {}
    }
    Component {
        id: explorePage
        MExplorePage {}
    }
    Component {
        id: profilePage
        MProfilePage {}
    }
    Component {
        id: myAccountPage
        MMyAccountPage {}
    }


    /*
      loadPage() manages the page switching through out the application
      */
    function loadPage(page, navbarItem = undefined) {
        if (page === stackView.currentItem)
            return

        stackView.pageHasNavbar = navbarItem !== undefined

        if (navbarItem !== undefined) {
            navbar.currentItem = navbarItem
        }

        stackView.replace(page)
    }
}

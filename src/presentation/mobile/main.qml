import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Window
import "startPage"
import "loginPage"
import "registerPage"

ApplicationWindow {
    id: baseRoot
    visible: true
    width: 640
    height: 480
    visibility: Window.Maximized
    title: qsTr("Librum")

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: startPage

        popEnter: null
        popExit: null
        pushEnter: null
        pushExit: null
        replaceEnter: null
        replaceExit: null
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


    /*
      loadPage() manages the page switching through out the application
      */
    function loadPage(page) {
        stackView.replace(page)
    }
}

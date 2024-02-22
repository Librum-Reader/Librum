import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.icons
import Librum.fonts
import CustomComponents

Pane {
    id: root
    property alias libraryItem: libraryItem
    property alias exploreItem: exploreItem
    property alias profileItem: profileItem
    property MNavbarItem currentItem: null

    horizontalPadding: 0
    verticalPadding: 0
    background: Rectangle {
        color: "#EFEFEF"
    }

    RowLayout {
        anchors.centerIn: parent
        height: parent.height
        spacing: 30

        MNavbarItem {
            id: libraryItem
            selected: root.currentItem === libraryItem
            icon: Icons.mobileLibrary
            text: "Library"

            onClicked: loadPage(homePage, navbar.libraryItem)
        }

        MNavbarItem {
            icon: Icons.mobileFolder
            text: "Folders"
            iconHeight: 18
            topMargin: 1
        }

        MNavbarItem {
            id: exploreItem
            selected: root.currentItem === exploreItem
            icon: Icons.mobileExplore
            text: "Explore"

            onClicked: loadPage(explorePage, navbar.exploreItem)
        }

        MNavbarItem {
            id: profileItem
            selected: root.currentItem === profileItem
            icon: Icons.mobileProfile
            text: "Profile"

            onClicked: loadPage(profilePage, navbar.profileItem)
        }
    }
}

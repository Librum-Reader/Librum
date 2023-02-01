import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0


Page
{
    id: root
    property var shortcutListModel: ListModel
    {
        ListElement { text: "Up"; shortcut: "Arrow-Up" }
        ListElement { text: "Down"; shortcut: "Arrow-Down" }
        ListElement { text: "Next page"; shortcut: "Arrow-Right" }
        ListElement { text: "Previous page"; shortcut: "Arrow-Left" }
        ListElement { text: "Search"; shortcut: "Ctrl+F" }
        ListElement { text: "Full Screen"; shortcut: "Ctrl+Alt+F" }
        ListElement { text: "Change application theme"; shortcut: "-" }
        ListElement { text: "Go to home"; shortcut: "Ctrl+Alt+H" }
        ListElement { text: "Zoom in"; shortcut: "Ctrl++" }
        ListElement { text: "Zoom out"; shortcut: "Ctrl+-" }
        ListElement { text: "Open chapters"; shortcut: "Tab" }
        ListElement { text: "Open bookmarks"; shortcut: "Ctrl+B" }
        ListElement { text: "Start of document"; shortcut: "Ctrl+Pos1" }
        ListElement { text: "End of document"; shortcut: "Ctrl+End" }
        ListElement { text: "Print page"; shortcut: "Ctrl+P" }
        ListElement { text: "Highlighter"; shortcut: "Alt+1" }
        ListElement { text: "Open notes"; shortcut: "Alt+2" }
        ListElement { text: "Create bookmark here"; shortcut: "Alt+3" }
        ListElement { text: "Pause/Resume speaking"; shortcut: "F5" }
        ListElement { text: "Reload Librum"; shortcut: "-" }
    }
    
    topPadding: 64
    horizontalPadding: 48
    background: Rectangle { anchors.fill: parent; color: Style.pagesBackground }
    
    onWidthChanged: if(searchButton.opened) searchButton.close()
    
    
    Shortcut
    {
        sequence: StandardKey.New
        onActivated: addShortcutPopup.open()
    }
    
    
    ColumnLayout
    {
        id: layout
        anchors.fill: parent
        spacing: 0
        
        
        RowLayout
        {
            id: pageTitleRow
            Layout.fillWidth: true
            spacing: 0
            
            MTitle
            {
                id: pageTitle
                titleText: "Shortcuts"
                descriptionText: "Make your own experience"
                titleSize: 25
                descriptionSize: 13.25
            }
            
            Item { Layout.fillWidth: true }
            
            MButton
            {
                id: addBooksButton
                Layout.preferredWidth: 160
                Layout.preferredHeight: 40
                Layout.topMargin: 22
                Layout.alignment: Qt.AlignBottom
                borderWidth: 0
                backgroundColor: Style.colorBasePurple
                text: "Add shortcut"
                textColor: Style.colorFocusedButtonText
                fontWeight: Font.Bold
                fontSize: 13
                imagePath: Icons.plusWhite
                
                onClicked:
                {
                    addShortcutPopup.preselectedSettingIndex = -1;
                    addShortcutPopup.open();
                }
            }
        }
        
        Pane
        {
            id: container
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 32
            Layout.bottomMargin: 44
            topPadding: 60
            leftPadding: internal.containerPadding
            padding: 0
            clip: true
            background: Rectangle
            {
                color: Style.colorBackground
                border.color: Style.colorLightBorder
                radius: 4
                antialiasing: true
            }
            
            
            ColumnLayout
            {
                id: containerLayout
                anchors.fill: parent
                spacing: 0
                
                
                /*
                  The shortcuts header labeling the different columns
                  */
                RowLayout
                {
                    id: headerLayout
                    Layout.fillWidth: true
                    Layout.rightMargin: internal.containerPadding
                    spacing: 0
                    
                    
                    Label
                    {
                        id: actionsLabel
                        Layout.leftMargin: 12
                        text: "ACTION"
                        color: Style.colorLightText
                        font.pointSize: 10.25
                        font.bold: true
                    }
                    
                    Item
                    { 
                        id: headerLabelSpacer
                        Layout.preferredWidth: internal.verticalSettingSpacing + 90
                    }
                    
                    Label
                    {
                        id: shortcutsLabel
                        text: "SHORTCUTS"
                        color: Style.colorLightText
                        font.pointSize: 10.25
                        font.bold: true
                    }
                    
                    Item { Layout.fillWidth: true }
                    
                    MSearchButton
                    {
                        id: searchButton
                        implicitWidth: 34
                        implicitHeight: 32
                        imageSize: 14
                        // Make sure that the searchButton does not overlap other items
                        expansionWidth: (headerLabelSpacer.width <= 445 ? headerLabelSpacer.width : 445)
                    }
                }
                
                /*
                  The actual shortcuts view
                  */
                ScrollView
                {
                    id: shortcutScrollArea
                    Layout.topMargin: 20
                    Layout.rightMargin: 20
                    Layout.bottomMargin: 32
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    
                    Component.onCompleted:
                    {
                        // contentItem is the ScrollView's underlying Flickable
                        contentItem.flickDeceleration = 1000;
                        contentItem.maximumFlickVelocity = 1000;
                    }
                    
                    
                    ListView
                    {
                        id: listView
                        anchors.rightMargin: 28
                        anchors.fill: parent
                        clip: true
                        interactive: false
                        model: root.shortcutListModel
                        delegate: MShortcutDelegate
                        {
                            onGapWidthChanged: (spacing) => internal.verticalSettingSpacing = spacing
                            onEditClicked:
                                (index) =>
                                {
                                    addShortcutPopup.preselectedSettingIndex = index;
                                    addShortcutPopup.open();
                                }
                            
                            onDeleteClicked: (index) => {}
                        }
                        
                        
                        MouseArea
                        {
                            id: mouseEventInterceptor
                            anchors.fill: parent
                            propagateComposedEvents: true
                            
                            onWheel: (wheel) => listView.scroll(wheel.angleDelta.y > 0)
                            // Propagate click/pressed signals to lower MouseAreas
                            onClicked: (mouse) => mouse.accepted = false
                            onPressed: (mouse) => mouse.accepted = false
                        }
                        
                        
                        function scroll(up)
                        {
                            let scrollSpeed = 550;
                            listView.flick(0, up ? scrollSpeed : -scrollSpeed)
                        }
                    }
                }
            }
        }
    }
    
    MAddShortcutPopup
    {
        id: addShortcutPopup
        x: Math.round(root.width / 2 - implicitWidth / 2 - settingsSidebar.width / 2 - sidebar.width / 2 - root.horizontalPadding)
        y: Math.round(root.height / 2 - implicitHeight / 2 - 115)
        
        actions: root.shortcutListModel
    }
    
    QtObject
    {
        id: internal
        property int containerPadding: 48
        property int verticalSettingSpacing: 340
    }
}
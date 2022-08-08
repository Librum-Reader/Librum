import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons





Page
{
    id: root
    topPadding: 64
    horizontalPadding: 48
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.pagesBackground
    }
    
    onWidthChanged:
    {
        if(searchButton.opened)
            searchButton.close();
    }
    
    
    Shortcut
    {
        sequence: StandardKey.New
        onActivated: addShortcutPopup.open()
    }
    
    
    ColumnLayout
    {
        id: layout
        property int insideMargin : 48
        property int outsideMargin : 48
        
        anchors.fill: parent
        spacing: 0
        
        
        RowLayout
        {
            id: titleRow
            Layout.fillWidth: true
            spacing: 0
            
            MTitle
            {
                id: title
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
                fontColor: Style.colorBackground
                fontWeight: Font.Bold
                fontSize: 13
                imagePath: Icons.plusWhite
                
                onClicked: addShortcutPopup.open()
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
            leftPadding: layout.insideMargin
            rightPadding: 0
            bottomPadding: 0
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
                id: inDetailsLayout
                property int gapWidth: 340
                
                anchors.fill: parent
                spacing: 0
                
                
                RowLayout
                {
                    id: headerLayout
                    Layout.fillWidth: true
                    Layout.rightMargin: layout.outsideMargin
                    spacing: 0
                    
                    
                    Label
                    {
                        id: actionsLabel
                        Layout.leftMargin: 12
                        text: "ACTION"
                        color: Style.colorLightText3
                        font.pointSize: 10.25
                        font.family: Style.defaultFontFamily
                        font.bold: true
                    }
                    
                    Item
                    { 
                        id: headerLabelSpacer
                        Layout.preferredWidth: inDetailsLayout.gapWidth + 90
                    }
                    
                    Label
                    {
                        id: shortcutsLabel
                        text: "SHORTCUTS"
                        color: Style.colorLightText3
                        font.pointSize: 10.25
                        font.family: Style.defaultFontFamily
                        font.bold: true
                    }
                    
                    Item { Layout.fillWidth: true }
                    
                    MSearchButton
                    {
                        id: searchButton
                        implicitWidth: 34
                        implicitHeight: 32
                        imageSize: 14
                        expansionWidth: (headerLabelSpacer.width <= 445 ? headerLabelSpacer.width : 445)
                    }
                }
                
                ScrollView
                {
                    Layout.topMargin: 20
                    Layout.rightMargin: 20
                    Layout.bottomMargin: 32
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    
                    Component.onCompleted:
                    {
                        contentItem.maximumFlickVelocity = 4000;
                        contentItem.flickDeceleration = 2200;
                    }
                    
                    
                    ListView
                    {
                        id: listView
                        property int moveSpeed : 550
                        
                        anchors.rightMargin: 28
                        anchors.fill: parent
                        clip: true
                        interactive: false
                        
                        model: ListModel
                        {
                            ListElement { action: "Up"; shortcuts: "SCROLL-UP" }
                            ListElement { action: "Down"; shortcuts: "SCROLL-DOWN" }
                            ListElement { action: "Next page"; shortcuts: "ARROW-RIGHT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                            ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                        }
                        
                        delegate: MShortcutDelegate
                        {
                            onGapWidthChanged: (newWidth) => inDetailsLayout.gapWidth = newWidth
                            onEditClicked: addShortcutPopup.open()
                            onDeleteClicked: console.log("delete")
                        }
                        
                        MouseArea
                        {
                            anchors.fill: parent
                            propagateComposedEvents: true
                            
                            onWheel: (wheel) =>
                                     {
                                         listView.moveContent( wheel.angleDelta.y > 0)
                                     }
                        }
                        
                        
                        function moveContent(up)
                        {
                            listView.flick(0, up ? listView.moveSpeed : -listView.moveSpeed)
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
    }
}
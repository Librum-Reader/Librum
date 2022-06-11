import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents


Page
{
    id: root
    topPadding: 64
    horizontalPadding: 48
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.pagesBackground
    }
    
    onWidthChanged:
    {
        if(searchButton.opened)
            searchButton.close();
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
            id: headerRow
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
                backgroundColor: properties.colorBasePurple
                text: "Add shortcut"
                fontColor: properties.colorBackground
                fontBold: true
                fontSize: 13
                imagePath: properties.iconPlusWhite
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
            verticalPadding: 0
            background: Rectangle
            {
                color: properties.colorBackground
                border.color: properties.colorLightBorder
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
                        color: properties.colorLightText3
                        font.pointSize: 10.25
                        font.family: properties.defaultFontFamily
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
                        color: properties.colorLightText3
                        font.pointSize: 10.25
                        font.family: properties.defaultFontFamily
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
                    Layout.bottomMargin: 75
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    
                    
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
                        
                        delegate: Item
                        {
                            id: shortcutDelegate
                            height: 52
                            width: listView.width
                            
                            ColumnLayout
                            {
                                anchors.fill: parent
                                spacing: 0
                                
                                Rectangle
                                {
                                    id: topBorder
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 2
                                    color: properties.colorLightGray
                                }
                                
                                RowLayout
                                {
                                    id: contentRow
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    spacing: 0
                                    
                                    Label
                                    {
                                        id: actionText
                                        Layout.leftMargin: 12
                                        Layout.preferredWidth: 150
                                        Layout.alignment: Qt.AlignVCenter
                                        text: action
                                        color: properties.colorBaseText
                                        font.pointSize: 12
                                        font.family: properties.defaultFontFamily
                                        font.weight: Font.DemiBold
                                    }
                                    
                                    Item
                                    {
                                        Layout.fillWidth: true
                                        Layout.maximumWidth: 247
                                        
                                        onWidthChanged:
                                        {
                                            inDetailsLayout.gapWidth = width;
                                        }
                                    }
                                    
                                    Label
                                    {
                                        id: shortcutsText
                                        Layout.preferredWidth: 172
                                        Layout.alignment: Qt.AlignVCenter
                                        text: shortcuts
                                        color: properties.colorBaseText
                                        font.pointSize: 12
                                        font.family: properties.defaultFontFamily
                                        font.weight: Font.DemiBold
                                    }
                                    
                                    Item { Layout.fillWidth: true }
                                    
                                    Image
                                    {
                                        id: editIcon
                                        Layout.alignment: Qt.AlignVCenter
                                        sourceSize.width: 21
                                        source: properties.iconEdit
                                        fillMode: Image.PreserveAspectFit
                                    }
                                    
                                    
                                    Image
                                    {
                                        id: deleteIcon
                                        Layout.leftMargin: 28
                                        Layout.rightMargin: 6
                                        Layout.alignment: Qt.AlignVCenter
                                        sourceSize.width: 18
                                        source: properties.iconTrash
                                        fillMode: Image.PreserveAspectFit
                                    }
                                }
                            }
                        }
                        
                        MouseArea
                        {
                            anchors.fill: parent
                            
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
}

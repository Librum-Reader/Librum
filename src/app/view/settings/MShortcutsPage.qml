import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents


Page
{
    id: root
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.pagesBackground
    }
    
    ColumnLayout
    {
        id: layout
        property int marginToLeft: 48
        property int marginToRight: 48
        property int inRectLeftMargin : 40
        
        width: root.width - marginToLeft - marginToRight
        spacing: 0
        
        
        MTitle
        {
            id: title
            Layout.topMargin: 64
            Layout.leftMargin: layout.marginToLeft
            titleText: "Shortcuts"
            descriptionText: "Make your own experience"
            titleSize: 25
            descriptionSize: 13.25
        }
        
        Rectangle
        {
            id: details
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 809
            Layout.topMargin: 32
            Layout.leftMargin: layout.marginToLeft
            color: properties.colorBackground
            border.color: properties.colorLightBorder
            radius: 4
            antialiasing: true
            
            
            ColumnLayout
            {
                id: inDetailsLayout
                
                width: parent.width
                spacing: 0
                
                
                RowLayout
                {
                    id: headerLayout
                    Layout.preferredWidth: parent.width
                    Layout.topMargin: 62
                    spacing: 0
                    
                    
                    Label
                    {
                        id: actionsLabel
                        Layout.leftMargin: 60
                        text: "ACTION"
                        color: properties.colorLightText3
                        font.pointSize: 10.25
                        font.family: properties.defaultFontFamily
                        font.bold: true
                    }
                    
                    Label
                    {
                        id: shortcutsLabel
                        Layout.leftMargin: 280
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
                        Layout.rightMargin: layout.marginToRight
                        imageSize: 14
                    }
                }
                
                ListView
                {
                    id: contentListView
                    Layout.topMargin: 16
                    Layout.leftMargin: layout.marginToLeft
                    Layout.rightMargin: layout.marginToRight
                    Layout.fillWidth: true
                    Layout.preferredHeight: 500
                    
                    model: ListModel
                    {
                        ListElement { action: "Up"; shortcuts: "SCROLL-UP" }
                        ListElement { action: "Down"; shortcuts: "SCROLL-DOWN" }
                        ListElement { action: "Next page"; shortcuts: "ARROW-RIGHT" }
                        ListElement { action: "Previous page"; shortcuts: "ARROW-LEFT" }
                    }
                    
                    delegate: Item
                    {
                        id: shortcutDelegate
                        height: 52
                        width: parent.width
                        
                        ColumnLayout
                        {
                            height: parent.height
                            width: parent.width
                            spacing: 0

                            Rectangle
                            {
                                id: topBorder
                                Layout.preferredWidth: parent.width
                                Layout.preferredHeight: 1
                                color: properties.colorLightGray
                            }
                            
                            RowLayout
                            {
                                id: contentRow
                                Layout.preferredWidth: parent.width
                                Layout.preferredHeight: parent.height
                                spacing: 0
                                
                                Label
                                {
                                    Layout.leftMargin: 12
                                    Layout.preferredWidth: 336
                                    Layout.alignment: Qt.AlignVCenter
                                    text: action
                                    color: properties.colorBaseText
                                    font.pointSize: 12.5
                                }
                                
                                Label
                                {
                                    Layout.alignment: Qt.AlignVCenter
                                    text: shortcuts
                                    color: properties.colorBaseText
                                    font.pointSize: 12.5
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
                }
            }
        }
    }
}
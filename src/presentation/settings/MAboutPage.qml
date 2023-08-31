import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.elements
import Librum.controllers
import Librum.style
import Librum.icons


MFlickWrapper
{
    id: root
    contentHeight: page.implicitHeight
    
    
    Page
    {
        id: page
        width: parent.width
        horizontalPadding: 48
        bottomPadding: 22
        background: Rectangle { anchors.fill: parent; color: Style.colorPageBackground }
        
        
        
        ColumnLayout
        {
            id: layout
            width: parent.width
            spacing: 26
            
            
            MTitle
            {
                id: pageTitle
                Layout.topMargin: 64
                titleText: "About"
                descriptionText: "About this application"
                titleSize: 25
                descriptionSize: 13.25
            }
            
            Pane
            {
                id: details
                Layout.fillWidth: true
                Layout.topMargin: 6
                topPadding: 24
                horizontalPadding: internal.pagePadding
                bottomPadding: 21
                background: Rectangle
                {
                    color: Style.colorContainerBackground
                    border.color: Style.colorContainerBorder
                    radius: 4
                    antialiasing: true
                }
                
                
                ColumnLayout
                {
                    id: inDetailsLayout
                    anchors.fill: parent
                    spacing: 0
                    
                    
                    Label
                    {
                        id: detailsTitle
                        text: "Details"
                        color: Style.colorText
                        font.pointSize: 16.5
                        font.weight: Font.DemiBold
                    }
                    
                    Label
                    {
                        Layout.topMargin: 15
                        text: "CURRENT VERSION"
                        color: Style.colorLightText
                        font.pointSize: 10.25
                        font.weight: Font.Bold
                    }
                    
                    Label
                    {
                        Layout.topMargin: 1
                        text: AppInfoController.currentVersion
                        color: Style.colorText
                        font.pointSize: 13.5
                    }
                    
                    Label
                    {
                        Layout.topMargin: 12
                        text: "QT VERSION"
                        color: Style.colorLightText
                        font.pointSize: 10.25
                        font.weight: Font.Bold
                    }
                    
                    Label
                    {
                        Layout.topMargin: 1
                        text: AppInfoController.currentQtVersion
                        color: Style.colorText
                        font.pointSize: 13.5
                    }
                    
                    MButton
                    {
                        Layout.topMargin: 21
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 32
                        borderColor: Style.colorGreenBorder
                        backgroundColor: Style.colorGreenBackground
                        imagePath: Icons.checkGreen
                        imageSize: 20
                        text: "Up-to date"
                        fontSize: 10.5
                        textColor: Style.colorGreenText
                        imageSpacing: 6
                        opacityOnPressed: 0.8
                        
                        onClicked: loadSettingsPage(updatesPage, settingsSidebar.updatesItem)
                    }
                }
            }
            
            Pane
            {
                id: creator
                Layout.fillWidth: true
                topPadding: 24
                horizontalPadding: internal.pagePadding
                bottomPadding: 30
                background: Rectangle
                {
                    color: Style.colorContainerBackground
                    border.color: Style.colorContainerBorder
                    radius: 4
                    antialiasing: true
                }
                
                
                ColumnLayout
                {
                    id: inCreatorLayout
                    anchors.fill: parent
                    spacing: 0
                    
                    
                    Label
                    {
                        id: creatorTitle
                        text: "Creator"
                        color: Style.colorText
                        font.pointSize: 16.5
                        font.weight: Font.DemiBold
                    }
                    
                    Label
                    {
                        Layout.topMargin: 18
                        text: "COMPANY NAME"
                        color: Style.colorLightText
                        font.pointSize: 10.25
                        font.weight: Font.Bold
                    }
                    
                    Label
                    {
                        Layout.topMargin: 1
                        text: AppInfoController.companyName
                        color: Style.colorBasePurple
                        font.pointSize: 13.5
                    }
                    
                    Label
                    {
                        Layout.topMargin: 12
                        text: "WEBSITE"
                        color: Style.colorLightText
                        font.pointSize: 10.25
                        font.weight: Font.Bold
                    }
                    
                    Label
                    {
                        text: AppInfoController.website
                        color: Style.colorBasePurple
                        font.pointSize: 13.5
                        opacity: websiteLinkArea.pressed ? 0.8 : 1
                        
                        MouseArea
                        {
                            id: websiteLinkArea
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            
                            onClicked: Qt.openUrlExternally(AppInfoController.website)
                        }
                    }
                    
                    Label
                    {
                        Layout.topMargin: 12
                        text: "CONTACT"
                        color: Style.colorLightText
                        font.pointSize: 10.25
                        font.weight: Font.Bold
                    }
                    
                    Label
                    {
                        Layout.topMargin: 1
                        text: AppInfoController.companyEmail
                        color: Style.colorBasePurple
                        opacity: emailLinkArea.pressed ? 0.8 : 1
                        font.pointSize: 13.5
                        
                        MouseArea
                        {
                            id: emailLinkArea
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            
                            onClicked: Qt.openUrlExternally("mailto:" + AppInfoController.companyEmail)
                        }
                    }
                    
                    Label
                    {
                        Layout.topMargin: 12
                        text: "GITHUB"
                        color: Style.colorLightText
                        font.pointSize: 10.25
                        font.weight: Font.Bold
                    }
                    
                    Label
                    {
                        Layout.topMargin: 1
                        text: AppInfoController.githubLink
                        color: Style.colorBasePurple
                        opacity: githubLinkArea.pressed ? 0.8 : 1
                        font.pointSize: 13.5
                        
                        MouseArea
                        {
                            id: githubLinkArea
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            
                            onClicked: Qt.openUrlExternally(AppInfoController.githubLink)
                        }
                    }
                }
            }
            
            Pane
            {
                id: thisApp
                Layout.fillWidth: true
                Layout.minimumHeight: inThisAppLayout.height + 15
                topPadding: 24
                horizontalPadding: internal.pagePadding
                bottomPadding: 56
                background: Rectangle
                {
                    color: Style.colorContainerBackground
                    border.color: Style.colorContainerBorder
                    radius: 4
                    antialiasing: true
                }
                
                
                ColumnLayout
                {
                    id: inThisAppLayout
                    anchors.fill: parent
                    spacing: 0
                    
                    
                    Label
                    {
                        id: thisAppTitle
                        text: "This App"
                        color: Style.colorText
                        font.pointSize: 16.5
                        font.weight: Font.DemiBold
                    }
                    
                    RowLayout
                    {
                        id: heartRow
                        Layout.fillWidth: true
                        Layout.topMargin: 40
                        spacing: 0
                        
                        
                        Image
                        {
                            id: heartImage
                            source: Icons.heart
                            fillMode: Image.PreserveAspectFit
                            sourceSize.width: 80
                        }
                        
                        Label
                        {
                            id: thisAppText
                            Layout.fillWidth: true
                            Layout.leftMargin: 22
                            text: "This app is completely free for everyone who just wants to enjoy a book.\n" +
                                  "We hope you enjoy using it! If you do, feel free to leave us a rating and some feedback."
                            wrapMode: Text.WordWrap
                            color: Style.colorText
                            font.pointSize: 14.6
                            lineHeight: 1.05
                        }
                    }
                }
            }
        }
    }
    
    QtObject
    {
        id: internal
        property int pagePadding: 40
    }
}
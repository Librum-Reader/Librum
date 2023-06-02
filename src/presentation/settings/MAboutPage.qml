import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import Librum.elements 1.0
import Librum.models 1.0
import Librum.style 1.0
import Librum.icons 1.0


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
                        text: AppInformation.currentVersion
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
                        text: AppInformation.currentQtVersion
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
                        text: AppInformation.companyName
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
                        text: AppInformation.companyWebsite
                        color: Style.colorBasePurple
                        font.pointSize: 13.5
                        opacity: websiteLinkArea.pressed ? 0.8 : 1
                        
                        MouseArea
                        {
                            id: websiteLinkArea
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            
                            onClicked: Qt.openUrlExternally(AppInformation.companyWebsite)
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
                        text: AppInformation.companyEmail
                        color: Style.colorBasePurple
                        opacity: emailLinkArea.pressed ? 0.8 : 1
                        font.pointSize: 13.5
                        
                        MouseArea
                        {
                            id: emailLinkArea
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            
                            onClicked: Qt.openUrlExternally("mailto:" + AppInformation.companyEmail)
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
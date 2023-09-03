import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.elements
import Librum.controllers


Item
{
    id: root
    height: content.height
    
    Connections
    {
        target: AppInfoController

        function onDownloadingBinariesProgressChanged(progress)
        {
            windowsUpdatingPopup.setDownloadProgress(progress)
        }

        function onApplicaitonUpdateFailed()
        {
            windowsUpdatingPopup.close()
            updateFailedPopup.open()
        }
    }
    
    Pane
    {
        id: content
        anchors.left: parent.left
        anchors.right: parent.right
        horizontalPadding: 40
        topPadding: 32
        bottomPadding: 32
        background: Rectangle
        {
            color: Style.colorContainerBackground
            border.color: Style.colorContainerBorder
            radius: 4
            antialiasing: true
        }
        
        
        ColumnLayout
        {
            id: mainLayout
            width: parent.width
            spacing: 0
            
            Label
            {
                id: newUpdateTitle
                Layout.fillWidth: true
                text: "A new update is available!"
                wrapMode: Text.WordWrap
                color: Style.colorText
                font.pointSize: 23
                font.weight: Font.Bold
            }
            
            Label
            {
                Layout.fillWidth: true
                Layout.topMargin: 7
                text: "Download the new version to get great new improvements."
                wrapMode: Text.WordWrap
                color: Style.colorLightText
                font.pointSize: 15
            }
            
            Label
            {
                Layout.fillWidth: true
                Layout.topMargin: 32
                text: "The newest version is:"
                wrapMode: Text.WordWrap
                color: Style.colorLightText
                font.pointSize: 14.5
            }
            
            Label
            {
                Layout.fillWidth: true
                text: AppInfoController.newestVersion
                wrapMode: Text.WordWrap
                color: Style.colorLightText
                font.pointSize: 14.2
                font.bold: true
            }
            
            MButton
            {
                id: downloadButton
                Layout.preferredWidth: 152
                Layout.preferredHeight: 38
                Layout.topMargin: 14
                borderWidth: 0
                backgroundColor: Style.colorBasePurple
                imagePath: Icons.downloadSelected
                imageSize: 16
                imageSpacing: 8
                text: "Update"
                fontSize: 12.5
                fontWeight: Font.Bold
                textColor: Style.colorFocusedButtonText
                opacityOnPressed: 0.8

                onClicked:
                {
                    if(AppInfoController.operatingSystem === "WIN")
                    {
                        AppInfoController.updateApplication();
                        windowsUpdatingPopup.open()
                    }
                    else
                    {
                        unixUpdatePopup.open();
                    }
                }
            }
            
            Label
            {
                Layout.fillWidth: true
                Layout.topMargin: 56
                text: "See the exact changes on our website at:"
                wrapMode: Text.WordWrap
                color: Style.colorLightText
                font.pointSize: 13.5
            }
            
            Label
            {
                Layout.preferredWidth: implicitWidth
                Layout.minimumWidth: implicitWidth
                text: AppInfoController.newsWebsite
                wrapMode: Text.WordWrap
                font.underline: true
                color: Style.colorBasePurple
                font.pointSize: 13.5
                opacity: newsWebsiteLinkArea.pressed ? 0.8 : 1
                
                MouseArea
                {
                    id: newsWebsiteLinkArea
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    
                    onClicked: Qt.openUrlExternally(AppInfoController.newsWebsite)
                }
            }
        }
    }

    MWindowsUpdatingPopup
    {
        id: windowsUpdatingPopup
        x: root.width / 2 - implicitWidth / 2 - settingsSidebar.width / 2 - content.horizontalPadding
        y: root.height / 2 - content.topPadding
    }

    MWarningPopup
    {
        id: unixUpdatePopup
        x: root.width / 2 - implicitWidth / 2 - settingsSidebar.width / 2 - content.horizontalPadding
        y: root.height / 2 - content.topPadding - 70
        visible: false
        title: "Updating on Linux"
        message: "Please use your package manager to update Librum or download the newest version from our " +
                 '<a href="' + AppInfoController.website + '" style="text-decoration: none; color: '
                 + Style.colorBasePurple + ';">website</a>.'
        leftButtonText: "Close"
        rightButtonText: "Email Us"
        buttonsWidth: 180
        messageBottomSpacing: 10
        richText: true

        onOpenedChanged: if(opened) unixUpdatePopup.giveFocus()
        onRightButtonClicked: Qt.openUrlExternally("mailto:" + AppInfoController.companyEmail)
        onDecisionMade: close()
    }

    MWarningPopup
    {
        id: updateFailedPopup
        x: root.width / 2 - implicitWidth / 2 - settingsSidebar.width / 2 - content.horizontalPadding
        y: root.height / 2 - content.topPadding - 70
        visible: false
        title: "The Update Failed"
        message: "Please try again later or download the newest version from our " +
                 '<a href="' + AppInfoController.website + '" style="text-decoration: none; color: '
                 + Style.colorBasePurple + ';">website</a>.'
        leftButtonText: "Close"
        rightButtonText: "Email Us"
        buttonsWidth: 180
        messageBottomSpacing: 10
        richText: true

        onOpenedChanged: if(opened) updateFailedPopup.giveFocus()
        onRightButtonClicked: Qt.openUrlExternally("mailto:" + AppInfoController.companyEmail)
        onDecisionMade: close()
    }
}
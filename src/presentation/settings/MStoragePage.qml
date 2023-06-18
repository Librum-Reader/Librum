import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.controllers 1.0


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
            spacing: 0
            
            
            RowLayout
            {
                id: titleRow
                Layout.fillWidth: true
                spacing: 0
                
                
                MTitle
                {
                    id: pageTitle
                    Layout.topMargin: 64
                    titleText: "Storage"
                    descriptionText: "Your storage"
                    titleSize: 25
                    descriptionSize: 13.25
                }
                
                Item { Layout.fillWidth: true }
                
                MButton
                {
                    id: upgradeButton
                    Layout.preferredWidth: 118
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignBottom
                    borderWidth: 0
                    backgroundColor: Style.colorBasePurple
                    text: "Upgrade"
                    fontSize: 12
                    fontWeight: Font.Bold
                    textColor: Style.colorFocusedButtonText
                    imagePath: Icons.heartHallow
                    imageSize: 18
                }
            }
            
            Pane
            {
                id: container
                Layout.fillWidth: true
                Layout.topMargin: 32
                padding: 28
                background: Rectangle
                {
                    color: Style.colorContainerBackground
                    border.color: Style.colorContainerBorder
                    radius: 4
                    antialiasing: true
                }
                
                
                ColumnLayout
                {
                    width: parent.width
                    spacing: 26
                    
                    RowLayout
                    {
                        width: parent.width
                        spacing: 26
                        
                        Pane
                        {
                            id: tierPane
                            Layout.fillWidth: true
                            Layout.preferredWidth: 470
                            Layout.maximumWidth: 470
                            Layout.minimumWidth: 235
                            Layout.preferredHeight: 325
                            background: Rectangle
                            {
                                anchors.fill: parent
                                color: Style.colorPageBackground
                                radius: 4
                            }
                            
                            ColumnLayout
                            {
                                width: parent.width
                                
                                Label
                                {
                                    id: tierTitle
                                    Layout.alignment: Qt.AlignLeft
                                    Layout.leftMargin: 24
                                    Layout.topMargin: 6
                                    text: "YOUR TIER"
                                    color: Style.colorPageSubtitle
                                    font.weight: Font.Bold
                                    font.pointSize: 10.5
                                }
                                
                                Label
                                {
                                    id: tierName
                                    Layout.alignment: Qt.AlignHCenter
                                    Layout.topMargin: 54
                                    text: "BASIC"
                                    color: Style.colorTitle
                                    font.weight: Font.Medium
                                    font.pointSize: 22
                                }
                                
                                Label
                                {
                                    id: storageAmount
                                    Layout.alignment: Qt.AlignHCenter
                                    Layout.topMargin: -8
                                    text: "2GB"
                                    color: Style.colorLightText
                                    font.weight: Font.Bold
                                    font.pointSize: 44
                                }
                            }
                        }
                        
                        Pane
                        {
                            id: usedStoragePane
                            Layout.fillWidth: true
                            Layout.minimumWidth: 600
                            Layout.preferredHeight: 325
                            background: Rectangle
                            {
                                anchors.fill: parent
                                color: Style.colorPageBackground
                                radius: 4
                            }
                        }
                        
                    }
                    
                    RowLayout
                    {
                        width: parent.width
                        spacing: 28
                        
                        Pane
                        {
                            id: bookAmountPane
                            Layout.fillWidth: true
                            Layout.preferredWidth: 470
                            Layout.maximumWidth: 470
                            Layout.minimumWidth: 235
                            Layout.preferredHeight: 325
                            background: Rectangle
                            {
                                anchors.fill: parent
                                color: Style.colorPageBackground
                                radius: 4
                            }
                        }
                    }
                }
            }
        }
    }
}
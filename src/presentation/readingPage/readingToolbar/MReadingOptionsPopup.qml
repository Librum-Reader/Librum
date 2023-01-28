import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.0
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0


Popup
{
    id: root
    implicitWidth: 212
    padding: 0
    closePolicy: Popup.CloseOnReleaseOutsideParent | Popup.CloseOnEscape
    background: Rectangle { color: "transparent"; radius: 4 }
    
    
    ColumnLayout
    {
        id: layout
        width: parent.width
        spacing: 0
        
        
        Image
        {
            id: dropletIcon
            Layout.alignment: Qt.AlignRight
            Layout.topMargin: -1
            Layout.rightMargin: 12
            source: Icons.dropupLightGray
            sourceSize.width: 10
            fillMode: Image.PreserveAspectFit
        }
        
        Pane
        {
            id: container
            Layout.fillWidth: true
            implicitWidth: parent.width
            padding: 0
            background: Rectangle
            {
                color: Style.colorBackground
                border.color: Style.colorLightBorder
                border.width: 1
                radius: 4
            }
            
            
            ColumnLayout
            {
                id: contentLayout
                width: parent.width
                spacing: 0
                
                
                RowLayout
                {
                    id: actionRow
                    Layout.leftMargin: (parent.width - implicitWidth) / 2
                    Layout.topMargin: 9
                    Layout.bottomMargin: 9
                    spacing: 12
                    
                    
                    MButton
                    {
                        id: printButton
                        Layout.preferredWidth: 42
                        Layout.preferredHeight: 32
                        backgroundColor: Style.colorLightGray2
                        opacityOnPressed: 0.8
                        borderWidth: 0
                        imagePath: Icons.printer
                        imageSize: 22
                    }
                    
                    MButton
                    {
                        id: downloadButton
                        Layout.preferredWidth: 42
                        Layout.preferredHeight: 32
                        backgroundColor: Style.colorLightGray2
                        opacityOnPressed: 0.8
                        borderWidth: 0
                        imagePath: Icons.downloadBlack
                        imageSize: 22
                        
                        onClicked: downloadFileDialog.open()
                    }
                    
                    MButton
                    {
                        id: shareButton
                        Layout.preferredWidth: 42
                        Layout.preferredHeight: 32
                        backgroundColor: Style.colorLightGray2
                        opacityOnPressed: 0.8
                        borderWidth: 0
                        imagePath: Icons.share
                        imageSize: 22
                    }
                }
                
                Rectangle { Layout.preferredHeight: 1; Layout.fillWidth: true; color: Style.colorLightBorder }
                
                
                ColumnLayout
                {
                    id: checkBoxLayout
                    Layout.fillWidth: true
                    Layout.topMargin: 14
                    Layout.leftMargin: 14
                    Layout.bottomMargin: 16
                    spacing: 10
                    
                    MLabeledCheckBox
                    {
                        id: ttsCheckbox
                        Layout.fillWidth: true
                        Layout.topMargin: 1
                        boxHeight: 19
                        boxWidth: 19
                        text: "Read out (TTS)"
                        fontSize: 12
                        fontWeight: Font.Medium
                        fontColor: Style.colorBaseText
                        spacing: 8
                    }
                    
                    MLabeledCheckBox
                    {
                        id: continuousPagesCheckbox
                        Layout.fillWidth: true
                        boxHeight: 19
                        boxWidth: 19
                        verticalTextOffset: 1
                        text: "Continuous pages"
                        fontSize: 12
                        fontWeight: Font.Medium
                        fontColor: Style.colorBaseText
                        imageDefaultVisibility: true
                        spacing: 8
                    }
                    
                    MLabeledCheckBox
                    {
                        id: displayHorizontallyCheckbox
                        Layout.fillWidth: true
                        boxHeight: 19
                        boxWidth: 19
                        verticalTextOffset: 1
                        text: "Display vertically"
                        fontSize: 12
                        fontWeight: Font.Medium
                        fontColor: Style.colorBaseText
                        imageDefaultVisibility: true
                        spacing: 8
                    }
                }
                
                Rectangle { Layout.preferredHeight: 1; Layout.fillWidth: true; color: Style.colorLightBorder }
                
                
                ColumnLayout
                {
                    id: buttonLayout
                    Layout.fillWidth: true
                    Layout.topMargin: 14
                    Layout.leftMargin: 14
                    Layout.bottomMargin: 16
                    spacing: 10
                    
                    
                    MButton
                    {
                        id: brightnessButton
                        Layout.fillWidth: true
                        Layout.preferredHeight: 21
                        centerContentHorizontally: false
                        backgroundColor: "transparent"
                        opacityOnPressed: 0.7
                        borderWidth: 0
                        text: "Adjust brightness"
                        fontColor: Style.colorBaseText
                        fontSize: 12
                        fontWeight: Font.Medium
                        imagePath: Icons.sun
                        imageSize: 21
                    }
                    
                    MButton
                    {
                        id: syncButton
                        Layout.fillWidth: true
                        Layout.preferredHeight: 21
                        centerContentHorizontally: false
                        backgroundColor: "transparent"
                        opacityOnPressed: 0.7
                        borderWidth: 0
                        text: "Sync book"
                        fontColor: Style.colorBaseText
                        fontSize: 12
                        fontWeight: Font.Medium
                        imagePath: Icons.sync
                        imageSize: 16
                        imageLeftMargin: 2
                        imageSpacing: 11
                    }
                    
                    MButton
                    {
                        id: moreButton
                        Layout.fillWidth: true
                        Layout.preferredHeight: 21
                        centerContentHorizontally: false
                        backgroundColor: "transparent"
                        opacityOnPressed: 0.7
                        borderWidth: 0
                        text: "More options"
                        fontColor: Style.colorBaseText
                        fontSize: 12
                        fontWeight: Font.Medium
                        imagePath: Icons.settingsGrayHallow
                        imageSize: 20
                        imageLeftMargin: 1
                        imageSpacing: 8
                        

                        onClicked:
                        {
                            loadSettingsAppearancePage();
                            root.close();
                        }                   
                    }
                }
            }
        }
    }
    
    FileDialog
    {
        id: downloadFileDialog
        acceptLabel: "Save"
        fileMode: FileDialog.SaveFile
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
    }
}
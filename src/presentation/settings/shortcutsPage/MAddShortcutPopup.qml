import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons


Popup
{
    id: root
    signal applied
    
    implicitWidth: 594
    focus: true
    padding: 0
    background: Rectangle
    {
        color: Style.colorBackground
        radius: 6
    }
    
    modal: true
    Overlay.modal: Rectangle
    {
        color: "#aa32324D"
        opacity: 1
    }
    
    
    ColumnLayout
    {
        id: mainLayout
        anchors.fill: parent
        spacing: 0
        
        
        MButton
        {
            id: closeButton
            Layout.preferredHeight: 32
            Layout.preferredWidth: 32
            Layout.topMargin: 12
            Layout.rightMargin: 14
            Layout.alignment: Qt.AlignTop | Qt.AlignRight
            backgroundColor: "transparent"
            opacityOnPressed: 0.7
            borderColor: "transparent"
            radius: 6
            borderColorOnPressed: Style.colorLightBorder
            imagePath: Icons.closeBlack
            imageSize: 14
            
            onClicked: root.close()
        }
        
        Pane
        {
            id: content
            Layout.fillWidth: true
            Layout.topMargin: 18
            topPadding: 0
            horizontalPadding: 52
            bottomPadding: 42
            background: Rectangle
            {
                color: "transparent"
                radius: 6
            }
            
            
            ColumnLayout
            {
                id: contentLayout
                width: parent.width
                spacing: 0
                
                Label
                {
                    id: title
                    text: "Edit Shortcuts"
                    font.weight: Font.Bold
                    font.pointSize: 17
                    font.family: Style.defaultFontFamily
                    color: Style.colorBaseTitle
                }
                
                
                RowLayout
                {
                    id: buttonLayout
                    Layout.fillWidth: true
                    Layout.topMargin: 42
                    spacing: 26
                    
                    
                    MComboBox
                    {
                        id: actionComboBox
                        Layout.fillWidth: true
                        Layout.preferredHeight: 60
                        headerText: "Action"
                        headerFontSize: 12
                        headerFontColor: Style.colorBaseTitle
                        headerFontWeight: Font.DemiBold
                        titleFontColor: Style.colorLightText2
                        titleFontSize: 12.5
                        titleSpacing: 3
                        titleEmptyText: "None selected"
                        contentFontSize: 12.5
                        
                        imagePath: Icons.dropdownGray
                        imageSize: 11
                        maxPopupHeight: 208
                        
                        listContent: listModel
                        
                        ListModel
                        {
                            id: listModel
                            ListElement { content: "Any" }
                            ListElement { content: "Pdf" }
                            ListElement { content: "Any" }
                            ListElement { content: "Pdf" }
                            ListElement { content: "Epub"  }
                            ListElement { content: "Mobi" }
                            ListElement { content: "Epub"  }
                            ListElement { content: "Mobi" }
                        }
                    }
                    
                    ColumnLayout
                    {
                        id: keyInputButton
                        spacing: 2
                        
                        Label
                        {
                            id: recordText
                            Layout.fillWidth: true
                            text: "Key"
                            font.weight: Font.DemiBold
                            font.pointSize: 12
                            font.family: Style.defaultFontFamily
                            color: Style.colorBaseTitle
                        }
                        
                        MButton
                        {
                            id: recordButton
                            Layout.fillWidth: true
                            Layout.preferredHeight: 38
                            radius: 4
                            borderColor: Style.colorLightBorder
                            backgroundColor: "transparent"
                            text: "Press to record"
                            fontSize: 13
                            fontColor: Style.colorBrightText
                            fontWeight: Font.Bold
                        }
                    }
                }
            }
        }
    }
}
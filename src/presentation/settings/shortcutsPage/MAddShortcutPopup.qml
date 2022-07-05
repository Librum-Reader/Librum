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
    closePolicy: Popup.CloseOnPressOutside
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
    
    onOpenedChanged: if(opened) downloadButton.forceActiveFocus()
    
    
    Shortcut
    {
        sequence: "Ctrl+R"
        onActivated: recordBox.startRecording()
    }
    
    
    ColumnLayout
    {
        id: mainLayout
        width: parent.width
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
                        itemHeight: 32
                        headerText: "Action"
                        headerFontSize: 12
                        headerFontColor: Style.colorBaseTitle
                        headerFontWeight: Font.DemiBold
                        titleFontColor: Style.colorLightText3
                        titleFontSize: 12.5
                        titleSpacing: 3
                        titleEmptyText: "None selected"
                        contentFontColor: Style.colorLightText3
                        contentFontSize: 12.5
                        defaultIndex: -1
                        
                        imagePath: Icons.dropdownGray
                        imageSize: 11
                        maxPopupHeight: 208
                        
                        listContent: listModel
                        
                        ListModel
                        {
                            id: listModel
                            ListElement { content: "Page adsdsa asd asd af fda sasd" }
                            ListElement { content: "Any" }
                            ListElement { content: "Pdf" }
                            ListElement { content: "Epub"  }
                            ListElement { content: "Mobi" }
                            ListElement { content: "Epub"  }
                            ListElement { content: "Mobi" }
                            ListElement { content: "Epub"  }
                            ListElement { content: "Mobi" }
                        }
                    }
                    
                    
                    MRecordKeyBox
                    {
                        id: recordBox
                        Layout.fillWidth: true
                        Layout.preferredHeight: 60
                        itemToRedirectFocusTo: downloadButton
                    }
                }
                
                RowLayout
                {
                    id: decisionLayout
                    Layout.topMargin: 96
                    spacing: 16
                    
                    
                    MButton
                    {
                        id: downloadButton
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 38
                        active: true
                        borderWidth: active ? 0 : 1
                        borderColor: Style.colorLightBorder
                        backgroundColor: active ? Style.colorBasePurple : "transparent"
                        text: "Apply"
                        fontColor: active ? Style.colorBrightText : Style.colorLightText2
                        fontWeight: Font.Bold
                        fontSize: 12
                        
                        onClicked: root.close()
                        
                        Keys.onPressed:
                            (event) =>
                            {
                                if(event.key === Qt.Key_Right || event.key === Qt.Key_Tab)
                                {
                                    downloadButton.active = false;
                                    cancelButton.active = true;
                                }
                                else if(event.key === Qt.Key_Return)
                                {
                                    root.close();
                                }
                            }
                        
                        KeyNavigation.right: cancelButton
                        KeyNavigation.tab: cancelButton
                    }
                    
                    MButton
                    {
                        id: cancelButton
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 38
                        borderWidth: active ? 0 : 1
                        borderColor: Style.colorLightBorder
                        backgroundColor: active ? Style.colorBasePurple : "transparent"
                        opacityOnPressed: 0.7
                        text: "Cancel"
                        fontColor: active ? Style.colorBrightText : Style.colorLightText2
                        fontWeight: Font.Bold
                        fontSize: 12
                        
                        onClicked: root.close()
                        
                        Keys.onPressed:
                            (event) =>
                            {
                                if(event.key === Qt.Key_Left || event.key === Qt.Key_Tab)
                                {
                                    cancelButton.active = false;
                                    downloadButton.active = true;
                                }
                                else if(event.key === Qt.Key_Return)
                                {
                                    root.close();
                                }
                            }
                        
                        KeyNavigation.left: downloadButton
                        KeyNavigation.tab: downloadButton
                    }
                }
            }
        }
    }
}
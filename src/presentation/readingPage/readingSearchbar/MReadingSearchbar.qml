import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0


Item
{
    id: root
    signal textChanged(string newText)
    signal nextButtonWasClicked
    signal previousButtonWasClicked
    
    implicitHeight: 48
    implicitWidth: 1000
    
    // Make sure to remove the focus from the textinput when the searchbar is closed
    onVisibleChanged: visible ? inputField.forceActiveFocus() : root.forceActiveFocus()
    
    // Close on pressing escape
    Keys.onPressed:
        (event) =>
        {
            if(event.key === Qt.Key_Escape)
            {
                root.visible = false;
                event.accepted = true;
            }
        }
    
    ColumnLayout
    {
        id: layout
        anchors.fill: parent
        spacing: 0
        
        
        Rectangle
        {
            id: topBorder
            color: Style.colorDarkSeparator
            Layout.preferredHeight: 1
            Layout.fillWidth: true
        }
        
        Pane
        {
            id: container
            Layout.fillHeight: true
            Layout.fillWidth: true
            padding: 8
            background: Rectangle { color: Style.colorContainerBackground }
            
            
            RowLayout
            {
                id: contentLayout
                spacing: 12
                anchors.fill: parent
                
                
                MButton
                {
                    id: closeButton
                    Layout.preferredWidth: 32
                    Layout.preferredHeight: 30
                    backgroundColor: Style.colorHighlight
                    opacityOnPressed: 0.8
                    borderWidth: 0
                    radius: 4
                    imagePath: Icons.closePurple
                    imageSize: 12
                    
                    onClicked: root.visible = false
                }
                
                MButton
                {
                    id: optionsButton
                    Layout.preferredWidth: 82
                    Layout.preferredHeight: 30
                    text: "Options"
                    fontSize: 12
                    textColor: Style.colorUnfocusedButtonText
                    fontWeight: Font.Normal
                    backgroundColor: Style.colorHighlight
                    opacityOnPressed: 0.8
                    borderWidth: 0
                    radius: 4
                    
                    onClicked: optionsPopup.opened ? optionsPopup.close() : optionsPopup.open()
                }
                
                Pane
                {
                    id: inputFieldContainer
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    horizontalPadding: 12
                    verticalPadding: 0
                    background: Rectangle
                    {
                        color: Style.colorButtonBackground
                        border.color: Style.colorContainerBorder
                        border.width: 1
                        radius: 4
                    }
                    
                    
                    RowLayout
                    {
                        id: inputFieldLayout
                        anchors.fill: parent
                        spacing: 6
                        
                        
                        Image
                        {
                            id: searchIcon
                            source: Icons.searchGray
                            sourceSize.width: 14
                            fillMode: Image.PreserveAspectFit
                        }
                        
                        TextField
                        {
                            id: inputField
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignVCenter
                            leftPadding: 4
                            selectByMouse: true
                            color: Style.colorBaseInputText
                            font.pointSize: 12
                            placeholderText: "Find"
                            placeholderTextColor: Style.colorPlaceholderText
                            background: Rectangle { anchors.fill: parent; color: "transparent" }
                            
                            onTextChanged: root.textChanged(text)
                        }
                    }
                }
                
                MButton
                {
                    id: nextButton
                    Layout.preferredWidth: 81
                    Layout.preferredHeight: 30
                    text: "Next"
                    fontSize: 12
                    textColor: Style.colorUnfocusedButtonText
                    fontWeight: Font.Normal
                    backgroundColor: Style.colorHighlight
                    opacityOnPressed: 0.8
                    borderWidth: 0
                    radius: 4
                    imagePath: Icons.arrowtopPurpleDown
                    imageSize: 13
                    imageSpacing: 10
                    
                    onClicked: root.nextButtonWasClicked()
                }
                
                MButton
                {
                    id: previousButton
                    Layout.preferredWidth: 110
                    Layout.preferredHeight: 30
                    text: "Previous"
                    fontSize: 12
                    textColor: Style.colorUnfocusedButtonText
                    fontWeight: Font.Normal
                    backgroundColor: Style.colorHighlight
                    opacityOnPressed: 0.8
                    borderWidth: 0
                    radius: 4
                    imagePath: Icons.arrowtopPurpleUp
                    imageSize: 13
                    imageSpacing: 10
                    
                    onClicked: root.previousButtonWasClicked()
                }
            }
        }
    }
    
    MReadingSearchbarOptionsPopup
    {
        id: optionsPopup
        x: optionsButton.x + 8
        y: -optionsPopup.height - 1
    }
    
    
    function giveFocus()
    {
        inputField.forceActiveFocus();
    }
}
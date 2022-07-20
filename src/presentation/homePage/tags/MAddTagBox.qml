import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons


Item
{
    id: root
    signal addTag(string name)
    
    implicitHeight: 36
    implicitWidth: 400
    
    
    RowLayout
    {
        id: layout
        anchors.fill: parent
        spacing: 8
        
        
        Pane
        {
            id: box
            Layout.fillWidth: true
            Layout.fillHeight: true
            padding: 0
            background: Rectangle
            {
                color: "transparent"
                border.color: Style.colorLightBorder
                radius: 4
            }
            
            
            RowLayout
            {
                id: inBoxLayout
                anchors.fill: parent
                spacing: 4
                
                
                TextField
                {
                    id: inputField
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    verticalAlignment: TextInput.AlignVCenter
                    leftPadding: 12
                    rightPadding: 4
                    selectByMouse: true
                    color: Style.colorBaseText
                    font.pointSize: 12
                    font.family: Style.defaultFontFamily
                    font.weight: Font.Normal
                    placeholderText: "Add a tag..."
                    placeholderTextColor: Style.colorLightText
                    background: Rectangle
                    {   
                        anchors.fill: parent
                        color: "transparent"
                        radius: 4
                    }
                    
                    
                    Keys.onReturnPressed:
                    {
                        if(inputField.text !== "")
                            root.addTag(inputField.text);
                        
                        inputField.clear();
                    }
                }
                
                Item
                {
                    id: iconBlock
                    Layout.preferredWidth: icon.width + 18
                    Layout.preferredHeight: icon.height + 14
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    Layout.rightMargin: 6
                    
                    
                    Image
                    {
                        id: icon
                        anchors.centerIn: parent
                        sourceSize.width: 10
                        source: Icons.dropdownGray
                        rotation: 180
                        fillMode: Image.PreserveAspectFit
                        
                        
                        NumberAnimation
                        {
                            id: closeAnim
                            target: icon
                            property: "rotation"
                            to: 180
                            duration: 175
                            easing.type: Easing.InOutQuad
                        }
                        
                        NumberAnimation
                        {
                            id: openAnim
                            target: icon
                            property: "rotation"
                            to: 360
                            duration: 175
                            easing.type: Easing.InOutQuad
                        }
                    }
                    
                    MouseArea
                    {
                        anchors.fill: parent
                        
                        onClicked:
                        {
                            if(popup.opened)
                            {
                                popup.close();
                                closeAnim.start();
                                return;
                            }
                                
                            popup.open();
                            openAnim.start();
                        }
                    }
                }
            }
        }
        
        Label
        {
            id: addLabel
            Layout.fillHeight: true
            verticalAlignment: Text.AlignVCenter
            text: "ADD"
            opacity: mouseArea.pressed ? 0.8 : 1
            color: Style.colorBasePurple
            font.pointSize: 11.5
            font.weight: Font.Bold
            font.family: Style.defaultFontFamily
            
            
            MouseArea
            {
                id: mouseArea
                anchors.fill: parent
                
                onClicked:
                {
                    if(inputField.text !== "")
                        root.addTag(inputField.text);
                    
                    inputField.clear();
                }
            }
        }
    }
    
    
    MAddTagBoxPopup
    {
        id: popup
        width: box.width
        y: root.height + 6
        
        onItemSelected: root.addTag(currentlySelectedData)
    }
    
    
    
    function giveFocus()
    {
        inputField.forceActiveFocus();
    }
}
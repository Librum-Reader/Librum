import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons


Item
{
    id: root
    implicitHeight: 49
    implicitWidth: 1000
    
    
    ColumnLayout
    {
        id: mainLayout
        anchors.fill: parent
        spacing: 0
        
        
        Rectangle
        {
            id: topBorder
            color: Style.colorLightBorder
            Layout.preferredHeight: 1
            Layout.fillWidth: true
        }
        
        
        Pane
        {
            id: container
            Layout.fillHeight: true
            Layout.fillWidth: true
            padding: 8
            background: Rectangle
            {
                color: Style.colorBackground
            }
            
            
            RowLayout
            {
                id: itemLayout
                spacing: 12
                anchors.fill: parent
                
                
                MButton
                {
                    id: closeButton
                    Layout.preferredWidth: 32
                    Layout.preferredHeight: 32
                    backgroundColor: Style.colorLightPurple2
                    opacityOnPressed: 0.8
                    borderWidth: 0
                    buttonRadius: 4
                    imagePath: Icons.closePurple
                    imageSize: 12
                }
                
                MButton
                {
                    id: optionsButton
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 32
                    text: "Options"
                    fontSize: 13
                    fontColor: Style.colorLightText3
                    fontWeight: Font.Normal
                    backgroundColor: Style.colorLightPurple2
                    opacityOnPressed: 0.8
                    borderWidth: 0
                    buttonRadius: 4
                }
                
                Pane
                {
                    id: inputFieldContainer
                    Layout.fillWidth: true
                    Layout.preferredHeight: 32
                    horizontalPadding: 12
                    verticalPadding: 0
//                    verticalPadding: 9
                    background: Rectangle
                    {
                        color: Style.colorBackground
                        border.color: Style.colorLightBorder
                        border.width: 1
                        radius: 4
                    }
                    
                    
                    RowLayout
                    {
                        id: inInputLayout
                        anchors.fill: parent
                        spacing: 8
                        
                        
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
                            selectByMouse: true
                            color: Style.colorBaseText
                            font.pointSize: 12
                            font.family: Style.defaultFontFamily
                            placeholderText: "Find"
                            placeholderTextColor: Style.colorLightText
                            background: Rectangle
                            {
                                anchors.fill: parent
                                color: "transparent"
                            }
                        }
                    }
                }
            
                MButton
                {
                    id: nextButton
                    Layout.preferredWidth: 81
                    Layout.preferredHeight: 32
                    text: "Next"
                    fontSize: 13
                    fontColor: Style.colorLightText3
                    fontWeight: Font.Normal
                    backgroundColor: Style.colorLightPurple2
                    opacityOnPressed: 0.8
                    borderWidth: 0
                    buttonRadius: 4
                    imagePath: Icons.arrowtopPurpleDown
                    imageSize: 13
                    imageSpacing: 10
                }
                
                MButton
                {
                    id: previousButton
                    Layout.preferredWidth: 110
                    Layout.preferredHeight: 32
                    text: "Previous"
                    fontSize: 13
                    fontColor: Style.colorLightText3
                    fontWeight: Font.Normal
                    backgroundColor: Style.colorLightPurple2
                    opacityOnPressed: 0.8
                    borderWidth: 0
                    buttonRadius: 4
                    imagePath: Icons.arrowtopPurpleUp
                    imageSize: 13
                    imageSpacing: 10
                }
            }
        }
    }
}
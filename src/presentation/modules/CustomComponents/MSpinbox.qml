import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.icons


Item
{
    id: root
    property bool invalid: false
    property int value: 14
    
    implicitWidth: 72
    implicitHeight: 34
    
    
    Pane
    {
        id: container
        anchors.fill: parent
        padding: 0
        background: Rectangle
        {
            color: Style.colorBackground
            border.color: root.invalid ? Style.colorBaseRed : Style.colorLightBorder
            border.width: root.invalid ? 2 : 1
            radius: 4
        }
        
        
        RowLayout
        {
            id: mainLayout
            anchors.fill: parent
            spacing: 0
            
            
            TextField
            {
                id: inputField
                Layout.fillHeight: true
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                selectByMouse: true
                color: Style.colorLightText3
                font.pointSize: 12
                font.weight: Font.Bold
                font.family: Style.defaultFontFamily
                validator: IntValidator { bottom: 1; top: 99 }
                text: root.value.toString()
                background: Rectangle   
                {   
                    anchors.fill: parent
                    radius: 4
                    color: "transparent"
                }
                
                onTextEdited:
                {
                    
                    if(text <= 0)
                    {
                        root.invalid = true;
                    }
                    else
                    {
                        root.value = text;
                        root.invalid = false;
                    }
                }
            }
            
            ColumnLayout
            {
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                Layout.rightMargin: 14
                spacing: 4
                
                
                Image
                {
                    source: Icons.dropdownGray
                    sourceSize.width: 9
                    fillMode: Image.PreserveAspectFit
                    rotation: 180
                    
                    MouseArea
                    {
                        anchors.fill: parent
                        
                        onClicked:
                        {
                            if(root.value < 99)
                                root.value += 1;
                        }
                    }
                }
                
                Image
                {
                    source: Icons.dropdownGray
                    sourceSize.width: 9
                    fillMode: Image.PreserveAspectFit
                    
                    MouseArea
                    {
                        anchors.fill: parent
                        
                        onClicked:
                        {
                            if(root.value > 1)
                                root.value -= 1;
                        }
                    }
                }
            }
        }
    }
}
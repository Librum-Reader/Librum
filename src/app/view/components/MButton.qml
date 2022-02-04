import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope
{
    id: root
    
    property int buttonWidth : 100
    property int buttonHeight : 40
    property int buttonRadius : 4
    property color backgroundColor : "red"
    
    property color fontColor : "black"
    property string textContent : "Text here"
    property bool fontBold : false
    property int fontSize : 11

    signal clicked()
    
    
    Rectangle
    {
        id: prvt
        height: root.buttonHeight
        width:  root.buttonWidth
        color: root.backgroundColor
        radius: root.buttonRadius
        
        Label
        {
            id: loginButtonText
            anchors.centerIn: parent
            text: root.textContent
            color: root.fontColor
            font.bold: root.fontBold
            font.pointSize: root.fontSize
        }
        
        MouseArea
        {
            anchors.fill: parent
            
            onClicked: root.clicked()
            onPressedChanged:
            {
                if(pressed)
                    loginButton.opacity = 0.9;
                else
                    loginButton.opacity = 1;
            }
        }
    }
}
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Rectangle
{
    id: root
    height: 40
    width:  parent.width
    Layout.topMargin: 26
    color: "#4945FF"
    radius: 4
    
    property var inputField1
    property var inputField2
    
    signal clicked()
    signal clearInputs()
    
    Label
    {
        id: loginButtonText
        anchors.centerIn: parent
        text: "Login"
        color: "white"
        font.bold: true
        font.pointSize: 11
    }
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked:
        {
            root.clicked()
            root.clearInputs();
        }
        
        onPressedChanged:
        {
            if(pressed)
                loginButton.opacity = 0.9
            else
                loginButton.opacity = 1
        }
    }
}
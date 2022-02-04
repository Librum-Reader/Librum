import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page
{
    id: root
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.loginWindowBackground
    }
    
    property int baseHeight : 800
    property int baseWidth : 542
    
    ColumnLayout
    {
        id: layout
        anchors.centerIn: parent
        width: root.baseWidth
        
        Rectangle
        {
            id: containerRect
            height: root.baseHeight
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            radius: 4
            color: properties.loginContentBackground
        }
        
        Label
        {
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 14
            text: "Already have an account? Login"
            font.pointSize: 10
            color: properties.colorBasePurple
            
            MouseArea
            {
                anchors.fill: parent
                onClicked: loadPage("LoginPage");
            }
        }
    }
}

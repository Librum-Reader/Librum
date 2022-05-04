import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents

FocusScope
{
    id: root
    implicitWidth: layout.width
    implicitHeight: layout.height
    
    
    RowLayout
    {
        id: layout
        width: parent.width
        
        MCheckBox
        {
            id: rememberMeCheckBox
            Layout.preferredWidth: 20
            Layout.preferredHeight: 20
        }
        
        Label
        {
            id: rememberMeText
            text: "Remember me"
            Layout.alignment: Qt.AlignVCenter
            Layout.leftMargin: 4
            font.pointSize: 11
            color: properties.colorMediumText
        }
        
        Item
        {
            Layout.fillWidth: true
        }
        
        Label
        {
            id: forgotPasswordLabel
            text: "Forgot password?"
            Layout.alignment: Qt.AlignVCenter
            Layout.leftMargin: 3
            font.pointSize: 10
            color: properties.colorBasePurple
            
            MouseArea
            {
                anchors.fill: parent
                
                onClicked:
                {
                    loadPage("ForgotPasswordPage");
                }
            }
        }
    }
}

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../components"

FocusScope
{
    id: root
    implicitWidth: layout.width
    implicitHeight: layout.height
    
    
    RowLayout
    {
        id: layout
        
        MCheckBox
        {
            id: rememberMeCheckBox
            Layout.preferredWidth: 20
            Layout.preferredHeight: 20
            imagePath: "/resources/images/check.svg"
            borderRadius: 4
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
        
        Item {
            Layout.preferredWidth: 131
            Layout.preferredHeight: 1
        }
        
        Label
        {
            id: forgotPasswordLabel
            text: "Forgot password?"
            Layout.alignment: Qt.AlignVCenter
            Layout.leftMargin: 3
            font.pointSize: 10
            color: properties.colorBasePurple
        }
    }
}

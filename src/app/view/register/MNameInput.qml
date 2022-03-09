import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../components"


FocusScope
{
    id: root
    implicitWidth: layout.implicitWidth
    implicitHeight: layout.implicitHeight
    
    RowLayout
    {
        id: layout
        Layout.alignment: Qt.AlignHCenter
        spacing: 28
        
        MLabeledInputBox
        {
            id: firstNameInput
            Layout.preferredWidth: 186
            Layout.preferredHeight: 50
            headerText: 'First name'
            placeholderContent: "Kai"
            placeholderColor: properties.colorLightText
            autoFocus: true
        }
        
        MLabeledInputBox
        {
            id: lastNameInput
            Layout.preferredWidth: 186
            Layout.preferredHeight: 50
            headerText: "Last name"
            placeholderContent: "Doe"
            placeholderColor: properties.colorLightText
        }
    }
}

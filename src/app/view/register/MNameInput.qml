import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../components"


FocusScope
{
    id: root
    implicitWidth: nameInputRow.implicitWidth
    implicitHeight: nameInputRow.implicitHeight
    
    RowLayout
    {
        id: nameInputRow
        spacing: 28
        Layout.alignment: Qt.AlignHCenter
        
        MLabeledInputBox
        {
            id: firstNameInput
            Layout.preferredWidth: 186
            Layout.preferredHeight: 50
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
            headerText: 'First name'
            placeholderContent: "Kai"
            placeholderColor: properties.colorLightText
        }
        
        MLabeledInputBox
        {
            id: lastNameInput
            Layout.preferredWidth: 186
            Layout.preferredHeight: 50
            Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
            headerText: "Last name"
            placeholderContent: "Doe"
            placeholderColor: properties.colorLightText
        }
    }
}

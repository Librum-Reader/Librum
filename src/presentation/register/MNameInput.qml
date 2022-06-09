import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents


Item
{
    id: root
    implicitWidth: 372
    implicitHeight: 50
    
    RowLayout
    {
        id: layout
        Layout.alignment: Qt.AlignHCenter
        spacing: 28
        
        MLabeledInputBox
        {
            id: firstNameInput
            Layout.preferredWidth: root.width / 2 - layout.spacing / 2
            Layout.preferredHeight: root.height
            headerText: 'First name'
            placeholderContent: "Kai"
            placeholderColor: properties.colorLightText
            autoFocus: true
        }
        
        MLabeledInputBox
        {
            id: lastNameInput
            Layout.preferredWidth: root.width / 2 - layout.spacing / 2
            Layout.preferredHeight: root.height
            headerText: "Last name"
            placeholderContent: "Doe"
            placeholderColor: properties.colorLightText
        }
    }
}

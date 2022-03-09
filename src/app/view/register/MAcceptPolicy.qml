import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../components"


FocusScope
{
    id: root
    property bool activated : false
    
    implicitWidth: layout.width
    implicitHeight: layout.height
    
    
    RowLayout
    {
        id: layout
        Layout.fillWidth: true
        spacing: 10
        
        MCheckBox
        {
            id: checkBox
            Layout.preferredWidth: 20
            Layout.preferredHeight: 20
            imagePath: "/resources/images/check.svg"
            borderColor: properties.colorDarkBorder
            borderRadius: 4
            
            onClicked: root.activated = !root.activated;
        }
        
        Label
        {
            id: text
            Layout.fillWidth: true
            text: 'I accept the <font color=' + properties.colorBasePurple + '>terms</font> 
                   and the <font color=' + properties.colorBasePurple + '>privacy policy.</font>'
            textFormat: Text.RichText
            wrapMode: Text.WordWrap
            font.pointSize: 11
            color: properties.colorMediumText
        }
    }
}

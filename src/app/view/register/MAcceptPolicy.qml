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
        Layout.fillWidth: true
        spacing: 4
        
        MCheckBox
        {
            id: iAcceptCheckBox
            Layout.preferredWidth: 20
            Layout.preferredHeight: 20
            imagePath: "/resources/images/check.svg"
            borderColor: properties.colorDarkBorder
            borderRadius: 4
        }
        
        Label
        {
            id: iAcceptTextFirst
            Layout.fillWidth: true
            Layout.leftMargin: 6
            text: 'I accept the <font color=' + properties.colorBasePurple + '>terms</font> 
                   and the <font color=' + properties.colorBasePurple + '>privacy policy.</font>'
            textFormat: Text.RichText
            wrapMode: Text.WordWrap
            font.pointSize: 11
            color: properties.colorMediumText
        }
    }
}

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents


Item
{
    id: root
    property bool activated : false
    
    implicitWidth: 200
    implicitHeight: layout.height
    
    
    RowLayout
    {
        id: layout
        width: parent.width
        spacing: 10
        
        MCheckBox
        {
            id: checkBox
            Layout.preferredWidth: 20
            Layout.preferredHeight: 20
            
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

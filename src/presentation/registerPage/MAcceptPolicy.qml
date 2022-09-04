import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Librum.style 1.0
import CustomComponents 1.0


Item
{
    id: root
    property bool activated : false
    signal keyUp()
    signal keyDown()
    
    implicitWidth: 200
    implicitHeight: layout.height
    
    
    RowLayout
    {
        id: layout
        width: parent.width
        spacing: 10
        
        MCheckBox
        {
            id: acceptCheckBox
            Layout.preferredWidth: 20
            Layout.preferredHeight: 20
            
            onClicked: root.activated = !root.activated;
            
            Keys.onPressed: 
                (event) =>
                {
                    if(event.key === Qt.Key_Return)
                    {
                        acceptCheckBox.actionOnClicked();
                        root.activated = !root.activated;
                    }
                    else if(event.key === Qt.Key_Down)
                    {
                        root.keyDown();
                    }
                    else if(event.key === Qt.Key_Up)
                    {
                        root.keyUp();
                    }
                }
        }
        
        Label
        {
            id: text
            Layout.fillWidth: true
            text: 'I accept the <font color=' + Style.colorBasePurple + '>terms</font> 
                   and the <font color=' + Style.colorBasePurple + '>privacy policy.</font>'
            textFormat: Text.RichText
            wrapMode: Text.WordWrap
            font.pointSize: 11
            color: Style.colorMediumText
        }
    }
    
    
    function giveFocus()
    {
        acceptCheckBox.forceActiveFocus();
    }
}

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import CustomComponents


Item
{
    id: root
    property bool activated: acceptCheckBox.checked
    property bool hasError: false
    signal keyUp()
    signal keyDown()
    
    implicitWidth: 200
    implicitHeight: layout.height
    
    onActivatedChanged: if(root.hasError) root.clearError()
    
    
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
            
            Keys.onPressed: 
                (event) =>
                {
                    if(event.key === Qt.Key_Return)
                    {
                        acceptCheckBox.toggle();
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
            text: 'I accept the <font color=' + Style.colorBasePurple + '>Terms of Service</font> 
                       and the <font color=' + Style.colorBasePurple + '>privacy policy.</font>'
            textFormat: Text.RichText
            wrapMode: Text.WordWrap
            font.pointSize: 11
            color: Style.colorText
        }
    }
    
    
    function giveFocus()
    {
        acceptCheckBox.forceActiveFocus();
    }
    
    function setError() 
    {
        root.hasError = true;
        acceptCheckBox.borderWidth = 2;
        acceptCheckBox.borderColor = Style.colorErrorBorder;
        acceptCheckBox.uncheckedBackgroundColor = Style.colorErrorBackground;
    }
    
    function clearError()
    {
        root.hasError = false;
        acceptCheckBox.borderWidth = 1;
        acceptCheckBox.borderColor = Style.colorCheckboxBorder;
        acceptCheckBox.uncheckedBackgroundColor = "transparent";
    }
}

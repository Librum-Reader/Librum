import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import Librum.models
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
            textFormat: Text.RichText
            text: 'I accept the <a href="' + AppInformation.companyWebsite + '/TermsOfService " style="text-decoration: none; color: ' + Style.colorBasePurple + ';">Terms of Service</a>' +
                  ' and the <a href="' + AppInformation.companyWebsite + '/privacypolicy " style="text-decoration: none; color: ' + Style.colorBasePurple + ';">Privacy Policy</a>' 
            onLinkActivated: Qt.openUrlExternally(link)
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

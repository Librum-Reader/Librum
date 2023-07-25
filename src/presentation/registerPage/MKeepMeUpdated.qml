import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import CustomComponents


Item
{
    id: root
    property bool checked : false
    signal keyUp()
    signal keyDown()
    
    implicitWidth: 100
    implicitHeight: layout.height
    
    
    RowLayout
    {
        id: layout
        Layout.fillWidth: true
        spacing: 4
        
        MCheckBox
        {
            id: updatesCheckBox
            Layout.preferredWidth: 20
            Layout.preferredHeight: 20
            
            onClicked: root.checked = !root.checked;
            
            Keys.onPressed: 
                (event) =>
                {
                    if(event.key === Qt.Key_Return)
                    {
                        updatesCheckBox.toggle();
                        root.checked = !root.checked;
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
        
        Item
        {
            id: keepMeUpdatedText
            Layout.fillWidth: true
            Layout.preferredHeight: keepMeUpdatedTextFirst.implicitHeight
            Layout.leftMargin: 6
            
            Column
            {
                spacing: 2
                
                Label
                {
                    id: keepMeUpdatedTextFirst
                    text: "Keep me updated about new features and"
                    wrapMode: Text.WordWrap
                    font.pointSize: 11
                    color: Style.colorText
                }
                
                Label
                {
                    id: keepMeUpdatedTextSecond
                    text: "upcoming improvements."
                    wrapMode: Text.WordWrap
                    font.pointSize: 11
                    color: Style.colorText
                }
            }
        }
    }
    
    function giveFocus()
    {
        updatesCheckBox.giveFocus();
    }
}

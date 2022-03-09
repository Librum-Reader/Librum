import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "../components"


FocusScope
{
    id: root
    implicitWidth: keepMeUpdatedRow.width
    implicitHeight: keepMeUpdatedRow.height    
    
    
    RowLayout
    {
        id: keepMeUpdatedRow
        Layout.fillWidth: true
        spacing: 4
        
        MCheckBox
        {
            id: updatesCheckBox
            Layout.preferredWidth: 20
            Layout.preferredHeight: 20
            imagePath: "/resources/images/check.svg"
            borderColor: properties.colorDarkBorder
            borderRadius: 4
        }
        
        Item
        {
            id: keepMeUpdatedText
            Layout.preferredHeight: keepMeUpdatedTextFirst.implicitHeight
            Layout.fillWidth: true
            Layout.leftMargin: 6
            
            Column
            {
                spacing: 2
                
                Label
                {
                    id: keepMeUpdatedTextFirst
                    text: "Keep me updated about the new features and"
                    wrapMode: Text.WordWrap
                    font.pointSize: 11
                    color: properties.colorMediumText
                }
                
                Label
                {
                    id: keepMeUpdatedTextSecond
                    text: "upcoming improvements."
                    wrapMode: Text.WordWrap
                    font.pointSize: 11
                    color: properties.colorMediumText
                }
            }
        }
    }
}

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0


Popup
{
    id: root
    implicitWidth: 190
    closePolicy: Popup.CloseOnReleaseOutsideParent | Popup.CloseOnEscape
    padding: 0
    background: Rectangle
    {
        color: "transparent"
    }
    
    
    ColumnLayout
    {
        id: mainLayout
        width: parent.width
        spacing: 0
        
        
        Pane
        {
            id: container
            Layout.fillWidth: true
            verticalPadding: 15
            horizontalPadding: 12
            background: Rectangle
            {
                color: Style.colorPopupBackground
                border.color: Style.colorContainerBorder
                border.width: 1
                radius: 4
            }
            
            
            ColumnLayout
            {
                id: itemLayout
                anchors.fill: parent
                spacing: 12
                
                
                MLabeledCheckBox
                {
                    Layout.fillWidth: true
                    boxWidth: 18
                    boxHeight: 18
                    spacing: 8
                    imageSize: 10
                    text: "Case sensitive"
                    fontSize: 12
                }
                
                MLabeledCheckBox
                {
                    Layout.fillWidth: true
                    boxWidth: 18
                    boxHeight: 18
                    spacing: 8
                    imageSize: 10
                    text: "Highlight all"
                    fontSize: 12
                }
                
                MLabeledCheckBox
                {
                    Layout.fillWidth: true
                    boxWidth: 18
                    boxHeight: 18
                    spacing: 8
                    imageSize: 10
                    text: "Whole words"
                    fontSize: 12
                }
            }
        }
        
        Image
        {
            id: dropletIcon
            Layout.leftMargin: 13
            Layout.topMargin: -1
            source: Icons.dropupLightGray
            sourceSize.width: 10
            fillMode: Image.PreserveAspectFit
            rotation: 180
        }
    }
}
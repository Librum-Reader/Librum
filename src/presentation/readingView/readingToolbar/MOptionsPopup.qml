import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.icons


Popup
{
    id: root
    implicitWidth: 212
    implicitHeight: 448  // Remove
    padding: 0
    closePolicy: Popup.CloseOnReleaseOutsideParent | Popup.CloseOnEscape
    background: Rectangle
    {
        color: "transparent"
        radius: 4
    }
    
    
    ColumnLayout
    {
        id: mainLayout
        width: parent.width
        height: parent.height  // Remove
        spacing: 0
        
        
        Image
        {
            id: dropletIcon
            Layout.alignment: Qt.AlignRight
            Layout.topMargin: -1
            Layout.rightMargin: 12
            source: Icons.dropupLightGray
            sourceSize.width: 10
            fillMode: Image.PreserveAspectFit
        }
        
        Pane
        {
            id: container
            Layout.fillWidth: true
            Layout.fillHeight: true  // Remove
            verticalPadding: 8
            horizontalPadding: 0
            background: Rectangle
            {
                color: Style.colorBackground
                border.color: Style.colorLightBorder
                border.width: 1
                radius: 4
            }
        }
    }
}
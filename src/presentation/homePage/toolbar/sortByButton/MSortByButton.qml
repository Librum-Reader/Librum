import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import Librum.icons


Item
{
    id: root
    signal sortBySelected
    
    implicitWidth: 104
    implicitHeight: 36
    
    
    ColumnLayout
    {
        anchors.fill: parent
        spacing: 4
        
        
        Pane
        {
            id: container
            Layout.fillWidth: true
            Layout.fillHeight: true
            padding: 0
            background: Rectangle
            {
                color: Style.colorBackground
                border.width: 1
                border.color: Style.colorLightBorder
                radius: 5
            }
            
            
            RowLayout
            {
                anchors.centerIn: parent
                spacing: 8
                
                Label
                {
                    id: sortByLabel
                    color: Style.colorBaseText
                    text: "Sort by"
                    font.pointSize: 12
                    font.family: Style.defaultFontFamily
                    font.weight: Font.Bold
                }
                
                Image
                {
                    id: sortByArrowIcon
                    sourceSize.height: 6
                    source: Icons.dropdownGray
                    fillMode: Image.PreserveAspectFit
                    rotation: 180
                    
                    NumberAnimation
                    {
                        id: closeAnim
                        target: sortByArrowIcon
                        property: "rotation"
                        to: 180
                        duration: 175
                        easing.type: Easing.InOutQuad
                    }
                    
                    NumberAnimation
                    {
                        id: openAnim
                        target: sortByArrowIcon
                        property: "rotation"
                        to: 0
                        duration: 175
                        easing.type: Easing.InOutQuad
                    }
                }
            }
        }
    }
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked: selectionPopup.opened ? selectionPopup.close() : selectionPopup.open()
    }
    
    MSortByPopup
    {
        id: selectionPopup
        y: root.height + 6
        closePolicy: Popup.CloseOnReleaseOutsideParent | Popup.CloseOnEscape
        
        onOpened: openAnim.start()
        onClosed:
        {
            closeAnim.start()
            root.sortBySelected();
        }
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}
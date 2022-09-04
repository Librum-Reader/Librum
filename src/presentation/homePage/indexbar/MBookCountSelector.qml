import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Librum.style 1.0
import Librum.icons 1.0


Item
{
    id: root
    property int selectedAmountOfBooks: 12
    signal selectedBookCount(int amount)
    
    implicitWidth: 58
    implicitHeight: 32
    
    
    Rectangle
    {
        id: container
        anchors.fill: parent
        color: Style.colorBackground
        border.color: Style.colorLightBorder
        radius: 4
        antialiasing: true
        
        
        RowLayout
        {
            id: layout
            anchors.centerIn: parent
            spacing: 5
            
            
            Label
            {
                id: bookCountLabel
                text: root.selectedAmountOfBooks.toString()
                color: Style.colorBaseText
                font.family: Style.defaultFontFamily
                font.pointSize: 11
                font.bold: true
            }
            
            Image
            {
                id: dropDownArrowImage
                source: Icons.dropdownGray
                sourceSize.width: 8
                fillMode: Image.PreserveAspectFit
            }
        }
    }
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked: selectorPopup.opened ? selectorPopup.close() : selectorPopup.open()
    }
    
    MBookCountSelectorPopup
    {
        id: selectorPopup
        y: -implicitHeight - 6
        closePolicy: Popup.CloseOnReleaseOutsideParent | Popup.CloseOnEscape
        
        onOpenedChanged: opened ? closeAnim.start() : openAnim.start()
        
        onSelected:
            (content) =>
            {
                root.selectedAmountOfBooks = content;
                root.selectedBookCount(content);
                close();
            }
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
    
    
    NumberAnimation
    {
        id: closeAnim
        target: dropDownArrowImage
        property: "rotation"
        to: -180
        duration: 175
        easing.type: Easing.InOutQuad
    }
    
    NumberAnimation
    {
        id: openAnim
        target: dropDownArrowImage
        property: "rotation"
        to: 0
        duration: 175
        easing.type: Easing.InOutQuad
    }
}
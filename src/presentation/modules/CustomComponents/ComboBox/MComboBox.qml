import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item
{
    id: root
    property alias listContent: selectionPopup.listContent
    property int headerToBoxSpacing: 2
    property int popupSpacing: 5
    property alias maxPopupHeight: selectionPopup.maxHeight
    
    property string headerText
    property int headerFontWeight: Font.Bold
    property double headerFontSize: 10.5
    property color headerFontColor: properties.colorLightText3
    
    property string titleEmptyText: "Any"
    property int titleFontWeight: Font.Normal
    property int titleFontSize: 11
    property color titleFontColor: properties.colorBaseText
    
    property alias contentFontSize: selectionPopup.fontSize
    property alias contentFontColor: selectionPopup.fontColor
    property alias contentFontWeight: selectionPopup.fontWeight
    property alias contentFontFamily: selectionPopup.fontFamily
    
    property string imagePath
    property int imageSpacing: 4
    property int imageSize: 6
    
    property int radius: 4
    signal clicked
    
    implicitWidth: 100
    implicitHeight: 47
    
    
    ColumnLayout
    {
        id: mainLayout
        anchors.fill: parent
        spacing: root.headerToBoxSpacing
        
        
        Label
        {
            id: header
            Layout.fillWidth: true
            text: root.headerText
            font.family: properties.defaultFontFamily
            font.pointSize: root.headerFontSize
            font.weight: root.headerFontWeight
            color: root.headerFontColor
        }
        
        Pane
        {
            id: container
            Layout.fillWidth: true
            Layout.preferredHeight: root.height - header.height
            verticalPadding: 6
            horizontalPadding: 8
            background: Rectangle
            {
                color: properties.colorBackground
                border.width: 1
                border.color: properties.colorLightBorder
                radius: root.radius
                antialiasing: true
            }
            
            
            RowLayout
            {
                id: inContainerLayout
                anchors.fill: parent
                spacing: root.imageSpacing
                
                
                Label
                {
                    id: title
                    Layout.alignment: Qt.AlignLeft
                    // @disable-check M325
                    text: selectionPopup.selectedContent == null ? root.titleEmptyText : selectionPopup.selectedContent
                    font.pointSize: root.titleFontSize
                    font.family: properties.defaultFontFamily
                    font.weight: root.titleFontWeight
                    color: root.titleFontColor
                }
                
                Image
                {
                    id: icon
                    Layout.alignment: Qt.AlignRight
                    sourceSize.height: root.imageSize
                    source: root.imagePath
                    fillMode: Image.PreserveAspectFit
                    
                    NumberAnimation
                    {
                        id: closeAnim
                        target: icon
                        property: "rotation"
                        to: 0
                        duration: 175
                        easing.type: Easing.InOutQuad
                    }
                    
                    NumberAnimation
                    {
                        id: openAnim
                        target: icon
                        property: "rotation"
                        to: -180
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
    
    MComboBoxPopup
    {
        id: selectionPopup
        y: mainLayout.y + mainLayout.height + root.popupSpacing
        
        width: parent.width
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        
        onOpenedChanged:
        {
            if(opened)
            {
                closeAnim.start();
            }
            else
            {
                giveFocus();
                openAnim.start();
            }
        }
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}
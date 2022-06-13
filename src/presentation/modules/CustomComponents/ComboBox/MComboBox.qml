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
    property string backgroundColor: properties.colorBackground
    property alias highlightColor: selectionPopup.highlightColor
    property alias defaultIndex: selectionPopup.defaultIndex
    
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
            visible: root.headerText.length > 0
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
            Layout.fillHeight: true
            verticalPadding: 6
            horizontalPadding: 8
            background: Rectangle
            {
                color: root.backgroundColor
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
                    Layout.alignment: root.centerTitle ? Qt.AlignHCenter : Qt.AlignLeft
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
                    sourceSize.width: root.imageSize
                    source: root.imagePath
                    rotation: 180
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
        backgroundColor: root.backgroundColor
        
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
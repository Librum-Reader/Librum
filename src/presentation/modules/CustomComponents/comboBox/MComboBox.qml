import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0


Item
{
    id: root
    property alias text: selectionPopup.selectedContent
    property bool multiSelect: false
    property string boxBackgroundColor: Style.colorBackground
    property alias model: selectionPopup.model
    property alias checkBoxStyle: selectionPopup.checkBoxStyle
    property alias checkBoxSize: selectionPopup.checkBoxSize
    property alias checkBoxImageSize: selectionPopup.checkBoxImageSize
    property alias itemHeight: selectionPopup.itemHeight
    property alias maxHeight: selectionPopup.maxHeight
    property alias defaultIndex: selectionPopup.defaultIndex
    property alias fontSize: selectionPopup.fontSize
    property alias fontWeight: selectionPopup.fontWeight
    
    property string headerText
    property int headerFontWeight: Font.Bold
    property double headerFontSize: 10.5
    property color headerFontColor: Style.colorLightText3
    
    property string titleEmptyText: "Any"
    property double titleFontSize: 11
    property color titleFontColor: Style.colorBaseText
    property int titleSpacing: 0
    
    property string image
    property int imageSize: 6
    
    signal clicked
    signal itemChanged
    
    implicitHeight: 47
    
    
    ColumnLayout
    {
        id: mainLayout
        anchors.fill: parent
        spacing: 2
        
        
        Label
        {
            id: header
            Layout.fillWidth: true
            visible: root.headerText.length > 0
            text: root.headerText
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
            horizontalPadding: 10
            background: Rectangle
            {
                color: root.boxBackgroundColor
                border.width: 1
                border.color: Style.colorLightBorder
                radius: 4
            }
            
            
            RowLayout
            {
                id: inContainerLayout
                width: parent.width
                anchors.centerIn: parent
                spacing: 4
                
                
                Label
                {
                    id: title
                    Layout.fillWidth: true
                    Layout.alignment: root.centerTitle ? Qt.AlignHCenter : Qt.AlignLeft
                    leftPadding: root.titleSpacing
                    rightPadding: root.titleSpacing
                    text: selectionPopup.selectedContent === "" ? root.titleEmptyText : selectionPopup.selectedContent
                    font.pointSize: root.titleFontSize
                    font.weight: Font.Normal
                    color: selectionPopup.selectedContent === "" ? Style.colorLightText3 : root.titleFontColor
                    elide: Text.ElideRight
                }
                
                Image
                {
                    id: icon
                    Layout.alignment: Qt.AlignRight
                    sourceSize.width: root.imageSize
                    source: root.image
                    rotation: -180
                    fillMode: Image.PreserveAspectFit
                    
                    NumberAnimation
                    {
                        id: closeAnim
                        target: icon
                        property: "rotation"
                        to: -180
                        duration: 175
                        easing.type: Easing.InOutQuad
                    }
                    
                    NumberAnimation
                    {
                        id: openAnim
                        target: icon
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
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        
        onClicked: selectionPopup.opened ? selectionPopup.close() : selectionPopup.open()
    }
    
    MComboBoxPopup
    {
        id: selectionPopup
        property int popupSpacing: 5
        property bool fitsToBottom: false
        
        model: root.model
        y: mainLayout.y + (fitsToBottom ? 
                               mainLayout.height + popupSpacing : - popupSpacing - implicitHeight)
        width: parent.width
        multiSelect: root.multiSelect
        
        onClosed: closeAnim.start()
        onItemChanged: root.itemChanged()
        
        onOpened: 
        {
            fitsToBottom = checkIfPopupFitsBelow();
            openAnim.start();
        }
        
        
        function checkIfPopupFitsBelow()
        {
            let globalMousePos = mapToGlobal(mouseArea.mouseX, mouseArea.mouseY);
            if((globalMousePos.y + selectionPopup.height + popupSpacing + mouseArea.mouseY) >= baseRoot.height)
                return false;
            
            return true;
        }
    }
    
    
    function selectItem(index)
    {
        selectionPopup.selectItem(index);
    }
    
    function deselectCurrenItem()
    {
        selectionPopup.deselectCurrenItem();
    }
    
    function closePopup()
    {
        selectionPopup.close();
    }
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}
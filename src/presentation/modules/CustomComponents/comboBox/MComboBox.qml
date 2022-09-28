import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0

Item
{
    id: root
    property alias selectionPopup: selectionPopup
    property bool multiSelect: false
    property int textPadding: 10
    property int headerToBoxSpacing: 2
    property int popupSpacing: 5
    property string boxBackgroundColor: Style.colorBackground
    property string popupBackgroundColor: Style.colorBackground
    property int radius: 4
    
    property string headerText
    property int headerFontWeight: Font.Bold
    property double headerFontSize: 10.5
    property color headerFontColor: Style.colorLightText3
    
    property string titleEmptyText: "Any"
    property int titleFontWeight: Font.Normal
    property double titleFontSize: 11
    property color titleFontColor: Style.colorBaseText
    property int titleSpacing: 0
    
    property string imagePath
    property int imageSpacing: 4
    property int imageSize: 6
    
    signal clicked
    signal itemChanged
    
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
            horizontalPadding: textPadding
            background: Rectangle
            {
                color: root.boxBackgroundColor
                border.width: 1
                border.color: Style.colorLightBorder
                radius: root.radius
            }
            
            
            RowLayout
            {
                id: inContainerLayout
                width: parent.width
                anchors.centerIn: parent
                spacing: root.imageSpacing
                
                
                Label
                {
                    id: title
                    Layout.fillWidth: true
                    Layout.alignment: root.centerTitle ? Qt.AlignHCenter : Qt.AlignLeft
                    leftPadding: root.titleSpacing
                    rightPadding: root.titleSpacing
                    text: selectionPopup.selectedContents === "" ? root.titleEmptyText : selectionPopup.selectedContents
                    font.pointSize: root.titleFontSize
                    font.weight: root.titleFontWeight
                    color: selectionPopup.selectedContents === "" ? Style.colorLightText3 : root.titleFontColor
                    elide: Text.ElideRight
                }
                
                Image
                {
                    id: icon
                    Layout.alignment: Qt.AlignRight
                    sourceSize.width: root.imageSize
                    source: root.imagePath
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
        property bool fitsToBottom: false
        
        y: mainLayout.y + (fitsToBottom ? 
                               mainLayout.height + root.popupSpacing : - root.popupSpacing - implicitHeight)
        backgroundColor: root.popupBackgroundColor
        width: parent.width
        multiSelect: root.multiSelect
        
        onOpened: 
        {
            fitsToBottom = popupFitsToTheBottom();
            openAnim.start();
        }

        onClosed: closeAnim.start()
        onItemChanged: root.itemChanged()
        
        
        function popupFitsToTheBottom()
        {
            let globalMousePos = mapToGlobal(mouseArea.mouseX, mouseArea.mouseY);
            if((globalMousePos.y + selectionPopup.height + root.popupSpacing + mouseArea.mouseY) >= baseRoot.height)
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
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}
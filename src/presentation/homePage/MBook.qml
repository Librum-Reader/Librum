import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0
import Librum.icons 1.0


Item 
{
    id: root
    signal leftButtonClicked(int index)
    signal rightButtonClicked(int index, var mouse)
    signal moreOptionClicked(int index, var mouse)
    
    
    implicitWidth: 190
    implicitHeight: 322
    
    
    ColumnLayout
    {
        id: layout
        anchors.fill: parent
        spacing: 0
        
        
        Item
        {
            id: upperBookPartRounding
            Layout.preferredHeight: 10
            Layout.fillWidth: true
            clip: true
            
            Rectangle
            {
                id: upperFiller
                height: parent.height + 4
                width: parent.width
                radius: 4
                color: Style.colorLightBorder
            }
        }
        
        Item
        {
            id: upperBookPartRoundingOverlay
            Layout.topMargin: -10
            Layout.preferredHeight: 10
            Layout.fillWidth: true
            visible: !model.downloaded
            clip: true
            z: 2
            
            Rectangle
            {
                height: upperFiller.height
                width: upperFiller.width
                color: "#32324D"
                opacity: 0.5
                radius: 4
            }
        }
        
        
        Rectangle
        {
            id: upperRect
            Layout.fillWidth: true
            Layout.preferredHeight: 230
            color: Style.colorLightBorder
            
            Rectangle
            {
                id: bookCoverOverlay
                anchors.fill: parent
                visible: !model.downloaded
                color: "#32324D"
                opacity: 0.5
                z: 2
            }
            
            Image
            {
                anchors.centerIn: bookCoverOverlay
                visible: !model.downloaded
                sourceSize.width: 52
                fillMode: Image.PreserveAspectFit
                source: Icons.downloadWhite
                opacity: 1
                z: 3
            }
            
            ColumnLayout
            {
                id: topLayout
                anchors.centerIn: parent
                spacing: 0
                
                Image
                {
                    id: bookCover
                    visible: source != ""
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: -10
                    source: cover
                }
                
                Label
                {
                    id: noImageLabel
                    Layout.alignment: Qt.AlignCenter
                    visible: bookCover.source == ""
                    text: "." + model.format
                    color: Style.colorDarkGray
                    font.pointSize: 20
                    font.bold: true
                }
            }
        }
        
        
        Rectangle
        {
            id: lowerRect
            Layout.fillWidth: true
            Layout.preferredHeight: 90
            color: Style.colorBackground
            border.width: 1
            border.color: Style.colorLightBorder3
            
            ColumnLayout
            {
                id: bottomLayout
                property int inBookMargin : 14
                
                width: parent.width - inBookMargin*2
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 0
                
                Label
                {
                    id: bookName
                    Layout.fillWidth: true
                    Layout.preferredHeight: 34
                    Layout.topMargin: 5
                    clip: true
                    text: model.title === "" ? "Unknown" : model.title
                    font.weight: Font.Medium
                    verticalAlignment: Text.AlignVCenter
                    color: Style.colorBaseTitle
                    font.pointSize: 11
                    lineHeight: 0.8
                    wrapMode: TextInput.WrapAtWordBoundaryOrAnywhere
                    elide: Text.ElideRight
                }
                
                Label
                {
                    id: authorsName
                    Layout.fillWidth: true
                    Layout.topMargin: 4
                    clip: true
                    text: model.authors === "" ? "Unknown" : model.authors
                    color: Style.colorLightText
                    font.pointSize: 10
                    elide: Text.ElideRight
                }
                
                RowLayout
                {
                    id: lowerRow
                    Layout.fillWidth: true
                    spacing: 0
                    
                    Rectangle
                    {
                        id: percentageBox
                        Layout.preferredWidth: 46
                        Layout.preferredHeight: 18
                        Layout.topMargin: 4
                        color: Style.colorLightPurple2
                        radius: 2
                        
                        Label
                        {
                            id: percentageLabel
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignBottom
                            text: model.percentageRead + "%"
                            font.weight: Font.DemiBold
                            color: Style.colorBaseTitle
                            font.pointSize: 10
                        }
                    }
                    
                    Item { Layout.fillWidth: true }
                    
                    Image
                    {
                        id: moreIcon
                        Layout.preferredHeight: 20
                        Layout.rightMargin: -2
                        source: Icons.dots
                        fillMode: Image.PreserveAspectFit
                        antialiasing: false
                        
                        MouseArea
                        {
                            id: moreMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                        }
                    }
                }
            }
        }
    }
    
    MouseArea
    {
        anchors.fill: parent
        acceptedButtons: Qt.AllButtons
        
        onClicked:
            (mouse) =>
            {
                if(mouse.button === Qt.LeftButton)
                {
                    if(moreMouseArea.containsMouse)
                    {
                        root.moreOptionClicked(root.index, mouse);
                        return;
                    }
                    
                    root.leftButtonClicked(root.index);
                }
                
                else if(mouse.button === Qt.RightButton) root.rightButtonClicked(root.index, mouse);
            }
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}
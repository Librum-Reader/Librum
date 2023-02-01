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
        
        /**
          A Item with rounded corners which is overlapping with the top half of
          the book to create a rounded top, while the rest of the book is rectangluar
          */
        Item
        {
            id: upperBookRounding
            Layout.preferredHeight: 10
            Layout.fillWidth: true
            clip: true
            
            Rectangle
            {
                id: upperRoundingFiller
                height: parent.height + 4
                width: parent.width
                radius: 4
                color: Style.colorBookImageBackground
            }
        }
        
        /**
          An overlay over the upper-book-rounding to get it to be transparent and modal,
          when the book is not currently downloaded. It leads to visual bugs to apply
          the properties directly to the upper-book-rounding item. Moving a separate object
          over it is working fine without any visual bugs
          */
        Item
        {
            id: upperBookRoundingDimmer
            Layout.topMargin: -upperBookRounding.height
            Layout.preferredHeight: 10
            Layout.fillWidth: true
            visible: !model.downloaded
            clip: true
            z: 2
            
            Rectangle
            {
                id: dimmerRect
                height: upperRoundingFiller.height
                width: upperRoundingFiller.width
                color: "#32324D"
                opacity: 0.5
                radius: 4
            }
        }
        
        
        Rectangle
        {
            id: upperBookPart
            Layout.fillWidth: true
            Layout.preferredHeight: 230
            color: Style.colorBookImageBackground
            
            Rectangle
            {
                id: bookCoverDimmer
                anchors.fill: parent
                visible: !model.downloaded
                color: "#32324D"
                opacity: 0.5
                z: 2
            }
            
            Image
            {
                id: downloadBookIcon
                anchors.centerIn: bookCoverDimmer
                visible: !model.downloaded
                sourceSize.width: 52
                fillMode: Image.PreserveAspectFit
                source: Icons.downloadWhite
                opacity: 1
                z: 3
            }
            
            ColumnLayout
            {
                id: upperPartLayout
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
                
                /*
                  The item displaying when no book cover exists (usually a ".format" label)
                 */
                Label
                {
                    id: noImageLabel
                    Layout.alignment: Qt.AlignCenter
                    visible: bookCover.source == ""
                    text: "." + model.format
                    color: Style.colorNoImageLabel
                    font.pointSize: 20
                    font.bold: true
                }
            }
        }
        
        
        Rectangle
        {
            id: lowerBookPart
            Layout.fillWidth: true
            Layout.preferredHeight: 90
            color: Style.colorBackground
            border.width: 1
            border.color: Style.colorBookBorder
            
            
            ColumnLayout
            {
                id: bottomPartLayout
                width: parent.width - internal.lowerBookPartPadding * 2
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 0
                
                Label
                {
                    id: title
                    Layout.fillWidth: true
                    Layout.preferredHeight: 34
                    Layout.topMargin: 5
                    clip: true
                    text: model.title === "" ? "Unknown" : model.title
                    font.weight: Font.Medium
                    verticalAlignment: Text.AlignVCenter
                    color: Style.colorTitle
                    font.pointSize: 11
                    lineHeight: 0.8
                    wrapMode: TextInput.WrapAtWordBoundaryOrAnywhere
                    elide: Text.ElideRight
                }
                
                Label
                {
                    id: authors
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
                    id: statusRow
                    Layout.fillWidth: true
                    spacing: 0
                    
                    Rectangle
                    {
                        id: percentageBox
                        Layout.preferredWidth: 46
                        Layout.preferredHeight: 18
                        Layout.topMargin: 4
                        color: Style.colorHighlight
                        radius: 2
                        
                        Label
                        {
                            id: percentageLabel
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignBottom
                            text: model.bookProgressPercentage + "%"
                            font.weight: Font.DemiBold
                            color: Style.colorTitle
                            font.pointSize: 10
                        }
                    }
                    
                    Item { Layout.fillWidth: true }
                    
                    Image
                    {
                        id: moreOptionsIcon
                        Layout.preferredHeight: 20
                        Layout.rightMargin: -2
                        source: Icons.dots
                        fillMode: Image.PreserveAspectFit
                        antialiasing: false
                        
                        MouseArea
                        {
                            id: moreOptionsArea
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
        
        // Delegate mouse clicks events to parent
        onClicked:
            (mouse) =>
            {
                if(mouse.button === Qt.LeftButton)
                {
                    if(moreOptionsArea.containsMouse)
                    {
                        root.moreOptionClicked(root.index, mouse);
                        return;
                    }
                    
                    root.leftButtonClicked(root.index);
                }
                else if(mouse.button === Qt.RightButton)
                {
                    root.rightButtonClicked(root.index, mouse);
                }
            }
    }
    
    QtObject
    {
        id: internal
        property int lowerBookPartPadding: 14
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}
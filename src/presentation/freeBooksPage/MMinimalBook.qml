import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0
import Librum.icons 1.0


Item 
{
    id: root
    implicitWidth: 190
    implicitHeight: 304
    
    ColumnLayout
    {
        id: layout
        anchors.fill: parent
        spacing: 0
        
        /**
          A rectangle with rounded corners which is overlapping with the top half of
          the book to create a rounded top, while the rest of the book is rectangluar
          */
        Rectangle
        {
            id: upperBookPartRounding
            Layout.fillWidth: true
            Layout.preferredHeight: 16
            radius: 4
            color: Style.colorLightBorder
        }
        
        /**
          The upper book half which contains the book cover
          */
        Rectangle
        {
            id: bookCoverRect
            Layout.fillWidth: true
            Layout.preferredHeight: 230
            Layout.topMargin: -6
            color: Style.colorLightBorder
            
            ColumnLayout
            {
                anchors.centerIn: parent
                spacing: 0
                
                Image
                {
                    id: bookCover
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: -10
                    sourceSize.height: 241
                    source: Icons.bookCover
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
        
        /**
          The lower book half which contains the book information
          */
        Rectangle
        {
            id: informationRect
            Layout.fillWidth: true
            Layout.preferredHeight: 65
            color: Style.colorBackground
            border.width: 1
            border.color: Style.colorLightBorder3
            
            ColumnLayout
            {
                id: informationRectLayout
                width: parent.width - internal.paddingInsideBook * 2
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 0
                
                Label
                {
                    id: bookName
                    Layout.fillWidth: true
                    Layout.topMargin: 4
                    text: "The books title which can be 2 lines long ..."
                    font.weight: Font.Medium
                    color: Style.colorTitle
                    font.pointSize: 11
                    lineHeight: 0.8
                    wrapMode: TextInput.WordWrap
                }
                
                Label
                {
                    id: authors
                    Layout.fillWidth: true
                    Layout.topMargin: 5                    
                    text: "Stephen R. Covey"
                    color: Style.colorLightText
                    font.pointSize: 10
                }
            }
        }
    }
    
    QtObject
    {
        id: internal
        property int paddingInsideBook : 14
    }
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }    
}
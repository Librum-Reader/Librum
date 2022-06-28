import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import Librum.icons


Item 
{
    id: root
    implicitWidth: 190
    implicitHeight: 300
    
    ColumnLayout
    {
        id: layout
        width: parent.width
        height: parent.height
        spacing: 0
        
        Rectangle
        {
            id: upperBookPartRounding
            Layout.fillWidth: true
            Layout.preferredHeight: 16
            radius: 4
            color: Style.colorLightBorder
        }
        
        Rectangle
        {
            id: upperRect
            Layout.fillWidth: true
            Layout.preferredHeight: 230
            Layout.topMargin: -6
            color: Style.colorLightBorder
            
            ColumnLayout
            {
                id: topLayout
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
        
        
        Rectangle
        {
            id: lowerRect
            Layout.fillWidth: true
            Layout.preferredHeight: 61
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
                    Layout.topMargin: 4
                    text: "The books title which can be 2 lines long ..."
                    font.weight: Font.Medium
                    color: Style.colorBaseTitle
                    font.pixelSize: 14
                    font.family: Style.defaultFontFamily
                    lineHeight: 0.8
                    wrapMode: TextInput.WordWrap
                }
                
                Label
                {
                    id: authorName
                    Layout.fillWidth: true
                    Layout.topMargin: 1                    
                    text: "Stephen R. Covey"
                    color: Style.colorLightText
                    font.pointSize: 9
                    font.family: Style.defaultFontFamily
                }
            }
        }
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }    
}
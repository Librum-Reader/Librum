import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope 
{
    id: root
    implicitWidth: 190
    implicitHeight: 322
    
    ColumnLayout
    {
        id: layout
        width: parent.width
        height: parent.height
        spacing: 0
        
        Rectangle
        {
            id: upperBookPartRounding
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 16
            radius: 4
            color: properties.colorLightBorder
        }
        
        Rectangle
        {
            id: upperRect
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 230
            Layout.topMargin: -6
            color: properties.colorLightBorder
            
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
                    source: properties.imageBookCover
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
        
        
        Rectangle
        {
            id: lowerRect
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 82
            color: properties.colorBackground
            border.width: 1
            border.color: properties.colorLightBorder3
            
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
                    Layout.preferredWidth: parent.width
                    Layout.topMargin: 4
                    text: "The books title which can be 2 lines long ..."
                    font.weight: Font.Medium
                    color: properties.colorBaseTitle
                    font.pixelSize: 14
                    font.family: properties.defaultFontFamily
                    lineHeight: 0.8
                    wrapMode: TextInput.WordWrap
                }
                
                Label
                {
                    id: authorName
                    Layout.preferredWidth: parent.width
                    Layout.topMargin: 1                    
                    text: "Stephen R. Covey"
                    color: properties.colorLightText
                    font.pointSize: 9
                    font.family: properties.defaultFontFamily
                }
                
                RowLayout
                {
                    id: lowerRow
                    width: parent.width
                    spacing: 0
                    
                    Rectangle
                    {
                        id: percentageBox
                        Layout.preferredWidth: 42
                        Layout.preferredHeight: 16
                        Layout.topMargin: 6
                        color: properties.darkBackground
                        radius: 2
                        
                        Label
                        {
                            id: percentageLabel
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignBottom
                            text: "27%"
                            font.weight: Font.Bold
                            color: properties.colorBaseTitle
                            font.pointSize: 10
                            font.family: properties.defaultFontFamily
                        }
                    }

                    Image
                    {
                        id: moreIcon
                        Layout.preferredHeight: 20
                        Layout.leftMargin: 98
                        source: properties.iconDots
                        fillMode: Image.PreserveAspectFit
                        antialiasing: false
                    }
                }
            }
        }
    }
}
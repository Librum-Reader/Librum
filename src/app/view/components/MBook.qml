import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Item {
    width: 190
    height: 322
    
    ColumnLayout
    {
        id: mainLayout
        width: parent.width
        height: parent.height
        spacing: 0
        
        Rectangle
        {
            id: upperRectRounder
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 16
            radius: 4
            color: "#DCDCE4"
        }
        
        Rectangle
        {
            id: upperRect
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 230
            Layout.topMargin: -6
            color: "#DCDCE4"
            
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
                    source: "/resources/images/book-cover.png"
                    fillMode: Image.PreserveAspectFit
                }
            }
        }
        
        
        Rectangle
        {
            id: lowerRect
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 82
            color: "#FFFFFF"
            border.width: 1
            border.color: properties.colorLightBorder3
            
            ColumnLayout
            {
                id: bottomLayout
                width: parent.width - 14*2
                spacing: 0
                
                property int inBookMargin : 14
                
                Label
                {
                    id: bookName
                    Layout.preferredWidth: parent.width
                    Layout.topMargin: 4
                    Layout.leftMargin: bottomLayout.inBookMargin
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
                    Layout.leftMargin: bottomLayout.inBookMargin
                    text: "Stephen R. Covey"
                    color: properties.colorLightText
                    font.pointSize: 9
                    font.family: properties.defaultFontFamily
                }
                
                RowLayout
                {
                    id: lowerRow
                    width: parent.width
                    Layout.leftMargin: bottomLayout.inBookMargin
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
                        Layout.leftMargin: 101
                        source: "/resources/images/dots-horizontal.svg"
                        fillMode: Image.PreserveAspectFit
                        antialiasing: false
                    }
                }
            }
        }
    }
}
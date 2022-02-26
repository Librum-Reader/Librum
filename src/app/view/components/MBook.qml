import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Item {
    width: 190
    height: 324
    
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
        }
        
        Rectangle
        {
            id: spacer
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 1
            color: "#E0E0DE"
        }
        
        Rectangle
        {
            id: lowerRect
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 83
            color: "#FFFFFF"
            
            ColumnLayout
            {
                id: inBookLayout
                width: parent.width - 14*2
                spacing: 0
                
                property int inBookMargin : 14
                
                Label
                {
                    id: bookName
                    Layout.preferredWidth: parent.width
                    Layout.topMargin: 4
                    Layout.leftMargin: inBookLayout.inBookMargin
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
                    Layout.leftMargin: inBookLayout.inBookMargin
                    text: "Stephan R. Covey"
                    font.weight: Font.Medium
                    color: properties.colorLightText
                    font.pointSize: 9
                    font.family: properties.defaultFontFamily
                    wrapMode: TextInput.NoWrap
                }
                
                RowLayout
                {
                    id: lowerRow
                    width: parent.width
                    Layout.leftMargin: inBookLayout.inBookMargin
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
                            text: "27%"
                            font.weight: Font.Bold
                            color: properties.colorBaseTitle
                            font.pixelSize: 12
                            font.family: properties.colorBaseTitle
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
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents

Popup
{
    id: root
    property int contentPadding: 16
    
    implicitWidth: 829
    implicitHeight: 566
    padding: 0
    
    modal: true
    Overlay.modal: Rectangle
    {
        color: "#aa32324D"
        opacity: 1
    }
    
    background: Rectangle
    {
        radius: 4
        color: properties.colorLightGray
    }
    
    
    ColumnLayout
    {
        id: mainLayout
        anchors.fill: parent
        spacing: 0
        
        
        Pane
        {
            id: header
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 64
            Layout.minimumHeight: 36
            horizontalPadding: root.contentPadding
            
            background: Rectangle
            {
                color: "transparent"
            }
            
            
            RowLayout
            {
                id: headerLayout
                anchors.fill: parent
                spacing: 0
                
                
                Label
                {
                    id: title
                    
                    Layout.alignment: Qt.AlignLeft
                    text: "Get a new book"
                    verticalAlignment: Text.AlignBottom
                    font.family: properties.defaultFontFamily
                    font.weight: Font.Bold
                    
                    
                    font.pointSize: 14
                    color: properties.colorBaseTitle
                }
                
                MCloseButton
                {
                    Layout.alignment: Qt.AlignRight
                }
            }
        }
        
        Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: properties.colorSeparator }
        
        Pane
        {
            id: content
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 434
            Layout.minimumHeight: 360
            Layout.leftMargin: 1
            verticalPadding: 32
            horizontalPadding: 25
            
            background: Rectangle
            {
                color: properties.colorBackground
            }
            
            
            ColumnLayout
            {
                id: contentLayout
                width: parent.width
                spacing: 0
                
                
                RowLayout
                {
                    id: informationLayout
                    Layout.fillWidth: true
                    spacing: 22
                    
                    
                    Rectangle
                    {
                        id: imageRect
                        Layout.preferredWidth: 201
                        Layout.preferredHeight: 273
                        color: properties.colorLightBorder
                        radius: 4
                        
                        Image
                        {
                            id: bookCover
                            anchors.centerIn: parent
                            sourceSize.height: imageRect.height - 2
                            source: properties.imageBookCover
                            fillMode: Image.PreserveAspectFit
                        }
                    }
                    
                    ColumnLayout
                    {
                        id: factLayout
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignTop
                        spacing: 2
                        
                        
                        Label
                        {
                            text: "TITLE"
                            color: properties.colorBaseTitle
                            font.family: properties.defaultFontFamily
                            font.pointSize: 12
                            font.weight: Font.Bold
                        }
                        
                        Label
                        {
                            text: "The 7 habits of highly effective people"
                            color: properties.colorLightText3
                            font.family: properties.defaultFontFamily
                            font.pointSize: 12.25
                            font.weight: Font.Medium
                        }
                        
                        
                        Label
                        {
                            Layout.topMargin: 12
                            text: "AUTHOR"
                            color: properties.colorBaseTitle
                            font.family: properties.defaultFontFamily
                            font.pointSize: 12
                            font.weight: Font.Bold
                        }
                        
                        Label
                        {
                            text: "Stephen R. Covey"
                            color: properties.colorLightText3
                            font.family: properties.defaultFontFamily
                            font.pointSize: 12.25
                            font.weight: Font.Medium
                        }
                        
                        
                        Label
                        {
                            Layout.topMargin: 12
                            text: "PUBLICATION"
                            color: properties.colorBaseTitle
                            font.family: properties.defaultFontFamily
                            font.pointSize: 12
                            font.weight: Font.Bold
                        }
                        
                        Label
                        {
                            text: "United States: Dodd, Mead and Company, 1922."
                            color: properties.colorLightText3
                            font.family: properties.defaultFontFamily
                            font.pointSize: 12.25
                            font.weight: Font.Medium
                        }
                        
                        
                        Label
                        {
                            Layout.topMargin: 12
                            text: "LANGUAGE"
                            color: properties.colorBaseTitle
                            font.family: properties.defaultFontFamily
                            font.pointSize: 12
                            font.weight: Font.Bold
                        }
                        
                        Label
                        {
                            text: "English"
                            color: properties.colorLightText3
                            font.family: properties.defaultFontFamily
                            font.pointSize: 12.25
                            font.weight: Font.Medium
                        }
                    }
                }
                
                Label
                {
                    Layout.topMargin: 26
                    text: "CONTENTS:"
                    color: properties.colorBaseTitle
                    font.family: properties.defaultFontFamily
                    font.pointSize: 12
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.preferredWidth: parent.width
                    text: "Your habits determine your character and later define your life." +
                           "Don’t blame outside factors when you fail in life. Also, don’t think" +
                           "that succeeding in one area of your life will mean that you’re destined for triumph."
                    color: properties.colorLightText3
                    font.family: properties.defaultFontFamily
                    font.pointSize: 12.25
                    font.weight: Font.Medium
                    wrapMode: Text.WordWrap
                }
            }
        }
        
        Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: properties.colorSeparator }
        
        Pane
        {
            id: footer
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.maximumHeight: 64
            Layout.minimumHeight: 36
            horizontalPadding: root.contentPadding
            
            background: Rectangle
            {
                color: "transparent"
            }
            
            
            RowLayout
            {
                id: footerLayout
                anchors.fill: parent
                spacing: 0
                
                
                MButton
                {
                    id: cancelButton
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignLeft
                    borderColor: properties.colorLightBorder
                    backgroundColor: properties.colorBackground
                    text: "Cancel"
                    fontColor: properties.colorBaseText
                    fontBold: true
                    fontSize: 12
                }
                
                MButton
                {
                    id: downloadButton
                    Layout.preferredWidth: 137
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignRight
                    borderColor: properties.colorLightBorder
                    backgroundColor: properties.colorBasePurple
                    text: "Download"
                    fontColor: properties.colorBrightText
                    fontBold: true
                    fontSize: 12
                    imagePath: properties.iconDownloadWhite
                    imageSize: 18
                }
            }
        }
    }
}
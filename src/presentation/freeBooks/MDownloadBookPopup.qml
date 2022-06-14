import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons


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
        color: Style.colorLightGray
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
                    font.family: Style.defaultFontFamily
                    font.weight: Font.Bold
                    
                    
                    font.pointSize: 14
                    color: Style.colorBaseTitle
                }

                MButton
                {
                    Layout.preferredWidth: 32
                    Layout.preferredHeight: 32
                    Layout.alignment: Qt.AlignRight
                    borderColor: Style.colorLightBorder
                    imagePath: Icons.closeBlack
                    imageSize: 12
                    opacityOnPressed: 0.7
                    
                    onClicked: root.close()
                }
            }
        }
        
        Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: Style.colorSeparator }
        
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
                color: Style.colorBackground
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
                        color: Style.colorLightBorder
                        radius: 4
                        
                        Image
                        {
                            id: bookCover
                            anchors.centerIn: parent
                            sourceSize.height: imageRect.height - 2
                            source: Icons.bookCover
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
                            color: Style.colorBaseTitle
                            font.family: Style.defaultFontFamily
                            font.pointSize: 12
                            font.weight: Font.Bold
                        }
                        
                        Label
                        {
                            text: "The 7 habits of highly effective people"
                            color: Style.colorLightText3
                            font.family: Style.defaultFontFamily
                            font.pointSize: 12.25
                            font.weight: Font.Medium
                        }
                        
                        
                        Label
                        {
                            Layout.topMargin: 12
                            text: "AUTHOR"
                            color: Style.colorBaseTitle
                            font.family: Style.defaultFontFamily
                            font.pointSize: 12
                            font.weight: Font.Bold
                        }
                        
                        Label
                        {
                            text: "Stephen R. Covey"
                            color: Style.colorLightText3
                            font.family: Style.defaultFontFamily
                            font.pointSize: 12.25
                            font.weight: Font.Medium
                        }
                        
                        
                        Label
                        {
                            Layout.topMargin: 12
                            text: "PUBLICATION"
                            color: Style.colorBaseTitle
                            font.family: Style.defaultFontFamily
                            font.pointSize: 12
                            font.weight: Font.Bold
                        }
                        
                        Label
                        {
                            text: "United States: Dodd, Mead and Company, 1922."
                            color: Style.colorLightText3
                            font.family: Style.defaultFontFamily
                            font.pointSize: 12.25
                            font.weight: Font.Medium
                        }
                        
                        
                        Label
                        {
                            Layout.topMargin: 12
                            text: "LANGUAGE"
                            color: Style.colorBaseTitle
                            font.family: Style.defaultFontFamily
                            font.pointSize: 12
                            font.weight: Font.Bold
                        }
                        
                        Label
                        {
                            text: "English"
                            color: Style.colorLightText3
                            font.family: Style.defaultFontFamily
                            font.pointSize: 12.25
                            font.weight: Font.Medium
                        }
                    }
                }
                
                Label
                {
                    Layout.topMargin: 26
                    text: "CONTENTS:"
                    color: Style.colorBaseTitle
                    font.family: Style.defaultFontFamily
                    font.pointSize: 12
                    font.weight: Font.Bold
                }
                
                Label
                {
                    Layout.preferredWidth: parent.width
                    Layout.topMargin: 2
                    text: "Your habits determine your character and later define your life." +
                           "Don’t blame outside factors when you fail in life. Also, don’t think" +
                           "that succeeding in one area of your life will mean that you’re destined for triumph."
                    color: Style.colorLightText3
                    font.family: Style.defaultFontFamily
                    font.pointSize: 12.25
                    font.weight: Font.Medium
                    wrapMode: Text.WordWrap
                }
            }
        }
        
        Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: Style.colorSeparator }
        
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
                    borderColor: Style.colorLightBorder
                    backgroundColor: Style.colorBackground
                    opacityOnPressed: 0.7
                    text: "Cancel"
                    fontColor: Style.colorBaseText
                    fontWeight: Font.Bold
                    fontSize: 12
                    
                    onClicked: root.close()
                }
                
                MButton
                {
                    id: downloadButton
                    Layout.preferredWidth: 137
                    Layout.preferredHeight: 38
                    Layout.alignment: Qt.AlignRight
                    borderWidth: 0
                    backgroundColor: Style.colorBasePurple
                    text: "Download"
                    fontColor: Style.colorBrightText
                    fontWeight: Font.Bold
                    fontSize: 12
                    imagePath: Icons.downloadWhite
                    imageSize: 18
                    
                    onClicked: 
                    {
                        // Download book
                        root.close()
                    }
                }
            }
        }
    }
}
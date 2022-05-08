import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents


Page
{
    id: root
    topPadding: 64
    horizontalPadding: 48
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.pagesBackground
    }
    
    
    ColumnLayout
    {
        id: layout
        width: parent.width
        spacing: 0
        
        
        MTitle
        {
            id: title
            titleText: "Support us"
            descriptionText: "Thanks for considering"
            titleSize: 25
            descriptionSize: 13.25
        }
        
        Pane
        {
            id: container
            Layout.fillWidth: true
            Layout.topMargin: 32
            topPadding: 24
            horizontalPadding: 40
            bottomPadding: 38
            background: Rectangle
            {
                color: properties.colorBackground
                border.color: properties.colorLightBorder
                radius: 4
                antialiasing: true
            }
            
            
            ColumnLayout
            {
                id: inDetailsLayout
                width: parent.width
                spacing: 0
                
                Label
                {
                    id: detailsTitle
                    text: "Us"
                    color: properties.colorBaseText
                    font.pointSize: 20
                    font.family: properties.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                Label
                {
                    Layout.fillWidth: true
                    Layout.topMargin: 15
                    text: "We are a small team of freelancing developers creating free apps for the community, we love"
                    wrapMode: Text.WordWrap
                    color: properties.colorLightText3
                    font.pointSize: 14
                    font.family: properties.defaultFontFamily
                }
                
                RowLayout
                {
                    id: teamTextColumn
                    Layout.fillWidth: true
                    spacing: 0
                    
                    
                    Label
                    {
                        Layout.fillWidth: true
                        text: "working on fun projects, supporting our community and trying to make the world a better place."
                        wrapMode: Text.WordWrap
                        color: properties.colorLightText3
                        font.pointSize: 14
                        font.family: properties.defaultFontFamily
                    }
                }
                
                Label
                {
                    id: workText
                    Layout.fillWidth: true
                    Layout.topMargin: 18
                    text: "We are working in our own name, so we are not getting payed in any way."
                    wrapMode: Text.WordWrap
                    color: properties.colorLightText3
                    font.pointSize: 14
                    font.family: properties.defaultFontFamily
                }
                
                RowLayout
                {
                    id: supportText
                    Layout.fillWidth: true
                    spacing: 0
                    
                    
                    Label
                    {
                        text: "We are thankful for any support."
                        wrapMode: Text.WordWrap
                        color: properties.colorLightText3
                        font.pointSize: 14
                        font.family: properties.defaultFontFamily
                    }
                    
                    Image
                    {
                        id: seedImage
                        Layout.alignment: Qt.AlignBottom
                        Layout.leftMargin: 2
                        Layout.bottomMargin: 3
                        source: properties.iconSeed
                        sourceSize.width: 21
                        fillMode: Image.PreserveAspectFit
                    }
                    
                    Item { Layout.fillWidth: true }
                }
                
                Label
                {
                    id: coffeeText
                    Layout.fillWidth: true
                    Layout.topMargin: 36
                    text: "If you feel like supporting us and our new projects, feel free to buy us a coffee"
                    wrapMode: Text.WordWrap
                    color: properties.colorLightText3
                    font.pointSize: 14
                    font.family: properties.defaultFontFamily
                }
                
                MButton
                {
                    id: coffeeButton
                    Layout.preferredWidth: 168
                    Layout.preferredHeight: 38
                    Layout.topMargin: 10
                    backgroundColor: properties.colorCoffeeBrown
                    borderColor: "transparent"
                    imagePath: properties.iconCoffee
                    imageSize: 20
                    imageSpacing: 10
                    textContent: "Buy us a coffee"
                    fontSize: 11.9
                    fontColor: properties.colorBaseTitle
                    buttonRadius: 4
                }
                
                Label
                {
                    id: otherPaymentsText
                    Layout.fillWidth: true
                    Layout.topMargin: 40
                    text: "We also offer lots of different donation methods, from paypal, to crypto currencies. If you prefer any other method,<br>" + 
                          "feel free to check out our website. <font color=" + properties.colorBasePurple + "> " 
                          + properties.companyWebsite + "/dontations</font>"
                    wrapMode: Text.WordWrap
                    textFormat: Text.RichText
                    color: properties.colorLightText3
                    font.pointSize: 14
                    font.family: properties.defaultFontFamily
                }
                
                RowLayout
                {
                    id: heartColumn
                    Layout.topMargin: 48
                    spacing: 0
                    
                    Image
                    {
                        id: heartImage
                        Layout.alignment: Qt.AlignBottom
                        source: properties.iconHeartGray
                        sourceSize.width: 80
                        fillMode: Image.PreserveAspectFit
                    }
                    
                    Label
                    {
                        id: leaveText
                        Layout.fillWidth: true                        
                        Layout.leftMargin: 16
                        text: "If you dont, we hope you enjoy our app and wish you the best."
                        wrapMode: Text.WordWrap
                        color: properties.colorLightText3
                        font.pointSize: 14
                        font.family: properties.defaultFontFamily
                    }
                }
            }
        }
    }
}
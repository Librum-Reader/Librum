import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents


Page
{
    id: root
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.pagesBackground
    }
    
    
    ColumnLayout
    {
        id: layout
        property int marginToLeft: 48
        property int marginToRight: 48
        property int inRectLeftMargin : 40
        
        width: root.width - marginToLeft - marginToRight
        spacing: 0
        
        
        MTitle
        {
            id: title
            Layout.topMargin: 64
            Layout.leftMargin: layout.marginToLeft
            titleText: "Support us"
            descriptionText: "Thanks for considering"
            titleSize: 25
            descriptionSize: 13.25
        }
        
        Rectangle
        {
            id: container
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 500
            Layout.topMargin: 32
            Layout.leftMargin: layout.marginToLeft
            color: properties.colorBackground
            border.color: properties.colorLightBorder
            radius: 4
            antialiasing: true
            
            
            ColumnLayout
            {
                id: inDetailsLayout
                spacing: 0
                
                Label
                {
                    id: detailsTitle
                    Layout.leftMargin: layout.inRectLeftMargin
                    Layout.topMargin: 24
                    text: "Us"
                    color: properties.colorBaseText
                    font.pointSize: 20
                    font.family: properties.defaultFontFamily
                    font.weight: Font.DemiBold
                }
                
                RowLayout
                {
                    id: teamText
                    spacing: 0
                    
                    
                    Label
                    {
                        Layout.leftMargin: layout.inRectLeftMargin
                        Layout.topMargin: 15
                        text: "We are a small team of freelancing developers creating free apps for the community, we love\n" +
                              "working on fun projects, supporting our community and trying to make the world a better place."
                        color: properties.colorLightText3
                        font.pointSize: 14
                        font.family: properties.defaultFontFamily
                    }
                    
                    Image
                    {
                        id: heartHandImage
                        Layout.alignment: Qt.AlignBottom
                        Layout.leftMargin: 8
                        Layout.bottomMargin: 5
                        source: properties.iconSettingsSupportUsGray
                        sourceSize.width: 23
                        fillMode: Image.PreserveAspectFit
                    }
                }
                
                Label
                {
                    id: workText
                    Layout.leftMargin: layout.inRectLeftMargin
                    Layout.topMargin: 18
                    text: "We are working in our own name, so we are not getting payed in any way."
                    color: properties.colorLightText3
                    font.pointSize: 14
                    font.family: properties.defaultFontFamily
                }
                
                RowLayout
                {
                    id: supportText
                    spacing: 0
                    
                    
                    Label
                    {
                        Layout.leftMargin: layout.inRectLeftMargin
                        text: "We are thankful for any support."
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
                }
                
                Label
                {
                    id: coffeeText
                    Layout.leftMargin: layout.inRectLeftMargin
                    Layout.topMargin: 36
                    text: "If you feel like supporting us and our new projects, feel free to buy us a coffee"
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
                    Layout.leftMargin: layout.inRectLeftMargin
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
                    Layout.leftMargin: layout.inRectLeftMargin
                    Layout.topMargin: 32
                    text: "We also offer lots of different donation methods, from paypal, to crypto currencies. If you prefer any other method,<br>" + 
                          "feel free to check out our website. <font color=" + properties.colorBasePurple + "> " 
                          + properties.companyWebsite + "/dontations</font>"
                    textFormat: Text.RichText
                    color: properties.colorLightText3
                    font.pointSize: 14
                    font.family: properties.defaultFontFamily
                }
                
                RowLayout
                {
                    id: heartColumn
                    Layout.topMargin: 36
                    Layout.leftMargin: layout.inRectLeftMargin
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
                        Layout.leftMargin: 16
                        text: "If you dont, we hope you enjoy our app and wish you the best."
                        color: properties.colorLightText3
                        font.pointSize: 14
                        font.family: properties.defaultFontFamily
                    }
                }
            }
        }
    }
}
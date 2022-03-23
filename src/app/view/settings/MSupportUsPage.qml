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
            id: details
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
                    spacing: 0
                    
                    
                    Label
                    {
                        Layout.leftMargin: layout.inRectLeftMargin
                        Layout.topMargin: 14
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
                    Layout.leftMargin: layout.inRectLeftMargin
                    Layout.topMargin: 16
                    text: "We are working in our own name, so we are not getting payed in any way."
                    color: properties.colorLightText3
                    font.pointSize: 14
                    font.family: properties.defaultFontFamily
                }
                
                RowLayout
                {
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
                    Layout.leftMargin: layout.inRectLeftMargin
                    Layout.topMargin: 30
                    text: "If you feel like supporting us and our new projects, feel free to buy us a coffee"
                    color: properties.colorLightText3
                    font.pointSize: 14
                    font.family: properties.defaultFontFamily
                }
                
                MButton
                {
                    Layout.preferredWidth: 162
                    Layout.preferredHeight: 34
                    Layout.topMargin: 25
                    Layout.leftMargin: layout.inRectLeftMargin
                    backgroundColor: properties.colorCoffeeBrown
                    borderColor: "transparent"
                    imagePath: properties.iconCoffee
                    imageSize: 19
                    imageSpacing: 10
                    textContent: "Buy us a coffee"
                    fontSize: 11.5
                    fontColor: properties.colorBaseTitle
                    buttonRadius: 4
                }
            }
        }
    }
}
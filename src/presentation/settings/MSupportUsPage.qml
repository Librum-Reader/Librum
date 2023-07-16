import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.elements
import Librum.models


MFlickWrapper
{
    id: root
    contentHeight: page.implicitHeight
    
    
    Page
    {
        id: page
        topPadding: 64
        width: parent.width
        horizontalPadding: 48
        bottomPadding: 22
        background: Rectangle { anchors.fill: parent; color: Style.colorPageBackground }
        
        
        ColumnLayout
        {
            id: layout
            width: parent.width
            spacing: 0
            
            
            MTitle
            {
                id: pageTitle
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
                    color: Style.colorContainerBackground
                    border.color: Style.colorContainerBorder
                    radius: 4
                    antialiasing: true
                }
                
                
                ColumnLayout
                {
                    id: contentLayout
                    width: parent.width
                    spacing: 0
                    
                    
                    Label
                    {
                        id: contentTitle
                        text: "Us"
                        color: Style.colorText
                        font.pointSize: 20
                        font.weight: Font.DemiBold
                    }
                    
                    Label
                    {
                        Layout.fillWidth: true
                        Layout.topMargin: 15
                        text: "We are a small team of freelancing developers creating free apps for the community. We love\n" +
                              "working on fun projects, supporting our community and trying to make the world a better place."
                        wrapMode: Text.WordWrap
                        color: Style.colorLightText
                        font.pointSize: 14
                    }
                    
                    Label
                    {
                        Layout.fillWidth: true
                        Layout.topMargin: 18
                        text: "We are working in our own name, so we are not getting paid in any way."
                        wrapMode: Text.WordWrap
                        color: Style.colorLightText
                        font.pointSize: 14
                    }
                    
                    // RowLayout needed to put icon next to text
                    RowLayout
                    {
                        id: supportText
                        Layout.fillWidth: true
                        spacing: 0
                        
                        Label
                        {
                            text: "We are thankful for any support."
                            wrapMode: Text.WordWrap
                            color: Style.colorLightText
                            font.pointSize: 14
                        }
                        
                        Image
                        {
                            id: seedImage
                            Layout.alignment: Qt.AlignBottom
                            Layout.leftMargin: 2
                            Layout.bottomMargin: 3
                            source: Icons.seed
                            sourceSize.width: 21
                            fillMode: Image.PreserveAspectFit
                        }
                        
                        Item { id: widthFiller; Layout.fillWidth: true }
                    }
                    
                    Label
                    {
                        id: supportOnPatreonText
                        Layout.fillWidth: true
                        Layout.topMargin: 36
                        text: "If you feel like supporting us and our new projects, feel free to support us on patreon"
                        wrapMode: Text.WordWrap
                        color: Style.colorLightText
                        font.pointSize: 14
                    }
                    
                    MButton
                    {
                        id: patreonButton
                        Layout.preferredWidth: 146
                        Layout.preferredHeight: 38
                        Layout.topMargin: 10
                        borderWidth: 0
                        backgroundColor: Style.colorBasePurple
                        imagePath: Icons.patreon
                        imageSize: 18
                        imageSpacing: 10
                        text: "Support us"
                        fontWeight: Font.DemiBold
                        fontSize: 12
                        textColor: Style.colorFocusedButtonText
                        radius: 4
                        
                        onClicked: Qt.openUrlExternally("https://www.patreon.com/librumreader")
                    }
                    
                    Label
                    {
                        id: otherPaymentsText
                        Layout.fillWidth: true
                        Layout.topMargin: 40
                        text: "We also offer lots of different donation methods, from paypal, to crypto currencies. If you prefer any other method,<br>" + 
                              "feel free to check out our website at <a href=\"" + 
                              AppInformation.companyWebsite + "/donate\" style=\"color: " + Style.colorBasePurple + "; text-decoration: none;\"> " + 
                              AppInformation.companyWebsite + "/donate </a>"
                        onLinkActivated: Qt.openUrlExternally(link)
                        wrapMode: Text.WordWrap
                        textFormat: Text.RichText
                        color: Style.colorLightText
                        font.pointSize: 14
                        
                        MouseArea
                        {
                            id: mouseArea
                            anchors.fill: parent
                            acceptedButtons: Qt.NoButton // Don't eat the mouse clicks
                            cursorShape: otherPaymentsText.hoveredLink != "" ? Qt.PointingHandCursor : Qt.ArrowCursor
                        }
                    }
                    
                    // RowLayout needed to put icon next to text
                    RowLayout
                    {
                        id: heartColumn
                        Layout.topMargin: 48
                        spacing: 0
                        
                        Image
                        {
                            id: heartImage
                            Layout.alignment: Qt.AlignBottom
                            source: Icons.heart
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
                            color: Style.colorLightText
                            font.pointSize: 14
                        }
                    }
                }
            }
        }
    }
}
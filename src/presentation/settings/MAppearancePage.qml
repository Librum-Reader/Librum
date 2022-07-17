import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons


Page
{
    id: root
    horizontalPadding: 48
    bottomPadding: 22
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.pagesBackground
    }
    
    
    ColumnLayout    
    {
        id: layout
        property int insideMargin : 40
        
        width: parent.width
        spacing: 26
        
        RowLayout
        {
            id: titleRow
            Layout.fillWidth: true
            spacing: 0
            
            
            MTitle
            {
                id: title
                Layout.topMargin: 64
                titleText: "Appearance"
                descriptionText: "Make your own experience"
                titleSize: 25
                descriptionSize: 13.25
            }
            
            Item { Layout.fillWidth: true }
            
            MButton
            {
                id: saveButton
                Layout.preferredWidth: 90
                Layout.preferredHeight: 38
                Layout.alignment: Qt.AlignBottom
                borderWidth: 0
                backgroundColor: Style.colorBasePurple
                text: "Save"
                fontSize: 12
                fontWeight: Font.Bold
                fontColor: Style.colorBackground
                imagePath: Icons.checkWhite
                imageSize: 14
                
                onClicked: root.saveAppearanceSettings()
            }
        }
        
        Pane
        {
            id: details
            Layout.fillWidth: true
            Layout.topMargin: 6
            topPadding: 24
            horizontalPadding: layout.insideMargin
            bottomPadding: 21
            background: Rectangle
            {
                color: Style.colorBackground
                border.color: Style.colorLightBorder
                radius: 4
                antialiasing: true
            }
            
            
            ColumnLayout
            {
                width: parent.width
                spacing: 12
                
                MDualToggle
                {
                    
                }
                
                MSpinbox
                {
                    
                }
                
                MRadioButton
                {
                    
                }
            }
        }
    }
    
    function saveAppearanceSettings()
    {
        
    }
}
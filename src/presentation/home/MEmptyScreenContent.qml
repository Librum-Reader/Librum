import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents


Item
{
    id: root
    signal clicked
    
    implicitWidth: 1713
    implicitHeight: 873
    
    
    Image
    {
        id: emptyBackgroundImage
        source: properties.imageEmptyHomeBackground
        sourceSize.width: parent.width
        sourceSize.height: parent.height
        
        ColumnLayout
        {
            id: inEmptyBackgroundLayout
            width: parent.width
            
            Image
            {
                id: fileSwiftImage
                Layout.preferredWidth: 250
                Layout.preferredHeight: 135
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 225
                source: properties.imageFileSwift
            }
            
            Label
            {
                id: addBooksQuestion
                Layout.preferredWidth: 250
                Layout.alignment: Qt.AlignHCenter
                text: "Quiet empty here, what about importing your first book?"
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 14
                font.family: properties.defaultFontFamily
                font.weight: Font.DemiBold
                color: properties.colorLightText3
                wrapMode: Text.WordWrap
            }
            
            MButton
            {
                id: emptyAddBooksButton
                Layout.preferredWidth: 134
                Layout.preferredHeight: 42
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 20
                backgroundColor: properties.colorLightPurple
                backgroundOpacityOnPressed: 0.75
                borderColor: properties.colorMediumPurple
                text: "Add book"
                fontColor: properties.colorBasePurple
                fontBold: true
                fontSize: 13.5
                imagePath: properties.iconPlusBlack
                imageSize: 16
                
                onClicked:
                {
                    root.clicked();
                }
            }
        }
    }
}

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style


Item
{
    id: root
    signal clicked
    
    implicitWidth: 1713
    implicitHeight: 873
    
    
    Image
    {
        id: emptyBackgroundImage
        source: properties.illustrationEmptyHomeBackground
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
                source: properties.illustrationFileSwift
            }
            
            Label
            {
                id: addBooksQuestion
                Layout.preferredWidth: 250
                Layout.alignment: Qt.AlignHCenter
                text: "Quiet empty here, what about importing your first book?"
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 14
                font.family: Style.defaultFontFamily
                font.weight: Font.DemiBold
                color: Style.colorLightText3
                wrapMode: Text.WordWrap
            }
            
            MButton
            {
                id: emptyAddBooksButton
                Layout.preferredWidth: 134
                Layout.preferredHeight: 42
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 20
                backgroundColor: Style.colorLightPurple
                opacityOnPressed: 0.75
                borderColor: Style.colorMediumPurple
                text: "Add book"
                fontColor: Style.colorBasePurple
                fontWeight: Font.Bold
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

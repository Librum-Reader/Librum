import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons


Item
{
    id: root
    signal clicked
    
    
    Image
    {
        id: emptyBackgroundImage
        source: Icons.emptyHomeBackground
        sourceSize.width: parent.width
        sourceSize.height: parent.height
        
        ColumnLayout
        {
            id: inEmptyBackgroundLayout
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -100
            
            Image
            {
                id: fileSwiftImage
                sourceSize.width: 250
                sourceSize.height: 135
                source: Icons.fileSwift
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
                imagePath: Icons.plusBlack
                imageSize: 16
                
                onClicked: root.clicked();
            }
        }
    }
}

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0
import Librum.icons 1.0
import CustomComponents 1.0

Item
{
    id: root
    signal clearFilters()
    
    implicitWidth: layout.implicitWidth
    implicitHeight: layout.implicitHeight
    
    ColumnLayout
    {
        id: layout
        anchors.fill: parent
        spacing: 20
        
        Label
        {
            id: text
            text: "No book satisfies the filter conditions"
            color: Style.colorBaseTitle
            font.pointSize: 22
            font.weight: Font.Medium
        }
        
        MButton
        {
            id: removeFiltersButton
            Layout.preferredWidth: 170
            Layout.preferredHeight: 38
            Layout.alignment: Qt.AlignHCenter
            backgroundColor: Style.colorLightHighlight
            opacityOnPressed: 0.75
            borderColor: Style.colorLightPurple
            text: "Remove Filters"
            textColor: Style.colorBasePurple
            fontWeight: Font.Bold
            fontSize: 12.5
            imagePath: Icons.closePurple
            imageSize: 11
            imageToRight: true
            
            onClicked: root.clearFilters()
        }
    }
}
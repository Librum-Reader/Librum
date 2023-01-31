import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Librum.style 1.0


Item
{
    id: root
    property string image
    property int imageWidth: 15
    property string text
    property int textSpacing: 5
    signal clicked
    
    implicitWidth: 125
    implicitHeight: layout.implicitHeight
    
    
    RowLayout
    {
        id: layout
        anchors.fill: parent
        
        
        Image
        {
            id: icon
            source: root.image
            fillMode: Image.PreserveAspectFit
            sourceSize.width: root.imageWidth
        }
        
        Label
        {
            id: label
            Layout.leftMargin: root.textSpacing
            text: root.text
            color: Style.colorLightText
            font.pointSize: 10.5
            font.weight: Font.Medium
        }
        
        Item { Layout.fillWidth: true }
    }
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked: root.clicked()
    }
}
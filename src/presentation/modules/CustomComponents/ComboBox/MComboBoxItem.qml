import QtQuick
import QtQuick.Controls

Item
{
    id: root
    property string itemContent : content
    property ListView container
    property int radius
    
    property int fontSize
    property color fontColor
    property int fontWeight
    property string fontFamily
    
    implicitWidth: container.width
    implicitHeight: 28
    
    
    Rectangle
    {
        anchors.fill: parent
        radius: root.radius
        color: "transparent"
        
        Label
        {
            id: label
            anchors.fill: parent
            anchors.leftMargin: 6
            verticalAlignment: Text.AlignVCenter
            color: root.fontColor
            text: content
            font.pointSize: root.fontSize
            font.family: root.fontFamily
            font.weight: root.fontWeight
        }
    }
    
    
    MouseArea
    {
        anchors.fill: parent
        onClicked:
        {
            container.currentIndex = index;
            closeComboBox();
        }
    }
    
    Keys.onPressed: 
        (event) =>
        {
            if(event.key === Qt.Key_Return)
            {
                if(comobBoxIsOpened())
                {
                    closeComboBox();
                }
            }
        }
}
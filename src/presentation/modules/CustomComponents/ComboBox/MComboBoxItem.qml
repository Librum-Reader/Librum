import QtQuick
import QtQuick.Controls

Item
{
    property string content : languageName
    property ListView container
    
    property int fontSize
    property color fontColor
    property int fontWeight
    property string fontFamily
    
    
    implicitWidth: container.width
    implicitHeight: label.implicitHeight + 8
    
    
    Rectangle
    {
        anchors.fill: parent
        radius: root.radius
        color: "transparent"
        
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                container.currentIndex = index;
                closeComboBox();
            }
        }
        
        Label
        {
            id: label
            anchors.fill: parent
            anchors.margins: 4
            anchors.verticalCenter: parent.verticalCenter
            color: root.fontColor
            text: languageName
            font.pointSize: root.fontSize
            font.family: root.fontFamily
            font.weight: root.fontWeight
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
import QtQuick
import QtQuick.Controls

Item
{
    property string content : languageName
    property ListView container
    
    width: container.width
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
                container.currentIndex = index
                closeComboBox();
            }
        }
        
        Label
        {
            id: label
            anchors.fill: parent
            anchors.margins: 4
            anchors.verticalCenter: parent.verticalCenter
            color: properties.colorBaseText
            text: languageName
            font.pointSize: 11
            font.family: properties.defaultFontFamily
            font.weight: Font.Normal
        }
    }
    
    Keys.onPressed: (event) =>
                    {
                        if(event.key === Qt.Key_Return)
                        {
                            if(comobBoxIsOpened())
                            closeComboBox();
                        }
                    }
}
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.platform
import Librum.style
import Librum.icons


Item
{
    id: root
    property bool imageSelected: imagePath.length > 0
    property string imagePath: ""
    
    implicitWidth: 100
    implicitHeight: 100
    
    
    Pane
    {
        id: container
        anchors.fill: parent
        padding: 1
        clip: true
        background: Rectangle
        {
            color: Style.colorBackground
            border.color: Style.colorLightBorder
            radius: 4
        }
        
        
        DropArea
        {
            id: dropArea
            anchors.fill: parent
            
            onDropped:
                (drop) => root.imagePath = drop.urls[0];
            
            
            Image
            {
                id: image
                anchors.fill: parent
                visible: imageSelected
                source: imagePath
                sourceSize.height: emptyDropArea.height
                fillMode: Image.PreserveAspectFit
            }
            
            Rectangle
            {
                id: addFileImage
                anchors.fill: parent
                z: 2
                opacity: imageSelected ? 1 : 0.8
                visible: dropArea.containsDrag
                color: Style.colorBackground
                radius: 4
                
                
                Image
                {
                    anchors.centerIn: parent
                    source: Icons.addFileIllustration
                    sourceSize.width: 220
                    fillMode: Image.PreserveAspectFit
                }
            }
            
            Pane
            {
                id: emptyDropArea
                anchors.fill: parent
                visible: !root.imageSelected
                padding: 0
                clip: true
                background: Rectangle
                {
                    color: Style.colorLightGray
                    radius: 4
                }
                
                
                ColumnLayout
                {
                    id: emptyDropAreaLayout
                    width: parent.width
                    anchors.centerIn: parent
                    spacing: 10
                    
                    
                    Image
                    {
                        id: emptyDropAreaIcon
                        Layout.alignment: Qt.AlignCenter
                        Layout.topMargin: 10
                        source: Icons.addImage
                        sourceSize.width: 40
                        fillMode: Image.PreserveAspectFit
                    }
                    
                    Label
                    {
                        id: emptyDropAreaText
                        Layout.fillWidth: true
                        Layout.maximumWidth: 237
                        Layout.alignment: Qt.AlignCenter
                        text: "Click to select an image or drop it into in this area"
                        horizontalAlignment: Qt.AlignHCenter
                        wrapMode: Text.WordWrap
                        color: Style.colorLightText2
                        font.pointSize: 12
                        font.family: Style.defaultFontFamily
                        font.weight: Font.DemiBold
                        lineHeight: 0.9
                    }
                }
            }
        }
    }
    
    
    MouseArea
    {
        anchors.fill: parent
        
        onClicked: fileDialog.open()
    }
    
    
    FileDialog
    {
        id: fileDialog
        fileMode: FileDialog.OpenFiles
        folder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)
        nameFilters: ["png files (*.png)", "jpg files (*.jpg)", "jpeg files (*.jpeg)"]
        
        onAccepted: root.imagePath = file
    }
}
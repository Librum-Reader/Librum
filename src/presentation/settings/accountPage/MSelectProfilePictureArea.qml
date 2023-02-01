import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Qt.labs.platform 1.0
import Librum.style 1.0
import Librum.icons 1.0


Item
{
    id: root
    property string image: ""
    signal imageSelected()
    
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
            border.color: Style.colorContainerBorder
            radius: 4
        }
        
        
        DropArea
        {
            id: dropArea
            anchors.fill: parent
            
            onDropped: (drop) =>
                       {
                           root.image = drop.urls[0];
                           root.imageSelected();
                       }
            
            /*
              The image which is displayed when the dropArea owns an image
              */
            Image
            {
                id: profilePicture
                anchors.fill: parent
                visible: internal.imageExists
                source: root.image
                sourceSize.height: emptyDropArea.height
                fillMode: Image.PreserveAspectFit
            }
            
            /*
              The illustation you see while hovering over the dropArea with a file
              */
            Rectangle
            {
                id: fileDropIllustration
                anchors.fill: parent
                z: 2
                opacity: internal.imageExists ? 1 : 0.8
                visible: dropArea.containsDrag
                color: Style.colorBackground
                radius: 4
                
                
                Image
                {
                    id: fileDropImage
                    anchors.centerIn: parent
                    source: Icons.addFileIllustration
                    sourceSize.width: 220
                    fillMode: Image.PreserveAspectFit
                }
            }
            
            /*
              The empty look of the drop area
              */
            Pane
            {
                id: emptyDropArea
                anchors.fill: parent
                visible: !internal.imageExists
                padding: 0
                clip: true
                background: Rectangle { color: Style.colorDropArea; radius: 4 }
                
                
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
                        color: Style.colorSubtitle
                        font.pointSize: 12
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
        nameFilters: ["All files (*)", "png files (*.png)", "jpg files (*.jpg)", "jpeg files (*.jpeg)"]
        
        onAccepted: root.image = file
    }
    
    QtObject
    {
        id: internal
        property bool imageExists: root.image.length > 0
    }
}
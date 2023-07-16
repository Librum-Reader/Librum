import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.platform
import Librum.style
import Librum.icons
import Librum.controllers


Item
{
    id: root
    property string currentImage: ""  // Being set to "!" means DELETED
    
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
            color: Style.colorContainerBackground
            border.color: Style.colorContainerBorder
            radius: 4
        }
        
        
        DropArea
        {
            id: dropArea
            anchors.fill: parent
            
            onDropped: (drop) => root.currentImage = drop.urls[0]
            
            /*
              The image which is displayed when the dropArea owns an image
              */
            Image
            {
                id: profilePicture
                anchors.fill: parent
                visible: internal.imageExists
                source: root.currentImage.length > 0 ? (root.currentImage == "!" ? "" : root.currentImage) : UserController.profilePicture
                sourceSize.height: emptyDropArea.height
                fillMode: Image.PreserveAspectFit
                cache: false
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
                color: Style.colorContainerBackground
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
    
    Image
    {
        id: deletePictureButton
        z: 2
        anchors.top: parent.top
        anchors.right: parent.right
        sourceSize.width: 19
        anchors.topMargin: 8
        anchors.rightMargin: 8
        source: Icons.trashRed
        fillMode: Image.PreserveAspectFit
        opacity: deletePictureArea.pressed ? 0.6 : 0.8
        visible: (deletePictureArea.containsMouse || mainArea.containsMouse) && internal.imageExists
        
        MouseArea
        {
            id: deletePictureArea
            anchors.fill: parent
            hoverEnabled: true
            
            onClicked: root.currentImage = "!";
        }
    }
    
    MouseArea
    {
        id: mainArea
        anchors.fill: parent
        hoverEnabled: true
        
        onClicked: fileDialog.open()
    }
    
    FileDialog
    {
        id: fileDialog
        fileMode: FileDialog.OpenFiles
        folder: StandardPaths.writableLocation(StandardPaths.PicturesLocation)
        nameFilters: ["All files (*)", "png files (*.png)", "jpg files (*.jpg)", "jpeg files (*.jpeg)"]
        
        onAccepted: root.currentImage = file
    }
    
    QtObject
    {
        id: internal
        property bool imageExists: ((root.currentImage.length > 0) || (UserController.profilePicture.length > 0)) 
                                   && root.currentImage != "!"
    }
}
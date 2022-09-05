import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0


Item
{
    id: root
    property bool active: false
    property bool currentlyHovered: mouseArea.containsMouse
    property bool currentlyPressed: mouseArea.pressed
    
    property string text
    property color fontColor: "black"
    property int fontWeight: Font.Bold
    property double fontSize: 11
    property color backgroundColor: "white"
    property double opacityOnPressed: 0.9
    property int borderWidth: 1
    property color borderColor: "red"
    property color borderColorOnPressed: borderColor
    property int radius: 4
    property string imagePath
    property int imageSpacing: 8
    property int imageSize: 15
    property int imageLeftMargin: 0
    property int imageRotation: 0
    property bool imageToRight: false
    property bool centerContentVertically: true
    property bool centerContentHorizontally: true
    
    implicitHeight: 30
    
    signal clicked()
        
    
    Rectangle
    {
        id: container
        anchors.fill: parent
        color: root.backgroundColor
        border.width: root.borderWidth
        border.color: mouseArea.pressed ? root.borderColorOnPressed : root.borderColor
        radius: root.radius
        opacity: mouseArea.pressed ? root.opacityOnPressed : 1
        antialiasing: true
        
        
        RowLayout
        {
            id: layout
            anchors.verticalCenter: root.centerContentVertically ? parent.verticalCenter : undefined
            anchors.horizontalCenter: root.centerContentHorizontally ? parent.horizontalCenter : undefined
            spacing: root.imageSpacing
            
            Image
            {
                id: imageToLeft
                Layout.leftMargin: root.imageLeftMargin
                visible: root.imagePath.length > 0 && !root.imageToRight
                source: root.imagePath
                sourceSize.width: root.imageSize
                fillMode: Image.PreserveAspectFit
                rotation: root.imageRotation
            }
            
            Label
            {
                id: loginButtonText
                visible: text.length > 0
                Layout.preferredWidth: container.width < implicitWidth ? container.width : implicitWidth
                text: root.text
                font.weight: root.fontWeight
                font.pointSize: root.fontSize
                font.family: Style.defaultFontFamily
                color: root.fontColor
                elide: Text.ElideRight
                
                Component.onCompleted:
                {
                    if((container.height/loginButtonText.implicitHeight) / 2 > 0.1)
                    {
                        loginButtonText.Layout.topMargin = loginButtonText.Layout.topMargin - 1;
                    }
                }
            }
            
            Image
            {
                id: imageToRight
                visible: root.imagePath.length > 0 && root.imageToRight
                source: root.imagePath
                sourceSize.width: root.imageSize
                fillMode: Image.PreserveAspectFit
            }
        }
        
        MouseArea
        {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            
            onClicked: root.clicked()
        }
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }    
}
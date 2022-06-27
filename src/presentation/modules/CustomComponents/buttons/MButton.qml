import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style


Item
{
    id: root
    property bool active: false
    property string text
    property color fontColor: "black"
    property int fontWeight: Font.Bold
    property double fontSize: 11
    property color backgroundColor: "white"
    property double opacityOnPressed: 0.9
    property int borderWidth: 1
    property color borderColor: "red"
    property int buttonRadius: 4
    property string imagePath
    property int imageSpacing: 8
    property int imageSize: 15
    property int imageLeftMargin: 0
    property int imageRotation: 0
    property bool imageToRight: false
    property bool centerContentVertically: true
    property bool centerContentHorizontally: true
    
    implicitWidth: layout.width
    implicitHeight: 30
    
    signal clicked()
        
    
    Rectangle
    {
        id: container
        anchors.fill: parent
        color: root.backgroundColor
        border.width: root.borderWidth
        border.color: root.borderColor
        radius: root.buttonRadius
        opacity: (mouseArea.pressed ? root.opacityOnPressed : 1)
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
                text: root.text
                font.weight: root.fontWeight
                font.pointSize: root.fontSize
                font.family: Style.defaultFontFamily
                color: root.fontColor
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
            onClicked: root.clicked()
        }
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }    
}
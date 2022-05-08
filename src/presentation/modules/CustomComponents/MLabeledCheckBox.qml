import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope
{
    id: root
    property int boxWidth : 22
    property int boxHeight : 22
    property color borderColor : properties.colorDarkBorder
    property int borderRadius : 4
    property int borderWidth : 1
    property color uncheckedBackgroundColor : "transparent"
    property color checkedBackgroundColor : properties.colorBasePurple
    property string imagePath : properties.iconCheckWhite
    property int imageSize: container.width - 10
    property bool imageDefaultVisibility : false
    property bool checked : false
    property int spacing : 5
    property string textContent : "<text here>"
    property double fontPointSize : 12
    property double fontWeight : 1
    property color fontColor : "black"
    signal clicked()
    
    implicitWidth: 100
    implicitHeight: layout.height
    
    RowLayout
    {
        id: layout
        spacing: root.spacing
        
        Rectangle
        {
            id: container
            Layout.preferredWidth: root.boxWidth
            Layout.preferredHeight: root.boxHeight
            radius: root.borderRadius
            border.width: (root.checked ? 0 : root.borderWidth)
            border.color: root.borderColor
            color: (root.checked ? root.checkedBackgroundColor : root.uncheckedBackgroundColor)
            antialiasing: true
            
            
            Image
            {
                id: image
                anchors.centerIn: parent
                visible: (root.checked ? true : false)
                sourceSize.width: root.imageSize
                source: root.imagePath
                fillMode: Image.PreserveAspectFit
            }
            
            MouseArea
            {
                id: mouseArea
                anchors.fill: parent
                onClicked:
                {
                    root.clicked();
                    root.checked = !root.checked;
                }
            }
        }
    
        Label
        {
            id: text
            Layout.preferredWidth: root.width
            text: root.textContent
            font.weight: root.fontWeight
            font.pointSize: root.fontPointSize
            font.family: properties.defaultFontFamily
            color: root.fontColor
            wrapMode: Text.WordWrap
        }
    }
}

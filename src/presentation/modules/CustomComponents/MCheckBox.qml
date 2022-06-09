import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Item
{
    id: root
    property color borderColor : properties.colorDarkBorder
    property int borderRadius : 4
    property int borderWidth : 1
    property color uncheckedBackgroundColor : "transparent"
    property color checkedBackgroundColor : properties.colorBasePurple
    property string imagePath : properties.iconCheckWhite
    property bool imageDefaultVisibility : false
    property bool checked : false
    signal clicked()
    
    implicitWidth: 22
    implicitHeight: 22
    
    
    Rectangle
    {
        id: container
        anchors.fill: parent
        radius: root.borderRadius
        border.width: (root.checked ? 0 : root.borderWidth)
        border.color: root.activeFocus ? properties.colorLightBorder : root.borderColor
        color: (root.checked ? root.checkedBackgroundColor : root.uncheckedBackgroundColor)
        antialiasing: true
        
        Image
        {
            id: image
            anchors.centerIn: parent
            visible: (root.checked ? true : false)
            sourceSize.width: parent.width - 10
            source: root.imagePath
            fillMode: Image.PreserveAspectFit
        }
        
        MouseArea
        {
            id: mouseArea
            anchors.fill: parent
            onClicked: actionOnClicked();
        }
    }
    
    
    function actionOnClicked()
    {
        root.clicked();
        root.checked = !root.checked;
    }
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}

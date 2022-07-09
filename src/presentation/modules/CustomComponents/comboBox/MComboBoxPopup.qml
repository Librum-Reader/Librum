import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style
import CustomComponents


Popup
{
    id: root
    property var selectedItem: listView.currentItem
    property alias model: listView.model
    property int itemHeight: 28
    property color backgroundColor
    property int maxHeight: 208
    property int radius: 5
    property int defaultIndex: 0
    
    property double fontSize: 11
    property color fontColor: Style.colorBaseText
    property int fontWeight: Font.Normal
    property string fontFamily: Style.defaultFontFamily
    property string highlightColor: Style.colorLightPurple
    
    implicitWidth: 300
    padding: 8
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    focus: true
    background: Rectangle
    {
        color: root.backgroundColor
        border.width: 1
        border.color: Style.colorLightBorder
        radius: root.radius
        antialiasing: true
    }
    
    
    ColumnLayout
    {
        id: mainLayout
        width: parent.width
        
        ListView
        {
            id: listView
            Layout.fillWidth: true
            Layout.preferredHeight: contentHeight
            Layout.maximumHeight: root.maxHeight
            maximumFlickVelocity: 550
            currentIndex: root.defaultIndex
            keyNavigationEnabled: true
            clip: true
            focus: true
            boundsBehavior: Flickable.StopAtBounds
            
            delegate: MComboBoxItem
            {
                height: root.itemHeight
                container: listView
                fontSize: root.fontSize
                fontColor: root.fontColor
                fontWeight: root.fontWeight
                fontFamily: root.fontFamily
                radius: root.radius - 1
                
                onClose: root.close()
            }
            
            ScrollBar.vertical: ScrollBar { }
        }
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}
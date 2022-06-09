import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import librum.extensions.sidebar


Item
{
    id: root
    property int arrowRotation : 0
    property string initialLetters : "DL"
    signal clicked
    
    implicitWidth: closedWidth
    implicitHeight: 60
    
    Pane
    {
        id: container
        padding: 0
        anchors.fill: parent
        background: Rectangle
        {
            color: "transparent"
        }
        
        
        RowLayout
        {
            width: currentWidth + expandButton.width / 2.2  // Makes the button go over the border
            height: parent.height
            spacing: 0
            
            Rectangle
            {
                id: profilePicture
                Layout.preferredWidth: 36
                Layout.preferredHeight: 36
                Layout.alignment: Qt.AlignVCenter
                Layout.leftMargin: 18
                radius: width
                antialiasing: true
                color: "#DBCE5F"
                
                Label
                {
                    id: initials
                    anchors.centerIn: parent
                    text: root.initialLetters
                    font.pointSize: 12
                    font.bold: true
                    font.family: properties.defaultFontFamily
                    color: properties.colorBrightText
                }
                
                MouseArea
                {
                    anchors.fill: parent
                    
                    onClicked: root.clicked()
                }
            }
            
            Rectangle
            {
                id: expandButton
                Layout.preferredWidth: 23
                Layout.preferredHeight: 24
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                radius: 2
                border.width: 1
                border.color: properties.colorLightBorder
                
                Image
                {
                    id: rightArrowImage
                    rotation: root.arrowRotation
                    sourceSize.width: 20
                    sourceSize.height: 20
                    anchors.centerIn: parent
                    source: properties.iconArrowRightHallow
                }
                
                MouseArea
                {
                    id: expandMouseArea
                    anchors.fill: parent
                    
                    onClicked:
                    {
                        if(SidebarState.currentState === SidebarState.Opened)
                            closeSidebar();
                        else
                            openSidebar();
                    }
                }
            }
        }
    }
    
    
    function giveFocus()
        {
            root.forceActiveFocus();
        }    
}

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import librum.extensions.sidebar


FocusScope
{
    id: root
    property int arrowRotation : 0
    property string initialLetters : "DL"
    
    implicitWidth: closedWidth
    implicitHeight: 60
    
    Rectangle
    {
        id: container
        width: parent.width
        height: parent.height
        color: "transparent"
        
        RowLayout
        {
            width: currentWidth + expandButton.width / 2.2  // So that the button goes over the border
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
                    source: "/resources/images/right_icon.svg"
                }
                
                MouseArea
                {
                    id: expandMouseArea
                    anchors.fill: parent
                    
                    onClicked:
                    {
                        if(SidebarState.currentState === SidebarState.Opened)
                        {
                            animations.closeAnimation.start();
                            selectedTab.closeAnimation.start();
                            SidebarState.currentState = SidebarState.Closed;
                        }
                        else
                        {
                            animations.openAnimation.start();
                            selectedTab.openAnimation.start();
                            SidebarState.currentState = SidebarState.Opened;
                        }
                    }
                }
            }
        }
    }
}

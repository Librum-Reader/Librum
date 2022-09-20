import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Librum.elements 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.globals 1.0


Item
{
    id: root
    property int arrowRotation: 0
    signal clicked
    
    implicitWidth: closedWidth
    implicitHeight: 60
    
    Pane
    {
        id: container
        padding: 0
        anchors.fill: parent
        background: Rectangle { color: "transparent" }
        
        
        RowLayout
        {
            width: currentWidth + expandButton.width / 2.2  // To make the button go over the border
            height: parent.height
            spacing: 0
            
            Rectangle
            {
                id: profilePicture
                Layout.preferredWidth: 36
                Layout.preferredHeight: 36
                Layout.alignment: Qt.AlignVCenter
                Layout.leftMargin: 18
                clip: true
                radius: width
                antialiasing: true
                color: Globals.profilePicture.length === 0 ? "#DBCE5F" : "transparent"
                
                Label
                {
                    id: initials
                    anchors.centerIn: parent
                    visible: Globals.profilePicture.length === 0
                    text: Globals.firstName[0].toUpperCase() + Globals.lastName[0].toUpperCase() 
                    font.pointSize: 12
                    font.bold: true
                    color: Style.colorBrightText
                }
                
                Image
                {
                    id: image
                    visible: Globals.profilePicture.length > 0
                    anchors.centerIn: parent
                    Layout.leftMargin: 18
                    source: Globals.profilePicture
                    sourceSize.height: parent.height
                    fillMode: Image.PreserveAspectFit
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
                border.color: Style.colorLightBorder
                
                Image
                {
                    id: rightArrowImage
                    rotation: root.arrowRotation
                    sourceSize.width: 20
                    sourceSize.height: 20
                    anchors.centerIn: parent
                    source: Icons.arrowtopBlackRight
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

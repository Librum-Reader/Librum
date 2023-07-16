import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.elements
import Librum.style
import Librum.icons
import Librum.controllers


Item
{
    id: root
    property int currentSidebarWidth
    property int closedSidebarWidth
    property int arrowRotation
    signal clicked
    
    implicitWidth: root.closedSidebarWidth
    implicitHeight: 60
    
    
    Pane
    {
        id: container
        padding: 0
        anchors.fill: parent
        background: Rectangle { color: "transparent" }
        
        
        RowLayout
        {
            // Make the button go over the sidebar border
            width: root.currentSidebarWidth + expandButton.width / 2.2
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
                color: UserController.profilePicture.length === 0 ? Style.colorDefaultProfilePicture : "transparent"
                
                
                Label
                {
                    id: initials
                    anchors.centerIn: parent
                    visible: UserController.profilePicture.length === 0
                    text: UserController.firstName[0].toUpperCase() + UserController.lastName[0].toUpperCase()
                    font.pointSize: 12
                    font.bold: true
                    color: Style.colorFocusedButtonText
                }
                
                Image
                {
                    id: profileImage
                    visible: UserController.profilePicture.length > 0
                    anchors.centerIn: parent
                    Layout.leftMargin: 18
                    source: UserController.profilePicture
                    sourceSize.height: parent.height
                    fillMode: Image.PreserveAspectFit
                    cache: false
                }
                
                MouseArea
                {
                    anchors.fill: parent
                    
                    onClicked: root.clicked()
                }
            }
            
            /*
              Button to expand the sidebar
              */
            Rectangle
            {
                id: expandButton
                Layout.preferredWidth: 23
                Layout.preferredHeight: 24
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                color: Style.colorControlBackground
                opacity: expandButtonMouseArea.pressed ? 0.8 : 1
                radius: 2
                border.width: 1
                border.color: Style.colorButtonBorder
                
                
                Image
                {
                    id: rightArrowImage
                    rotation: root.arrowRotation
                    sourceSize.width: 20
                    sourceSize.height: 20
                    anchors.centerIn: parent
                    source: Icons.arrowheadNextIcon
                }
                
                MouseArea
                {
                    id: expandButtonMouseArea
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

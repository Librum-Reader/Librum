import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Librum.elements 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.globals 1.0
import Librum.controllers 1.0


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
                color: Globals.profilePicture.length === 0 ? Style.colorDefaultProfilePicture : "transparent"
                
                
                Label
                {
                    id: initials
                    anchors.centerIn: parent
                    visible: !internal.profilePictureExists
                    text: UserController.firstName[0].toUpperCase() + UserController.lastName[0].toUpperCase()
                    font.pointSize: 12
                    font.bold: true
                    color: Style.colorFocusedButtonText
                }
                
                Image
                {
                    id: profileImage
                    visible: internal.profilePictureExists
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
    
    QtObject
    {
        id: internal
        property bool profilePictureExists: Globals.profilePicture.length > 0
    }
    
    
    function giveFocus()
    {
        root.forceActiveFocus();
    }
}

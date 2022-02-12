import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


FocusScope
{
    id: root
    implicitWidth: 72
    implicitHeight: 1080
    
    property bool opened : true
    
    Rectangle
    {
        id: container
        color: properties.colorBackground
        height: parent.height
        width: parent.width
        
        ColumnLayout
        {
            id: mainLayout
            anchors
            {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            
            MLogo
            {
                id: logo
                Layout.preferredWidth: 44
                Layout.preferredHeight: 44
                Layout.topMargin: 14
                Layout.leftMargin: 14
            }
            
            Rectangle
            {
                id: logoSeparator
                Layout.preferredWidth: root.width
                Layout.preferredHeight: 2
                Layout.topMargin: 14
                color: properties.colorSidebarSeparator
            }
            
            Rectangle
            {
                id: downloadButtonContainer
                Layout.preferredWidth: 52
                Layout.preferredHeight: 44
                Layout.topMargin: 16
                Layout.leftMargin: 10
                radius: 4
                color: (focus ? properties.colorSidebarMark : "transparent")
              
                Image
                {
                    id: downloadImage
                    sourceSize.height: 45
                    sourceSize.width: 45
                    anchors.centerIn: parent
                    source: "/resources/images/download-icon.svg"
                }
            }
            
            Rectangle
            {
                id: smallSeperator1
                Layout.preferredWidth: (root.opened ? 44 : 44)
                Layout.preferredHeight: 2
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 16
                color: properties.colorSidebarSeparator
            }
            
            Rectangle
            {
                id: homeButtonContainer
                Layout.preferredWidth: 52
                Layout.preferredHeight: 44
                Layout.topMargin: 16
                Layout.leftMargin: 10
                radius: 4
                color: (focus ? properties.colorSidebarMark : "transparent")
                
                Image
                {
                    id: homeImage
                    sourceSize.width: 32
                    sourceSize.height: 32
                    anchors.centerIn: parent
                    source: "/resources/images/home-icon.svg"
                }
            }
            
            Rectangle
            {
                id: statisticsButtonContainer
                Layout.preferredWidth: 52
                Layout.preferredHeight: 44
                Layout.topMargin: 12
                Layout.leftMargin: 10
                radius: 4
                color: (focus ? properties.colorSidebarMark : "transparent")
              
                Image
                {
                    id: statisticsImage
                    sourceSize.width: 30
                    sourceSize.height: 30
                    anchors.centerIn: parent
                    source: "/resources/images/pieChart-icon.svg"
                }
            }
        }
    }
}
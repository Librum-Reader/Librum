import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.Elements
import Librum.Models


MFlickWrapper
{
    id: root
    contentHeight: page.implicitHeight
    
    
    Page
    {
        id: page
        width: parent.width
        horizontalPadding: 48
        bottomPadding: 22
        background: Rectangle
        {
            anchors.fill: parent
            color: Style.pagesBackground
        }
        
        
        ColumnLayout
        {
            spacing: 0
            width: parent.width
            
            MTitle
            {
                id: title
                Layout.topMargin: 64
                titleText: "Updates"
                descriptionText: "Any new update?"
                titleSize: 25
                descriptionSize: 13.25
            }
            
            
            Component { id: updatesAvailable; MUpdatesAvailable {} }
            Component { id: upToDate; MUpToDate {} }
            
            Loader
            {
                id: contentLoader
                Layout.fillWidth: true
                Layout.fillHeight: true
                sourceComponent: AppInformation.currentVersion === AppInformation.newestVersion ? upToDate : updatesAvailable
            }
        }
    }
}
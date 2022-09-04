import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.elements 1.0
import Librum.models 1.0


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
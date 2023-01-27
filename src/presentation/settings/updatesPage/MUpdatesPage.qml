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
        background: Rectangle { anchors.fill: parent; color: Style.pagesBackground }
        
        
        ColumnLayout
        {
            id: layout
            spacing: 0
            width: parent.width
            
            MTitle
            {
                id: pageTitle
                Layout.topMargin: 64
                titleText: "Updates"
                descriptionText: "Any new update?"
                titleSize: 25
                descriptionSize: 13.25
            }
            
            
            // Pages to render
            Component { id: updatesAvailable; MUpdatesAvailable {} }
            Component { id: upToDate; MUpToDate {} }
            
            
            /*
              The loader loads the fitting page, depending on whether the
              application is up to date or not
              */
            Loader
            {
                id: contentLoader
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.topMargin: 32
                sourceComponent: AppInformation.currentVersion === AppInformation.newestVersion ? upToDate : updatesAvailable
            }
        }
    }
}
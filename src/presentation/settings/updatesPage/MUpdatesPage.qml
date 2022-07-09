import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import CustomComponents
import Librum.style
import Librum.icons
import Librum.Elements


Page
{
    id: root
    horizontalPadding: 48
    background: Rectangle
    {
        anchors.fill: parent
        color: Style.pagesBackground
    }
    
    Component { id: updatesAvailable; MUpdatesAvailable {} }
    Component { id: upToDate; MUpToDate {} }
    
    Loader
    {
        id: contentLoader
        anchors.fill: parent
        sourceComponent: AppInformation.currentVersion === AppInformation.newestVersion ? upToDate : updatesAvailable
    }
}
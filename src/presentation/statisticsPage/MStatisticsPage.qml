import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.style

Page
{
    id: root
    background: Rectangle { color: Style.colorPageBackground }
    
    Item { id: topSpacer; height: parent.height / 2.3 }
    
    Label
    {
        id: title
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: topSpacer.bottom
        text: "Statistics Page"
        color: Style.colorTitle
        font.pointSize: 22
        font.bold: true
    }
    
    Label
    {
        id: description
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: title.bottom
        anchors.topMargin: 6
        text: "Currently in Development"
        color: Style.colorPageSubtitle
        font.pointSize: 16
        font.bold: true
    }
}
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.style 1.0

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
        text: "Add-ons Page"
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
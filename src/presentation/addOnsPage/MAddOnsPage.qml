import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page
{
    id: root
    background: Rectangle { color: "gray" }
    
    Label
    {
        anchors.centerIn: parent
        text: "Add-ons"
        font.pointSize: 22
        font.bold: true
    }
}
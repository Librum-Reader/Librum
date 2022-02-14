import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components/sidebar"


Page
{
    id: root
    
    MSidebar
    {
        anchors.left: parent.left
        focus: true
    }
}
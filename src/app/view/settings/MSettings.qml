import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents


Page
{
    id: root
    background: Rectangle
    {
        anchors.fill: parent
        color: properties.pagesBackground
    }
    
    
    RowLayout
    {
        id: layout
        anchors.fill: parent
        
        
        MSettingsSidebar
        {
            id: sidebar
            height: parent.height
        }
        
//        StackView
//        {
//            id: pageManager
//            Layout.fillWidth: true
//            Layout.fillHeight: true
//            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
//            initialItem: aboutPage
//        }
    }
    
    
//    Component { id: aboutPage; MAboutPage{} }
}
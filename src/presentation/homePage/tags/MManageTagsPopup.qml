import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0


Popup
{
    id: root
    implicitWidth: 516
    implicitHeight: layout.height
    padding: 0
    horizontalPadding: 52
    background: Rectangle
    {
        color: Style.colorBackground
        radius: 6
    }
    modal: true
    Overlay.modal: Rectangle
    {
        color: "#aa32324D"
        opacity: 1
    }
    
    onOpened: addTagBox.giveFocus()
    
    
    MFlickWrapper
    {
        anchors.fill: parent
        contentHeight: layout.height
        
        
        ColumnLayout
        {
            id: layout
            width: parent.width
            spacing: 0
            
            
            MButton
            {
                id: closeButton
                Layout.preferredHeight: 32
                Layout.preferredWidth: 32
                Layout.topMargin: 12
                Layout.rightMargin: -38
                Layout.alignment: Qt.AlignTop | Qt.AlignRight
                backgroundColor: "transparent"
                opacityOnPressed: 0.7
                borderColor: "transparent"
                radius: 6
                borderColorOnPressed: Style.colorLightBorder
                imagePath: Icons.closeBlack
                imageSize: 14
                
                onClicked: root.close()
            }
            
            Label
            {
                id: title
                Layout.topMargin: 20
                text: "Manage Tags"
                font.weight: Font.Bold
                font.pointSize: 17
                color: Style.colorBaseTitle
            }
            
            
            MAddTagBox
            {
                id: addTagBox
                Layout.topMargin: 46
                Layout.fillWidth: true
                
                onAddTag: (name) => tags.append({"text": name})
            }
            
            
            Label
            {
                id: informationLabel
                Layout.fillWidth: true
                Layout.topMargin: 32
                Layout.leftMargin: 1
                text: tags.count + " TAGS  -  Clean architecture, a simple guide"
                color: Style.colorLightText
                font.pointSize: 9.5
                font.weight: Font.Medium
                elide: Text.ElideRight
            }
            
            Rectangle
            {
                id: separator
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                Layout.topMargin: 4
                color: Style.colorMediumPurple
            }
            
            
            ListView
            {
                id: listView
                property var currentSelected
                
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                Layout.maximumHeight: 228
                Layout.minimumHeight: 76
                Layout.topMargin: 8
                maximumFlickVelocity: 550
                currentIndex: -1
                clip: true
                boundsBehavior: Flickable.StopAtBounds
                ScrollBar.vertical: ScrollBar {}
                
                model: ListModel
                {
                    id: tags
                    ListElement { text: "Technology" }
                    ListElement { text: "Favourite" }
                }
                
                delegate: MTagItem
                {
                    width: listView.width
                    
                    onRemoveTag: (index) => tags.remove(index)
                }
            }
            
            MButton 
            {
                id: loginButton
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Layout.topMargin: 34
                Layout.bottomMargin: 42
                borderWidth: 0
                backgroundColor: Style.colorBasePurple
                fontSize: 12
                fontColor: Style.colorBrightText
                fontWeight: Font.Bold
                text: "Apply"
                
                onClicked: root.close()
            }
        }
    }
    
    
    function removeTag()
    {
        console.log("Remove tag")
    }
}
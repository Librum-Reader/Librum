import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.Elements
import Librum.style
import Librum.icons


Item
{
    id: root
    implicitWidth: 300
    implicitHeight: 600
    
    Rectangle
    {
        anchors.fill: parent
        color: "transparent"
        
        
        ColumnLayout
        {
            anchors.fill: parent
            spacing: 0
            
            
            Label
            {
                id: header
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 26
                text: "Contents"
                font.family: Style.defaultFontFamily
                font.pointSize: 14
                font.weight: Font.Medium
                color: Style.colorBaseText
            }
            
            Rectangle
            {
                id: searchBox
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                Layout.margins: 12
                Layout.alignment: Qt.AlignBottom
                radius: 4
                color: Style.colorBackground
                border.color: Style.colorLightBorder
                
                TextField
                {
                    id: inputField
                    anchors.fill: parent
                    leftPadding: 15
                    rightPadding: 15
                    selectByMouse: true
                    color: Style.colorBaseText
                    font.pointSize: 12
                    font.family: Style.defaultFontFamily
                    font.weight: Font.Normal
                    placeholderText: "Search..."
                    placeholderTextColor: Style.colorLightText3
                    background: Rectangle   
                    {   
                        anchors.fill: parent
                        radius: 4
                        color: "transparent"
                    }
                }
            }
            
            Pane
            {
                id: treeViewContainer
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 12
                Layout.topMargin: 0
                verticalPadding: 6
                horizontalPadding: 4
                background: Rectangle
                {
                    radius: 4
                    color: Style.colorBackground
                    border.color: Style.colorLightBorder
                }
                
                
                ScrollView
                {
                    anchors.fill: parent
                    
                    
                    TreeView
                    {
                        id: treeView
                        property Item selectedItem
                        
                        anchors.fill: parent
                        anchors.rightMargin: 12
                        boundsMovement: Flickable.StopAtBounds
                        clip: true
                        maximumFlickVelocity: 550
                        
                        model: ChapterTreeModel { }
                        delegate: Item
                        {
                            id: treeDelegate
                            required property TreeView treeView
                            required property int row
                            required property bool isTreeNode
                            required property bool expanded
                            required property int hasChildren
                            required property int depth
                            
                            readonly property real indent: 16
                            property bool selected: false
                            
                            implicitWidth: treeView.width
                            implicitHeight: labelBox.height
                            
                            
                            RowLayout
                            {
                                id: deleagteLayout
                                anchors.fill: parent
                                anchors.leftMargin: treeDelegate.depth * treeDelegate.indent + (!indicator.visible ? indicator.width + spacing : 0)
                                spacing: 4
                                
                                
                                Item { Layout.fillHeight: true; Layout.preferredWidth: 2 }
                                
                                Image
                                {
                                    id: indicator
                                    visible: treeDelegate.isTreeNode && treeDelegate.hasChildren
                                    source: Icons.arrowRightGray
                                    sourceSize.width: 7
                                    fillMode: Image.PreserveAspectFit
                                    rotation: treeDelegate.expanded ? 90 : 0
                                    
                                    TapHandler
                                    {
                                        onTapped: treeView.toggleExpanded(row)
                                    }
                                    
                                    
                                    Behavior on rotation
                                    {
                                        NumberAnimation { duration: 150 }
                                    }
                                }
                                
                                Rectangle
                                {
                                    id: labelBox
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 22
                                    
                                    color: treeDelegate.selected ? Style.colorSidebarMark
                                                             : itemHoverDetector.containsMouse ? Style.colorLightGray 
                                                                                               : "transparent"
                                    radius: 2
                                    
                                    
                                    Label
                                    {
                                        id: label
                                        anchors.fill: parent
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignLeft
                                        anchors.leftMargin: 4
                                        text: model.display
                                        font.family: Style.defaultFontFamily
                                        font.pointSize: 10.5
                                        font.weight: treeDelegate.selected ? Font.Medium : Font.Normal
                                        color: treeDelegate.selected ? Style.colorBasePurple : Style.colorBaseText
                                        elide: Text.ElideRight
                                    }
                                    
                                    MouseArea
                                    {
                                        id: itemHoverDetector
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        
                                        onClicked:
                                        {
                                            treeDelegate.changeSelectedItem(treeDelegate);
                                        }
                                    }
                                }
                            }
                            
                            
                            function changeSelectedItem(item)
                            {
                                if(treeView.selectedItem != null)
                                    treeView.selectedItem.selected = false;
                                
                                item.selected = true;
                                treeView.selectedItem = item;
                            }
                        }
                    }
                }
            }
        }
    }
}
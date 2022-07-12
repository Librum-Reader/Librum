import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.Elements
import Librum.style
import Librum.icons


Item
{
    id: root
    implicitWidth: 250
    implicitHeight: 600
    
    Rectangle
    {
        anchors.fill: parent
        color: Style.colorLightGray2
        
        
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
                font.pointSize: 15
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
                verticalPadding: 16
                horizontalPadding: 10
                background: Rectangle
                {
                    radius: 4
                    color: Style.colorBackground
                    border.color: Style.colorLightBorder
                }
                
                
                TreeView
                {
                    id: treeView
                    anchors.fill: parent
                    
                    model: ChapterTreeModel { id: model  }
                    delegate: Item
                    {
                        id: treeDelegate
                        required property TreeView treeView
                        required property int row
                        required property bool isTreeNode
                        required property bool expanded
                        required property int hasChildren
                        required property int depth
                        readonly property real indent: 20
                        
                        implicitWidth: treeView.width
                        implicitHeight: label.implicitHeight * 1.3
                        
                        
                        RowLayout
                        {
                            id: deleagteLayout
                            anchors.fill: parent
                            anchors.leftMargin: treeDelegate.depth * treeDelegate.indent + (!indicator.visible ? indicator.width + spacing : 0)
                            spacing: 8
                            
                            
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
                            
                            Label
                            {
                                id: label
                                Layout.fillWidth: true
                                text: model.display
                                font.family: Style.defaultFontFamily
                                font.pointSize: 10.5
                                color: Style.colorBaseText
                                elide: Text.ElideRight
                            }
                        }
                    }
                }
            }
        }
    }
}
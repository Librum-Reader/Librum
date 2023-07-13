import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.elements
import Librum.models
import Librum.style
import Librum.icons
import Qt.labs.qmlmodels
import QtQuick.Shapes


Item
{
    id: root
    property var chapterModel
    signal switchPage(int pageNumber)
    
    implicitWidth: 300
    implicitHeight: 600
    
    
    Rectangle
    {
        id: background
        anchors.fill: parent
        color: "transparent"
        
        
        ColumnLayout
        {
            id: layout
            anchors.fill: parent
            spacing: 0
            
            
            Label
            {
                id: header
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 26
                text: "Contents"
                font.pointSize: 14
                font.weight: Font.Medium
                color: Style.colorText
            }
            
            Rectangle
            {
                id: searchBox
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                Layout.margins: 12
                Layout.alignment: Qt.AlignBottom
                radius: 4
                color: Style.colorContainerBackground
                border.color: Style.colorButtonBorder
                
                
                TextField
                {
                    id: inputField
                    anchors.fill: parent
                    leftPadding: 15
                    rightPadding: 15
                    selectByMouse: true
                    color: Style.colorText
                    font.pointSize: 11
                    font.weight: Font.Normal
                    placeholderText: "Search..."
                    placeholderTextColor: Style.colorPlaceholderText
                    background: Rectangle
                    {   
                        anchors.fill: parent
                        radius: 4
                        color: "transparent"
                    }
                    
                    onTextEdited: documentItem.tableOfContents.filterString = text
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
                    color: Style.colorContainerBackground
                    border.color: Style.colorContainerBorder
                }
                
                
                ScrollView
                {
                    id: scrollBar
                    property bool isEnabled: contentHeight > height
                    anchors.fill: parent
                    
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    onIsEnabledChanged:
                    {
                        if(isEnabled)
                            ScrollBar.vertical.policy = ScrollBar.AlwaysOn;
                        else
                            ScrollBar.vertical.policy = ScrollBar.AlwaysOff;
                    }
                    
                    
                    TreeView
                    {
                        id: treeView
                        property int indent: 18
                        
                        anchors.fill: parent
                        anchors.margins: 1
                        anchors.rightMargin: scrollBar.isEnabled ? 18 : 1
                        model: documentItem.tableOfContents
                        clip: true
                        focus: true
                        
                        delegate: Rectangle
                        {
                            id: treeNode
                            required property string title
                            required property int pageNumber
                            required property TreeView treeView
                            required property bool expanded
                            required property int hasChildren
                            required property int depth
                            
                            implicitWidth: treeView.width - 2  // L/R margins
                            width: implicitWidth
                            implicitHeight: Math.max(treeNodeLabel.height)
                            color: "transparent"
                            
                            
                            RowLayout
                            {
                                id: nodeLayout
                                anchors.left: parent.left
                                anchors.right: parent.right
                                spacing: 0
                                
                                
                                Image
                                {
                                    id: indicator
                                    Layout.preferredWidth: implicitWidth
                                    Layout.leftMargin: treeNode.depth * treeView.indent
                                    Layout.alignment: Qt.AlignVCenter
                                    visible: treeNode.hasChildren
                                    opacity: pageSwitchTrigger.pressed || indicatorArea.pressed ? 0.7 : 1
                                    source: Icons.arrowheadNextIcon
                                    sourceSize.width: 20
                                    fillMode: Image.PreserveAspectFit
                                    rotation: treeNode.expanded ? 90 : 0
                                    
                                    MouseArea
                                    {
                                        id: indicatorArea
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        
                                        onClicked: treeView.toggleExpanded(row)
                                    }
                                }
                                
                                Text
                                {
                                    id: treeNodeLabel
                                    Layout.fillWidth: true
                                    Layout.leftMargin: treeNode.hasChildren 
                                                       ? indicator.width * 0.1 
                                                       : indicator.width * 1.1 + depth * treeView.indent
                                    Layout.alignment: Qt.AlignVCenter
                                    clip: true
                                    color: Style.colorText
                                    opacity: pageSwitchTrigger.pressed ? 0.7 : 1
                                    font.pixelSize: 14
                                    elide: Text.ElideRight
                                    text: treeNode.title
                                    
                                    MouseArea
                                    {
                                        id: pageSwitchTrigger
                                        anchors.fill: parent
                                        
                                        onClicked: root.switchPage(model.pageNumber)
                                    }
                                }
                                
                                Text
                                {
                                    id: pageNumberLabel
                                    Layout.preferredWidth: implicitWidth
                                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                                    Layout.leftMargin: 6
                                    color: Style.colorText
                                    opacity: pageSwitchTrigger.pressed ? 0.7 : 1
                                    font.pixelSize: 14
                                    text: treeNode.pageNumber
                                }
                            }
                        }
                        
                    }
                    
                    
                    Component.onCompleted:
                    {
                        // contentItem is the TreeView's underlying Flickable
                        contentItem.flickDeceleration = 10000;
                        contentItem.maximumFlickVelocity = 2000;
                        contentItem.boundsBehavior = Flickable.StopAtBounds
                        contentItem.boundsMovement = Flickable.StopAtBounds
                    }
                }
            }
        }
    }
}
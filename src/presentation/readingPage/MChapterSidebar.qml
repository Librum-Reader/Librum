import QtQuick 2.15
import QtQuick.Controls 1.5
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Librum.elements 1.0
import Librum.models 1.0
import Librum.style 1.0
import Librum.icons 1.0
import QtQuick.TreeView 2.15 as Chapters


Item
{
    id: root
    property var chapterModel
    
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
                    anchors.fill: parent
                    
                    
                    Chapters.TreeView
                    {
                        id: treeView
                        anchors.fill: parent
                        anchors.margins: 1
                        model: root.chapterModel
                        clip: true
                        focus: true
                        navigationMode: Chapters.TreeView.List
            
                        states: State
                        {
                            when: plainModeBox.checked == false
                            PropertyChanges
                            {
                                target: treeView
                                styleHints.indent: 18
                                styleHints.columnPadding: 20
                                styleHints.font.bold: true
                                styleHints.foregroundOdd: "#001a66"
                                styleHints.backgroundOdd: "#e6ecff"
                                styleHints.foregroundEven: "#001a66"
                                styleHints.backgroundEven: "#ccd9ff"
                                styleHints.foregroundCurrent: "#ebf0fa"
                                styleHints.backgroundCurrent: "#2e5cb8"
                                styleHints.foregroundHovered: styleHints.foregroundCurrent
                                styleHints.backgroundHovered: styleHints.backgroundCurrent
                                styleHints.indicator: styleHints.backgroundCurrent
                                styleHints.indicatorCurrent: styleHints.foregroundCurrent
                                styleHints.indicatorHovered: styleHints.foregroundCurrent
                                styleHints.overlay: navigationMode === QtMarketplace.TreeView.Table ? Qt.rgba(1, 1, 1) : "transparent"
                            }
                        }
            
                        onCurrentModelIndexChanged:
                        {
                            var label = model.data(currentModelIndex, treeView.textRole)
                            selectedLabel.text = "Selected row " + currentIndex.row + ", label: " + label
                        }
            
                        Keys.onReturnPressed:
                        {
                            // Set the second file inside the root folder as current:
                            var rootIndex = fileSystemModel.index(0, 0)
                            var childIndex = fileSystemModel.index(1, 0, rootIndex)
                            currentIndex = mapFromModel(childIndex)
                            if (!currentIndex.valid)
                                selectedLabel.text = childIndex + " is not visible"
                        }
                    }
                }
            }
        }
    }
}
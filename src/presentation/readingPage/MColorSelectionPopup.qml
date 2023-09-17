import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style
import Librum.icons
import Librum.controllers

Popup
{
    id: root
    property string highlight: ""
    signal colorSelected(color color)
    
    width: selectionOptionsLayout.width
    height: 32
    padding: 0
    background: Rectangle
    {
        color: Style.colorControlBackground
        radius: 4
    }
    
    Image
    {
        id: triangleDecoration
        x: parent.width / 2 - implicitWidth / 2
        y: parent.y + parent.height - 1
        source: Icons.popupDroplet
        rotation: 180
    }
    
    RowLayout
    {
        id: selectionOptionsLayout
        height: parent.height
        spacing: 0
        
        component ColorItem: Rectangle
        {
            id: colorItem
            property string colorName
            
            Layout.fillHeight: true
            Layout.preferredWidth: 36
            color: "transparent"
            
            Rectangle
            {
                width: parent.width / 2.8
                height: width
                anchors.centerIn: parent
                radius: width
                color: SettingsController.appearanceSettings[colorItem.colorName]
            }
            
            MouseArea
            {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onContainsMouseChanged: activeFocusItem.setPointingCursor()
                
                onClicked:
                {
                    // Change highlight color
                    activeFocusItem.changeHighlightColor(colorSelectionPopup.highlight, 
                                                         SettingsController.appearanceSettings[colorItem.colorName],
                                                         SettingsController.appearanceSettings.HighlightOpacity);
                    
                    // Remember the last color used
                    SettingsController.setSetting(SettingKeys.DefaultHighlightColorName, 
                                                  colorItem.colorName, 
                                                  SettingGroups.Appearance);
                    root.close();
                }
            }
        }
        
        component Separator: Rectangle
        {
            Layout.fillHeight: true
            Layout.preferredWidth: 2
            color: Style.colorSeparator
        }
        
        ColorItem
        {
            colorName: "HighlightColorA"
        }
        
        Separator {}
        
        ColorItem
        {
            colorName: "HighlightColorB"
        }
        
        Separator {}
        
        ColorItem
        {
            colorName: "HighlightColorC"
        }
        
        Separator {}
        
        ColorItem
        {
            colorName: "HighlightColorD"
        }
        
        Separator {}
        
        ColorItem
        {
            colorName: "HighlightColorE"
        }
    }
}
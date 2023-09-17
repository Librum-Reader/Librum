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
    signal highlightOptionSelected(string uuid)
    
    width: getWidth()
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
        // We need to manually calculate the width because QML bugs lead to the RowLayout not being
        // updated properly when an item inside of it changes its visibility.
        spacing: 2
        
        component SelectionOptionsPopupItem: Rectangle
        {
            id: action
            property string text
            property color textColor: Style.colorText
            property var clickedFunction: function() {}
            
            Layout.fillHeight: true
            Layout.preferredWidth: actionText.implicitWidth
            color: "transparent"
            opacity: actionArea.pressed ? 0.8 : 1
            
            Label
            {
                id: actionText
                height: parent.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                padding: 10
                text: action.text
                color:  action.textColor
                font.pointSize: 12
            }
            
            MouseArea
            {
                id: actionArea
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true
                onContainsMouseChanged: activeFocusItem.setPointingCursor()
                
                onClicked: {
                    action.clickedFunction();
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
        
        SelectionOptionsPopupItem
        {
            id: copyAction
            text: "Copy"
            clickedFunction: function() {
                if(root.highlight == "")
                    activeFocusItem.copySelectedText();
                else
                    activeFocusItem.copyTextFromHighlight(root.highlight);
            }
        }
        
        Separator { id: separator1 }
        
        SelectionOptionsPopupItem
        {
            id: highlightAction
            text: "Highlight"
            clickedFunction: function() {
                let defaultColorName = SettingsController.appearanceSettings.DefaultHighlightColorName;
                let uuid = activeFocusItem.createHighlightFromCurrentSelection(SettingsController.appearanceSettings[defaultColorName],
                                                                               SettingsController.appearanceSettings.HighlightOpacity);
                root.highlightOptionSelected(uuid);
            }
        }
        
        Separator { visible: root.highlight != "" }
        
        SelectionOptionsPopupItem
        {
            id: removeAction
            text: "Remove"
            textColor: Style.colorErrorText
            visible: root.highlight != ""
            clickedFunction: function() {
                activeFocusItem.removeHighlight(root.highlight);
            }
        }
    }
    
    function getWidth()
    {
        return separator1.width * 2 + selectionOptionsLayout.spacing * 4 + 
                copyAction.width + highlightAction.width + (root.highlight === "" ? 0 : removeAction.width);
    }
}
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
    
    width: internal.getWidth()
    height: 32
    padding: 0
    background: Rectangle
    {
        color: Style.colorControlBackground
        radius: 4
    }
    
    Shortcut
    {
        sequence: SettingsController.shortcuts.CreateHighlight
        onActivated: {
            internal.createHighlight()
            root.close()
        }
    }
    
    Shortcut
    {
        sequence: SettingsController.shortcuts.RemoveHighlight
        onActivated: {
            if(root.highlight != "")
                activeFocusItem.removeHighlight(root.highlight);
            
            root.close()
        }
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
                    activeFocusItem.copyHighlightedText(root.highlight);
            }
        }
        
        Separator { id: separator1 }
        
        SelectionOptionsPopupItem
        {
            id: highlightAction
            text: "Highlight"
            clickedFunction: function() {
                let uuid = internal.createHighlight();
                root.highlightOptionSelected(uuid);
            }
        }
        
        Separator { }
        
        SelectionOptionsPopupItem
        {
            id: lookUpAction
            text: "Look Up"
            clickedFunction: function() {
                let text = ""
                if(root.highlight == "")
                    text = activeFocusItem.getSelectedText();
                else
                    text = activeFocusItem.getHighlightedText(root.highlight);

                // Return if the character count exceeds 100 because no word-entry will exist.
                if(text.length > 100)
                    return;
                
                DictionaryController.getDefinitionForWord(text);
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
    
    QtObject
    {
        id: internal
        
        function getWidth()
        {
            return separator1.width * 3 + selectionOptionsLayout.spacing * 6 + 
                    copyAction.width + highlightAction.width + lookUpAction.width
                       + (root.highlight === "" ? 0 : removeAction.width);
        }
        
        function createHighlight()
        {
            let defaultColorName = SettingsController.appearanceSettings.DefaultHighlightColorName;
            let uuid = activeFocusItem.createHighlightFromCurrentSelection(SettingsController.appearanceSettings[defaultColorName],
                                                                           SettingsController.appearanceSettings.HighlightOpacity);
            
            return uuid;
        }
    }
}

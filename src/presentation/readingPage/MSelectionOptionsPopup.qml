import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Librum.style

Popup
{
    id: selectionOptionsPopup
    property string highlight: ""
    property bool containsMouse: copyActionArea.containsMouse ||
                                 highlightActionArea.containsMouse ||
                                 removeActionArea.containsMouse
    
    width: selectionOptionsLayout.width
    height: 32
    padding: 0
    background: Rectangle
    {
        color: Style.colorControlBackground
        radius: 4
    }
    
    
    RowLayout
    {
        id: selectionOptionsLayout
        height: parent.height
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
                padding: 8
                text: action.text
                color:  action.textColor
                font.pointSize: 11
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
                    selectionOptionsPopup.close();
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
            text: "Copy"
            clickedFunction: function() {
                if(selectionOptionsPopup.highlight == "")
                    activeFocusItem.copySelectedText();
                else
                    activeFocusItem.copyTextFromHighlight(selectionOptionsPopup.highlight);
            }
        }
        
        Separator {}
        
        SelectionOptionsPopupItem
        {
            text: "Highlight"
            clickedFunction: function() {
                activeFocusItem.createHighlightFromCurrentSelection();
            }
        }
        
        Separator {}
        
        SelectionOptionsPopupItem
        {
            id: removeAction
            text: "Remove"
            textColor: Style.colorErrorText
            visible: selectionOptionsPopup.highlight != ""
            clickedFunction: function() {
                activeFocusItem.removeHighlight(selectionOptionsPopup.highlight);
            }
        }
    }
}
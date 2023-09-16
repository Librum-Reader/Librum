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
        
        Rectangle
        {
            id: copyAction
            Layout.fillHeight: true
            Layout.preferredWidth: copyActionText.implicitWidth
            color: "transparent"
            opacity: copyActionArea.pressed ? 0.8 : 1
            
            Label
            {
                id: copyActionText
                height: parent.height
                color:  Style.colorText
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 11
                padding: 8
                text: "Copy"
            }
            
            MouseArea
            {
                id: copyActionArea
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true
                onContainsMouseChanged: activeFocusItem.setPointingCursor()
                
                onClicked: {
                    if(selectionOptionsPopup.highlight == "")
                        activeFocusItem.copySelectedText();
                    else
                        activeFocusItem.copyTextFromHighlight(selectionOptionsPopup.highlight);
                        
                    selectionOptionsPopup.close();
                }
            }
        }
        
        Rectangle
        {
            Layout.fillHeight: true
            Layout.preferredWidth: 2
            color: Style.colorSeparator
        }
        
        Rectangle
        {
            id: highlightAction
            Layout.fillHeight: true
            Layout.preferredWidth: highlightActionText.implicitWidth
            color: "transparent"
            opacity: highlightActionArea.pressed ? 0.8 : 1
            
            Label
            {
                id: highlightActionText
                height: parent.height
                color:  Style.colorText
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 11
                padding: 8
                text: "Highlight"
            }
            
            MouseArea
            {
                id: highlightActionArea
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true
                onContainsMouseChanged: activeFocusItem.setPointingCursor()
                
                onClicked: {
                    activeFocusItem.createHighlightFromCurrentSelection();
                    selectionOptionsPopup.close();
                }
            }
        }
        
        Rectangle
        {
            Layout.fillHeight: true
            Layout.preferredWidth: 2
            color: Style.colorSeparator
        }
        
        Rectangle
        {
            id: removeAction
            Layout.fillHeight: true
            Layout.preferredWidth: removeActionText.implicitWidth
            color: "transparent"
            opacity: removeActionArea.pressed ? 0.8 : 1
            visible: selectionOptionsPopup.highlight !== ""
            
            Label
            {
                id: removeActionText
                height: parent.height
                color:  Style.colorErrorText
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 11
                padding: 8
                text: "Remove"
            }
            
            MouseArea
            {
                id: removeActionArea
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true
                onContainsMouseChanged: activeFocusItem.setPointingCursor()
                
                onClicked: {
                    activeFocusItem.removeHighlight(selectionOptionsPopup.highlight);
                    selectionOptionsPopup.close();
                }
            }
        }
    }
}
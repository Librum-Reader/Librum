import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons


Popup
{
    id: root
    property string leftButtonText: "Accept"
    property string rightButtonText: "Decline"
    property string title: "Do you Accept?"
    property string message: "This is a message"
    property int buttonsWidth: 120
    property int messageBottomSpacing: 0
    property bool singleButton: false
    signal leftButtonClicked
    signal rightButtonClicked
    signal decisionMade
    
    implicitWidth: 646
    implicitHeight: layout.height
    padding: 0
    closePolicy: Popup.NoAutoClose
    background: Rectangle { color: "transparent"; radius: 4 }
    modal: true
    Overlay.modal: Rectangle { color: Style.colorPopupDim; opacity: 1 }
    
    
    MFlickWrapper
    {
        id: flickWrapper
        anchors.fill: parent
        contentHeight: layout.height
        
        ColumnLayout
        {
            id: layout
            width: parent.width
            spacing: -92
            
            
            Image
            {
                id: warningIllustration
                z: 2
                Layout.alignment: Qt.AlignHCenter
                Layout.rightMargin: 10
                source: Icons.attentionPurple
                sourceSize.width: 250
                fillMode: Image.PreserveAspectFit
            }
            
            Pane
            {
                id: backgroundRect
                Layout.fillWidth: true
                topPadding: 86
                horizontalPadding: 62
                bottomPadding: 62
                background: Rectangle { color: Style.colorPopupBackground; radius: 6 }
                
                
                ColumnLayout
                {
                    id: inRectLayout
                    width: parent.width
                    spacing: 22
                    
                    
                    Label
                    {
                        id: title
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 18
                        text: root.title
                        color: Style.colorTitle
                        font.weight: Font.Medium
                        font.pointSize: 42
                    }
                    
                    Label
                    {
                        id: message
                        Layout.alignment: Qt.AlignHCenter
                        Layout.bottomMargin: root.messageBottomSpacing
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                        text: root.message
                        horizontalAlignment: Qt.AlignHCenter
                        color: Style.colorLightText
                        font.weight: Font.Medium
                        font.pointSize: 15
                    }
                    
                    RowLayout
                    {
                        id: buttonRow
                        Layout.preferredWidth: parent.width
                        Layout.preferredHeight: leftButton.height
                        Layout.topMargin: 24
                        spacing: 42
                        
                        
                        MButton
                        {
                            id: leftButton
                            Layout.preferredWidth: root.singleButton ? parent.width : root.buttonsWidth
                            Layout.preferredHeight: 40
                            Layout.alignment: Qt.AlignBottom | Qt.AlignRight
                            borderWidth: activeFocus ? 0 : 1
                            backgroundColor: activeFocus ? Style.colorBasePurple : "transparent"
                            opacityOnPressed: 0.7
                            text: root.leftButtonText
                            fontSize: 12.75
                            fontWeight: Font.Bold
                            textColor: activeFocus ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                            
                            onClicked: internal.leftButtonClicked()
                            
                            KeyNavigation.tab: rightButton
                            KeyNavigation.right: rightButton
                            Keys.onReturnPressed: internal.leftButtonClicked()
                        }
                        
                        MButton
                        {
                            id: rightButton
                            visible: !root.singleButton
                            Layout.preferredWidth: root.buttonsWidth
                            Layout.preferredHeight: 40
                            Layout.alignment: Qt.AlignBottom | Qt.AlignLeft
                            borderWidth: focus ? 0 : 1
                            backgroundColor: focus ? Style.colorBasePurple : "transparent"
                            opacityOnPressed: 0.7
                            text: root.rightButtonText
                            fontSize: 12.75
                            fontWeight: Font.Bold
                            textColor: focus ? Style.colorFocusedButtonText : Style.colorUnfocusedButtonText
                            
                            onClicked: internal.rightButtonClicked()
                            
                            KeyNavigation.tab: leftButton
                            KeyNavigation.left: leftButton
                            Keys.onReturnPressed: internal.rightButtonClicked()
                        }
                    }
                }
            }
        }
    }
    
    QtObject
    {
        id: internal
        
        function leftButtonClicked()
        {
            root.leftButtonClicked();
            decisionMade();
        }
        
        function rightButtonClicked()
        {
            root.rightButtonClicked();
            decisionMade();
        }
    }
    
    function giveFocus()
    {
        leftButton.forceActiveFocus();
    }
}
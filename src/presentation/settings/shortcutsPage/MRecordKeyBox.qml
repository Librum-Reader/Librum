import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Qt5Compat.GraphicalEffects
import Librum.style
import Librum.icons


Item
{
    id: root
    property var itemToRedirectFocusTo
    property bool recording: false
    
    
    Glow
    {
        id: backgroundGlow
        visible: root.recording
        width: button.width
        height: button.height
        x: button.x
        y: button.y
        spread: 0
        transparentBorder: true
        z: -1
        color: "#A92224"
        
        source: button
    }
    
    ColumnLayout
    {
        id: mainLayout
        anchors.fill: parent
        spacing: 2
        
        Label
        {
            id: header
            text: "Key"
            font.weight: Font.DemiBold
            font.pointSize: 12
            font.family: Style.defaultFontFamily
            color: Style.colorBaseTitle
        }
        
        
        Rectangle
        {
            id: button
            property var pressedKeys: undefined
            
            Layout.fillWidth: true
            Layout.preferredHeight: 38
            radius: 4
            color: Style.colorBackground
            border.color: root.recording ? "#E8B9BA" : Style.colorLightBorder
            opacity: mouseArea.pressed ? 0.8 : 1
            
            onActiveFocusChanged: activeFocus ? root.startRecording() : root.stopRecording()
            
            
            RowLayout
            {
                id: contentLayout
                anchors.fill: parent
                spacing: 4
                
                
                Label
                {
                    id: recordLabel
                    Layout.fillWidth: true
                    Layout.leftMargin: 12
                    text: button.pressedKeys ? button.pressedKeys : "Press to record"
                    font.pointSize: 13
                    font.family: Style.defaultFontFamily
                    color: Style.colorLightText3
                    elide: Text.ElideLeft
                }
                
                Image
                {
                    id: microphoneIcon
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin: 12
                    source: root.recording ? Icons.active_microphone : Icons.microphone
                    sourceSize.width: 18
                    fillMode: Image.PreserveAspectFit
                }
            }
            
            Keys.onPressed: 
                (event) =>
                {
                    event.accepted = true;
                    
                    if(event.key === Qt.Key_Return)
                    {
                        stopRecording();
                        return;
                    }
                        
                    button.pressedKeys = getStringFromInput(event);
                }
            
            function getStringFromInput(event)
            {
                let result = "";
                
                if(event.modifiers & Qt.ShiftModifier)
                    result += "SHIFT+";
                if(event.modifiers & Qt.ControlModifier)
                    result += "CTRL+";
                if(event.modifiers & Qt.AltModifier)
                    result += "ALT+";
                if(event.modifiers & Qt.MetaModifier)
                    result += "META+";
                if(event.key === Qt.Key_Left)
                    result += "LEFT+"
                if(event.key === Qt.Key_Right)
                    result += "RIGHT+"
                if(event.key === Qt.Key_Up)
                    result += "UP+"
                if(event.key === Qt.Key_Down)
                    result += "DOWN+"
                if(event.key === Qt.Key_Escape)
                    result += "ESC+"
                if(event.key === Qt.Key_Delete)
                    result += "DELETE+"
                if(event.key === Qt.Key_Insert)
                    result += "INSERT+"
                if(event.key === Qt.Key_Home)
                    result += "HOME+"
                if(event.key === Qt.Key_PageUp)
                    result += "PAGE UP+"
                if(event.key === Qt.Key_PageUp)
                    result += "PAGE DOWN+"
                if(event.key === Qt.Key_F1)
                    result += "F1+"
                if(event.key === Qt.Key_F2)
                    result += "F2+"
                if(event.key === Qt.Key_F3)
                    result += "F3+"
                if(event.key === Qt.Key_F4)
                    result += "F4+"
                if(event.key === Qt.Key_F5)
                    result += "F5+"
                if(event.key === Qt.Key_F6)
                    result += "F6+"
                if(event.key === Qt.Key_F7)
                    result += "F7+"
                if(event.key === Qt.Key_F8)
                    result += "F8+"
                if(event.key === Qt.Key_F9)
                    result += "F9+"
                if(event.key === Qt.Key_F10)
                    result += "F10+"
                if(event.key === Qt.Key_F11)
                    result += "F11+"
                if(event.key === Qt.Key_F12)
                    result += "F12+"
                if(event.key === Qt.Key_F13)
                    result += "F13+"
                if(event.key === Qt.Key_F14)
                    result += "F14+"
                if(event.key === Qt.Key_F15)
                    result += "F15+"
                if(event.key === Qt.Key_F16)
                    result += "F16+"
                if(event.key === Qt.Key_F17)
                    result += "F17+"
                if(event.key === Qt.Key_F18)
                    result += "F18+"
                
                if(event.text !== "")
                    result += event.text.toUpperCase() + "+";
                
                return result.substring(0, result.length - 1);
            }
        }
    }
    
    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        
        onClicked:
        {
            if(root.recording)
            {
                stopRecording();
                root.forceActiveFocus();
                return;
            }
            
            startRecording();
            button.forceActiveFocus();
        }
    }
    
    
    SequentialAnimation
    {
        loops: Animation.Infinite
        id: buttonPulsatingAnimation
        
        
        PropertyAnimation
        {
            target: backgroundGlow
            property: "radius"
            duration: 600
            to: 16
        }
        
        PropertyAnimation
        {
            target: backgroundGlow
            property: "radius"
            duration: 600
            to: 2
        }
    }
    
    
    function startRecording()
    {
        root.recording = true;
        buttonPulsatingAnimation.start();
    }
    
    function stopRecording()
    {
        itemToRedirectFocusTo.forceActiveFocus();
        root.recording = false;
        buttonPulsatingAnimation.stop();
    }
}
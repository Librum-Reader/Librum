import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import CustomComponents 1.0
import Librum.elements 1.0
import QtGraphicalEffects 1.15
import Librum.style 1.0
import Librum.icons 1.0


Item
{
    id: root
    property var itemToRedirectFocusTo
    property bool recording: false
    property string originalSequence: ""
    
    
    // Creates the glowing effect around the button
    Glow
    {
        id: backgroundGlow
        visible: root.recording
        width: button.width
        height: button.height
        x: button.x
        y: button.y
        spread: 0.05
        radius: 12
        samples: 24
        transparentBorder: true
        color: "#aaA92224"
        source: button
    }
    
    ColumnLayout
    {
        id: layout
        anchors.fill: parent
        spacing: 2
        
        
        Label
        {
            id: header
            text: "Key"
            font.weight: Font.DemiBold
            font.pointSize: 12
            color: Style.colorTitle
        }
        
        Rectangle
        {
            id: button
            Layout.fillWidth: true
            Layout.preferredHeight: 38
            radius: 4
            color: Style.colorButtonBackground
            border.color: root.recording ? "#E8B9BA" : Style.colorButtonBorder
            opacity: mouseArea.pressed ? 0.8 : 1
            
            onActiveFocusChanged: activeFocus ? root.startRecording() : root.stopRecording()
            
            
            KeySequenceRecorder
            {
                id: keySequenceRecorder
                originalSequence: root.originalSequence
                
                onReturnPressed: stopRecording()
            }
            
            
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
                    text: internal.getRecordText()
                    font.pointSize: 13
                    color: Style.colorLightInputText
                    elide: Text.ElideLeft
                }
                
                Image
                {
                    id: microphoneIcon
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin: 12
                    source: root.recording ? Icons.activeMicrophone : Icons.microphone
                    sourceSize.width: 18
                    fillMode: Image.PreserveAspectFit
                }
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
            to: 17
        }
        
        PropertyAnimation
        {
            target: backgroundGlow
            property: "radius"
            duration: 600
            to: 7
        }
    }
    
    QtObject
    {
        id: internal
        
        
        function getRecordText()
        {
            if(keySequenceRecorder.currentSequence)
                return keySequenceRecorder.currentSequence; 
            
            if(keySequenceRecorder.originalSequence)
                return keySequenceRecorder.originalSequence;
            
            return "Press to record";
        }
    }
    
    
    function clear()
    {
        keySequenceRecorder.resetSequence();
    }
    
    function startRecording()
    {
        keySequenceRecorder.forceActiveFocus();
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
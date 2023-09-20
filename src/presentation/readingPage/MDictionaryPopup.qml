import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.elements
import Librum.style
import Librum.icons
import Librum.controllers 

Popup {
    id: root
    property string word
    property var previouslyFocusedPage
    
    implicitWidth: 500
    implicitHeight: 540
    padding: 16
    bottomPadding: 28
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    background: Rectangle {
        color: Style.colorPopupBackground
        radius: 6
    }
    
    onOpened: {
        previouslyFocusedPage = activeFocusItem;
        root.forceActiveFocus();
    }
    
    onClosed: DictionaryController.clearData();
    
    Connections
    {
        target: DictionaryController
        
        function onStartedGettingDefinition() {
            loadingAnimation.playing = true;
            loadingAnimation.visible = true;
            
            dictionaryList.visible = false;
            notFound.visible = false;
        }
        
        function onGettingDefinitionFailed() {
            loadingAnimation.playing = false;
            loadingAnimation.visible = false;
            
            notFound.visible = true;
        }
        
        function onGettingDefinitionSucceeded() {
            loadingAnimation.playing = false;
            loadingAnimation.visible = false;
            
            dictionaryList.visible = true;
        }
    }
    
    MouseArea
    {
        id: actionArea
        anchors.fill: parent
        cursorShape: Qt.ArrowCursor
        hoverEnabled: true
        onContainsMouseChanged: root.previouslyFocusedPage.resetCursorToDefault()
    }
    
    ColumnLayout
    {
        anchors.fill: parent
        
        RowLayout
        {
            Layout.fillWidth: true
            spacing: 8
            
            MButton
            {
                id: backButton
                Layout.preferredWidth: 32
                Layout.preferredHeight: 32
                backgroundColor: Style.colorReadingViewButtonBackground
                borderWidth: 0
                imagePath: Icons.readingViewBack
                imageSize: 8
                opacityOnPressed: 0.7
                
                onClicked: DictionaryController.goToPreviousWord()
            }
            
            Pane
            {
                id: container
                Layout.fillWidth: true
                Layout.preferredHeight: 32
                padding: 0
                background: Rectangle
                {
                    color: Style.colorContainerBackground
                    border.width: 1
                    border.color: Style.colorContainerBorder
                    radius: 5
                }
                
                TextField
                {
                    id: inputField
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 12
                    color: Style.colorBaseInputText
                    text: root.word
                    font.pointSize: 11
                    placeholderText: "Search"
                    placeholderTextColor: Style.colorPlaceholderText
                    selectByMouse: true
                    background: Rectangle
                    {
                        anchors.fill: parent
                        radius: 4
                        color: "transparent"
                    }
                    
                    onEditingFinished: DictionaryController.getDefinitionForWord(text);
                }
            }
        }
        
        Label
        {
            text: root.word.toUpperCase()
            Layout.fillWidth: true
            Layout.maximumHeight: 30
            elide: Text.ElideRight
            wrapMode: Text.NoWrap
            Layout.topMargin: 20
            color: Style.colorText
            font.pointSize: 20
            font.weight: Font.DemiBold
            
            clip: true
        }
        
        Label
        {
            text: "English"
            Layout.topMargin: -4
            color: Style.colorText
            font.pointSize: 11
        }
        
        Pane
        {
            id: contentPane
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 22
            padding: 12
            background: Rectangle
            {
                color: Style.colorControlBackground
                border.width: 1
                border.color: Style.colorContainerBorder
                radius: 5
            }
            
            AnimatedImage {
                id: loadingAnimation
                anchors.centerIn: parent
                source: Icons.loadingAnimation
                width: 110
                fillMode: Image.PreserveAspectFit
            }
            
            ListView
            {
                id: dictionaryList
                anchors.fill: parent
                model: DictionaryController.definition.wordTypes.length
                spacing: 16
                clip: true
                boundsBehavior: Flickable.StopAtBounds
                boundsMovement: Flickable.StopAtBounds
                
                ScrollBar.vertical: ScrollBar {
                    visible: dictionaryList.contentHeight > dictionaryList.height
                }
                
                delegate: Item
                {
                    id: type
                    property int index: modelData
                    
                    width: dictionaryList.width
                    height: clmLayout.implicitHeight
                    
                    ColumnLayout
                    {
                        id: clmLayout
                        width: parent.width
                        spacing: 2
                        
                        RowLayout
                        {
                            width: parent.width
                            
                            Rectangle
                            {
                                id: numberCircle
                                color: Style.colorBasePurple
                                width: 22
                                height: width
                                radius: width
                                
                                Label
                                {
                                    anchors.centerIn: parent
                                    text: modelData + 1
                                    color: Style.colorBannerText
                                    font.pointSize: 10
                                    font.bold: true
                                }
                            }
                            
                            Label
                            {
                                id: partOfSpeech
                                Layout.fillWidth: true
                                Layout.leftMargin: 4
                                Layout.bottomMargin: 1
                                Layout.alignment: Qt.AlignVCenter
                                wrapMode: Text.WordWrap
                                text: DictionaryController.definition.wordTypes[modelData].partOfSpeech
                                color: Style.colorText
                                font.pointSize: 13
                                font.weight: Font.DemiBold
                                textFormat: Text.RichText
                            }
                        }
                        
                        Repeater
                        {
                            id: repeater
                            model: DictionaryController.definition.wordTypes[type.index].definitions.length
                            
                            delegate: Item {
                                id: definitionItem
                                property int index: modelData
                                
                                Layout.topMargin: 6
                                Layout.leftMargin: 32
                                Layout.fillWidth: true
                                Layout.preferredHeight: defLayout.implicitHeight
                                
                                ColumnLayout
                                {
                                    id: defLayout
                                    width: parent.width
                                    
                                    RowLayout
                                    {
                                        Layout.fillWidth: true
                                        spacing: 4
                                        
                                        Label
                                        {
                                            text: (modelData + 1) + "."
                                            Layout.alignment: Qt.AlignTop
                                            wrapMode: Text.WordWrap
                                            color: Style.colorText
                                            font.pointSize: 11
                                            textFormat: Text.RichText
                                        }
                                        
                                        Label
                                        {
                                            id: definitionText
                                            Layout.fillWidth: true
                                            text: DictionaryController.definition.wordTypes[type.index].definitions[modelData].definition
                                            wrapMode: Text.WordWrap
                                            color: Style.colorText
                                            font.pointSize: 11
                                            textFormat: Text.RichText
                                            
                                            MouseArea
                                            {
                                                id: mouseArea
                                                anchors.fill: parent
                                                cursorShape: definitionText.hoveredLink !== "" ? Qt.PointingHandCursor : Qt.ArrowCursor
                                                
                                                onClicked: followWiktionaryLink()
                                                
                                                function followWiktionaryLink() {
                                                    if(definitionText.hoveredLink !== "")
                                                    {
                                                        let link = definitionText.hoveredLink;
                                                        if(link.startsWith("http"))
                                                        {
                                                            Qt.openUrlExternally(link);
                                                            return;
                                                        }
                                                        else if(link.startsWith("/wiki/Wiktionary"))
                                                        {
                                                            Qt.openUrlExternally("https://wiktionary.org/" + link);
                                                            return
                                                        }
                                                        
                                                        // Some words have metadata pre/appended to the link
                                                        // which we need to remove before searching for the word.
                                                        let fixedWord = link;
                                                        if(link.startsWith("/wiki/"))
                                                            fixedWord = link.replace("/wiki/","");
                                                        if(fixedWord.startsWith("Appendix:Glossary#"))
                                                            fixedWord = fixedWord.replace("Appendix:Glossary#", "");
                                                        
                                                        DictionaryController.getDefinitionForWord(fixedWord);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    
                                    Repeater
                                    {
                                        model: DictionaryController.definition.wordTypes[type.index].definitions[definitionItem.index].examples.length
                                        
                                        delegate: Label
                                        {
                                            id: example
                                            Layout.fillWidth: true
                                            Layout.leftMargin: 28
                                            text: DictionaryController.definition.wordTypes[type.index].definitions[definitionItem.index].examples[modelData]
                                            wrapMode: Text.WordWrap
                                            color: Style.colorLightText
                                            font.pointSize: 10
                                            font.weight: Font.Light
                                            textFormat: Text.RichText
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            Item
            {
                id: notFound
                anchors.fill: parent
                visible: false
                
                ColumnLayout
                {
                    width: parent.width
                    anchors.centerIn: parent
                    spacing: 4
                    
                    Image
                    {
                        id: warningIllustration
                        z: 2
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: -18
                        source: "file://home/creapermann/Downloads/458.svg"
                        sourceSize.width: 180
                        fillMode: Image.PreserveAspectFit
                    }
                    
                    Label
                    {
                        color: Style.colorText
                        Layout.alignment: Qt.AlignHCenter
                        font.pointSize: 14
                        text: "No definitions found"
                    }
                    
                    Label {
                        id: searchOnlineLink
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 4
                        text: '<a href="update" style="color: ' + Style.colorBasePurple + '; text-decoration: underline;">Search online</a>'
                        textFormat: Text.RichText
                        onLinkActivated: (link) => Qt.openUrlExternally(link)
                        font.pointSize: 14
                        color: Style.colorText
                        
                        MouseArea
                        {
                            anchors.fill: parent
                            cursorShape: searchOnlineLink.hoveredLink !== "" ? Qt.PointingHandCursor : Qt.ArrowCursor
                            
                            onClicked: 
                            {
                                if(searchOnlineLink.hoveredLink !== "")
                                    Qt.openUrlExternally("https://google.com/search?q=" + root.word);
                            }
                        }
                    }
                }
            }
        }
    }
    
    Label {
        id: wiktionaryLink
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -21
        anchors.rightMargin: 2
        horizontalAlignment: Text.AlignRight
        text: 'Source: <a href="https://wiktionary.org" style="text-decoration: none; color: ' + Style.colorBasePurple + ';">Wiktionary</a>'
        textFormat: Text.RichText
        font.pointSize: 9
        color: Style.colorText
        
        MouseArea
        {
            anchors.fill: parent
            cursorShape: wiktionaryLink.hoveredLink !== "" ? Qt.PointingHandCursor : Qt.ArrowCursor
            
            onClicked: 
            {
                if(wiktionaryLink.hoveredLink !== "")
                    Qt.openUrlExternally(wiktionaryLink.hoveredLink);
            }
        }
    }
}
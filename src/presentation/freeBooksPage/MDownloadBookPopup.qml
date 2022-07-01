import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import CustomComponents
import Librum.style
import Librum.icons


Popup
{
    id: root
    property int contentPadding: 16
    
    implicitWidth: 751
    focus: true
    padding: 0
    
    modal: true
    Overlay.modal: Rectangle
    {
        color: "#aa32324D"
        opacity: 1
    }
    
    background: Rectangle
    {
        radius: 6
        color: Style.colorBackground
    }
    
    onOpenedChanged: if(opened) downloadButton.forceActiveFocus()
    
    
    ColumnLayout
    {
        id: mainLayout
        anchors.fill: parent
        spacing: 0
        
        
        MButton
        {
            id: closeButton
            Layout.preferredHeight: 32
            Layout.preferredWidth: 32
            Layout.topMargin: 16
            Layout.rightMargin: 18
            Layout.alignment: Qt.AlignTop | Qt.AlignRight
            backgroundColor: "transparent"
            opacityOnPressed: 0.7
            borderColor: "transparent"
            radius: 6
            borderColorOnPressed: Style.colorLightBorder
            imagePath: Icons.closeBlack
            imageSize: 14
        }
        
        Pane
        {
            id: content
            Layout.fillWidth: true
            Layout.topMargin: 10
            topPadding: 0
            horizontalPadding: 52
            bottomPadding: 42
            background: Rectangle
            {
                color: "transparent"
                radius: 6
            }
            
            
            ColumnLayout
            {
                id: contentLayout
                width: parent.width
                spacing: 0
                
                Label
                {
                    id: title
                    text: "Download book"
                    font.weight: Font.Bold
                    font.pointSize: 17
                    font.family: Style.defaultFontFamily
                    color: Style.colorBaseTitle
                }
                
                RowLayout
                {
                    id: bookInformation
                    spacing: 28
                    Layout.fillWidth: true
                    Layout.topMargin: 32
                    
                    
                    Rectangle
                    {
                        id: bookCoverContainer
                        Layout.preferredWidth: 198
                        Layout.preferredHeight: 258
                        color: Style.colorLightBorder
                        radius: 4
                        
                        Image
                        {
                            id: bookCover
                            anchors.centerIn: parent
                            Layout.alignment: Qt.AlignHCenter
                            sourceSize.height: bookCoverContainer.height - 2
                            source: Icons.bookCover
                            fillMode: Image.PreserveAspectFit
                        }
                    }
                    
                    ColumnLayout
                    {
                        id: bookDetails
                        Layout.fillWidth: true
                        spacing: 16
                        
                        
                        MLabeledInputBox
                        {
                            id: titleField
                            Layout.fillWidth: true
                            Layout.topMargin: -5
                            boxHeight: 34
                            headerText: "Title"
                            headerFontWeight: Font.Bold
                            headerFontSize: 11.5
                            text: "The 7 habits of highly effective people"
                            headerToBoxSpacing: 3
                            inputFontSize: 12
                            inputFontColor: Style.colorLightText3
                            textPadding: 12
                            borderWidth: 1
                            borderRadius: 4
                            readOnly: true
                        }
                        
                        MLabeledInputBox
                        {
                            id: authorField
                            Layout.fillWidth: true
                            boxHeight: 34
                            headerText: "Author"
                            headerFontWeight: Font.Bold
                            headerFontSize: 11.5
                            text: "Stephen R. Covey"
                            headerToBoxSpacing: 3
                            inputFontSize: 12
                            inputFontColor: Style.colorLightText3
                            borderWidth: 1
                            borderRadius: 4
                            readOnly: true
                        }
                        
                        MLabeledInputBox
                        {
                            id: publicationField
                            Layout.fillWidth: true
                            boxHeight: 34
                            headerText: "Publication"
                            headerFontWeight: Font.Bold
                            headerFontSize: 11.5
                            text: "United States: Dodd, Mead and Company,1922."
                            headerToBoxSpacing: 3
                            inputFontSize: 12
                            inputFontColor: Style.colorLightText3
                            borderWidth: 1
                            borderRadius: 4
                            readOnly: true
                        }
                        
                        MLabeledInputBox
                        {
                            id: languageField
                            Layout.fillWidth: true
                            boxHeight: 34
                            headerText: "Language"
                            headerFontWeight: Font.Bold
                            headerFontSize: 11.5
                            text: "English"
                            headerToBoxSpacing: 3
                            inputFontSize: 12
                            inputFontColor: Style.colorLightText3
                            borderWidth: 1
                            borderRadius: 4
                            readOnly: true
                        }
                    }
                }
                
                
                ColumnLayout
                {
                    id: contentFieldLayout
                    Layout.fillWidth: true
                    Layout.topMargin: 28
                    spacing: 3
                    
                    
                    Label
                    {
                        id: contentFieldHeader
                        Layout.fillWidth: true
                        text: "Content"
                        font.family: Style.defaultFontFamily
                        font.pointSize: 11.5
                        font.weight: Font.Bold
                        color: Style.colorBaseTitle
                    }
                    
                    Rectangle
                    {
                        id: contentField
                        Layout.fillWidth: true
                        Layout.preferredHeight: 78
                        color: "transparent"
                        radius: 5
                        border.width: 1
                        border.color: Style.colorLightBorder
                        
                        
                        TextArea
                        {
                            id: contentTextArea
                            anchors.fill: parent
                            leftPadding: 12
                            rightPadding: 12
                            topPadding: 8
                            bottomPadding: 8
                            selectByMouse: true
                            text: "Your habits determine your character and later define your life. Don’t blame outside factors when you fail in life. Also, don’t think that succeeding in one area of your life will mean that you’re destined for triumph."
                            wrapMode: Text.WordWrap
                            color: Style.colorLightText3
                            font.pointSize: 12
                            font.family: Style.defaultFontFamily
                            readOnly: true
                            background: Rectangle   
                            {   
                                anchors.fill: parent
                                radius: contentField.radius
                                color: "transparent"
                            }
                        }
                    }
                }
                
                
                RowLayout
                {
                    id: buttonLayout
                    Layout.topMargin: 42
                    spacing: 16
                    
                    
                    MButton
                    {
                        id: downloadButton
                        Layout.preferredWidth: 137
                        Layout.preferredHeight: 38
                        active: true
                        borderWidth: active ? 0 : 1
                        borderColor: Style.colorLightBorder2
                        backgroundColor: active ? Style.colorBasePurple : "transparent"
                        text: "Download"
                        fontColor: active ? Style.colorBackground : Style.colorBaseTitle
                        fontWeight: Font.Bold
                        fontSize: 12
                        imagePath: active ? Icons.downloadWhite : Icons.downloadBlack
                        imageSize: 18
                        
                        onClicked: root.close()
                        
                        Keys.onPressed: 
                            (event) =>
                            {
                                if(event.key === Qt.Key_Right || event.key === Qt.Key_Tab)
                                {
                                    downloadButton.active = false;
                                    cancelButton.active = true;
                                }
                            }
                        
                        KeyNavigation.right: cancelButton
                        KeyNavigation.tab: cancelButton
                    }
                    
                    MButton
                    {
                        id: cancelButton
                        Layout.preferredWidth: 100
                        Layout.preferredHeight: 38
                        borderWidth: active ? 0 : 1
                        borderColor: Style.colorLightBorder2
                        backgroundColor: active ? Style.colorBasePurple : "transparent"
                        opacityOnPressed: 0.7
                        text: "Cancel"
                        fontColor: active ? Style.colorBackground : Style.colorBaseTitle
                        fontWeight: Font.Bold
                        fontSize: 12
                        
                        onClicked: root.close()
                        
                        Keys.onPressed: 
                            (event) =>
                            {
                                if(event.key === Qt.Key_Left || event.key === Qt.Key_Tab)
                                {
                                    cancelButton.active = false;
                                    downloadButton.active = true;
                                }
                            }
                        
                        KeyNavigation.left: downloadButton
                        KeyNavigation.tab: downloadButton
                    }
                }
            }
        }
    }
}
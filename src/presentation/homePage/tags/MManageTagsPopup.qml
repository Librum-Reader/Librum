import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import CustomComponents 1.0
import Librum.style 1.0
import Librum.icons 1.0
import Librum.controllers 1.0
import Librum.globals 1.0


Popup
{
    id: root
    implicitWidth: 516
    implicitHeight: layout.height
    padding: 0
    horizontalPadding: 52
    background: Rectangle
    {
        color: Style.colorPopupBackground
        radius: 6
    }
    modal: true
    Overlay.modal: Rectangle
    {
        color: Style.colorPopupDim
        opacity: 1
    }
    
    onOpenedChanged: 
    {
        if(opened)
        {
            addTagBox.giveFocus();
            informationLabel.text = Qt.binding(function() {
                return Globals.bookTags.length + " TAGS  -  " + Globals.selectedBook.title;
            });
        }
        else
        {
            addTagBox.close();
            addTagBox.clearInputField();
        }
    }
    
    
    MFlickWrapper
    {
        anchors.fill: parent
        contentHeight: layout.height
        
        
        ColumnLayout
        {
            id: layout
            width: parent.width
            spacing: 0
            
            
            MButton
            {
                id: closeButton
                Layout.preferredHeight: 32
                Layout.preferredWidth: 32
                Layout.topMargin: 12
                Layout.rightMargin: -38
                Layout.alignment: Qt.AlignTop | Qt.AlignRight
                backgroundColor: "transparent"
                opacityOnPressed: 0.7
                borderColor: "transparent"
                radius: 6
                borderColorOnPressed: Style.colorButtonBorder
                imagePath: Icons.closePopup
                imageSize: 14
                
                onClicked: root.close()
            }
            
            Label
            {
                id: popupTitle
                Layout.topMargin: 20
                text: "Manage Tags"
                font.weight: Font.Bold
                font.pointSize: 17
                color: Style.colorTitle
            }
            
            
            MAddTagBox
            {
                id: addTagBox
                Layout.topMargin: 46
                Layout.fillWidth: true
                
                onAddTag: (name) => 
                          {
                              // Cant use return value, bc. it is null if tag already exists
                              UserController.addTag(name);
                              
                              let tagUuid = UserController.getTagUuidForName(name);
                              BookController.addTag(Globals.selectedBook.uuid, name, tagUuid);
                          }
            }
            
            
            Label
            {
                id: informationLabel
                Layout.fillWidth: true
                Layout.topMargin: 32
                Layout.leftMargin: 1
                color: Style.colorSubtitle
                font.pointSize: 9.5
                font.weight: Font.Medium
                elide: Text.ElideRight
            }
            
            Rectangle
            {
                id: separator
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                Layout.topMargin: 4
                color: Style.colorDarkSeparator
            }
            
            
            ListView
            {
                id: listView
                property var currentSelected
                property string oldText
                
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                Layout.maximumHeight: 228
                Layout.minimumHeight: 76
                Layout.topMargin: 8
                maximumFlickVelocity: 550
                currentIndex: -1
                clip: true
                boundsBehavior: Flickable.StopAtBounds
                ScrollBar.vertical: ScrollBar {}
                model: Globals.bookTags
                delegate: MTagItem
                {
                    width: listView.width
                    
                    onRemoveTag: (index) => 
                                 {
                                     BookController.removeTag(Globals.selectedBook.uuid,
                                                              Globals.bookTags[index].uuid);
                                 }
                    
                    onStartedRenaming: (oldText) =>
                                       {
                                           listView.oldText = oldText;
                                       }
                    
                    onRenamedTag: (index, text) =>
                                  {
                                      let currentItem = listView.itemAtIndex(index);
                                      let uuid = UserController.getTagUuidForName(listView.oldText);
                                      
                                      let success = UserController.renameTag(uuid, text);
                                      if(success)
                                      {
                                          BookController.renameTags(listView.oldText, text);
                                      }
                                  }
                }
            }
            
            MButton 
            {
                id: doneButton
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Layout.topMargin: 34
                Layout.bottomMargin: 42
                borderWidth: 0
                backgroundColor: Style.colorBasePurple
                fontSize: 12
                textColor: Style.colorFocusedButtonText
                fontWeight: Font.Bold
                text: "Done"
                
                onClicked: root.close()
            }
        }
    }
    
    
    function removeTag()
    {
        ;
    }
}
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Librum.elements
import Librum.style
import Librum.globals
import Librum.controllers
import "DocumentNavigation.js" as NavigationLogic

Pane {
  id: root
  property var lastSelectedPage
  property var bookController
  property alias documentView: documentView
  signal zoomFactorChanged(real factor)
  signal clicked

  padding: 0
  background: Rectangle {
    color: "transparent"
  }
  // Disable pressing tab to focus other elements
  Keys.onTabPressed: event => {
                       event.accepted = true
                     }

  Connections {
    target: DictionaryController

    function onGettingDefinitionFailed() {}

    function onStartedGettingDefinition(word) {
      if (!dictionaryPopup.opened) {
        dictionaryPopup.x = Qt.binding(function () {
          return root.width / 2 - dictionaryPopup.width / 2
        })
        dictionaryPopup.y = Qt.binding(function () {
          return root.height / 2 - dictionaryPopup.height / 2
        })
        dictionaryPopup.open()
      }

      dictionaryPopup.word = word
    }
  }

  DocumentView {
    id: documentView
    height: parent.height
    width: Math.min(implicitWidth, parent.width)
    anchors.centerIn: parent
    bookController: root.bookController
    clip: true

    Component.onCompleted: documentView.forceActiveFocus()
  }

  ScrollBar {
    id: verticalScrollbar
    width: pressed ? 14 : 12
    hoverEnabled: true
    active: true
    policy: ScrollBar.AlwaysOn
    orientation: Qt.Vertical
    size: documentView.height / documentView.contentHeight
    minimumSize: 0.04
    position: documentView.contentY / documentView.contentHeight
    onPositionChanged: if (pressed)
                         documentView.contentY = position * documentView.contentHeight
    anchors.top: parent.top
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    horizontalPadding: 4

    contentItem: Rectangle {
      color: Style.colorScrollBarHandle
      opacity: verticalScrollbar.pressed ? 0.8 : 1
      radius: 4
    }

    background: Rectangle {
      implicitWidth: 26
      implicitHeight: 200
      color: verticalScrollbar.pressed
             || verticalScrollbar.hovered ? Style.colorContainerBackground : "transparent"
    }
  }

  ScrollBar {
    id: horizontalScrollbar
    height: hovered ? 12 : 10
    hoverEnabled: true
    active: true
    policy: ScrollBar.AlwaysOn
    visible: documentView.contentWidth > documentView.width
    orientation: Qt.Horizontal
    size: documentView.width / documentView.contentWidth
    minimumSize: 0.04
    position: documentView.contentX / documentView.contentWidth
    onPositionChanged: documentView.contentX = position * documentView.contentWidth
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    horizontalPadding: 4

    contentItem: Rectangle {
      color: Style.colorScrollBarHandle
      opacity: horizontalScrollbar.pressed ? 0.8 : 1
      radius: 4
    }

    background: Rectangle {
      implicitWidth: 26
      implicitHeight: 200
      color: "transparent"
    }
  }

  MSelectionOptionsPopup {
    id: selectionOptionsPopup
    property real highlightCenterX
    property real highlightBottomY

    onNewWidth: internal.openSelectionOptionsPopup(-1, -1)

    onExplanationOptionSelected: text => {
                                   explanationPopup.question = text
                                   explanationPopup.open()
                                 }

    onTranslationOptionSelected: text => {
                                   translationPopup.input = text.replace(
                                     /\r|\n/g, '')
                                   translationPopup.open()
                                 }
  }

  MDictionaryPopup {
    id: dictionaryPopup
  }

  MExplanationPopup {
    id: explanationPopup
    x: root.width / 2 - explanationPopup.width / 2
    y: root.height / 2 - explanationPopup.height / 2
  }

  MTranslationPopup {
    id: translationPopup
    x: root.width / 2 - translationPopup.width / 2
    y: root.height / 2 - translationPopup.height / 2
  }

  function setZoom(newZoom) {
    documentView.currentZoom = newZoom
  }
}

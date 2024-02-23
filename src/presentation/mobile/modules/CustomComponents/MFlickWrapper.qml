import QtQuick


/**
 A component supposed to be wrapped around pages or popups to make
 them scrollable by wrapping a pre-configured Flickable around them.
*/
Flickable {
    id: root
    flickableDirection: Flickable.VerticalFlick
    maximumFlickVelocity: 300
    flickDeceleration: 2500
}

import QtQuick 2.15

/**
 A component supposed to be wrapped around pages or popups to make
 them scrollable by wrapping a pre-configured Flickable around them.
*/
Flickable
{
    id: root
    flickableDirection: Flickable.VerticalFlick
    boundsMovement: Flickable.StopAtBounds
    maximumFlickVelocity: 1000
    flickDeceleration: 5000
}
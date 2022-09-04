import QtQuick 2.15

Flickable
{
    id: root
    flickableDirection: Flickable.VerticalFlick
    boundsMovement: Flickable.StopAtBounds
    maximumFlickVelocity: 1000
    flickDeceleration: 5000
}
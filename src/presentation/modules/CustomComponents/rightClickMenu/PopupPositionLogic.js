function getBookOptionsPopupXCoord(currentXPosition, absoluteXPosition, rootObject)
{
    let distanceToEnd = distToHorizontalEnd(absoluteXPosition, rootObject);
    if(distanceToEnd <= 0)
    {
        // if the popup would go outside of its container, add the nagative
        // distance to make it perfectly fit
        return currentXPosition + distanceToEnd;
    }
    
    return currentXPosition;
}

/**
 Calculates the space from the potential popup position to the end
 of the given container
 */
function distToHorizontalEnd(mouseX, container)
{
    let popupX = mouseX + root.implicitWidth;
    return container.width - popupX;
}



function getBookOptionsPopupYCoord(currentYPosition, absoluteYPosition, rootObject)
{
    let distanceToEnd = distToVerticalEnd(absoluteYPosition, rootObject)
    if(distanceToEnd <= 0)
    {
        // if the popup would go outside of its container, add the nagative
        // distance to make it perfectly fit
        return currentYPosition + distanceToEnd;
    }
    
    return currentYPosition;
}

/**
 Calculates the space from the potential popup position to the end
 of the given container
*/
function distToVerticalEnd(mouseY, container)
{
    let popupY = mouseY + root.implicitHeight;
    return container.height - popupY;
}
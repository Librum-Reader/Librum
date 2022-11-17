function handleWheel(wheel)
{
    // Generate factors between 0.8 and 1.2
    let factor = (((wheel.angleDelta.y / 120)+1) / 5 ) + 0.8;
    
    if (wheel.modifiers & Qt.ControlModifier)
    {
        root.zoom(factor);
    }
    else if(wheel.angleDelta.x === 0)  // No horizontal ScrollBar
    {
        if(factor > 1)
            flick(listView.scrollSpeed);
        else
            flick(-listView.scrollSpeed);
        
        updateCurrentPageCounter();
    }
}

function updateCurrentPageCounter()
{
    // Set current page
    let newPage = Math.round(listView.contentY / listView.currentItem.height);
    if(newPage != root.document.currentPage)
        root.document.currentPage = newPage;
}

/**
 * Changes the current move direction of the listview, without actually
 * moving visibly. This is neccessary since the listview only chaches
 * delegates in the direction of the current move direction.
 * If we e.g. scroll downwards and then go to the previousPage
 * by setting the contentY, the previous pages are not cached
 * which might lead to visible loading while moving through the
 * book with the arrow keys
 */
function setMoveDirection(direction)
{
    if(direction === "up")
    {
        listView.flick(0, -1000);
        listView.cancelFlick();
    }
    else if(direction === "down")
    {
        listView.flick(0, 1000);
        listView.cancelFlick();
    }
}

function zoom(factor)
{
    let newWidth = listView.contentWidth * factor;
    
    // Prevent from too high/low zooms
    if (newWidth < listView.normalWidth / 6 || newWidth > listView.normalWidth * 5)
    {
        return;
    }
    
    let mappedPoint = mapToItem(listView, Qt.point(mouseArea.mouseX, mouseArea.mouseY));
    listView.resizeContent(Math.round(newWidth), Math.round(newWidth / listView.currentItem.pageRatio),
                           mappedPoint);
    listView.returnToBounds();
}

function flick(factor)
{
    listView.flick(0, factor);
    
    updateCurrentPageCounter();
}

function nextPage()
{
    let pageHeight = listView.currentItem.height;
    let currentPageStartY = root.document.currentPage * pageHeight;
    listView.contentY = currentPageStartY + pageHeight;
    
    updateCurrentPageCounter();
    setMoveDirection("up");
}

function previousPage()
{
    let pageHeight = listView.currentItem.height;
    let currentPageStartY = root.document.currentPage * pageHeight;
    listView.contentY = currentPageStartY - pageHeight;
    
    updateCurrentPageCounter();
    setMoveDirection("down");
}
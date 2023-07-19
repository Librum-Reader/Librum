/**
  Handle the wheel usage, scroll or zoom depending on pressed modifiers
  */
function handleWheel(wheel)
{
    let factor = wheel.angleDelta.y > 0 ? 1.13 : 0.88;
    
    if (wheel.modifiers & Qt.ControlModifier)
    {
        zoom(root.document.zoom * factor);
    }
    // angleDelta.x is the "horizontal scroll" mode some mouses support by
    // e.g. pushing the scroll button to the left/right. Make sure not to
    // scroll vertically when a "horizontal scroll" is performed.
    else if(wheel.angleDelta.x === 0)
    {
        if(factor > 1)
            flick(pageView.scrollSpeed);
        else
            flick(-pageView.scrollSpeed);
    }
}

// Calculate the current page and update the document.
function updateCurrentPageCounter()
{
    // A new page starts if it is over the middle of the screen (vertically).
    let pageHeight = pageView.currentItem.height + pageView.pageSpacing;
    let currentPos = pageView.contentY - pageView.originY + pageView.height/2;
    let pageNumber = Math.floor(currentPos / pageHeight);
    
    if(pageNumber !== root.document.currentPage)
        root.document.currentPage = pageNumber;
}


/**
  Changes the current move direction of the listview, without actually
  moving visibly. This is neccessary since the listview only chaches
  delegates in the direction of the current move direction.
  If we e.g. scroll downwards and then go to the previousPage
  by setting the contentY, the previous pages are not cached
  which might lead to visible loading while moving through the
  book with the arrow keys.
  */
function setMoveDirection(direction)
{
    if(direction === "up")
    {
        flick(-1000);
        pageView.cancelFlick();
    }
    else if(direction === "down")
    {
        flick(1000);
        pageView.cancelFlick();
    }
}


function zoom(newZoomFactor)
{
    // Clamp to max / min zoom factors
    newZoomFactor = Math.max(0.15, Math.min(newZoomFactor, 4));
    if (newZoomFactor === root.document.zoom)
        return;
    
    let defaultPageHeight = Math.round(pageView.currentItem.height / root.document.zoom)
    let newPageHeight = Math.round(defaultPageHeight * newZoomFactor) + pageView.getPageSpacing(newZoomFactor);
    let currentPageHeight = pageView.currentItem.height + pageView.getPageSpacing(root.document.zoom);
    let currentPageNumber = root.document.currentPage;
    let currentPos = pageView.contentY - pageView.originY;
    
    let pageOffset = currentPos - (currentPageHeight * currentPageNumber);
    
    root.document.zoom = newZoomFactor;
    pageView.forceLayout();
    pageView.contentY = newPageHeight * currentPageNumber + pageOffset + pageView.originY;
}


function flick(factor)
{
    pageView.flick(0, factor);
}


function setPage(newPageNumber)
{
    if(newPageNumber < 0 || newPageNumber > root.document.pageCount)
        return;
    
    pageView.currentIndex = newPageNumber;
    pageView.positionViewAtIndex(newPageNumber, ListView.Beginning);
    root.document.currentPage = newPageNumber;
    
    if(newPageNumber > root.document.currentPage)
        setMoveDirection("up");
    else if(newPageNumber < root.document.currentPage)
        setMoveDirection("down");
}
/**
  Handle the wheel usage, scroll or zoom depending on pressed modifiers
  */
function handleWheel(wheel)
{
    // Normalize to factors between 0.8 and 1.2
    let factor = (((wheel.angleDelta.y / 120)+1) / 6 ) + 0.8;
    
    if (wheel.modifiers & Qt.ControlModifier)
    {
        zoom(factor);
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
    let pageHeight = pageView.currentItem.height;
    let currentPos = pageView.contentY + pageView.height/2;
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


function zoom(factor)
{
    let newZoomFactor = pageView.zoomFactor * factor;
    let newHeight = Math.round(pageView.defaultHeight * newZoomFactor);
    if (newHeight < pageView.defaultHeight / 5 || newHeight > pageView.defaultHeight * 3)
        return;
    
    let oldPageHeight = pageView.currentItem.height;
    let currentPage = root.document.currentPage;
    let currentPos = pageView.contentY + pageView.height/2;
    
    let oldPageOffsetNumber = currentPos - (oldPageHeight * currentPage);
    
    
    pageView.zoomFactor = newZoomFactor;
    pageView.contentY = newHeight * currentPage + oldPageOffsetNumber * newZoomFactor;
    pageView.forceLayout();
}


function flick(factor)
{
    pageView.flick(0, factor);
}


function setPage(newPageNumber)
{
    let pageHeight = pageView.currentItem != null ? pageView.currentItem.height : 0
    let newPageY = pageHeight * newPageNumber;
    
    if(newPageNumber > root.document.currentPage)
        setMoveDirection("up");
    else if(newPageNumber < root.document.currentPage)
        setMoveDirection("down");
    
    pageView.contentY = newPageY;
}
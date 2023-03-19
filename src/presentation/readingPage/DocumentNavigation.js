/**
  Handle the wheel usage, scroll or zoom depending on pressed modifiers
  */
function handleWheel(wheel)
{
    // Normalize to factors between 0.8 and 1.2
    let factor = (((wheel.angleDelta.y / 120)+1) / 5 ) + 0.8;
    
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
            flick(tableView.scrollSpeed);
        else
            flick(-tableView.scrollSpeed);
    }
}

// Calculate the current page and update the document.
function updateCurrentPageCounter()
{
    // A new page starts if it is over the middle of the screen (vertically).   
    let pageHeight = getPage(root.document.currentPage).height + tableView.rowSpacing;
    let currentPos = tableView.contentY + tableView.height/2;
    let pageNumber = Math.floor(currentPos / pageHeight);
    
    
    if(pageNumber !== root.document.currentPage)
        root.document.currentPage = pageNumber;
}


function zoom(factor)
{
    let newWidth = tableView.contentWidth * factor;
    if (newWidth < tableView.defaultWidth / 6 || newWidth > tableView.defaultWidth * 5)
        return;
    
//    tableView.contentWidth *= factor;
}


function flick(factor)
{
    tableView.flick(0, factor);
}

function setPage(newPageNumber)
{
    let newPageHeight = getPage(root.document.currentPage).height + tableView.rowSpacing;
    let newPageY = newPageHeight * newPageNumber;
    
    tableView.contentY = newPageY;
}

function getPage(pageNumber)
{
    return tableHelper.itemAtCell(0, pageNumber);
}


/**
  Handle the wheel usage, scroll or zoom depending on pressed modifiers
  */
function handleWheel(wheel) {
    let factorX = wheel.angleDelta.x > 0 ? 1.13 : 0.88
    let factorY = wheel.angleDelta.y > 0 ? 1.13 : 0.88

    if (wheel.modifiers & Qt.ControlModifier) {
        zoom(root.bookController.zoom * factorY)
    } // angleDelta.x is the "horizontal scroll" mode some mouses support by
    // e.g. pushing the scroll button to the left/right.
    else if (wheel.angleDelta.x !== 0) {
        if (factorX > 1)
            flick(pageView.scrollSpeed / 3, 0)
        else
            flick(-pageView.scrollSpeed / 3, 0)
    } else {
        if (factorY > 1)
            flick(0, pageView.scrollSpeed)
        else
            flick(0, -pageView.scrollSpeed)
    }
}

// Calculate the current page and update the document.
function updateCurrentPageCounter() {
    // A new page starts if it is over the middle of the screen (vertically).
    let middleOfScreen = pageView.contentY + pageView.height / 2
    let pageNumber = pageView.indexAt(pageView.currentItem.width / 2,
                                      middleOfScreen)

    // If the middle of the screen is in the free space between two pages, take the lower one
    if (pageNumber === -1) {
        let pageSpacing = pageView.getPageSpacing(root.bookController.zoom)
        pageNumber = pageView.indexAt(pageView.currentItem.width / 2,
                                      middleOfScreen + pageSpacing)
    }

    if (pageNumber !== root.bookController.currentPage)
        root.bookController.currentPage = pageNumber
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
function setMoveDirection(direction) {
    if (direction === "up") {
        flick(0, -1000)
        pageView.cancelFlick()
    } else if (direction === "down") {
        flick(0, 1000)
        pageView.cancelFlick()
    }
}

function zoom(newZoomFactor) {
    // Clamp to max / min zoom factors
    newZoomFactor = Math.max(0.15, Math.min(newZoomFactor, 10))
    if (newZoomFactor === root.bookController.zoom)
        return

    let defaultPageHeight = Math.round(
            pageView.currentItem.height / root.bookController.zoom)
    let newPageHeight = Math.round(
            defaultPageHeight * newZoomFactor) + pageView.getPageSpacing(
            newZoomFactor)

    let pageOffset = getYOffset()

    root.bookController.zoom = newZoomFactor
    pageView.forceLayout()
    pageView.contentY = newPageHeight * root.bookController.currentPage
            + pageOffset + pageView.originY
}

function flick(x, y) {
    pageView.flick(x, y)
}

function setPage(newPageNumber) {
    if (newPageNumber < 0 || newPageNumber > root.bookController.pageCount)
        return

    pageView.currentIndex = newPageNumber
    pageView.positionViewAtIndex(newPageNumber, ListView.Beginning)
    root.bookController.currentPage = newPageNumber

    if (newPageNumber > root.bookController.currentPage)
        setMoveDirection("up")
    else if (newPageNumber < root.bookController.currentPage)
        setMoveDirection("down")
}

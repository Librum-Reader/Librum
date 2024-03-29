
/**
  If multi select is enabled, multiple items can be selected, thus a list must
  be managed, which knows of all selected items to prevent selecting the same
  item twice and create a result string from all selected items.
  */
function addItemToSelectedItems(index) {
    let newText = listView.itemAtIndex(index).getContent()
    if (internal.selectedItems.length > 0 && internal.selectedItems.includes(
                newText)) {
        // Remove item from array if it already exists
        internal.selectedItems.splice(internal.selectedItems.indexOf(newText),
                                      1)
    } else {
        internal.selectedItems.push(newText)
    }

    // Create one result string from all selected items
    root.selectedContent = internal.selectedItems.join(', ')
}

function selectItem(index) {

    // Set the listView's current item to the new item
    listView.currentIndex = index
    let newItem = listView.currentItem

    if (root.multiSelect) {
        newItem.selected = !newItem.selected
        return
    }

    if (newItem === listView.currentSelected) {
        if (root.allowUnselectingItems)
            deselectCurrentItem()
        return
    }

    // Deselect previous selected item if it exists
    if (listView.currentSelected != null)
        listView.currentSelected.selected = false

    newItem.selected = true
    listView.currentSelected = newItem
    root.selectedContent = listView.currentSelected.getContent()
}

function deselectCurrentItem() {
    if (listView.currentSelected == null)
        return

    listView.currentSelected.selected = false
    listView.currentSelected = null
    root.selectedContent = ""
}

function addItemToResult(index)
{
    let newText = listView.itemAtIndex(index).text;
    
    if(listView.selectedItemsStore.includes(newText))
        listView.selectedItemsStore = removeFromArray(listView.selectedItemsStore, newText);
    else
    {
        listView.selectedItemsStore.push(newText);
    }
    
    root.selectedContents = formatArray(listView.selectedItemsStore);
}

function formatArray(arrayToComposeFrom)
{
    let temp = "";
    for(let i = 0; i < arrayToComposeFrom.length; i++)
    {
        temp += arrayToComposeFrom[i];
        
        if(i < arrayToComposeFrom.length - 1)
            temp += ", "
    }
    
    return temp;
}



function removeFromArray(arr, value)
{ 
    return arr.filter(function(ele)
    { 
        return ele !== value; 
    });
}



function changeSelectionMarker(index)
{
    listView.currentIndex = index;
    
    if(root.multiSelect)
    {
        listView.currentItem.selected = !listView.currentItem.selected;
        return;
    }
    
    
    if(listView.currentItem === listView.currentSelected)
    {
        listView.currentSelected.selected = false;
        root.selectedContents = "";
        return;
    }
    
    if(listView.currentSelected != null)
        listView.currentSelected.selected = false;
    
    listView.itemAtIndex(index).selected = true;
    listView.currentSelected = listView.itemAtIndex(index);
    root.selectedContents = listView.currentSelected.text;
}
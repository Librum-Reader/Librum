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
        listView.currentSelected.selected = !listView.currentSelected.selected;
        root.selectedContents = (listView.currentSelected.selected === true ? listView.currentSelected.text : "");
        return;
    }
    
    if(listView.currentSelected != null)
        listView.currentSelected.selected = false;
    
    
    let newItem = listView.itemAtIndex(index);
    newItem.selected = true;
    listView.currentSelected = newItem;
    root.selectedContents = listView.currentSelected.text;
}
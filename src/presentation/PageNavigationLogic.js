function checkIfNewPageIsTheSameAsOld(sidebarItem)
{
    return sidebar.currentItem === sidebarItem;
}

function terminateActionOfCurrentPage(page, sidebarItem)
{
    let currentPage = pageManager.currentItem;
    if(currentPage instanceof MSettings)
    {
        if(!currentPage.saveSettingsPage(switchPage, page, sidebarItem))
            return false;
    }
    
    return true;
}

function switchPage(page, sidebarItem)
{
    pageManager.replace(page);
    
    // Set sidebar
    pageManager.pageHasSidebar = sidebarItem !== undefined;
    if(pageManager.pageHasSidebar)
        sidebar.changeSelectedItem(sidebarItem);
    
    // The reading page can have a lower minimum width than the other pages
    if(page === readingPage)
        baseRoot.minimumWidth = 550;
    else
        baseRoot.minimumWidth = 650;
    
}
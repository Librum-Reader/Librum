import QtQuick 2.15

/*
  MPageCleanup is a component which allows to save pages via. a unified interface.
  
  @var "action" - The action performed to save the current page. It only returns true if there is something to save.
  @var "savingPageFinishedSignal" - The signal emitted when a page was successfully saved
  @var "callbackFunction" - The function to continue executing after the page was saved
  
  Example:
  Page A is a setting page with unsaved data, the "savePageAction" is a function which
  opens a "Save settings" dialog. The "savingPageFinishedSignal" is the "ClickedSave"
  signal of that dialog and the "callbackFunction" is a "switchPage" function.
  |
  This way the application makes sure to save the current page before switching to another one.
  */
Item
{
    id: root
    property var savePageAction
    property var savingPageFinishedSignal
    property var callbackFunction
    
    function cleanUp()
    {
        if(savePageAction())
        {
            savingPageFinishedSignal.connect(callbackFunction);
            return false;
        }
        
        return true;
    }
}
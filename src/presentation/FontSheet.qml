pragma Singleton

import QtQuick
import Librum.controllers


/**
 We make all font sizes dependent on the system font size, so that the font sizes look fine
 on all operating systems. E.g. point size 11 looks fine on Linux but too small on MacOS.
*/
Item {
    id: fontSheet

    property int hugeSize: AppInfoController.systemFontSize + 4.5
    property int largeSize: AppInfoController.systemFontSize + 4
    property int bigSize: AppInfoController.systemFontSize + 3
    property int baseSize: AppInfoController.systemFontSize + 2
    property int mediumSize: AppInfoController.systemFontSize + 1.5
    property int smallSize: AppInfoController.systemFontSize + 1
    property int tinySize: AppInfoController.systemFontSize

    property int smallTitleSize: AppInfoController.systemFontSize + 5
    property int modestTitleSize: AppInfoController.systemFontSize + 6
    property int baseTitleSize: AppInfoController.systemFontSize + 7
    property int mediumTitleSize: AppInfoController.systemFontSize + 8
    property int bigTitleSize: AppInfoController.systemFontSize + 10
    property int veryBigTitleSize: AppInfoController.systemFontSize + 11
    property int largeTitleSize: AppInfoController.systemFontSize + 13
    property int hugeTitleSize: AppInfoController.systemFontSize + 14
    property int enormousTitleSize: AppInfoController.systemFontSize + 17

    property int baseHeaderSize: AppInfoController.systemFontSize + 33
    property int bigHeaderSize: AppInfoController.systemFontSize + 37
}

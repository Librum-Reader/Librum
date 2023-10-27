pragma Singleton

import QtQuick
import Librum.controllers


/**
 We make all font sizes dependent on the system font size, so that the font sizes look fine
 on all operating systems. E.g. point size 11 looks fine on Linux but too small on MacOS.
*/
Item {
    id: fontSheet

    //    property real largeSize: AppInfoController.systemFontSize + 4
    //    property real bigSize: AppInfoController.systemFontSize + 3
    //    property real baseSize: AppInfoController.systemFontSize + 2
    //    property real mediumSize: AppInfoController.systemFontSize + 1.5
    //    property real smallSize: AppInfoController.systemFontSize + 1
    //    property real tinySize: AppInfoController.systemFontSize

    //    property real smallTitleSize: AppInfoController.systemFontSize + 5
    //    property real modestTitleSize: AppInfoController.systemFontSize + 6
    //    property real baseTitleSize: AppInfoController.systemFontSize + 7
    //    property real mediumTitleSize: AppInfoController.systemFontSize + 8
    //    property real bigTitleSize: AppInfoController.systemFontSize + 10
    //    property real veryBigTitleSize: AppInfoController.systemFontSize + 11
    //    property real largeTitleSize: AppInfoController.systemFontSize + 13
    //    property real hugeTitleSize: AppInfoController.systemFontSize + 14
    //    property real enormousTitleSize: AppInfoController.systemFontSize + 17

    //    property real baseHeaderSize: AppInfoController.systemFontSize + 33
    //    property real bigHeaderSize: AppInfoController.systemFontSize + 37
    property real size13: AppInfoController.systemFontSize + 4
    property real size12: AppInfoController.systemFontSize + 3
    property real size11: AppInfoController.systemFontSize + 2
    property real size10dot5: AppInfoController.systemFontSize + 1.5
    property real size10: AppInfoController.systemFontSize + 1
    property real size9: AppInfoController.systemFontSize

    property real size14: AppInfoController.systemFontSize + 5
    property real size15: AppInfoController.systemFontSize + 6
    property real size16: AppInfoController.systemFontSize + 7
    property real size17: AppInfoController.systemFontSize + 8
    property real size19: AppInfoController.systemFontSize + 10
    property real sizeSize20: AppInfoController.systemFontSize + 11
    property real size22: AppInfoController.systemFontSize + 13
    property real size23: AppInfoController.systemFontSize + 14
    property real size26: AppInfoController.systemFontSize + 17

    property real size42: AppInfoController.systemFontSize + 33
    property real size46: AppInfoController.systemFontSize + 37
}

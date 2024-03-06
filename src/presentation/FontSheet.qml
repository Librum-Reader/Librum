pragma Singleton

import QtQuick
import Librum.controllers


/**
 We make all font sizes dependent on the system font size, so that the font sizes look fine
 on all operating systems. E.g. point size 11 looks fine on Linux but too small on MacOS.

 The system font size on linux on windows is 9, thus the following names are based on 9.
*/
Item {
    id: fontSheet

    property real systemSize: AppInfoController.systemFontSize

    property real size9: systemSize
    property real size9dot5: systemSize + 0.5
    property real size10: systemSize + 1
    property real size10dot25: systemSize + 1.25
    property real size10dot5: systemSize + 1.5
    property real size10dot75: systemSize + 1.75
    property real size11: systemSize + 2
    property real size11dot5: systemSize + 2.5
    property real size11dot75: systemSize + 2.75
    property real size12: systemSize + 3
    property real size12dot25: systemSize + 3.25
    property real size12dot5: systemSize + 3.5
    property real size13: systemSize + 4
    property real size13dot25: systemSize + 4.25
    property real size13dot5: systemSize + 4.5
    property real size14: systemSize + 5
    property real size15: systemSize + 6
    property real size16: systemSize + 7
    property real size16dot5: systemSize + 7.5
    property real size17: systemSize + 8
    property real size19: systemSize + 10
    property real size20: systemSize + 11
    property real size21: systemSize + 12
    property real size22: systemSize + 13
    property real size23: systemSize + 14
    property real size25: systemSize + 16
    property real size26: systemSize + 17
    property real size28: systemSize + 19
    property real size42: systemSize + 33
    property real size46: systemSize + 37
}

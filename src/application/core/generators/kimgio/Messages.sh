#!/bin/sh
$EXTRACTRC *.rc >> rc.cpp
$XGETTEXT $(find . -name "*.cpp" -o -name "*.h") -o $podir/okular_kimgio.pot

#!/bin/sh
QT_VERSION=5.15.0
export QT_VERSION
QT_VER=5.15
export QT_VER
QT_VERSION_TAG=5150
export QT_VERSION_TAG
QT_INSTALL_DOCS=/usr/share/doc/qt
export QT_INSTALL_DOCS
BUILDDIR=/home/creapermann/Programming/Etovex/Testing/TreeView/libs/qttreeview/src
export BUILDDIR
exec /usr/bin/qdoc "$@"

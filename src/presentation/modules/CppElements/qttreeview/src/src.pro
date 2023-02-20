TEMPLATE = lib

QT += gui qml quick
QT += quick-private

CONFIG += qt plugin
CONFIG += no_cxx_module install_qml_files builtin_resources qtquickcompiler
CONFIG += qmltypes install_qmltypes

TARGET = qquicktreeviewplugin
TARGETPATH = QtQuick/TreeView

QML_IMPORT_NAME = QtQuick.TreeView

QML_IMPORT_MAJOR_VERSION = 2
QML_FILES += qt5/TreeView.qml
DYNAMIC_QMLDIR = \
"module QtQuick.TreeView" \
"plugin qquicktreeviewplugin" \
"classname QQuickTreeViewPlugin" \
"TreeView 2.15 qt5/TreeView.qml" \
"depends QtQuick.Shapes 2.15" \
"depends Qt.labs.qmlmodels 1.0"

SOURCES += \
    qquicktreeviewplugin.cpp \
    qquicktreeview.cpp \
    qquicktreemodeladaptor.cpp

HEADERS += \
    qquicktreeview_p.h \
    qquicktreeview_p_p.h \
    qquicktreemodeladaptor_p.h \

OTHER_FILES += \
    $$QML_FILES \
    ../README.md \
    ../Marketplace.md \
    ../Marketplace.json

load(qml_plugin)

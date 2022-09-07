cmake_minimum_required(VERSION 3.16)
project(okular VERSION "22.11.70")


# Language
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Qt Settings
set(CMAKE_AUTOUIC OFF)




# Setup
include(cmake/SetOptionalPackageProperties.cmake)

option(FORCE_NOT_REQUIRED_DEPENDENCIES "List (semicolon-separated) of dependencies that will be downgraded from REQUIRED to RECOMMENDED")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

if (ANDROID AND (NOT FORCE_NOT_REQUIRED_DEPENDENCIES) AND (NOT FORCE_NOT_REQUIRED_DEPENDENCIES STREQUAL ""))
    set(FORCE_NOT_REQUIRED_DEPENDENCIES "KF5Wallet;KF5JS;TIFF;JPEG;LibSpectre;KF5KExiv2;CHM;KF5KHtml;LibZip;DjVuLibre;EPub;Discount;")
endif()


# append the plugins from the install tree folder to the QT_PLUGIN_PATH
if(UNIX)
    set(ENV{QT_PLUGIN_PATH} "$ENV{QT_PLUGIN_PATH}:${KDE_INSTALL_FULL_QTPLUGINDIR}")
elseif(WIN32)
    set(ENV{QT_PLUGIN_PATH} "$ENV{QT_PLUGIN_PATH};${KDE_INSTALL_FULL_QTPLUGINDIR}")
endif()

if(NOT WIN32)
 set(MATH_LIB m)
else(NOT WIN32)
 set(MATH_LIB)
endif(NOT WIN32)





# KF5 Dependencies
find_package(KF5 ${KF5_REQUIRED_VERSION} REQUIRED COMPONENTS
    Parts
    Crash
    IconThemes
    TextWidgets
    Archive
    Bookmarks
    Completion
    Config
    ConfigWidgets
    CoreAddons
    I18n
    KIO
    ThreadWeaver
    WindowSystem
)

find_package(KF5Wallet ${KF5_REQUIRED_VERSION})
set_okular_optional_package_properties(KF5Wallet PROPERTIES
    PURPOSE "Required for document storing passwords in secure wallets.")
if(KF5Wallet_FOUND)
    add_definitions(-DWITH_KWALLET=1)
endif()

find_package(KF5JS ${KF5_REQUIRED_VERSION})
set_okular_optional_package_properties(KF5JS PROPERTIES
    PURPOSE "Required for supporting JavaScript in PDF documents")
if(KF5JS_FOUND)
    add_definitions(-DWITH_KJS=1)
endif()

find_package(KF5Purpose)
set_okular_optional_package_properties(KF5Purpose PROPERTIES
    DESCRIPTION "A framework for services and actions integration"
    PURPOSE "Required for enabling the share menu.")
if (KF5Purpose_FOUND)
    set(PURPOSE_FOUND 1)
else()
    set(PURPOSE_FOUND 0)
endif()

find_package(KF5Kirigami2)
set_package_properties(KF5Kirigami2 PROPERTIES
    DESCRIPTION "A QtQuick based components set"
    PURPOSE "Required at runtime by the mobile app"
    TYPE RUNTIME
)
if(KF5Kirigami2_VERSION VERSION_LESS "5.85")
    message(FATAL_ERROR "Disable mobile build. This requires Kirigami 5.85 or greater.")
    set(BUILD_MOBILE OFF)
endif()


if(NOT WIN32 AND NOT ANDROID AND NOT APPLE)
    find_package(KF5 ${KF5_REQUIRED_VERSION} REQUIRED COMPONENTS
        Activities
    )
    set_package_properties("KF5Activities" PROPERTIES
        DESCRIPTION  "Activities interface library"
        URL "https://api.kde.org/frameworks/kactivities/html/"
        PURPOSE "Required for Activities integration.")
endif()






# Qt Dependencies
find_package(Qt5 CONFIG REQUIRED COMPONENTS Core Test Widgets PrintSupport Svg Qml Quick DBus)
if(ANDROID)
    find_package(Qt5 CONFIG REQUIRED COMPONENTS AndroidExtras)
endif()

find_package(Qt5TextToSpeech CONFIG)
set_okular_optional_package_properties(Qt5TextToSpeech PROPERTIES
    PURPOSE "Enables speech features.")
if (Qt5TextToSpeech_FOUND)
    add_definitions(-DHAVE_SPEECH)
endif()

find_package(Phonon4Qt5 CONFIG REQUIRED)






# MIME-Type dependencies
find_package(ZLIB REQUIRED)

find_package(Poppler "0.86.0" COMPONENTS Qt5)
set_okular_optional_package_properties(Poppler PROPERTIES
        PURPOSE "Support for PDF files in okular.")

    
find_package(Freetype)
set_okular_optional_package_properties(Freetype PROPERTIES
        DESCRIPTION  "A font rendering engine"
        URL "https://www.freetype.org"
        PURPOSE "Provides freetype font support in the okular DVI generator.")

    
find_package(TIFF)
set_okular_optional_package_properties(TIFF PROPERTIES
        DESCRIPTION  "A library for reading and writing TIFF formatted files,"
        URL "http://www.libtiff.org"
        PURPOSE "Support for TIFF files in okular.")

    
find_package(JPEG)
set_okular_optional_package_properties(JPEG PROPERTIES
        DESCRIPTION  "A library for reading and writing JPEG image files."
        URL "https://www.ijg.org"
        PURPOSE "Support for PalmDB documents in okular.")

    
set(LIBSPECTRE_MINIMUM_VERSION "0.2")
find_package(LibSpectre "${LIBSPECTRE_MINIMUM_VERSION}")
set_okular_optional_package_properties(LibSpectre PROPERTIES
        DESCRIPTION  "A PostScript rendering library"
        URL "https://libspectre.freedesktop.org"
        PURPOSE "Support for PS files in okular.")

    
find_package(KF5KExiv2 CONFIG)
set_okular_optional_package_properties(KF5KExiv2 PROPERTIES
        DESCRIPTION  "Wrapper around Exiv2 library"
        URL "https://commits.kde.org/libkexiv2"
        PURPOSE "Support for exif rotation in image files.")

    
find_package(CHM)
set_okular_optional_package_properties(CHM PROPERTIES
        DESCRIPTION  "A library for dealing with Microsoft ITSS/CHM format files"
        URL "http://www.jedrea.com/chmlib"
        PURPOSE "Support CHM files in okular.")

    
find_package(KF5KHtml CONFIG)
set_okular_optional_package_properties(KF5KHtml PROPERTIES
        DESCRIPTION "HTML rendering library"
        PURPOSE "Support CHM files in okular.")

    
find_package(LibZip)
set_okular_optional_package_properties(LibZip PROPERTIES
        DESCRIPTION  "A library for reading, creating, and modifying zip archives"
        URL "https://libzip.org/"
        PURPOSE "Support CHM files in okular.")

    
find_package(DjVuLibre "3.5.17")
set_okular_optional_package_properties(DjVuLibre PROPERTIES
        DESCRIPTION  "A library for dealing with DjVu formatted files"
        URL "https://djvulibre.djvuzone.org"
        PURPOSE "Support for DjVu files in okular.")

    
find_package(EPub)
set_okular_optional_package_properties(EPub PROPERTIES
        DESCRIPTION  "A library for reading EPub documents"
        URL "http://sourceforge.net/projects/ebook-tools"
        PURPOSE "Support for EPub documents in Okular.")

    
find_package(QMobipocket "2" CONFIG)
set_okular_optional_package_properties(QMobipocket PROPERTIES
        DESCRIPTION  "A library for reading Mobipocket documents"
        URL "https://commits.kde.org/kdegraphics-mobipocket"
        PURPOSE "Support for Mobipocket documents in Okular.")

    
find_package(Discount)
set_okular_optional_package_properties(Discount PROPERTIES
        DESCRIPTION "A library that gives you formatting functions suitable for marking down entire documents or lines of text"
        URL "https://www.pell.portland.or.us/~orc/Code/discount/"
        PURPOSE "Support for Markdown documents in Okular.")





# Qt definitions
add_definitions(-DQT_USE_QSTRINGBUILDER)
add_definitions(-DTRANSLATION_DOMAIN="okular")
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)
add_definitions(-DQT_NO_CAST_FROM_ASCII)
add_definitions(-DQT_NO_CAST_FROM_BYTEARRAY)
add_definitions(-DQT_NO_CAST_TO_ASCII)
add_definitions(-DQT_DEPRECATED_WARNINGS_SINCE=0x050C00)
add_definitions(-DKF_DEPRECATED_WARNINGS_SINCE=0x054400)
add_definitions(-DQT_NO_KEYWORDS)




# ECM Setup
include(ECMInstallIcons)
include(ECMSetupVersion)
include(ECMOptionalAddSubdirectory)
include(GenerateExportHeader)
include(FeatureSummary)
include(ECMAddAppIcon)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMAddTests)
include(ECMAddAppIcon)
include(CMakePackageConfigHelpers)
include(ECMSetupQtPluginMacroNames)


ecm_setup_version(${PROJECT_VERSION}
                  VARIABLE_PREFIX OKULAR
                  VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/core/version.h"
                  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/Okular5ConfigVersion.cmake")


ecm_setup_qtplugin_macro_names(
    JSON_ARG2
        "OKULAR_EXPORT_PLUGIN"
    CONFIG_CODE_VARIABLE
        PACKAGE_SETUP_AUTOMOC_VARIABLES
)



include_directories(
    /usr/include/KF5/KBookmarks
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${PHONON_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/core/synctex
    ${CMAKE_CURRENT_BINARY_DIR}/core/
)



add_subdirectory( generators )


include(OkularConfigureChecks.cmake)




# Build the core

set(okularcore_SRCS
   core/action.cpp
   core/annotations.cpp
   core/area.cpp
   core/audioplayer.cpp
   core/bookmarkmanager.cpp
   core/chooseenginedialog.cpp
   core/document.cpp
   core/documentcommands.cpp
   core/fontinfo.cpp
   core/form.cpp
   core/generator.cpp
   core/generator_p.cpp
   core/misc.cpp
   core/movie.cpp
   core/observer.cpp
   core/debug.cpp
   core/page.cpp
   core/pagecontroller.cpp
   core/pagesize.cpp
   core/pagetransition.cpp
   core/rotationjob.cpp
   core/scripter.cpp
   core/sound.cpp
   core/sourcereference.cpp
   core/textdocumentgenerator.cpp
   core/textdocumentsettings.cpp
   core/textpage.cpp
   core/tilesmanager.cpp
   core/utils.cpp
   core/view.cpp
   core/fileprinter.cpp
   core/printoptionswidget.cpp
   core/signatureutils.cpp
   core/script/event.cpp
   core/synctex/synctex_parser.c
   core/synctex/synctex_parser_utils.c
)
qt5_add_resources(okularcore_SRCS
   core/script/builtin.qrc
)

ki18n_wrap_ui(okularcore_SRCS
   core/textdocumentsettings.ui
)

install( FILES
           core/action.h
           core/annotations.h
           core/area.h
           core/document.h
           core/fontinfo.h
           core/form.h
           core/generator.h
           core/global.h
           core/page.h
           core/pagesize.h
           core/pagetransition.h
           core/signatureutils.h
           core/sound.h
           core/sourcereference.h
           core/textdocumentgenerator.h
           core/textdocumentsettings.h
           core/textpage.h
           core/tile.h
           core/utils.h
           core/fileprinter.h
           core/printoptionswidget.h
           core/observer.h
           ${CMAKE_CURRENT_BINARY_DIR}/core/version.h
           ${CMAKE_CURRENT_BINARY_DIR}/core/okularcore_export.h
           ${CMAKE_CURRENT_BINARY_DIR}/settings_core.h
         DESTINATION ${KDE_INSTALL_INCLUDEDIR}/okular/core COMPONENT Devel)

install( FILES
           interfaces/configinterface.h
           interfaces/guiinterface.h
           interfaces/printinterface.h
           interfaces/saveinterface.h
           interfaces/viewerinterface.h
         DESTINATION ${KDE_INSTALL_INCLUDEDIR}/okular/interfaces COMPONENT Devel)

ki18n_wrap_ui(okularcore_SRCS
   core/chooseenginewidget.ui
)

kconfig_add_kcfg_files(okularcore_SRCS GENERATE_MOC conf/settings_core.kcfgc)


add_library(okularcore SHARED ${okularcore_SRCS})
generate_export_header(okularcore BASE_NAME okularcore EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/core/okularcore_export.h")




if (ANDROID)
    set(fileName ${CMAKE_BINARY_DIR}/Okular5Core-android-dependencies.xml)
    file(WRITE "${fileName}" "<rules><dependencies><lib name='Okular5Core'><depends>\n"
                                "<bundled file='${KDE_INSTALL_PLUGINDIR}/okular/generators' />\n"
                             "</depends></lib></dependencies></rules>\n")
    install(FILES ${fileName} DESTINATION ${KDE_INSTALL_LIBDIR})
endif()


# Special handling for linking okularcore on OSX/Apple
IF(APPLE)
    SET(OKULAR_IOKIT "-framework IOKit" CACHE STRING "Apple IOKit framework")
ENDIF(APPLE)

# Extra library needed by imported synctex code on Windows
if(WIN32)
    set(SHLWAPI shlwapi)
endif(WIN32)

target_link_libraries(okularcore
PRIVATE
    ${OKULAR_IOKIT}
    ${SHLWAPI}
    KF5::Archive
    KF5::KIOCore
    KF5::KIOWidgets
    KF5::I18n
    KF5::ThreadWeaver
    KF5::Bookmarks
    Phonon::phonon4qt5
    Qt5::Svg
    ${MATH_LIB}
    ZLIB::ZLIB
PUBLIC  # these are included from the installed headers
    KF5::CoreAddons
    KF5::XmlGui
    KF5::ConfigGui
    Qt5::PrintSupport
    Qt5::Widgets
)


if (KF5Wallet_FOUND)
    target_link_libraries(okularcore PRIVATE KF5::Wallet)
endif()
if (KF5JS_FOUND)
    target_sources(okularcore PRIVATE
            core/script/executor_kjs.cpp
            core/script/kjs_app.cpp
            core/script/kjs_console.cpp
            core/script/kjs_data.cpp
            core/script/kjs_display.cpp
            core/script/kjs_document.cpp
            core/script/kjs_field.cpp
            core/script/kjs_fullscreen.cpp
            core/script/kjs_field.cpp
            core/script/kjs_spell.cpp
            core/script/kjs_util.cpp
            core/script/kjs_event.cpp
            core/script/kjs_ocg.cpp
    )
    target_link_libraries(okularcore PRIVATE KF5::JS KF5::JSApi)
endif()

set_target_properties(okularcore PROPERTIES VERSION 10.0.0 SOVERSION 10 OUTPUT_NAME Okular5Core EXPORT_NAME Core)

install(TARGETS okularcore EXPORT Okular5Targets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES conf/okular.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})
install(FILES conf/okular_core.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})
install(FILES core/okularGenerator.desktop DESTINATION ${KDE_INSTALL_KSERVICETYPES5DIR})







########### install files ###############

install(FILES okular.upd DESTINATION ${KDE_INSTALL_KCONFUPDATEDIR})


if (${ECM_VERSION} STRGREATER "5.58.0")
    install(FILES okular.categories  DESTINATION  ${KDE_INSTALL_LOGGINGCATEGORIESDIR})
else()
    install(FILES okular.categories  DESTINATION ${KDE_INSTALL_CONFDIR})
endif()

ki18n_install(po)




########### cmake files #################

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/Okular5")
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/Okular5Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/Okular5Config.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
  PATH_VARS  INCLUDE_INSTALL_DIR CMAKE_INSTALL_PREFIX
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/Okular5Config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/Okular5ConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT Okular5Targets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE Okular5Targets.cmake NAMESPACE Okular::)

install(FILES
   core/stamps.svg
   DESTINATION ${KDE_INSTALL_DATADIR}/okular/pics)
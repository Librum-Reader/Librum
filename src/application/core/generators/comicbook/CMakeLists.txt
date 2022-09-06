remove_definitions(-DTRANSLATION_DOMAIN="okular")
add_definitions(-DTRANSLATION_DOMAIN="okular_comicbook")

########### next target ###############

set( okularGenerator_comicbook_PART_SRCS
     document.cpp
     directory.cpp
     unrar.cpp qnatsort.cpp
     unrarflavours.cpp
   )

add_library(okular_comicbook STATIC ${okularGenerator_comicbook_PART_SRCS})
target_link_libraries(okular_comicbook okularcore KF5::I18n KF5::Archive)

okular_add_generator(okularGenerator_comicbook generator_comicbook.cpp)
target_link_libraries(okularGenerator_comicbook okular_comicbook)
if (UNIX AND NOT ANDROID)
   find_package(KF5Pty REQUIRED)
   target_compile_definitions(okular_comicbook PRIVATE -DWITH_KPTY=1)
   target_link_libraries(okular_comicbook KF5::Pty)
endif ()

if (KArchive_HAVE_LZMA)
    target_compile_definitions(okular_comicbook PRIVATE -DWITH_K7ZIP=1)
endif()

########### autotests ###############

add_definitions( -DKDESRCDIR="${CMAKE_CURRENT_SOURCE_DIR}/" )
ecm_add_test(autotests/comicbooktest.cpp
    TEST_NAME "comicbooktest"
    LINK_LIBRARIES Qt5::Test KF5::CoreAddons okularcore okular_comicbook
)

########### install files ###############
install( FILES okularComicbook.desktop  DESTINATION  ${KDE_INSTALL_KSERVICES5DIR} )
install( PROGRAMS okularApplication_comicbook.desktop org.kde.mobile.okular_comicbook.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES org.kde.okular-comicbook.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR} )

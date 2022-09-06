remove_definitions(-DTRANSLATION_DOMAIN="okular")
add_definitions(-DTRANSLATION_DOMAIN="okular_chm")

add_subdirectory( kio-msits )

########### next target ###############

set(okularGenerator_chmlib_SRCS
   lib/ebook_chm.cpp
   lib/ebook_epub.cpp
   lib/ebook.cpp
   lib/ebook_chm_encoding.cpp
   lib/ebook_search.cpp
   lib/helper_entitydecoder.cpp
   lib/helper_search_index.cpp
   lib/helperxmlhandler_epubcontainer.cpp
   lib/helperxmlhandler_epubcontent.cpp
   lib/helperxmlhandler_epubtoc.cpp
   generator_chm.cpp
)

okular_add_generator(okularGenerator_chmlib ${okularGenerator_chmlib_SRCS})
target_include_directories(okularGenerator_chmlib PRIVATE ${CHM_INCLUDE_DIR} ${LIBZIP_INCLUDE_DIR})
target_link_libraries(okularGenerator_chmlib  okularcore ${CHM_LIBRARY} ${LIBZIP_LIBRARY} KF5::KHtml)

########### autotests ###############

add_definitions( -DKDESRCDIR="${CMAKE_CURRENT_SOURCE_DIR}/" )
ecm_add_test(autotests/chmgeneratortest.cpp
    TEST_NAME "chmgeneratortest"
    LINK_LIBRARIES Qt5::Test KF5::CoreAddons okularcore
)

target_compile_definitions(chmgeneratortest PRIVATE -DGENERATOR_PATH="$<TARGET_FILE:okularGenerator_chmlib>")

########### install files ###############
install( FILES okularChm.desktop  DESTINATION  ${KDE_INSTALL_KSERVICES5DIR} )
install( PROGRAMS okularApplication_chm.desktop org.kde.mobile.okular_chm.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES org.kde.okular-chm.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR} )

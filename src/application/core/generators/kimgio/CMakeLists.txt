remove_definitions(-DTRANSLATION_DOMAIN="okular")
add_definitions(-DTRANSLATION_DOMAIN="okular_kimgio")

########### next target ###############

okular_add_generator(okularGenerator_kimgio generator_kimgio.cpp)
target_link_libraries(okularGenerator_kimgio okularcore KF5::I18n)

if(KF5KExiv2_FOUND)
    target_compile_definitions(okularGenerator_kimgio PRIVATE -DWITH_KEXIV=1)
    target_link_libraries(okularGenerator_kimgio KF5::KExiv2)
endif()

if(BUILD_TESTING AND BUILD_DESKTOP AND KF5KExiv2_FOUND)
    add_definitions( -DKDESRCDIR="${CMAKE_CURRENT_SOURCE_DIR}/" )
    set( kimgiotest_SRCS tests/kimgiotest.cpp ${CMAKE_SOURCE_DIR}/gui/pagepainter.cpp ${CMAKE_SOURCE_DIR}/gui/guiutils.cpp ${CMAKE_SOURCE_DIR}/gui/debug_ui.cpp )
    ecm_add_test(${kimgiotest_SRCS} TEST_NAME "kimgiotest" LINK_LIBRARIES okularcore okularpart Qt5::Svg Qt5::Test)
    target_compile_definitions(kimgiotest PRIVATE -DGENERATOR_PATH="$<TARGET_FILE:okularGenerator_kimgio>")
endif()


########### install files ###############
install( FILES okularKimgio.desktop  DESTINATION  ${KDE_INSTALL_KSERVICES5DIR} )
install( PROGRAMS okularApplication_kimgio.desktop org.kde.mobile.okular_kimgio.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES org.kde.okular-kimgio.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR} )

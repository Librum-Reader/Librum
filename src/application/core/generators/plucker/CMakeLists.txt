remove_definitions(-DTRANSLATION_DOMAIN="okular")
add_definitions(-DTRANSLATION_DOMAIN="okular_plucker")

set(qunpluck_SRCS
    unpluck/config.cpp
    unpluck/image.cpp
    unpluck/qunpluck.cpp
    unpluck/unpluck.cpp
    unpluck/util.cpp
)

########### next target ###############

set(okularGenerator_plucker_SRCS
  generator_plucker.cpp
)

okular_add_generator(okularGenerator_plucker ${okularGenerator_plucker_SRCS} ${qunpluck_SRCS})

target_include_directories(okularGenerator_plucker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/unpluck)
target_link_libraries(okularGenerator_plucker okularcore KF5::I18n ${MATH_LIB} JPEG::JPEG ZLIB::ZLIB)

########### install files ###############
install( FILES okularPlucker.desktop  DESTINATION  ${KDE_INSTALL_KSERVICES5DIR} )
install( PROGRAMS okularApplication_plucker.desktop org.kde.mobile.okular_plucker.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES org.kde.okular-plucker.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR} )

remove_definitions(-DTRANSLATION_DOMAIN="okular")
add_definitions(-DTRANSLATION_DOMAIN="okular_xps")

########### next target ###############

set(okularGenerator_xps_SRCS
   generator_xps.cpp
)

okular_add_generator(okularGenerator_xps ${okularGenerator_xps_SRCS})

target_link_libraries(okularGenerator_xps okularcore KF5::Archive KF5::I18n Qt5::PrintSupport Qt5::Xml)

########### install files ###############
install( FILES okularXps.desktop  DESTINATION  ${KDE_INSTALL_KSERVICES5DIR} )
install( PROGRAMS okularApplication_xps.desktop org.kde.mobile.okular_xps.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES org.kde.okular-xps.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR} )

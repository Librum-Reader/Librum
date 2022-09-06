remove_definitions(-DTRANSLATION_DOMAIN="okular")
add_definitions(-DTRANSLATION_DOMAIN="okular_mobi")

########### next target ###############

set(okularGenerator_mobi_PART_SRCS
  mobidocument.cpp
  generator_mobi.cpp
  converter.cpp
)

okular_add_generator(okularGenerator_mobi ${okularGenerator_mobi_PART_SRCS})

target_link_libraries(okularGenerator_mobi okularcore qmobipocket KF5::I18n Qt5::Gui)

########### install files ###############
install( FILES okularMobi.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR} )
install( PROGRAMS okularApplication_mobi.desktop org.kde.mobile.okular_mobi.desktop DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES org.kde.okular-mobipocket.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR} )

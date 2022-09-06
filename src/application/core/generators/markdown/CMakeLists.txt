remove_definitions(-DTRANSLATION_DOMAIN="okular")
add_definitions(-DTRANSLATION_DOMAIN="okular_markdown")

########### next target ###############

set(okularGenerator_md_PART_SRCS
  converter.cpp
  generator_md.cpp
)


okular_add_generator(okularGenerator_md ${okularGenerator_md_PART_SRCS})

target_link_libraries(okularGenerator_md PRIVATE okularcore KF5::I18n PkgConfig::Discount)

########### install files ###############
install( FILES okularMd.desktop  DESTINATION  ${KDE_INSTALL_KSERVICES5DIR} )
install( PROGRAMS okularApplication_md.desktop org.kde.mobile.okular_md.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES org.kde.okular-md.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR} )


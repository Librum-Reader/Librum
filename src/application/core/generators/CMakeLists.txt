
function(okular_add_generator _target)
  kcoreaddons_add_plugin(${_target}
    JSON "lib${_target}.json"
    INSTALL_NAMESPACE "okular/generators"
    SOURCES ${ARGN}
  )
  set_target_properties(${_target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins/okular")
  get_property(aux_list GLOBAL PROPERTY okular_generator_list)
  list(APPEND aux_list ${_target})
  set_property(GLOBAL PROPERTY okular_generator_list ${aux_list})
endfunction()

# let's enable the generators properly configured

if(Poppler_Qt5_FOUND)
  add_subdirectory(poppler)
endif(Poppler_Qt5_FOUND)

if(LIBSPECTRE_FOUND)
  add_subdirectory(spectre)
endif(LIBSPECTRE_FOUND)

add_subdirectory( kimgio )

if(CHM_FOUND AND KF5KHtml_FOUND AND LIBZIP_FOUND)
  add_subdirectory( chm )
endif()

if(DJVULIBRE_FOUND)
  add_subdirectory(djvu)
endif(DJVULIBRE_FOUND)

add_subdirectory(dvi)

if(TIFF_FOUND)
  add_subdirectory(tiff)
endif(TIFF_FOUND)

add_subdirectory(xps)

add_subdirectory(fictionbook)

add_subdirectory(comicbook)

add_subdirectory(fax)

if(JPEG_FOUND AND ZLIB_FOUND)
  add_subdirectory(plucker)
endif(JPEG_FOUND AND ZLIB_FOUND)

if(EPUB_FOUND)
  add_subdirectory(epub)
endif(EPUB_FOUND)

add_subdirectory(txt)

if(QMobipocket_FOUND)
  add_subdirectory(mobipocket)
endif()

if(Discount_FOUND)
  add_subdirectory(markdown)
endif()

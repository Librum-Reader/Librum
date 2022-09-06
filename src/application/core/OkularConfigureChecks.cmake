
option(
   OKULAR_FORCE_DRM
   "Forces okular to check for DRM to decide if you can copy/print protected pdf. (default=no)"
   OFF
)
if (OKULAR_FORCE_DRM)
   set(_OKULAR_FORCE_DRM 1)
else (OKULAR_FORCE_DRM)
   set(_OKULAR_FORCE_DRM 0)
endif (OKULAR_FORCE_DRM)

# Check whether malloc_trim(3) is supported.
include(CheckSymbolExists)
check_symbol_exists(malloc_trim "malloc.h" HAVE_MALLOC_TRIM)

# at the end, output the configuration
configure_file(
   ${CMAKE_CURRENT_SOURCE_DIR}/config-okular.h.cmake
   ${CMAKE_CURRENT_BINARY_DIR}/config-okular.h
)


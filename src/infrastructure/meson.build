# Infrastructure library meson.build

infrastructure_sources = [
  'persistance/user_storage_access.cpp',
  'persistance/free_books_storage_access.cpp',
  'persistance/library_storage_access.cpp',
  'persistance/authentication_access.cpp',
  'persistance/app_info_access.cpp',
  'persistance/dictionary_access.cpp',
  'persistance/ai_explanation_access.cpp',
  'persistance/folder_storage_access.cpp',
]

infrastructure_headers = [
  'persistance/user_storage_access.hpp',
  'persistance/free_books_storage_access.hpp',
  'persistance/library_storage_access.hpp',
  'persistance/authentication_access.hpp',
  'persistance/app_info_access.hpp',
  'persistance/dictionary_access.hpp',
  'persistance/ai_explanation_access.hpp',
  'persistance/folder_storage_access.hpp',
  'persistance/api_error_helper.hpp',
  'data/endpoints.hpp',
]

# Headers that need MOC processing (have Q_OBJECT macro)
moc_headers = [
  'persistance/app_info_access.hpp',
  'persistance/dictionary_access.hpp',
  'persistance/user_storage_access.hpp',
  'persistance/authentication_access.hpp',
  'persistance/folder_storage_access.hpp',
  'persistance/ai_explanation_access.hpp',
  'persistance/library_storage_access.hpp',
  'persistance/free_books_storage_access.hpp',
]

# Qt6 preprocessing for headers with Q_OBJECT macro
qt6 = import('qt6')
qt6_preprocessed = qt6.preprocess(
  moc_headers: moc_headers,
  dependencies: qt6_dep,
)

# Include directories
infrastructure_inc = include_directories(
  'persistance',
)

infrastructure_private_inc = include_directories(
  'data',
  '.',
)

# Compile definitions
infrastructure_defines = []
if get_option('buildtype').startswith('debug')
  infrastructure_defines += ['-DQT_QML_DEBUG']
endif

# Create static library
infrastructure_lib = static_library('infrastructure',
  sources: [
    infrastructure_sources,
    infrastructure_headers,
    qt6_preprocessed,
  ],
  include_directories: [
    infrastructure_inc,
    infrastructure_private_inc,
  ],
  dependencies: [
    adapters_dep,
    application_dep,
    qt6_dep,
  ],
  cpp_args: cpp_args + infrastructure_defines,
  link_args: link_args,
  install: true,
  install_dir: get_option('libdir'),

)

# Export dependency for use in other subdirectories
infrastructure_dep = declare_dependency(
  include_directories: infrastructure_inc,
  compile_args: infrastructure_defines,
  link_with: infrastructure_lib,
)

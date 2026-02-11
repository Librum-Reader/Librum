# Presentation library meson.build

# Source files
presentation_sources = [
  'modules/CppElements/key_sequence_recorder.cpp',
  'modules/CppElements/page_view.cpp',
]

presentation_headers = [
  'modules/CppElements/key_sequence_recorder.hpp',
  'modules/CppElements/page_view.hpp',
]

# Resource files - paths relative to project root
resource_files = [
  '../../resources.qrc',
  'qmlSources.qrc',
]

# Translation files
translation_files = [
  'translations/librum_en.ts',
  'translations/librum_de.ts',
  'translations/librum_it.ts',
  'translations/librum_ru.ts',
  'translations/librum_ko.ts',
  'translations/librum_ar.ts',
  'translations/librum_zh.ts',
  'translations/librum_uk.ts',
  'translations/librum_pt.ts',
  'translations/librum_id.ts',
  'translations/librum_bn.ts',
]

# Include directories
presentation_inc = include_directories(
  'modules/CppElements',
)

# Preprocess Qt resources
qt6 = import('qt6')
qt6_preprocessed = qt6.preprocess(
  qresources: resource_files,
  moc_headers: presentation_headers,
  ui_files: [],  # No .ui files in presentation
  dependencies: qt6_dep,
)

# Compile translations
qt6_translations = qt6.compile_translations(
  ts_files: translation_files,
  install: true,
  install_dir: join_paths(get_option('datadir'), 'translations'),
)

# Compile definitions
presentation_defines = []
if get_option('buildtype').startswith('debug')
  presentation_defines += ['-DQT_QML_DEBUG']
endif

# Create static library
presentation_lib = static_library('presentation',
  sources: [
    presentation_sources,
    presentation_headers,
    qt6_preprocessed,
    qt6_translations,
  ],
  include_directories: presentation_inc,
  dependencies: [
    adapters_dep,
    application_dep,
    domain_dep,
    mupdf_dep,
    qt6_dep,
  ],
  cpp_args: cpp_args + presentation_defines + ['-DPRESENTATION_LIBRARY'],
  link_args: link_args,
  install: true,
  install_dir: get_option('libdir'),

)

# Export dependency for use in other subdirectories
presentation_dep = declare_dependency(
  include_directories: presentation_inc,
  compile_args: presentation_defines,
  link_with: presentation_lib,
)

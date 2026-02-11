# Domain library meson.build

domain_sources = [
  'entities/book.cpp',
  'entities/tag.cpp',
  'entities/user.cpp',
  'entities/highlight.cpp',
  'entities/rectf.cpp',
  'entities/bookmark.cpp',
  'entities/folder.cpp',
  'value_objects/login_model.cpp',
  'value_objects/register_model.cpp',
]

domain_headers = [
  'entities/book.hpp',
  'entities/tag.hpp',
  'entities/user.hpp',
  'entities/highlight.hpp',
  'entities/rectf.hpp',
  'entities/bookmark.hpp',
  'entities/folder.hpp',
  'entities/free_book.hpp',
  'value_objects/login_model.hpp',
  'value_objects/register_model.hpp',
  'value_objects/book_meta_data.hpp',
]

# Qt6 preprocessing for headers with Q_OBJECT macro
qt6 = import('qt6')
qt6_preprocessed = qt6.preprocess(
  moc_headers: ['entities/user.hpp'],
  dependencies: qt6_dep,
)

# Include directories
domain_inc = include_directories(
  'entities',
  'value_objects',
  '.'
)

# Compile definitions
domain_defines = []
if get_option('buildtype').startswith('debug')
  domain_defines += ['-DQT_QML_DEBUG']
endif

# Create static library
domain_lib = static_library('domain',
  sources: [
    domain_sources,
    domain_headers,
    qt6_preprocessed,
  ],
  include_directories: domain_inc,
  dependencies: [qt6_dep],
  cpp_args: cpp_args + domain_defines + ['-DDOMAIN_LIBRARY'],
  link_args: link_args,
  install: true,
  install_dir: get_option('libdir'),
)

# Export dependency for use in other subdirectories
domain_dep = declare_dependency(
  include_directories: domain_inc,
  compile_args: domain_defines,
  link_with: domain_lib,
)

# Domain unit tests
if get_option('build_tests')
  domain_test_sources = [
    '../../tests/domain_unit_tests/main.cpp',
    '../../tests/domain_unit_tests/book_tests.cpp',
    '../../tests/domain_unit_tests/user_tests.cpp',
    '../../tests/domain_unit_tests/tag_tests.cpp',
  ]

  domain_unit_tests = executable('domain_unit_tests',
    sources: domain_test_sources,
    include_directories: [domain_inc, include_directories('../../tests/domain_unit_tests')],
    dependencies: [
      gtest_dep,
      domain_dep,
      qt6_dep,
    ],
    cpp_args: cpp_args,
    link_args: link_args,
    install: false,
  )

  test('DomainUnitTests', domain_unit_tests,
    timeout: 300,
    is_parallel: false,
  )
endif

# Tests meson.build

if not get_option('build_tests')
  subdir_done()
endif

# gtest_dep is already defined in parent meson.build
# Use the existing gtest_dep variable (which actually points to gmock)
# Get library targets and create dependencies for tests
# Tests are now inline in src subdirectories, so these are not needed
# domain_lib = meson.get_target('domain')
# domain_inc = include_directories('../src/domain', '../src/domain/entities', '../src/domain/value_objects')
# domain_dep = declare_dependency(
#   include_directories: domain_inc,
#   link_with: domain_lib,
# )

# application_lib = meson.get_target('application')
# application_inc = include_directories(
#   '../src/application',
#   '../src/application/interfaces/services',
#   '../src/application/interfaces/gateways',
#   '../src/application/interfaces/utility',
#   '../src/application/interfaces/managers',
#   '../src/application/common/enums',
#   '../src/application/services',
#   '../src/application/utility',
#   '../src/application/managers',
#   '../src/application/core',
#   '../src/application/core/utils',
# )
# application_dep = declare_dependency(
#   include_directories: application_inc,
#   link_with: application_lib,
# )

# adapters_lib = meson.get_target('adapters')
# adapters_inc = include_directories(
#   '../src/adapters',
#   '../src/adapters/interfaces/persistance',
#   '../src/adapters/interfaces/controllers',
#   '../src/adapters/controllers',
#   '../src/adapters/gateways',
#   '../src/adapters/DTOs',
#   '../src/adapters/data_models/library_model',
#   '../src/adapters/data_models/free_books_model',
#   '../src/adapters/data_models/user_tags_model',
#   '../src/adapters/data_models/shortcuts_model',
#   '../src/adapters/data_models/bookmarks_model',
#   '../src/adapters/data_models/folders_model',
#   '../src/adapters/data_models/icon_model',
#   '../src/adapters/data_models/book_title_model',
#   '../src/adapters/data_models',
# )
# adapters_dep = declare_dependency(
#   include_directories: adapters_inc,
#   link_with: adapters_lib,
# )

# infrastructure_lib = meson.get_target('infrastructure')
# infrastructure_inc = include_directories(
#   '../src/infrastructure',
#   '../src/infrastructure/persistance',
#   '../src/infrastructure/data',
# )
# infrastructure_dep = declare_dependency(
#   include_directories: infrastructure_inc,
#   link_with: infrastructure_lib,
# )

# presentation_lib = meson.get_target('presentation')
# presentation_inc = include_directories(
#   '../src/presentation',
#   '../src/presentation/modules/CppElements',
# )
# presentation_dep = declare_dependency(
#   include_directories: presentation_inc,
#   link_with: presentation_lib,
# )

# Add test subdirectories
# Tests are now inline in src subdirectories
# subdir('domain_unit_tests')
# subdir('application_unit_tests')
# subdir('adapters_unit_tests')

# Optional: Run tests automatically after build
if get_option('enable_coverage')
  # Add coverage targets if needed
  message('Test coverage enabled - tests will be built with coverage instrumentation')
endif

# Test summary
message('''
Test configuration:
  Build tests: true
  Test directories: domain_unit_tests, application_unit_tests, adapters_unit_tests
  Coverage enabled: @0@
'''.format(get_option('enable_coverage')))

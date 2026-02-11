# Application library meson.build

application_sources = [
  'services/authentication_service.cpp',
  'services/free_books_service.cpp',
  'services/library_service.cpp',
  'services/user_service.cpp',
  'services/settings_service.cpp',
  'services/app_info_service.cpp',
  'services/book_service.cpp',
  'services/dictionary_service.cpp',
  'services/ai_explanation_service.cpp',
  'services/folder_service.cpp',
  'services/tools_service.cpp',
  'managers/library_storage_manager.cpp',
  'utility/local_library_tracker.cpp',
  'utility/book_merger.cpp',
  'utility/library_book_getter.cpp',
  'utility/external_book_getter.cpp',
  'core/page_generator.cpp',
  'core/metadata_extractor.cpp',
  'core/toc/toc_item.cpp',
  'core/toc/toc_model.cpp',
  'core/toc/filtered_toc_model.cpp',
  'core/utils/book_searcher.cpp',
  'core/utils/text_selector.cpp',
]

application_headers = [
  'interfaces/services/i_authentication_service.hpp',
  'interfaces/services/i_free_books_service.hpp',
  'interfaces/services/i_library_service.hpp',
  'interfaces/services/i_user_service.hpp',
  'interfaces/services/i_settings_service.hpp',
  'interfaces/services/i_app_info_service.hpp',
  'interfaces/services/i_book_service.hpp',
  'interfaces/services/i_dictionary_service.hpp',
  'interfaces/services/i_ai_explanation_service.hpp',
  'interfaces/services/i_folder_service.hpp',
  'interfaces/services/i_tools_service.hpp',
  'interfaces/gateways/i_free_books_storage_gateway.hpp',
  'interfaces/gateways/i_user_storage_gateway.hpp',
  'interfaces/gateways/i_library_storage_gateway.hpp',
  'interfaces/gateways/i_authentication_gateway.hpp',
  'interfaces/gateways/i_app_info_gateway.hpp',
  'interfaces/gateways/i_dictionary_gateway.hpp',
  'interfaces/gateways/i_ai_explanation_gateway.hpp',
  'interfaces/gateways/i_folder_storage_gateway.hpp',
  'interfaces/managers/i_library_storage_manager.hpp',
  'interfaces/managers/i_highlight_storage_manager.hpp',
  'interfaces/utility/i_metadata_extractor.hpp',
  'interfaces/utility/i_local_library_tracker.hpp',
  'interfaces/utility/i_book_getter.hpp',
  'services/authentication_service.hpp',
  'services/free_books_service.hpp',
  'services/library_service.hpp',
  'services/user_service.hpp',
  'services/settings_service.hpp',
  'services/app_info_service.hpp',
  'services/book_service.hpp',
  'services/dictionary_service.hpp',
  'services/ai_explanation_service.hpp',
  'services/folder_service.hpp',
  'services/tools_service.hpp',
  'managers/library_storage_manager.hpp',
  'common/enums/book_operation_status.hpp',
  'common/enums/setting_keys.hpp',
  'common/enums/setting_groups.hpp',
  'common/enums/error_code.hpp',
  'utility/local_library_tracker.hpp',
  'utility/merge_status.hpp',
  'utility/book_for_deletion.hpp',
  'utility/enum_utils.hpp',
  'utility/application_settings.hpp',
  'utility/book_merger.hpp',
  'utility/automatic_login_helper.hpp',
  'utility/error_code_converter.hpp',
  'utility/book_utils.hpp',
  'utility/save_book_helper.hpp',
  'utility/library_book_getter.hpp',
  'utility/external_book_getter.hpp',
  'core/page_generator.hpp',
  'core/metadata_extractor.hpp',
  'core/toc/toc_item.hpp',
  'core/toc/toc_model.hpp',
  'core/toc/filtered_toc_model.hpp',
  'core/utils/book_searcher.hpp',
  'core/utils/fz_utils.hpp',
  'core/utils/text_selector.hpp',
  'core/utils/search_options.hpp',
  'core/utils/mutool_utils.hpp',
]

# Headers that need MOC processing (have Q_OBJECT or Q_NAMESPACE macro)
moc_headers = [
  # Q_NAMESPACE headers
  'common/enums/book_operation_status.hpp',
  'common/enums/error_code.hpp',
  'common/enums/setting_groups.hpp',
  'common/enums/setting_keys.hpp',

  # Q_OBJECT headers
  'core/toc/toc_model.hpp',
  'core/toc/filtered_toc_model.hpp',
  'utility/book_merger.hpp',
  'interfaces/gateways/i_folder_storage_gateway.hpp',
  'interfaces/gateways/i_dictionary_gateway.hpp',
  'interfaces/gateways/i_user_storage_gateway.hpp',
  'interfaces/gateways/i_authentication_gateway.hpp',
  'interfaces/gateways/i_app_info_gateway.hpp',
  'interfaces/gateways/i_ai_explanation_gateway.hpp',
  'interfaces/gateways/i_library_storage_gateway.hpp',
  'interfaces/gateways/i_free_books_storage_gateway.hpp',
  'interfaces/managers/i_highlight_storage_manager.hpp',
  'interfaces/managers/i_library_storage_manager.hpp',
  'interfaces/services/i_authentication_service.hpp',
  'interfaces/services/i_book_service.hpp',
  'interfaces/services/i_dictionary_service.hpp',
  'interfaces/services/i_ai_explanation_service.hpp',
  'interfaces/services/i_app_info_service.hpp',
  'interfaces/services/i_tools_service.hpp',
  'interfaces/services/i_settings_service.hpp',
  'interfaces/services/i_library_service.hpp',
  'interfaces/services/i_folder_service.hpp',
  'interfaces/services/i_free_books_service.hpp',
  'interfaces/services/i_user_service.hpp',
  'managers/library_storage_manager.hpp',
  'services/dictionary_service.hpp',
  'services/folder_service.hpp',
  'services/settings_service.hpp',
  'services/app_info_service.hpp',
  'services/authentication_service.hpp',
  'services/tools_service.hpp',
  'services/user_service.hpp',
  'services/ai_explanation_service.hpp',
  'services/library_service.hpp',
  'services/free_books_service.hpp',
]

# Qt6 preprocessing for headers with Q_OBJECT or Q_NAMESPACE macro
qt6 = import('qt6')
qt6_preprocessed = qt6.preprocess(
  moc_headers: moc_headers,
  dependencies: qt6_dep,
)

# Include directories
application_inc = include_directories(
  '.',
  'interfaces/services',
  'interfaces/gateways',
  'interfaces/utility',
  'interfaces/managers',
  'common/enums',
  'services',
  'utility',
  'managers',
  'core',
  'core/utils',
)

# Compile definitions
application_defines = []
if get_option('buildtype').startswith('debug')
  application_defines += ['-DQT_QML_DEBUG']
endif

# Create static library
application_lib = static_library('application',
  sources: [
    application_sources,
    application_headers,
    qt6_preprocessed,
  ],
  include_directories: application_inc,
  dependencies: [
    domain_dep,
    rapidfuzz_dep,
    mupdf_dep,
    qt6_dep,
  ],
  cpp_args: cpp_args + application_defines + ['-DAPPLICATION_LIBRARY'],
  link_args: link_args,
  install: true,
  install_dir: get_option('libdir'),

)

# Export dependency for use in other subdirectories
application_dep = declare_dependency(
  include_directories: application_inc,
  compile_args: application_defines,
  link_with: application_lib,
)

# Application unit tests
if get_option('build_tests')
  application_test_sources = [
    '../../tests/application_unit_tests/main.cpp',
    '../../tests/application_unit_tests/services/library_service_tests.cpp',
    '../../tests/application_unit_tests/services/authentication_service_tests.cpp',
    '../../tests/application_unit_tests/services/user_service_tests.cpp',
    '../../tests/application_unit_tests/services/folder_service_tests.cpp',
    '../../tests/application_unit_tests/services/settings_service_tests.cpp',
    '../../tests/application_unit_tests/utility/enum_utils_tests.cpp',
    '../../tests/application_unit_tests/utility/book_merger_tests.cpp',
    '../../tests/application_unit_tests/utility/library_storage_manager_tests.cpp',
    '../../tests/application_unit_tests/utility/local_library_tracker_tests.cpp',
  ]

  # Test headers that need MOC processing
  test_moc_headers = [
    '../../tests/application_unit_tests/utility/test_enum.hpp',
  ]

  # Qt6 preprocessing for test headers
  qt6_test_preprocessed = qt6.preprocess(
    moc_headers: test_moc_headers,
    dependencies: qt6_dep,
  )

  application_unit_tests = executable('application_unit_tests',
    sources: application_test_sources + [qt6_test_preprocessed],
    include_directories: [application_inc, include_directories('../../tests/application_unit_tests'), include_directories('../../tests/application_unit_tests/utility'), include_directories('../../tests/application_unit_tests/services')],
    dependencies: [
      gtest_dep,
      application_dep,
      domain_dep,
      rapidfuzz_dep,
      mupdf_dep,
      qt6_dep,
    ],
    cpp_args: cpp_args,
    link_args: link_args,
    install: false,
  )

  test('ApplicationUnitTests', application_unit_tests,
    timeout: 300,
    is_parallel: false,
  )
endif

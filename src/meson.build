# src/meson.build - Main build file for Librum executable

# Include subdirectories in dependency order
# domain has no dependencies
subdir('domain')

# application depends on domain
subdir('application')

# adapters depends on domain and application
subdir('adapters')

# infrastructure depends on adapters
subdir('infrastructure')

# presentation depends on adapters, application, and domain
subdir('presentation')

# Collect include directories from all libraries
main_inc = [
  domain_inc,
  application_inc,
  adapters_inc,
  infrastructure_inc,
  presentation_inc,
]

# Main executable sources
librum_sources = [
  'main.cpp',
  'dependency_injection.hpp',
  'message_handler.hpp',
]

# Resource files
resource_files = [
  '../fonts.qrc',
  '../resources.qrc',
  'presentation/qmlSources.qrc',
]

# Windows-specific resource file
if host_machine.system() == 'windows'
  librum_sources += 'resources.rc'
endif

# Qt preprocessing for resources
qt6 = import('qt6')
qt6_preprocessed = qt6.preprocess(
  qresources: resource_files,
  dependencies: qt6_dep,
)

# Create main executable
librum_exe = executable('librum',
  sources: [
    librum_sources,
    qt6_preprocessed,
  ],
  include_directories: main_inc,
  link_with: [
    presentation_lib,
    adapters_lib,
    application_lib,
    infrastructure_lib,
    domain_lib,
  ],
  dependencies: [
    mupdf_dep,
    qt6_dep,
  ],
  cpp_args: cpp_args,
  link_args: link_args,
  install: true,
  win_subsystem: 'windows',  # Set WIN32_EXECUTABLE equivalent
)

# Handle platform-specific configurations
if host_machine.system() == 'windows'
  # On Windows, set WIN32_EXECUTABLE to avoid console
  # In Meson, win_subsystem: 'windows' handles this
  # Additional Windows-specific configurations can go here
elif host_machine.system() == 'android'
  # Android-specific configuration
  # Note: Android support requires additional setup in Meson
  # For now, we'll note this needs to be implemented
  message('Android build configuration not fully implemented in Meson')
endif

# Install rules for Unix/Linux
if host_machine.system() == 'linux'
  # Install executable
  # (Already handled by install: true in executable())

  # Install desktop file
  install_data(
    '../librum.desktop',
    install_dir: join_paths(get_option('datadir'), 'applications'),
  )

  # Install icon
  install_data(
    'logo.svg',
    install_dir: join_paths(get_option('datadir'), 'pixmaps'),
    rename: 'librum.svg',
  )
endif

# Optional: Android extra libraries configuration
# In CMake, this sets QT_ANDROID_EXTRA_LIBS
# Meson has different Android support; this would need custom integration

# Summary
message('''
Librum executable configured:
  Platform: @0@
  Install prefix: @1@
  Build type: @2@
'''.format(
  host_machine.system(),
  get_option('prefix'),
  get_option('buildtype')
))

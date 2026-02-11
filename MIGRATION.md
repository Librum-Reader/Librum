# Meson Build System Migration

This document details the migration of the Librum project from CMake to Meson build system.

## Overview

The Librum project has been migrated from CMake to Meson to simplify dependency management, remove git submodules, and create a more maintainable build system. This migration was completed on February 10, 2026 and represents a major improvement in build system organization.

## Why Migrate to Meson?

Several factors motivated the migration:

1. **Dependency Management**: Meson has better system package integration
2. **Simplified Build Configuration**: Meson configuration files are more readable and maintainable
3. **Submodule Elimination**: All git submodules have been replaced with system packages
4. **Modern Build Features**: Better cross-platform support and integration with modern tooling

## Key Changes

### 1. Removed Git Submodules
- Deleted entire `libs/` directory containing:
  - Boost.DI
  - googletest
  - MuPDF
  - rapidfuzz-cpp
- Removed `.gitmodules` file

### 2. Dependency Management
All dependencies are now managed as system packages:

| Dependency | Package Name | Installation Method |
|------------|--------------|---------------------|
| MuPDF | `libmupdf-dev` | System package |
| MuPDF C++ wrapper | `libmupdfcpp` | System package |
| rapidfuzz-cpp | `rapidfuzz-cpp-dev` | System package |
| Google Test/Mock | `googletest`, `libgmock-dev` | System package |
| Boost.DI | N/A (header-only) | System package |
| Qt6 | `qt6-base-dev` and related packages | System package |

### 3. Build Configuration Changes
- **Root file**: `meson.build` instead of `CMakeLists.txt`
- **Build options**: Configured via `meson_options.txt`
- **Library structure**: Each component (domain, application, adapters, infrastructure, presentation) has its own `meson.build`
- **Test integration**: Tests are now inline within each library's build file

### 4. Technical Fixes Applied

#### MuPDF Destructor Visibility
The system MuPDF package (v1.27.0) had non-exported destructors for simple types. Fixed by adding `-DNDEBUG` compile flag.

#### Qt6 Compatibility
- Changed `QQuickStyle::setStyle("Basic")` to `"Default"` for Qt6 compatibility
- Updated MOC processing for 50+ headers with `Q_OBJECT`, `Q_GADGET`, and `Q_NAMESPACE` macros
- Fixed Qt6 `IconImage` QML type issues by converting to `Image`

#### Static Library Dependencies
Removed `link_whole` to prevent multiple definition errors. Libraries are now linked directly.

#### Boost.DI Include Path
Updated include path to `/usr/include/boost-di/` since Boost.DI is not available as a standard system package.

## Building with Meson

### Prerequisites

#### Linux (Ubuntu/Debian)
```bash
sudo apt install meson ninja-build g++ python3-pip
sudo apt install libmupdf-dev rapidfuzz-cpp-dev googletest libgmock-dev
sudo apt install qt6-base-dev qt6-quick-dev qt6-quickcontrols2-dev qt6-tools-dev

# Install Boost.DI (not available as standard package)
sudo mkdir -p /usr/include/boost-di/boost
sudo wget -O /usr/include/boost-di/boost/di.hpp https://raw.githubusercontent.com/boost-ext/di/develop/include/boost/di.hpp
```

#### Manual Boost.DI Installation (Alternative)
If you prefer not to install to system directories:
```bash
git clone https://github.com/boost-ext/di.git
cp -r di/include/boost/di.hpp /usr/local/include/boost/  # or other include path
```

### Build Commands

```bash
# Configure build (without tests)
meson setup build --buildtype=debugoptimized

# Configure build with tests
meson setup build --buildtype=debugoptimized -Dbuild_tests=true

# Build project
cd build && ninja

# Run the application
./src/librum

# Run tests (if built)
ninja test
```

### Build Options

Available options in `meson_options.txt`:
- `build_tests`: Build unit tests (default: `false`)
- `enable_coverage`: Enable code coverage instrumentation (default: `false`)
- `use_sanitizers`: Enable address/undefined sanitizers (default: `false`)

## Test System Changes

The test system has been restructured:

1. **Inline Test Definition**: Tests are now defined within each library's `meson.build` file:
   - `src/domain/meson.build` contains domain unit tests
   - `src/application/meson.build` contains application unit tests
   - `src/adapters/meson.build` contains adapters unit tests

2. **Test Execution**:
   ```bash
   # Build with tests
   meson setup build -Dbuild_tests=true
   cd build && ninja

   # Run specific test executable
   ./src/domain/domain_unit_tests

   # Run all tests via Meson
   ninja test
   ```

3. **Test Dependencies**: All tests link against their respective libraries and Google Mock (`gmock`).

## Project Structure

```
Librum/
├── meson.build              # Root build configuration
├── meson_options.txt        # Build options
├── src/
│   ├── domain/meson.build   # Domain layer + tests
│   ├── application/meson.build  # Application layer + tests
│   ├── adapters/meson.build # Adapters layer + tests
│   ├── infrastructure/meson.build  # Infrastructure layer
│   ├── presentation/meson.build    # Presentation layer
│   └── meson.build          # Main executable configuration
├── tests/                   # Test source files (no build config)
│   ├── domain_unit_tests/
│   ├── application_unit_tests/
│   └── adapters_unit_tests/
└── resources/              # Application resources
```

## Remaining Issues and TODOs

### ✅ Completed
- [x] Main application builds and runs successfully
- [x] All dependencies managed as system packages
- [x] Unit tests build and execute
- [x] Qt6 MOC processing working for all headers
- [x] Resource files (QML, translations) properly integrated

### ⚠️ Partially Complete
- **Android Support**: CMake had Android-specific configurations. Meson Android support needs implementation.
- **Windows Symbol Exports**: `CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS` equivalent not configured in Meson.

### ❌ Not Migrated
- **Windows-specific build configurations**: Need validation on Windows platform

## Migration Verification Checklist

Before removing CMake files, verify:

1. [x] Main executable builds without errors
2. [x] All test executables build
3. [x] All tests pass
4. [x] Application runs without crashes
5. [x] Qt6 integration works (QML, translations, resources)
6. [x] All dependencies resolve from system packages
7. [ ] Windows build verification
8. [ ] Android build verification
9. [x] Code coverage working (optional)

## For Maintainers

### Adding New Source Files
When adding new source files to a library:

1. Add source file to the appropriate `*_sources` list
2. Add header file to the appropriate `*_headers` list (if needed)
3. If header contains `Q_OBJECT`, `Q_GADGET`, or `Q_NAMESPACE`:
   - Add to `moc_headers` list
   - Ensure Qt6 preprocessing includes it

### Adding New Dependencies
1. Add dependency to root `meson.build` if system-wide
2. Create appropriate `declare_dependency()` with correct include paths
3. Add dependency to library's dependency list

### Adding New Tests
1. Add test source to the appropriate test source list in the library's `meson.build`
2. Add include directories for test headers if needed
3. Test will automatically be linked with the library and Google Mock

## CI/CD Updates

GitHub Actions workflow has been updated:
- Linux job now uses Meson instead of CMake
- Submodules are no longer checked out
- System packages installed via apt
- Boost.DI installed manually during build

## Rollback Procedure

If issues are discovered, you can temporarily revert to CMake:

1. Ensure `CMakeLists.txt` files are still present
2. Ensure `cmake/` directory exists
3. Clone submodules: `git submodule update --init --recursive`
4. Build with CMake as before

**Note**: All CMake files have been successfully removed as the Meson build system is now fully functional.

## Known Issues

### 1. Translation File Warnings
Meson generates warnings for translation targets with path separators in their names:
```
WARNING: Target "qt6-compile-translations/librum_en.ts" has a path separator in its name.
```
This is cosmetic and doesn't affect functionality. To fix, translation file handling could be refactored.

### 2. Coverage Reporting Dependency
The coverage report generation depends on `gcovr` which may not be installed by default. Users need to install it separately:
```bash
pip install gcovr
```
The build system will warn if `gcovr` is not found but will still compile with coverage instrumentation.

### 3. Boost.DI Manual Installation
Boost.DI is not available as a standard system package and requires manual installation to `/usr/include/boost-di/` or another include path.

### 4. Windows Symbol Exports
The Windows symbol export macros (`ADAPTERS_EXPORT`, etc.) are configured but haven't been validated on Windows platform.

### 5. Static Library Linking Order
Some static library linking order issues may appear depending on compiler/linker. The current order in `src/meson.build` should work for most cases.

## Contact

For questions about the Meson migration, contact the development team or refer to:
- [Meson Build System Documentation](https://mesonbuild.com/)
- [Librum GitHub Repository](https://github.com/Librum-Reader/Librum)
- Project maintainers

---

*Migration completed: February 10, 2026*
*Document last updated: February 10, 2026*
*Meson version: 1.10.1+*
*Qt version: 6.10.2+*

## Platform-Specific TODO List

### ✅ Completed for Linux
- [x] Main application build and execution
- [x] Unit test integration and execution
- [x] Dependency management via system packages
- [x] Qt6 integration (MOC, resources, translations)
- [x] Code coverage reporting with gcovr
- [x] GitHub Actions CI pipeline
- [x] CMake files cleanup

### 📱 Android Support (To Be Implemented)
- [ ] Configure Meson for Android cross-compilation
- [ ] Add Android-specific Qt6 modules and dependencies
- [ ] Handle Android resource packaging (APK/AAB)
- [ ] Configure Android NDK toolchain
- [ ] Test on Android emulator/device
- [ ] Add Android CI pipeline

### 🍎 macOS Support (To Be Implemented)
- [ ] Verify macOS-specific compiler flags
- [ ] Configure macOS bundle/application packaging
- [ ] Handle macOS-specific Qt6 integration
- [ ] Test on macOS hardware
- [ ] Add macOS CI pipeline (GitHub Actions macOS runner)

### 🪟 Windows Support (To Be Implemented)
- [x] Windows-specific resource files (`.rc`) already configured in `src/meson.build`
- [ ] Configure Windows symbol exports (`ADAPTERS_EXPORT`, etc.) - see Known Issues
- [ ] Configure MSVC compiler compatibility
- [ ] Test Windows executable build
- [ ] Handle Windows installation (NSIS/Inno Setup)
- [ ] Add Windows CI pipeline

### 🔧 Build System Enhancements (Optional)
- [x] Code coverage reporting with gcovr targets
- [ ] Add more build options (debug symbols, optimization levels)
- [ ] Improve dependency detection with fallbacks
- [ ] Add cross-compilation support for embedded targets
- [ ] Create distribution packages (.deb, .rpm, .exe, .dmg)
- [ ] Add continuous deployment pipeline

### 📚 Documentation
- [x] README updated with Meson build instructions
- [x] MIGRATION.md created with complete migration documentation
- [ ] Update platform-specific build instructions for Windows/macOS/Android
- [ ] Create developer setup guide for each platform
- [ ] Document dependency installation procedures per platform
- [ ] Add troubleshooting guide for common issues

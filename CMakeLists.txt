cmake_minimum_required(VERSION 3.26.0)
cmake_policy(
  SET
  CMP0063
  NEW
)
cmake_policy(
  SET
  CMP0091
  NEW
)
cmake_policy(
  SET
  CMP0126
  NEW
)
list(
  APPEND
  CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake"
)

if(VCPKG_TARGET_ANDROID)
  cmake_policy(
    SET
    CMP0144
    NEW
  )
  include("tools/cmake/vcpkg_android.cmake")
endif()

include(caspersdk-set-osx-deployment-target)
caspersdk_set_osx_deployment_target(13.4)
project(CasperSDK VERSION 1.0.0)

if(VCPKG_TARGET_ANDROID)
  message("Adding symlinks, vcpkg installed directory: ${VCPKG_INSTALLED_DIR}")

  set(SYMLINK_DIR
      "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/cryptopp/CryptoPPConfig.cmake"
  )
  set(TARGET_DIR
      "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/cryptopp/cryptoppConfig.cmake"
  )

  # Create the symbolic link
  execute_process(
    COMMAND "${CMAKE_COMMAND}" -E create_symlink "${TARGET_DIR}"
            "${SYMLINK_DIR}" RESULT_VARIABLE result
  )
  if(NOT
     result
     EQUAL
     0
  )
    message(
      FATAL_ERROR
        "Failed to create symbolic link: ${SYMLINK_DIR} -> ${TARGET_DIR}"
    )
  endif()
endif()

# -----------------------------------------------------------------------------
# Setup

find_package(
  otcommon
  3
  CONFIG
  REQUIRED
)
otcommon_set_build_type()
otcommon_require_out_of_source_build()
otcommon_set_rpath()
# -----------------------------------------------------------------------------
# Set option defaults

if(CMAKE_BUILD_TYPE
   STREQUAL
   "Debug"
   OR CMAKE_BUILD_TYPE
      STREQUAL
      "RelWithDebInfo"
)
  set(CASPERSDK_DEBUG_BUILD ON)
  set(CASPERSDK_PEDANTIC_DEFAULT ON)
  set(CASPERSDK_BUILD_TESTS_DEFAULT ${PROJECT_IS_TOP_LEVEL})
  set(CMAKE_VERBOSE_MAKEFILE ON)
else()
  set(CASPERSDK_DEBUG_BUILD OFF)
  set(CASPERSDK_PEDANTIC_DEFAULT OFF)
  set(CASPERSDK_BUILD_TESTS_DEFAULT OFF)
endif()

if(DEFINED VCPKG_TARGET_TRIPLET)
  set(CASPERSDK_USE_VCPKG_TARGETS_DEFAULT ON)
else()
  set(CASPERSDK_USE_VCPKG_TARGETS_DEFAULT OFF)
endif()

# -----------------------------------------------------------------------------
# Options

option(
  CASPERSDK_STANDALONE
  "Configure as standalone library instead of a subproject"
  ${PROJECT_IS_TOP_LEVEL}
)
option(
  CASPERSDK_BUILD_TESTS
  "Build the unit tests."
  ${CASPERSDK_BUILD_TESTS_DEFAULT}
)
option(
  CASPERSDK_PEDANTIC_BUILD
  "Treat compiler warnings as errors."
  ${CASPERSDK_PEDANTIC_DEFAULT}
)
option(
  CASPERSDK_IWYU
  "Enable include-what-you-use static analysis"
  OFF
)
option(
  CASPERSDK_CLANG_TIDY
  "Enable clang-tidy"
  OFF
)

if(CASPERSDK_IWYU OR CASPERSDK_CLANG_TIDY)
  set(CMAKE_UNITY_BUILD OFF)
endif()

option(
  CASPERSDK_USE_VCPKG_TARGETS
  "Assume dependencies are managed by vcpkg"
  ${CASPERSDK_USE_VCPKG_TARGETS_DEFAULT}
)
option(
  CASPERSDK_STATIC_DEPENDENCIES
  "Link against static version of dependencies"
  ${CASPERSDK_USE_VCPKG_TARGETS}
)
option(
  CASPERSDK_INSTALL_HEADERS
  "Packing option to control whether or not headers are installed"
  ON
)
option(
  CASPERSDK_INSTALL_CMAKE
  "Packing option to control whether or not cmake package configuration is installed"
  ON
)
option(
  CASPERSDK_INSTALL_LICENSE
  "Packing option to control whether or not license file is installed"
  ON
)
option(
  CASPERSDK_MULTICONFIG
  "Install libraries to multi-config paths"
  OFF
)

if(NOT
   DEFINED
   CASPERSDK_HEADER_INSTALL_PATH
)
  set(CASPERSDK_HEADER_INSTALL_PATH "${CMAKE_INSTALL_INCLUDEDIR}")
endif()

if(NOT
   DEFINED
   CASPERSDK_LIBRARY_INSTALL_PATH
)
  set(CASPERSDK_LIBRARY_INSTALL_PATH "${CMAKE_INSTALL_LIBDIR}")
endif()

if(NOT
   DEFINED
   CASPERSDK_CMAKE_INSTALL_PATH
)
  set(CASPERSDK_CMAKE_INSTALL_PATH
      "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake"
  )
endif()

if(NOT
   DEFINED
   CASPERSDK_LICENSE_FILE_NAME
)
  set(CASPERSDK_LICENSE_FILE_NAME "LICENSE")
endif()

if(CASPERSDK_IWYU)
  otcommon_make_iwyu_target()
endif()

# -----------------------------------------------------------------------------
# Set compiler options

include(caspersdk-configure-cxx-target)
otcommon_set_project_language_standard(
  11
  20
  ${CASPERSDK_PEDANTIC_BUILD}
)

if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4217")
  set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4217")
endif()

# -----------------------------------------------------------------------------
# Print system information and build options

include(caspersdk-print-build-details)
caspersdk_print_build_details()

# -----------------------------------------------------------------------------
# Dependencies

if(CASPERSDK_STATIC_DEPENDENCIES)
  set(OPENSSL_USE_STATIC_LIBS ON)
else()
  set(OPENSSL_USE_STATIC_LIBS OFF)
endif()

otcommon_find_system_libraries()
include(caspersdk-find-dependencies)

if(CASPERSDK_CLANG_TIDY)
  find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)

  if(CLANG_TIDY_COMMAND)
    set(CMAKE_CXX_CLANG_TIDY
        "${CLANG_TIDY_COMMAND};-header-filter=(${CMAKE_CURRENT_SOURCE_DIR}|${CMAKE_CURRENT_BINARY_DIR})/(generated|include|src|tests)/.*.(h|t)pp;-fix-errors;-extra-arg=-Wno-unknown-warning-option;--warnings-as-errors=*"
    )
  else()
    message(FATAL_ERROR "clang-tidy not found")
  endif()
endif()

# -----------------------------------------------------------------------------
# Symbol visibility

set(CMAKE_CXX_VISIBILITY_PRESET default)
set(CMAKE_VISIBILITY_INLINES_HIDDEN OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# -----------------------------------------------------------------------------
# Build source

if(CASPERSDK_STANDALONE)
  # library type controlled by BUILD_SHARED_LIBS
  add_library(casper_sdk "")
else()
  add_library(casper_sdk STATIC "")
  target_include_directories(
    casper_sdk SYSTEM
    INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/external>
              $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
  )
endif()

target_sources(
  casper_sdk
  PUBLIC
    FILE_SET
    HEADERS
    BASE_DIRS
    "${${PROJECT_NAME}_SOURCE_DIR}/include"
    "${${PROJECT_NAME}_SOURCE_DIR}/external"
    "${${PROJECT_NAME}_BINARY_DIR}/external"
)
add_library(
  CasperSDK::casper_sdk
  ALIAS
  casper_sdk
)
set_target_properties(
  casper_sdk
  PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}"
    VERSION "1.0.0"
    SOVERSION 1
)

if(NOT WIN32)
  target_compile_definitions(casper_sdk INTERFACE CPPHTTPLIB_USE_POLL=true)
endif()

add_library(casper_sdk-common OBJECT "")
add_library(casper_sdk_cryptopp OBJECT "")
target_include_directories(
  casper_sdk-common SYSTEM
  PUBLIC "${${PROJECT_NAME}_SOURCE_DIR}/external"
         "${${PROJECT_NAME}_SOURCE_DIR}/external/cryptopp-pem"
)
target_include_directories(
  casper_sdk-common PUBLIC "${${PROJECT_NAME}_SOURCE_DIR}/include"
)
add_library(
  CasperSDK::casper_sdk_obj
  ALIAS
  casper_sdk
)

otcommon_pch_pic_workaround(casper_sdk)
otcommon_pch_pic_workaround(casper_sdk-common)
caspersdk_configure_cxx_target(casper_sdk)
caspersdk_configure_cxx_target(casper_sdk-common)

include(caspersdk-add-public-header)
include(caspersdk-add-sources)
include(caspersdk-link-external)
include(caspersdk-link-internal)

add_subdirectory(src)

otcommon_print_target_compiler_flags(casper_sdk-common)

if(CASPERSDK_BUILD_TESTS)
  add_subdirectory(tests)
endif()

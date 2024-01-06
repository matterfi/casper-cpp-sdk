macro(caspersdk_print_build_details)
  if(CASPERSDK_STANDALONE)
    message(STATUS "Configuring ${PROJECT_NAME} as a regular library")
  else()
    message(STATUS "Configuring ${PROJECT_NAME} as an object library")
  endif()

  include(otcommon-print-build-details)
  otcommon_print_build_details(CASPERSDK_PEDANTIC_BUILD CASPERSDK_BUILD_TESTS)

  message(STATUS "Packaging------------------------------------")
  message(STATUS "Install headers:          ${CASPERSDK_INSTALL_HEADERS}")
  message(STATUS "Install CMake package:    ${CASPERSDK_INSTALL_CMAKE}")
  message(STATUS "Library install path:     ${CASPERSDK_LIBRARY_INSTALL_PATH}")
  message(STATUS "Header install path:      ${CASPERSDK_HEADER_INSTALL_PATH}")
  message(STATUS "CMake install path:       ${CASPERSDK_CMAKE_INSTALL_PATH}")

  message(STATUS "Developer -----------------------------------")
  message(STATUS "precompiled headers:      ${CASPERSDK_PCH}")
  message(STATUS "iwyu:                     ${CMAKE_CXX_INCLUDE_WHAT_YOU_USE}")
  message(
    STATUS "fix_includes:             ${${PROJECT_NAME}_FIX_INCLUDES_ARGS}"
  )
  message(STATUS "clang-tidy:               ${CASPERSDK_CLANG_TIDY}")
endmacro()

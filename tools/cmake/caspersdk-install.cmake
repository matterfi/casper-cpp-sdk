macro(caspersdk_install)
  if(CASPERSDK_STANDALONE)
    install(
      TARGETS casper_sdk casper_sdk_cryptopp
      DESTINATION "${CASPERSDK_LIBRARY_INSTALL_PATH}"
      EXPORT CasperSDK
      COMPONENT caspersdk_library
      FILE_SET HEADERS
      DESTINATION "${CASPERSDK_HEADER_INSTALL_PATH}"
      INCLUDES
      DESTINATION "${CASPERSDK_HEADER_INSTALL_PATH}"
    )
  endif()

  if(CASPERSDK_STANDALONE)
    set(PACKAGE_INIT_STRING "@PACKAGE_INIT@")
    set(MODULES_DIR_STRING "@PACKAGE_MODULES_DIR@")

    install(
      FILES
        "${${PROJECT_NAME}_SOURCE_DIR}/tools/cmake/FindCryptoPP.cmake"
        "${${PROJECT_NAME}_SOURCE_DIR}/tools/cmake/caspersdk-find-dependencies.cmake"
      DESTINATION "${CASPERSDK_CMAKE_INSTALL_PATH}"
      COMPONENT caspersdk_cmake_modules
    )

    configure_file(
      "${${PROJECT_NAME}_SOURCE_DIR}/tools/cmake/CasperSDKConfig.cmake.in"
      "${${PROJECT_NAME}_BINARY_DIR}/tools/cmake/CasperSDKConfig.cmake.in"
      @ONLY
    )

    otcommon_generate_cmake_files(
      "${${PROJECT_NAME}_BINARY_DIR}/tools/cmake/CasperSDKConfig.cmake.in"
      "cmake"
      "${CASPERSDK_CMAKE_INSTALL_PATH}"
    )

    if(CASPERSDK_INSTALL_LICENSE)
      install(
        FILES "${${PROJECT_NAME}_SOURCE_DIR}/LICENSE"
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}"
        COMPONENT caspersdk_documentation
        RENAME "${CASPERSDK_LICENSE_FILE_NAME}"
      )
    endif()
  endif()
endmacro()

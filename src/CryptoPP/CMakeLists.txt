target_sources(
  casper_sdk_cryptopp
  PRIVATE
    "${${PROJECT_NAME}_SOURCE_DIR}/external/cryptopp-pem/cryptopp/pem_common.cpp"
    "${${PROJECT_NAME}_SOURCE_DIR}/external/cryptopp-pem/cryptopp/pem_read.cpp"
    "${${PROJECT_NAME}_SOURCE_DIR}/external/cryptopp-pem/cryptopp/pem_write.cpp"
    "${${PROJECT_NAME}_SOURCE_DIR}/external/cryptopp-pem/cryptopp/x509cert.cpp"
)

target_include_directories(
  casper_sdk_cryptopp
  PRIVATE "${${PROJECT_NAME}_SOURCE_DIR}/external/cryptopp-pem"
)

if(CASPERSDK_USE_VCPKG_TARGETS)
  target_link_libraries(casper_sdk_cryptopp PUBLIC cryptopp::cryptopp)
  caspersdk_link_external(casper_sdk_cryptopp)
else()
  target_link_libraries(casper_sdk_cryptopp PUBLIC "${CRYPTOPP_LIBRARIES}")
  target_link_libraries(casper_sdk PUBLIC "${CRYPTOPP_LIBRARIES}")
  target_include_directories(
    casper_sdk_cryptopp PUBLIC "${CRYPTOPP_INCLUDE_DIRS}"
  )
  target_include_directories(casper_sdk PUBLIC "${CRYPTOPP_INCLUDE_DIRS}")
endif()

set_target_properties(
  casper_sdk_cryptopp
  PROPERTIES
    C_STANDARD "11"
    CXX_STANDARD "17"
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
    C_INCLUDE_WHAT_YOU_USE ""
    CXX_INCLUDE_WHAT_YOU_USE ""
    C_CLANG_TIDY ""
    CXX_CLANG_TIDY ""
    UNITY_BUILD OFF
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
)
caspersdk_add_sources($<TARGET_OBJECTS:casper_sdk_cryptopp>)
caspersdk_link_internal(casper_sdk_cryptopp)

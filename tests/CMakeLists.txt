# include_directories(
#     ${SDK_ROOT}/lib/cryptopp-pem
# )

add_executable(
  casper_cpp_sdk_tests
  ClientTest.cpp
  RpcTest.cpp
  CLValueByteSerializerTest.cpp
  DeployItemByteSerializerTest.cpp
)
caspersdk_configure_cxx_target(casper_cpp_sdk_tests)
target_link_libraries(casper_cpp_sdk_tests PRIVATE CasperSDK::casper_sdk)
target_include_directories(
  casper_cpp_sdk_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_test(NAME casper_cpp_sdk_tests COMMAND casper_cpp_sdk_tests)

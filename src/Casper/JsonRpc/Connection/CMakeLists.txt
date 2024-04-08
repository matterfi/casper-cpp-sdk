caspersdk_add_public_header("JsonRpc/Connection" "HttpLibConnector.h")
caspersdk_add_public_header("JsonRpc/Connection" "httplib.h")

target_sources(casper_sdk-common PRIVATE "HttpLibConnector.cpp" "httplib.cpp")

if(NOT WIN32)
  target_compile_definitions(casper_sdk-common PUBLIC CPPHTTPLIB_USE_POLL=true)
endif()

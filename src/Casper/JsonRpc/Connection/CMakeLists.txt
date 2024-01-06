caspersdk_add_public_header("JsonRpc/Connection" "HttpLibConnector.h")
caspersdk_add_public_header("JsonRpc/Connection" "httplib.h")

target_sources(casper_sdk-common PRIVATE "HttpLibConnector.cpp" "httplib.cpp")

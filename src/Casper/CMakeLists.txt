add_subdirectory(Algorithm)
add_subdirectory(ByteSerializers)
add_subdirectory(JsonRpc)
add_subdirectory(Types)
add_subdirectory(Utils)

target_sources(casper_sdk-common PRIVATE "Base.cpp" "CasperClient.cpp")
target_compile_definitions(
  casper_sdk-common
  PRIVATE $<$<PLATFORM_ID:Darwin,Linux>:CPPHTTPLIB_USE_POLL=true>
)

caspersdk_add_public_header("" "Base.h")
caspersdk_add_public_header("" "CasperClient.h")

caspersdk_add_sources($<TARGET_OBJECTS:casper_sdk-common>)

caspersdk_link_external(Boost::headers)
caspersdk_link_external(OpenSSL::Crypto)
caspersdk_link_external(OpenSSL::SSL)
caspersdk_link_external(spdlog::spdlog)
caspersdk_link_external(ZLIB::ZLIB)

if(CASPERSDK_USE_VCPKG_TARGETS)
  caspersdk_link_external(
    unofficial::brotli::brotlidec unofficial::brotli::brotlienc
  )
  caspersdk_link_external(magic_enum::magic_enum)
endif()

if(Threads_FOUND)
  caspersdk_link_external(Threads::Threads)
endif()

if(CMAKE_DL_LIBS)
  caspersdk_link_external(${CMAKE_DL_LIBS})
endif()

if(LIB_RT)
  caspersdk_link_external(${LIB_RT})
endif()

if(WIN32 AND CASPERSDK_STATIC_DEPENDENCIES)
  caspersdk_link_external(CRYPT32.LIB)
endif()

include(caspersdk-install)
caspersdk_install()

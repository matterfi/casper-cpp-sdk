set(Boost_NO_WARN_NEW_VERSIONS 1)
find_package(
  Boost
  1.75.0
  QUIET
  REQUIRED
  OPTIONAL_COMPONENTS variant
)

if(CASPERSDK_USE_VCPKG_TARGETS)
  find_package(
    unofficial-brotli
    CONFIG
    REQUIRED
  )
  find_package(
    cryptopp
    CONFIG
    REQUIRED
  )
  find_package(
    magic_enum
    CONFIG
    REQUIRED
  )
  find_package(
    spdlog
    CONFIG
    REQUIRED
  )
else()
  # TODO brotli
  find_package(CryptoPP REQUIRED)
  # TODO magic_enum
  find_package(spdlog REQUIRED)
endif()

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

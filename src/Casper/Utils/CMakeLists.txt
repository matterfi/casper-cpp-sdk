caspersdk_add_public_header("Utils" "CEP57Checksum.h")
caspersdk_add_public_header("Utils" "CryptoUtil.h")
caspersdk_add_public_header("Utils" "File.h")
caspersdk_add_public_header("Utils" "LogConfigurator.h")
caspersdk_add_public_header("Utils" "StringUtil.h")

target_sources(
  casper_sdk-common
  PRIVATE
    "CEP57Checksum.cpp"
    "CryptoUtil.cpp"
    "File.cpp"
    "LogConfigurator.cpp"
    "StringUtil.cpp"
)

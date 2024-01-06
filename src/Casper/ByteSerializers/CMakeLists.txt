caspersdk_add_public_header("ByteSerializers" "BaseByteSerializer.h")
caspersdk_add_public_header("ByteSerializers" "CLValueByteSerializer.h")
caspersdk_add_public_header("ByteSerializers" "DeployApprovalByteSerializer.h")
caspersdk_add_public_header("ByteSerializers" "DeployByteSerializer.h")
caspersdk_add_public_header(
  "ByteSerializers" "ExecutableDeployItemByteSerializer.h"
)
caspersdk_add_public_header("ByteSerializers" "GlobalStateKeyByteSerializer.h")
caspersdk_add_public_header("ByteSerializers" "NamedArgByteSerializer.h")

target_sources(
  casper_sdk-common
  PRIVATE
    "BaseByteSerializer.cpp"
    "CLValueByteSerializer.cpp"
    "DeployApprovalByteSerializer.cpp"
    "DeployByteSerializer.cpp"
    "ExecutableDeployItemByteSerializer.cpp"
    "GlobalStateKeyByteSerializer.cpp"
    "NamedArgByteSerializer.cpp"
)

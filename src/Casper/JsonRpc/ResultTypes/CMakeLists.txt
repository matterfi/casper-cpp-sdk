caspersdk_add_public_header("JsonRpc/ResultTypes" "GetAuctionInfoResult.h")
caspersdk_add_public_header("JsonRpc/ResultTypes" "GetBalanceResult.h")
caspersdk_add_public_header("JsonRpc/ResultTypes" "GetBlockResult.h")
caspersdk_add_public_header("JsonRpc/ResultTypes" "GetBlockTransfersResult.h")
caspersdk_add_public_header("JsonRpc/ResultTypes" "GetDeployResult.h")
caspersdk_add_public_header("JsonRpc/ResultTypes" "GetDictionaryItemResult.h")
caspersdk_add_public_header("JsonRpc/ResultTypes" "GetEraInfoResult.h")
caspersdk_add_public_header("JsonRpc/ResultTypes" "GetItemResult.h")
caspersdk_add_public_header("JsonRpc/ResultTypes" "GetStateRootHashResult.h")
caspersdk_add_public_header("JsonRpc/ResultTypes" "GetStatusResult.h")
caspersdk_add_public_header("JsonRpc/ResultTypes" "InfoGetPeersResult.h")
caspersdk_add_public_header("JsonRpc/ResultTypes" "PutDeployResult.h")
caspersdk_add_public_header("JsonRpc/ResultTypes" "QueryGlobalStateResult.h")
caspersdk_add_public_header("JsonRpc/ResultTypes" "RpcResult.h")

target_sources(
  casper_sdk-common
  PRIVATE
    "GetAuctionInfoResult.cpp"
    "GetBalanceResult.cpp"
    "GetBlockResult.cpp"
    "GetBlockTransfersResult.cpp"
    "GetDeployResult.cpp"
    "GetDictionaryItemResult.cpp"
    "GetEraInfoResult.cpp"
    "GetItemResult.cpp"
    "GetStateRootHashResult.cpp"
    "GetStatusResult.cpp"
    "InfoGetPeersResult.cpp"
    "PutDeployResult.cpp"
    "QueryGlobalStateResult.cpp"
    "RpcResult.cpp"
)

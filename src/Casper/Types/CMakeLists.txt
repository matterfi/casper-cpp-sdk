caspersdk_add_public_header("Types" "AccessRights.h")
caspersdk_add_public_header("Types" "Account.h")
caspersdk_add_public_header("Types" "AuctionState.h")
caspersdk_add_public_header("Types" "Bid.h")
caspersdk_add_public_header("Types" "Block.h")
caspersdk_add_public_header("Types" "BlockInfo.h")
caspersdk_add_public_header("Types" "CLConverter.h")
caspersdk_add_public_header("Types" "CLType.h")
caspersdk_add_public_header("Types" "CLTypeParsed.h")
caspersdk_add_public_header("Types" "CLValue.h")
caspersdk_add_public_header("Types" "Configuration.h")
caspersdk_add_public_header("Types" "Contract.h")
caspersdk_add_public_header("Types" "ContractPackage.h")
caspersdk_add_public_header("Types" "Delegator.h")
caspersdk_add_public_header("Types" "DeployApproval.h")
caspersdk_add_public_header("Types" "Deploy.h")
caspersdk_add_public_header("Types" "DeployHeader.h")
caspersdk_add_public_header("Types" "DeployInfo.h")
caspersdk_add_public_header("Types" "ED25519Key.h")
caspersdk_add_public_header("Types" "EntryPoint.h")
caspersdk_add_public_header("Types" "EraEnd.h")
caspersdk_add_public_header("Types" "EraInfo.h")
caspersdk_add_public_header("Types" "EraSummary.h")
caspersdk_add_public_header("Types" "EraValidators.h")
caspersdk_add_public_header("Types" "ExecutableDeployItem.h")
caspersdk_add_public_header("Types" "ExecutionEffect.h")
caspersdk_add_public_header("Types" "ExecutionResult.h")
caspersdk_add_public_header("Types" "GlobalStateKey.h")
caspersdk_add_public_header("Types" "KeyAlgo.h")
caspersdk_add_public_header("Types" "KeyPair.h")
caspersdk_add_public_header("Types" "ModuleBytes.h")
caspersdk_add_public_header("Types" "NamedArg.h")
caspersdk_add_public_header("Types" "NamedKey.h")
caspersdk_add_public_header("Types" "NextUpgrade.h")
caspersdk_add_public_header("Types" "Operation.h")
caspersdk_add_public_header("Types" "Peer.h")
caspersdk_add_public_header("Types" "PublicKey.h")
caspersdk_add_public_header("Types" "Secp256k1Key.h")
caspersdk_add_public_header("Types" "SeigniorageAllocation.h")
caspersdk_add_public_header("Types" "Signature.h")
caspersdk_add_public_header("Types" "StoredContractByHash.h")
caspersdk_add_public_header("Types" "StoredContractByName.h")
caspersdk_add_public_header("Types" "StoredValue.h")
caspersdk_add_public_header("Types" "StoredVersionedContractByHash.h")
caspersdk_add_public_header("Types" "StoredVersionedContractByName.h")
caspersdk_add_public_header("Types" "TransferDeployItem.h")
caspersdk_add_public_header("Types" "Transfer.h")
caspersdk_add_public_header("Types" "TransformEntry.h")
caspersdk_add_public_header("Types" "Transform.h")
caspersdk_add_public_header("Types" "UnbondingPurse.h")
caspersdk_add_public_header("Types" "URef.h")
caspersdk_add_public_header("Types" "ValidatorBid.h")
caspersdk_add_public_header("Types" "ValidatorChanges.h")
caspersdk_add_public_header("Types" "ValidatorWeight.h")
caspersdk_add_public_header("Types" "VestingSchedule.h")

target_sources(
  casper_sdk-common
  PRIVATE
    "AccessRights.cpp"
    "Account.cpp"
    "AuctionState.cpp"
    "Bid.cpp"
    "Block.cpp"
    "BlockInfo.cpp"
    "CLConverter.cpp"
    "CLType.cpp"
    "CLTypeParsed.cpp"
    "CLValue.cpp"
    "Configuration.cpp"
    "Contract.cpp"
    "ContractPackage.cpp"
    "Delegator.cpp"
    "Deploy.cpp"
    "DeployApproval.cpp"
    "DeployHeader.cpp"
    "DeployInfo.cpp"
    "ED25519Key.cpp"
    "EntryPoint.cpp"
    "EraEnd.cpp"
    "EraInfo.cpp"
    "EraSummary.cpp"
    "EraValidators.cpp"
    "ExecutableDeployItem.cpp"
    "ExecutionEffect.cpp"
    "ExecutionResult.cpp"
    "GlobalStateKey.cpp"
    "KeyAlgo.cpp"
    "KeyPair.cpp"
    "ModuleBytes.cpp"
    "NamedArg.cpp"
    "NamedKey.cpp"
    "NextUpgrade.cpp"
    "Operation.cpp"
    "Peer.cpp"
    "PublicKey.cpp"
    "Secp256k1Key.cpp"
    "SeigniorageAllocation.cpp"
    "Signature.cpp"
    "StoredContractByHash.cpp"
    "StoredContractByName.cpp"
    "StoredValue.cpp"
    "StoredVersionedContractByHash.cpp"
    "StoredVersionedContractByName.cpp"
    "Transfer.cpp"
    "TransferDeployItem.cpp"
    "Transform.cpp"
    "TransformEntry.cpp"
    "URef.cpp"
    "UnbondingPurse.cpp"
    "ValidatorBid.cpp"
    "ValidatorChanges.cpp"
    "ValidatorWeight.cpp"
    "VestingSchedule.cpp"
)

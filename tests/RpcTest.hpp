#pragma once

#include <sstream>

#include "Casper/ByteSerializers/DeployByteSerializer.h"
#include "Casper/CasperClient.h"
#include "Casper/Types/CLConverter.h"
#include "Casper/Types/CLType.h"
#include "Casper/Types/GlobalStateKey.h"
#include "Casper/Types/PublicKey.h"
#include "Casper/Utils/CryptoUtil.h"


namespace Casper {

void test1(void);

void test2(void);

void infoGetPeers_Test(void);

void chainGetStateRootHash_with_blockHeightTest(void);

void chainGetStateRootHash_with_invalidBlockHeightTest(void);

void chainGetStateRootHash_with_blockHashTest(void);

void chainGetStateRootHash_with_emptyParameterTest(void);

void infoGetDeploy_with_deployHashTest(void);

void infoGetDeploy_with_invalidDeployHashTest(void);

void infoGetStatus_with_emptyParameterTest(void);

void chainGetBlockTransfers_with_blockHashTest(void);

void chainGetBlock_with_blockHashTest(void);

void chainGetEraInfoBySwitchBlock_with_blockHashTest(void);

void stateGetItem_with_keyTest(void);

void stateGetItem_with_invalidKeyTest(void);

void stateGetDictionaryItem_with_keyTest(void);

void stateGetBalance_with_urefTest(void);

void stateGetBalance_with_invalidUrefTest(void);

void stateGetAuctionInfo_with_blockHashTest(void);

void PutDeploy_Transfer_Test(void);

void PutDeploy_StoredContractByName_Test(void);

void PutDeploy_StoredContractByHash_Test(void);

void globalStateKey_Simple_Test(void);

void QueryGlobalState_with_keyTest(void);

}  // namespace Casper

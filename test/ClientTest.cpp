#include "CasperClient.h"
#include "Types/GlobalStateKey.cpp"
#include "Types/PublicKey.h"
#include "Utils/CryptoUtil.h"
#include <sstream>
#include "Types/CLType.h"
#include "Types/CLTypeParsedConverter.h"
#include "acutest.h"

// Helper function to compare two strings in a case insensitive way
bool iequals(const std::string& a, const std::string& b) {
  return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                    [](char a, char b) { return tolower(a) == tolower(b); });
}

/// Helper function to print a result object
template <typename T>
void printResult(const T& result, const std::string& rpc_call_name,
                 int indent = 2) {
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << rpc_call_name << std::endl;

  nlohmann::json resultJSON;
  nlohmann::to_json(resultJSON, result);

  std::cout << resultJSON.dump(indent) << std::endl;
}

/**
 * @brief Check the "info_get_peers" rpc function. The size of the return value
 * should be greater than 0.
 *
 */
void infoGetPeers_Test() {
  Casper::Client client(CASPER_TEST_ADDRESS);

  auto result = client.GetNodePeers();
  size_t result_size = result.peers.size();

  TEST_ASSERT(result_size > 0);
}

/**
 * @brief Check the "get_state_root_hash" rpc function with an example height.
 * Compare the result with the expected hash of the state.
 *
 */
void chainGetStateRootHash_with_blockHeightTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);

  uint64_t block_height = 10;
  std::string result = client.GetStateRootHash(block_height).state_root_hash;
  std::string expected_result =
      "4d180287e6eb3dad5173864e30d7653c01fcdef8bc3ee31db4a0707367154ccf";

  TEST_ASSERT(iequals(result, expected_result));
}

/**
 * @brief Check "get_state_root_hash" rpc function with an
 * invalid height.
 *
 *
 */
void chainGetStateRootHash_with_invalidBlockHeightTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);

  uint64_t block_height = 100000000;
  try {
    std::string result = client.GetStateRootHash(block_height).state_root_hash;
  } catch (const jsonrpccxx::JsonRpcException& e) {
    return;
  }

  TEST_ASSERT(false);
}

/**
 * @brief Check the "get_state_root_hash" rpc function with an example block
 * hash. Compare the result with the expected hash of the state.
 *
 */
void chainGetStateRootHash_with_blockHashTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);

  std::string block_hash =
      "9511adf5ed36ccba48d71840fd558d4397c9eb0470d1e25711b5419632a6f55a";
  std::string result = client.GetStateRootHash(block_hash).state_root_hash;
  std::string expected_result =
      "9aa3c10d4db2e02adb464458c7a09d1df2ed551be02d2c7bbdbe847d3731e84f";

  TEST_ASSERT(iequals(result, expected_result));
}

/**
 * @brief Check the "get_state_root_hash" rpc function without a variable.
 * Compare the result with an empty string.
 *
 */
void chainGetStateRootHash_with_emptyParameterTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);

  std::string result = client.GetStateRootHash().state_root_hash;

  TEST_ASSERT(result != "");
}

/**
 * @brief Check the "info_get_deploy" rpc function
 *
 */
void infoGetDeploy_with_deployHashTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);
  std::string deploy_hash =
      "8e535d2baed76141ab47fd93b04dd61f65a07893b7c950022978a2b29628edd7";

  nlohmann::json deploy_result = client.GetDeployInfo(deploy_hash);

  TEST_ASSERT(deploy_result.at("api_version") != "");
  TEST_ASSERT(deploy_result.find("deploy") != deploy_result.end());

  nlohmann::json& deploy_info = deploy_result.at("deploy");
  TEST_ASSERT(iequals(
      deploy_info.at("hash"),
      "8e535d2baed76141ab47fd93b04dd61f65a07893b7c950022978a2b29628edd7"));

  nlohmann::json& deploy_header = deploy_info.at("header");
  TEST_ASSERT(iequals(
      deploy_header.at("account"),
      "011fa7f49ed9887f1bd0bceac567dd6a38087e2896411d74d3f8d1c03a3f325828"));

  TEST_ASSERT(iequals(
      deploy_header.at("body_hash"),
      "11f5a10f791fd6ac8b12d52298b7d1db7bd91e8c15b5d1330fd16d792257693c"));
  TEST_ASSERT(iequals(deploy_header.at("chain_name"), "casper-test"));
  TEST_ASSERT(deploy_header.at("gas_price") == 1);
}

/**
 * @brief Check the "info_get_deploy" rpc function with an invalid deploy hash
 * parameter
 *
 */
void infoGetDeploy_with_invalidDeployHashTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);
  std::string deploy_hash = "ffffffffffffffffffff";

  try {
    nlohmann::json deploy_result = client.GetDeployInfo(deploy_hash);
  } catch (const jsonrpccxx::JsonRpcException& e) {
    return;
  }
  TEST_ASSERT(false);
}

/**
 * @brief Check the "info_get_status" rpc function. Check the result
 * variables.
 *
 */
void infoGetStatus_with_emptyParameterTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);
  Casper::GetStatusResult result = client.GetStatusInfo();

  TEST_ASSERT(result.api_version != "");

  std::string expected_chainspec_name = "casper-test";
  TEST_ASSERT(iequals(result.chainspec_name, expected_chainspec_name));

  TEST_ASSERT(result.starting_state_root_hash != "");

  if (result.our_public_signing_key.has_value()) {
    TEST_ASSERT(result.our_public_signing_key.value().ToString() != "");
  }

  if (result.last_added_block_info.has_value()) {
    auto& last_block = result.last_added_block_info.value();
    TEST_ASSERT(last_block.hash != "");
    TEST_ASSERT(last_block.height >= 0);
    TEST_ASSERT(last_block.timestamp != "");
    TEST_ASSERT(last_block.state_root_hash != "");
    TEST_ASSERT(last_block.creator.ToString() != "");
  }

  if (result.peers.size() > 0) {
    TEST_ASSERT(result.peers[0].address != "");
    TEST_ASSERT(result.peers[0].node_id != "");
  }

  TEST_ASSERT(result.build_version != "");
  TEST_ASSERT(result.uptime != "");
}

/**
 * @brief Check the "chain_get_block_transfers" rpc function.
 *
 */
void chainGetBlockTransfers_with_blockHashTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);

  // Call the rpc function
  std::string block_hash =
      "35f86b6ab5e13b823daee5d23f3373f6b35048e0b0ea993adfadc5ba8ee7aae5";
  Casper::StringUtil::toLower(block_hash);
  Casper::GetBlockTransfersResult result = client.GetBlockTransfers(block_hash);

  // Expected Values
  std::string expected_block_hash = block_hash;
  big_int expected_amount = 199000000000;
  std::string expected_deploy_hash =
      "8e535d2baed76141ab47fd93b04dd61f65a07893b7c950022978a2b29628edd7";
  std::string expected_from =
      "account-hash-"
      "308d2a0eCF66bDAcAC5Cf6184C732D83DCeB48A859169e5680FE17cF32Bb974F";
  big_int expected_gas = 0;
  std::string expected_source =
      "uref-5ce1d189e8ccafdd5a959088ffd870f54b29bd5afeb05950dddcc12ec7dcbe90-"
      "007";
  std::string expected_target =
      "uref-c9733355d61aa2a36721d9d1081eebcfe5dde94f82386b3d75163fee894d292a-"
      "007";

  TEST_ASSERT(result.api_version != "");

  TEST_ASSERT(result.block_hash.has_value());
  TEST_ASSERT(iequals(result.block_hash.value(), expected_block_hash));

  // check transfers
  TEST_ASSERT(result.transfers.has_value());
  TEST_ASSERT(result.transfers.value().size() > 0);

  auto& transfers = result.transfers.value();
  auto test_transfer = std::find_if(
      transfers.begin(), transfers.end(), [&](const Casper::Transfer& x) {
        return iequals(x.deploy_hash, expected_deploy_hash);
      });
  TEST_ASSERT(test_transfer != transfers.end());

  // Actual Values
  big_int current_amount = test_transfer->amount;
  std::string current_deploy_hash = test_transfer->deploy_hash;
  std::string current_from = test_transfer->from.ToString();
  big_int current_gas = test_transfer->gas;
  std::string current_source = test_transfer->source.ToString();
  std::string current_target = test_transfer->target.ToString();

  // tests for the first transfer
  TEST_ASSERT(current_amount == expected_amount);
  TEST_ASSERT(iequals(current_deploy_hash, expected_deploy_hash));
  TEST_ASSERT(iequals(current_from, expected_from));
  TEST_ASSERT(current_gas == expected_gas);
  TEST_ASSERT(iequals(current_source, expected_source));
  TEST_ASSERT(iequals(current_target, expected_target));
}

/**
 * @brief Check the "chain_get_block" rpc function
 *
 */
void chainGetBlock_with_blockHashTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);

  std::string block_hash =
      "acc4646f35cc1d59b24381547a4d2dc1c992a202b6165f3bf68d3f23c2b93330";
  Casper::GetBlockResult blockResult = client.GetBlock(block_hash);

  TEST_ASSERT(blockResult.api_version != "");

  TEST_ASSERT(blockResult.block.has_value());
  auto& current_block = blockResult.block.value();

  TEST_ASSERT(iequals(
      current_block.hash,
      "acc4646f35cc1d59b24381547a4d2dc1c992a202b6165f3bf68d3f23c2b93330"));

  // block header
  TEST_ASSERT(iequals(
      current_block.header.parent_hash,
      "e23b5f98258aff36716a8f60ca8d57c049216eedd88e6c7e14df7a6cfbadca73"));

  TEST_ASSERT(iequals(
      current_block.header.state_root_hash,
      "f5abb3964382e0dde4bc3ec38414f43f325f5dcc6493d5a7c4037972793fb302"));

  TEST_ASSERT(iequals(
      current_block.header.body_hash,
      "e1786ce884cf41abbc758b0795ee3223daec5fb8015791ced0f8ee66deec8ee3"));

  TEST_ASSERT(iequals(
      current_block.header.accumulated_seed,
      "35b5d33db0b43df3971831880f51023b37a468ad54494316ec26af4c61904532"));

  TEST_ASSERT(current_block.header.timestamp != "");
  TEST_ASSERT(current_block.header.era_id != 0);
  TEST_ASSERT(current_block.header.height == 532041);
  TEST_ASSERT(current_block.header.protocol_version != "");

  // block body
  TEST_ASSERT(current_block.body.deploy_hashes.size() >= 0);

  TEST_ASSERT(iequals(
      current_block.body.proposer.ToString(),
      "01cd807fb41345d8dD5A61da7991e1468173acbEE53920E4DFe0D28Cb8825AC664"));

  TEST_ASSERT(current_block.body.transfer_hashes.size() >= 0);

  // block proofs
  TEST_ASSERT(current_block.proofs.size() > 0);
  TEST_ASSERT(current_block.proofs[0].public_key.ToString() != "");
  TEST_ASSERT(current_block.proofs[0].signature.ToString() != "");
}

/**
 * @brief Check the "chain_get_era_info_by_switch_block" rpc function
 *
 */
void chainGetEraInfoBySwitchBlock_with_blockHashTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);
  Casper::GetEraInfoResult result = client.GetEraInfoBySwitchBlock(
      "d2077716e5b8796723c5720237239720f54e6ada54e3357f2c4896f2a51a6d8f");

  TEST_ASSERT(result.api_version != "");
  TEST_ASSERT(result.era_summary.has_value());
  TEST_ASSERT(result.era_summary.value().era_id != 0);
  TEST_ASSERT(result.era_summary.value().block_hash != "");
  TEST_ASSERT(result.era_summary.value().merkle_proof != "");
  TEST_ASSERT(result.era_summary.value().state_root_hash != "");
  TEST_ASSERT(result.era_summary.value().stored_value.era_info.has_value());
  TEST_ASSERT(result.era_summary.value()
                  .stored_value.era_info.value()
                  .seigniorage_allocations.size() > 0);

  int validator_cnt = 0;
  int delegator_cnt = 0;

  for (int i = 0; i < result.era_summary.value()
                          .stored_value.era_info.value()
                          .seigniorage_allocations.size();
       i++) {
    if (delegator_cnt == 1 && validator_cnt == 1) {
      break;
    }

    bool is_delegator = result.era_summary.value()
                            .stored_value.era_info.value()
                            .seigniorage_allocations[i]
                            .is_delegator;

    if (is_delegator == true && delegator_cnt == 0) {
      delegator_cnt++;
      TEST_ASSERT(result.era_summary.value()
                      .stored_value.era_info.value()
                      .seigniorage_allocations[i]
                      .delegator_public_key.ToString() != "");
      TEST_ASSERT(result.era_summary.value()
                      .stored_value.era_info.value()
                      .seigniorage_allocations[i]
                      .amount >= 0);
    } else if (is_delegator == false && validator_cnt == 0) {
      validator_cnt++;
      TEST_ASSERT(result.era_summary.value()
                      .stored_value.era_info.value()
                      .seigniorage_allocations[i]
                      .validator_public_key.ToString() != "");
      TEST_ASSERT(result.era_summary.value()
                      .stored_value.era_info.value()
                      .seigniorage_allocations[i]
                      .amount >= 0);
    }
  }
}

/**
 * @brief Check the "state_get_item" rpc function
 *
 */
void stateGetItem_with_keyTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);
  std::string state_root_hash =
      "39f2800688b94f68ca640b26c7d0f50a90d2ce9af55c9484e66151b544345303";
  std::string key =
      "transfer-"
      "9f5fe878c29fc3bf537c0509ec5abe1781a72bb6a3197a440e3e68247fba5909";

  Casper::GetItemResult result = client.GetItem(state_root_hash, key);

  // tests
  TEST_ASSERT(result.api_version != "");
  TEST_ASSERT(result.merkle_proof != "");

  TEST_ASSERT(result.stored_value.transfer.has_value());

  auto& current_transfer = result.stored_value.transfer.value();

  TEST_ASSERT(iequals(
      current_transfer.deploy_hash,
      "8e535d2baed76141ab47fd93b04dd61f65a07893b7c950022978a2b29628edd7"));

  TEST_ASSERT(iequals(
      current_transfer.from.ToString(),
      "account-hash-"
      "308d2a0eCF66bDAcAC5Cf6184C732D83DCeB48A859169e5680FE17cF32Bb974F"));

  TEST_ASSERT(iequals(
      current_transfer.source.ToString(),
      "uref-5ce1d189e8ccafdd5a959088ffd870f54b29bd5afeb05950dddcc12ec7dcbe90-"
      "007"));

  TEST_ASSERT(iequals(
      current_transfer.target.ToString(),
      "uref-c9733355d61aa2a36721d9d1081eebcfe5dde94f82386b3d75163fee894d292a-"
      "007"));

  TEST_ASSERT(current_transfer.amount == 199000000000);
  TEST_ASSERT(current_transfer.gas == 0);
}

/**
 * @brief Check the "state_get_item" rpc function with an invalid key and a
 * state root hash
 *
 */
void stateGetItem_with_invalidKeyTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);
  std::string state_root_hash = "NOT_EXIST_STATE_ROOT_HASH";
  std::string key = "NON_EXISTING_KEY";
  try {
    Casper::GetItemResult result = client.GetItem(state_root_hash, key);
  } catch (const jsonrpccxx::JsonRpcException& e) {
    return;
  }
  TEST_ASSERT(false);
}

/**
 * @brief Check the "state_get_dictionary_item" rpc function by URef
 *
 */
void stateGetDictionaryItem_with_keyTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);
  std::string state_root_hash =
      "322b8d17faea2ee780b9b952a25a86520d36a78e20113f0658ae0b29a68a7384";

  std::string item_key =
      "dictionary-"
      "5d3e90f064798d54e5e53643c4fce0cbb1024aadcad1586cc4b7c1358a530373";

  nlohmann::json dictionaryItemResult =
      client.GetDictionaryItem(state_root_hash, item_key);

  TEST_ASSERT(dictionaryItemResult.at("api_version") != "");

  TEST_ASSERT(dictionaryItemResult.at("dictionary_key") != "");
  TEST_ASSERT(dictionaryItemResult.at("merkle_proof") != "");

  TEST_ASSERT(!dictionaryItemResult.at("stored_value").at("CLValue").is_null());
  TEST_ASSERT(
      iequals(dictionaryItemResult.at("stored_value").at("CLValue").at("bytes"),
              "090000006162635F76616c7565"));

  TEST_ASSERT(iequals(
      dictionaryItemResult.at("stored_value").at("CLValue").at("cl_type"),
      "String"));
}

/**
 * @brief Check the "state_get_balance" rpc function
 *
 */
void stateGetBalance_with_urefTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);

  std::string purse_uref =
      "uref-54fd72455872082a254b0160e94a86245acd0c441f526688bda1261d0969057a-"
      "007";
  std::string state_root_hash =
      "66eb7e43886c908aae8246ba2d22aa30d21e1c187a38fa3093f14e4a4219dd6c";

  Casper::GetBalanceResult result =
      client.GetAccountBalance(purse_uref, state_root_hash);

  TEST_ASSERT(result.api_version != "");
  TEST_ASSERT(result.balance_value >= 0);
  TEST_ASSERT(result.merkle_proof != "");
}

/**
 * @brief Check the "state_get_balance" rpc function with an invalid uref and a
 * state root hash
 *
 */
void stateGetBalance_with_invalidUrefTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);

  std::string purse_uref = "non-uref-ffff";
  std::string state_root_hash = "ffff";
  try {
    Casper::GetBalanceResult result =
        client.GetAccountBalance(purse_uref, state_root_hash);
  } catch (const jsonrpccxx::JsonRpcException& e) {
    return;
  }
  TEST_ASSERT(false);
}

/**
 * @brief Check the "state_get_auction_info" rpc function
 *
 */
void stateGetAuctionInfo_with_blockHashTest() {
  Casper::Client client(CASPER_TEST_ADDRESS);

  std::string block_hash =
      "a5ce9e1ea4ff786cf1eb9dfbe3a79f70ae33d723134a060910a2db80daf85bab";

  Casper::GetAuctionInfoResult auction_result =
      client.GetAuctionInfo(block_hash);

  // tests
  TEST_ASSERT(auction_result.api_version != "");

  TEST_ASSERT(iequals(
      auction_result.auction_state.state_root_hash,
      "fb9847a919b0745e3bea1cc25f3ad4ad5fee0e18fe4bebd303a9e7a93508ddb8"));

  TEST_ASSERT(auction_result.auction_state.block_height == 569706);

  TEST_ASSERT(auction_result.auction_state.era_validators.size() > 0);
  TEST_ASSERT(auction_result.auction_state.era_validators[0].era_id > 0);
  TEST_ASSERT(
      auction_result.auction_state.era_validators[0].validator_weights.size() >
      0);
  TEST_ASSERT(auction_result.auction_state.era_validators[0]
                  .validator_weights[0]
                  .public_key.ToString() != "");
  TEST_ASSERT(auction_result.auction_state.era_validators[0]
                  .validator_weights[0]
                  .weight > 0);

  TEST_ASSERT(auction_result.auction_state.bids.size() > 0);
  TEST_ASSERT(auction_result.auction_state.bids[0].public_key.ToString() != "");

  TEST_ASSERT(auction_result.auction_state.bids[0]
                  .bid.validator_public_key.ToString() != "");
  TEST_ASSERT(
      auction_result.auction_state.bids[0].bid.bonding_purse.ToString() != "");
  TEST_ASSERT(auction_result.auction_state.bids[0].bid.staked_amount > 0);
  TEST_ASSERT(auction_result.auction_state.bids[0].bid.delegation_rate > 0);
}

/// <summary>
/// Check the Casper lower-case convert function
/// </summary>
void stringUtil_toLowerTest() {
  std::string str = "Hello World";
  std::string str_lower = "hello world";
  TEST_ASSERT(str_lower == Casper::StringUtil::toLower(str));
}

/// <summary>
/// Check the public key to account hash convert function
/// </summary>
void publicKey_getAccountHashTest() {
  Casper::PublicKey publicKey = Casper::PublicKey::FromHexString(
      "01cd807fb41345d8dd5a61da7991e1468173acbee53920e4dfe0d28cb8825ac664");

  std::string lower_case_account_hash = publicKey.GetAccountHash();
  Casper::StringUtil::toLower(lower_case_account_hash);

  std::string expected_account_hash =
      "account-hash-"
      "998c5fd4e7b568bedd78e05555c83c61893dc5d8546ce0bec8b30e1c570f21aa";

  TEST_ASSERT(iequals(lower_case_account_hash, expected_account_hash));
}

void serializeBoolTest() {
  std::string bool_bytes1 = "00";
  bool expected_value1 = false;
  bool actual_value1 = Casper::boolDecode(bool_bytes1);
  TEST_ASSERT(expected_value1 == actual_value1);

  std::string encoded_value1 = Casper::boolEncode(expected_value1);
  TEST_ASSERT(iequals(bool_bytes1, encoded_value1));

  // ---

  std::string bool_bytes2 = "01";
  bool expected_value2 = true;
  bool actual_value2 = Casper::boolDecode(bool_bytes2);
  TEST_ASSERT(expected_value2 == actual_value2);

  std::string encoded_value2 = Casper::boolEncode(expected_value2);
  TEST_ASSERT(iequals(bool_bytes2, encoded_value2));
}

void serializeI32Test() {
  std::string i32_bytes1 = "e8030000";
  int32_t expected_value1 = 1000;
  int32_t actual_value1 = Casper::i32Decode(i32_bytes1);
  TEST_ASSERT(actual_value1 == expected_value1);

  std::string encoded_value1 = Casper::i32Encode(expected_value1);
  TEST_ASSERT(iequals(i32_bytes1, encoded_value1));

  // ---

  std::string i32_bytes2 = "1d290d71";
  int32_t expected_value2 = 1896687901;
  int32_t actual_value2 = Casper::i32Decode(i32_bytes2);
  TEST_ASSERT(actual_value2 == expected_value2);

  std::string encoded_value2 = Casper::i32Encode(expected_value2);
  TEST_ASSERT(iequals(i32_bytes2, encoded_value2));
}

void serializeI64Test() {
  std::string i64_bytes1 = "7f33d9dcf601ab02";
  int64_t expected_value1 = 192249568872182655;
  int64_t actual_value1 = Casper::i64Decode(i64_bytes1);
  TEST_ASSERT(actual_value1 == expected_value1);

  std::string encoded_value1 = Casper::i64Encode(expected_value1);
  TEST_ASSERT(iequals(i64_bytes1, encoded_value1));
}

void serializeU8Test() {
  std::string u8_bytes1 = "07";
  uint8_t expected_value1 = 7;
  uint8_t actual_value1 = Casper::u8Decode(u8_bytes1);
  TEST_ASSERT(actual_value1 == expected_value1);

  std::string encoded_value1 = Casper::u8Encode(expected_value1);
  TEST_ASSERT(iequals(u8_bytes1, encoded_value1));
}

void serializeU32Test() {
  std::string u32_bytes1 = "01000000";
  uint32_t expected_value1 = 1;
  uint32_t actual_value1 = Casper::u32Decode(u32_bytes1);
  TEST_ASSERT(actual_value1 == expected_value1);

  std::string encoded_value1 = Casper::u32Encode(expected_value1);
  TEST_ASSERT(iequals(u32_bytes1, encoded_value1));

  // ---

  std::string u32_bytes2 = "00040000";
  uint32_t expected_value2 = 1024;
  uint32_t actual_value2 = Casper::u32Decode(u32_bytes2);
  TEST_ASSERT(actual_value2 == expected_value2);

  std::string encoded_value2 = Casper::u32Encode(expected_value2);
  TEST_ASSERT(iequals(u32_bytes2, encoded_value2));
}

void serializeU64Test() {
  std::string u64_bytes1 = "39f37bf07f010000";
  uint64_t expected_value1 = 1649007129401;
  uint64_t actual_value1 = Casper::u64Decode(u64_bytes1);
  TEST_ASSERT(actual_value1 == expected_value1);

  std::string encoded_value1 = Casper::u64Encode(expected_value1);
  TEST_ASSERT(iequals(u64_bytes1, encoded_value1));

  // ---

  std::string u64_bytes2 = "be7ab73d80010000";
  uint64_t expected_value2 = 1650302876350;
  uint64_t actual_value2 = Casper::u64Decode(u64_bytes2);
  TEST_ASSERT(actual_value2 == expected_value2);

  std::string encoded_value2 = Casper::u64Encode(expected_value2);
  TEST_ASSERT(iequals(u64_bytes2, encoded_value2));
}

void serializeU128Test() {
  std::string u128_bytes1 = "060000C0D0E0F0";
  big_int expected_value1{"264848365584384"};
  big_int actual_value1 = Casper::u128Decode(u128_bytes1);
  TEST_ASSERT(actual_value1 == expected_value1);

  std::string encoded_value1 = Casper::u128Encode(expected_value1);
  TEST_ASSERT(iequals(u128_bytes1, encoded_value1));
}

void serializeU256Test() {
  std::string u256_bytes1 =
      "20ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
  big_int expected_value1{
      "115792089237316195423570985008687907853269984665640564039457584007913129"
      "639935"};
  big_int actual_value1 = Casper::u256Decode(u256_bytes1);
  TEST_ASSERT(actual_value1 == expected_value1);

  std::string encoded_value1 = Casper::u256Encode(expected_value1);
  TEST_ASSERT(iequals(u256_bytes1, encoded_value1));

  // ---

  std::string u256_bytes2 = "020e08";
  big_int expected_value2{"2062"};
  big_int actual_value2 = Casper::u256Decode(u256_bytes2);
  TEST_ASSERT(actual_value2 == expected_value2);

  std::string encoded_value2 = Casper::u256Encode(expected_value2);
  TEST_ASSERT(iequals(u256_bytes2, encoded_value2));
}

void serializeU512Test() {
  std::string u512_bytes1 = "050e2389f603";
  big_int expected_value1{"17021084430"};
  big_int actual_value1 = Casper::u512Decode(u512_bytes1);
  TEST_ASSERT(actual_value1 == expected_value1);

  std::string encoded_value1 = Casper::u512Encode(expected_value1);
  TEST_ASSERT(iequals(u512_bytes1, encoded_value1));

  // ---

  std::string u512_bytes2 = "00";
  big_int expected_value2{"0"};
  big_int actual_value2 = Casper::u512Decode(u512_bytes2);
  TEST_ASSERT(actual_value2 == expected_value2);

  std::string encoded_value2 = Casper::u512Encode(expected_value2);
  TEST_ASSERT(iequals(u512_bytes2, encoded_value2));

  // ---

  std::string u512_bytes3 = "050e6b1623e8";
  big_int expected_value3{"997021084430"};
  big_int actual_value3 = Casper::u512Decode(u512_bytes3);
  TEST_ASSERT(actual_value3 == expected_value3);

  std::string encoded_value3 = Casper::u512Encode(expected_value3);
  TEST_ASSERT(iequals(u512_bytes3, encoded_value3));
}

void serializeUnitTest() {
  /*
  {
  "bytes": "",
  "parsed": null,
  "cl_type": "Unit"
}
  */

  /*
 Unit serializes to an empty byte array.
  */
}

void serializeStringTest() {
  std::string deposit_bytes = "070000006465706f736974";
  std::string hello_world_bytes = "0d00000048656c6c6f2c20576f726c6421";

  std::string expected_deposit = "deposit";
  std::string actual_deposit = Casper::StringUtil::hexToString(deposit_bytes);
  TEST_ASSERT(expected_deposit == actual_deposit);

  std::string encoded_deposit = Casper::StringUtil::stringToHex(actual_deposit);
  std::cout << "encoded_deposit: " << encoded_deposit << std::endl;
  TEST_ASSERT(iequals(deposit_bytes, encoded_deposit));

  std::string expected_hello_world = "Hello, World!";
  std::string actual_hello_world =
      Casper::StringUtil::hexToString(hello_world_bytes);
  TEST_ASSERT(expected_hello_world == actual_hello_world);

  std::string encoded_hello_world =
      Casper::StringUtil::stringToHex(actual_hello_world);
  std::cout << "encoded_hello_world: " << encoded_hello_world << std::endl;
  TEST_ASSERT(iequals(hello_world_bytes, encoded_hello_world));
}

void serializeURefTest() {
  //
}

void serializeKeyTest() {
  /*
{
  "bytes":
"0123cd4354304f4eb1dd6739cba66d41579936e2cec1553096d97aa4efb6b661e6",
  "parsed": {
    "Hash":
"hash-23cd4354304f4eb1dd6739cba66d41579936e2cec1553096d97aa4efb6b661e6"
  },
  "cl_type": "Key"
}
  */

  nlohmann::json key_json =
      R"({"bytes":"0123cd4354304f4eb1dd6739cba66d41579936e2cec1553096d97aa4efb6b661e6","parsed":{"Hash":"hash-23cd4354304f4eb1dd6739cba66d41579936e2cec1553096d97aa4efb6b661e6"},"cl_type":"Key"})";

  std::string key_bytes_str =
      "0123cd4354304f4eb1dd6739cba66d41579936e2cec1553096d97aa4efb6b661e6";
  std::string key_str =
      "hash-23cd4354304f4eb1dd6739cba66d41579936e2cec1553096d97aa4efb6b661e6";

  CryptoPP::SecByteBlock key_bytes =
      Casper::CEP57Checksum::Decode(key_bytes_str);

  nlohmann::json parsed_key;
  Casper::to_json(parsed_key, Casper::GlobalStateKey::FromBytes(key_bytes));
  std::cout << "parsed_key: " << parsed_key.dump(4) << std::endl;
  TEST_ASSERT(iequals(key_str, parsed_key));

  std::string actual_bytes_str = Casper::CEP57Checksum::Encode(key_bytes);

  TEST_ASSERT(iequals(key_bytes_str, actual_bytes_str));
}

void serializePublicKeyTest() {
  /*
{
  "bytes": "01b92e36567350dd7b339d709bfe341df6fda853e85315418f1bb3ddd414d9f5be",
  "parsed":
"01b92e36567350dd7b339d709bfe341df6fda853e85315418f1bb3ddd414d9f5be", "cl_type":
"PublicKey"
}

  */
  nlohmann::json public_key_json =
      R"({"bytes":"01b92e36567350dd7b339d709bfe341df6fda853e85315418f1bb3ddd414d9f5be","parsed":"01b92e36567350dd7b339d709bfe341df6fda853e85315418f1bb3ddd414d9f5be","cl_type":"PublicKey"})";

  std::string public_key_bytes_str =
      "01b92e36567350dd7b339d709bfe341df6fda853e85315418f1bb3ddd414d9f5be";

  CryptoPP::SecByteBlock public_key_bytes =
      Casper::CEP57Checksum::Decode(public_key_bytes_str);

  nlohmann::json parsed_public_key;
  Casper::to_json(parsed_public_key,
                  Casper::PublicKey::FromBytes(public_key_bytes));

  std::cout << "parsed_public_key: " << parsed_public_key.dump(4) << std::endl;

  std::string actual_bytes_str =
      Casper::CEP57Checksum::Encode(public_key_bytes);

  TEST_ASSERT(iequals(public_key_bytes_str, actual_bytes_str));

  std::string expected_public_key_str =
      "01b92e36567350dd7b339d709bfe341df6fda853e85315418f1bb3ddd414d9f5be";

  TEST_ASSERT(iequals(expected_public_key_str, parsed_public_key));
}

void serializeOptionTest() {
  /*
  Optional values serialize with a single byte tag, followed by the
  serialization of the value itself. The tag is equal to 0 if the value is
  missing, and 1 if it is present.
    E.g. None serializes as 0x00
    E.g. Some(10u32) serializes as 0x010a000000
*/

  /*
{
  "bytes": "0101000000140000007a627974652d32352d536f74686562795f312d32",
  "parsed": [
    "zbyte-25-Sotheby_1-2"
  ],
  "cl_type": {
    "Option": {
      "List": "String"
    }
  }
}
  */

  /*
 {
   "bytes": "017ce8020000000000",
   "parsed": 190588,
   "cl_type": {
     "Option": "U64"
   }
 }
  */
}

std::vector<uint8_t> hexToByteArray(const std::string& bytes_str) {
  /*
  "cl_type":{
    "ByteArray":32
  }
  */

  std::vector<uint8_t> ret;

  for (int i = 0; i < bytes_str.length() / 2; i++) {
    std::string byte_str = bytes_str.substr(i * 2, 2);
    uint8_t byte = Casper::u8Decode(byte_str);
    ret.push_back(byte);
  }
  return ret;
}

std::string byteArrayToHex(const std::vector<uint8_t>& bytes) {
  std::string ret;
  for (uint8_t byte : bytes) {
    std::string byte_str = Casper::u8Encode(byte);
    ret += byte_str;
  }
  return ret;
}

// 32 bytes fixed length - 64 hex characters
void serializeByteArrayTest() {
  std::string byte_array_bytes1 =
      "8541116c667bb15b43464a70fa681f8a50dcdf876f43a86b074de9597ca010e1";
  std::vector<uint8_t> expected_value1{
      0x85, 0x41, 0x11, 0x6c, 0x66, 0x7b, 0xb1, 0x5b, 0x43, 0x46, 0x4a,
      0x70, 0xfa, 0x68, 0x1f, 0x8a, 0x50, 0xdc, 0xdf, 0x87, 0x6f, 0x43,
      0xa8, 0x6b, 0x07, 0x4d, 0xe9, 0x59, 0x7c, 0xa0, 0x10, 0xe1};

  std::vector<uint8_t> actual_value1 = hexToByteArray(byte_array_bytes1);
  TEST_ASSERT(actual_value1 == expected_value1);

  std::string encoded_value1 = byteArrayToHex(expected_value1);
  TEST_ASSERT(iequals(byte_array_bytes1, encoded_value1));

  /*
  // TODO:
  {
  "bytes": "c2ca7eee6617ac22ddee0c2104477f359186517ba9a246cca445e4654e5562f5",
  "parsed": "c2ca7eee6617ac22ddee0c2104477f359186517ba9a246cca445e4654e5562f5",
  "cl_type": {
    "ByteArray": 32
  }
}
  */
}

void serializeMapTest() {
  /*
  {
  "bytes":
"0500000015000000636f6e74726163745f7061636b6167655f6861736840000000633236373761303834363632336362393235633563433241393265463539373544466436636338326439463831433864433034414445393835313733353546440a0000006576656e745f7479706507000000617070726f7665050000006f776e65724e0000004b65793a3a4163636f756e74283335623636353632646139393637336237303331376433344633354234456536364639353734304333653231313666346563316138373232353532463434643829070000007370656e6465724b0000004b65793a3a486173682832334344343335343330344634654231644436373339434241363664343135373939333665324345433135353330393644393761413465464236423636316536290500000076616c75654e000000313135373932303839323337333136313935343233353730393835303038363837393037383533323639393834363635363430353634303339343537353834303037393133313239363339393335",
  "parsed": [
    {
      "key": "contract_package_hash",
      "value":
"c2677a0846623cb925c5cC2A92eF5975DFd6cc82d9F81C8dC04ADE98517355FD"
    },
    {
      "key": "event_type",
      "value": "approve"
    },
    {
      "key": "owner",
      "value":
"Key::Account(35b66562da99673b70317d34F35B4Ee66F95740C3e2116f4ec1a8722552F44d8)"
    },
    {
      "key": "spender",
      "value":
"Key::Hash(23CD4354304F4eB1dD6739CBA66d41579936e2CEC1553096D97aA4eFB6B661e6)"
    },
    {
      "key": "value",
      "value":
"115792089237316195423570985008687907853269984665640564039457584007913129639935"
    }
  ],
  "cl_type": {
    "Map": {
      "key": "String",
      "value": "String"
    }
  }
}
  */

  // example2

  /*
 {
   "bytes":
 "0500000015000000636f6e74726163745f7061636b6167655f6861736840000000633236373761303834363632336362393235633563433241393265463539373544466436636338326439463831433864433034414445393835313733353546440a0000006576656e745f74797065070000006465706f736974090000007372635f7075727365560000005552656628454232383666333938396531416242353132626361436432623164374343644261396165303333633362343966453543393036363343423731623063443664382c20524541445f4144445f57524954452902000000746f4e0000004b65793a3a4163636f756e742865313964423836364431433437383945303735613066363634664337334234343031373066643534334164373844453535376534463233394434363836324544290500000076616c75650c000000393539303231303834343330",
   "parsed": [
     {
       "key": "contract_package_hash",
       "value":
 "c2677a0846623cb925c5cC2A92eF5975DFd6cc82d9F81C8dC04ADE98517355FD"
     },
     {
       "key": "event_type",
       "value": "deposit"
     },
     {
       "key": "src_purse",
       "value":
 "URef(EB286f3989e1AbB512bcaCd2b1d7CCdBa9ae033c3b49fE5C90663CB71b0cD6d8,
 READ_ADD_WRITE)"
     },
     {
       "key": "to",
       "value":
 "Key::Account(e19dB866D1C4789E075a0f664fC73B440170fd543Ad78DE557e4F239D46862ED)"
     },
     {
       "key": "value",
       "value": "959021084430"
     }
   ],
   "cl_type": {
     "Map": {
       "key": "String",
       "value": "String"
     }
   }
 }
  */
}

void serializeAnyTest() {
  /*
{
  "bytes":
"06000000050e6f1c4adf07200000008bf67d697b785deb0a6569c7210c80f90b0388a592a83cd66287e1dcb5a3905d2c000000414f4764754762527848696542316f505a6b2f484f305142635031554f74654e3556666b386a6e5561474c74",
  "parsed": null,
  "cl_type": "Any"
}
  */
}

// to store list inside map map["List"]: list_inner_type
Casper::CLTypeRVA createContainerMap(std::string key, Casper::CLTypeRVA value) {
  std::map<std::string, Casper::CLTypeRVA> cl_map_type;
  cl_map_type[key] = value;

  Casper::CLTypeRVA rva = cl_map_type;

  return rva;
}

Casper::CLTypeRVA createOption(Casper::CLTypeRVA value) {
  return createContainerMap("Option", value);
}

Casper::CLTypeRVA createList(Casper::CLTypeRVA value) {
  return createContainerMap("List", value);
}

Casper::CLTypeRVA createMap(Casper::CLTypeRVA key, Casper::CLTypeRVA value) {
  std::map<Casper::CLTypeRVA, Casper::CLTypeRVA> cl_map_type;
  cl_map_type[key] = value;

  Casper::CLTypeRVA rva = cl_map_type;

  return rva;
}

Casper::CLTypeRVA createTuple1(Casper::CLTypeRVA value1) {
  std::vector<Casper::CLTypeRVA> cl_tuple1_type;
  cl_tuple1_type.push_back(value1);

  Casper::CLTypeRVA rva = cl_tuple1_type;

  nlohmann::json j;
  Casper::to_json(j, rva);
  std::cout << j.dump() << std::endl;

  return createContainerMap("Tuple1", rva);
}

Casper::CLTypeRVA createTuple2(Casper::CLTypeRVA value1,
                               Casper::CLTypeRVA value2) {
  std::vector<Casper::CLTypeRVA> cl_tuple2_type;
  cl_tuple2_type.push_back(value1);
  cl_tuple2_type.push_back(value2);

  Casper::CLTypeRVA rva = cl_tuple2_type;

  return createContainerMap("Tuple2", rva);
}

Casper::CLTypeRVA createTuple3(Casper::CLTypeRVA value1,
                               Casper::CLTypeRVA value2,
                               Casper::CLTypeRVA value3) {
  std::vector<Casper::CLTypeRVA> cl_tuple3_type;
  cl_tuple3_type.push_back(value1);
  cl_tuple3_type.push_back(value2);
  cl_tuple3_type.push_back(value3);

  Casper::CLTypeRVA rva = cl_tuple3_type;

  return createContainerMap("Tuple3", rva);
}

bool threeWayCompare(Casper::CLTypeRVA rva) {
  // create a json from the initial type object
  nlohmann::json obj_to_json;
  Casper::to_json(obj_to_json, rva);

  std::cout << "json: " << obj_to_json.dump() << std::endl;

  // create a new type object object from the generated json
  Casper::CLTypeRVA json_to_obj;
  Casper::from_json(obj_to_json, json_to_obj);

  // create a new json from the generated CLType object
  nlohmann::json final_json;
  Casper::to_json(final_json, json_to_obj);

  std::cout << "final json: " << final_json.dump() << std::endl;

  // compare the final parsed json with the initial json
  bool result = obj_to_json == final_json;
  return result;
}

/// map<String, List<PublicKey>>
void cltype_test() {
  // String
  auto key_type = Casper::CLTypeEnum::String;

  // List<PublicKey>
  auto value_type = createList(Casper::CLTypeEnum::PublicKey);

  // map<String, List<PublicKey>>
  auto map_type = createMap(key_type, value_type);

  // assert that the serialization-deserialization is correct
  TEST_ASSERT(threeWayCompare(map_type));
}

/// List<String>
void cltype_str_list_test() {
  // List<String>
  Casper::CLTypeRVA list_type = createList(Casper::CLTypeEnum::String);

  // assert that the serialization-deserialization is correct
  TEST_ASSERT(threeWayCompare(list_type));
}

// TODO:
void cltype_json_test() {
  nlohmann::json j_map;

  j_map["cl_type"] = {{"Map", {{"key", "String"}, {"value", "String"}}}};

  nlohmann::json j_str_list;
  j_str_list["cl_type"] = {{"List", "String"}};

  /*
    std::cout << std::endl;
    std::cout << j_map.dump(2) << std::endl;

    std::cout << std::endl;
    std::cout << j_str_list.dump(2) << std::endl;
    */
}

/// Option<Bool>
void clType_option_test() {
  // Option<Bool>
  Casper::CLTypeRVA opt_type = createOption(Casper::CLTypeEnum::Bool);

  // assert that the serialization-deserialization is correct
  TEST_ASSERT(threeWayCompare(opt_type));
}

/// Option<List<Map<U64, U8>>>
void clType_option_recursiveTest() {
  // Option<List<Map<U64, U8>>>
  Casper::CLTypeRVA opt_type = createOption(
      createList(createMap(Casper::CLTypeEnum::U64, Casper::CLTypeEnum::U8)));

  // assert that the serialization-deserialization is correct
  TEST_ASSERT(threeWayCompare(opt_type));
}

/// Tuple1<Bool>
void clType_tuple1_test() {
  Casper::CLTypeRVA tuple1_type = createTuple1(Casper::CLTypeEnum::Unit);

  TEST_ASSERT(threeWayCompare(tuple1_type));
}

/// Tuple2<Bool, I32>
void clType_tuple2_test() {
  auto elem1 = Casper::CLTypeEnum::Bool;
  auto elem2 = Casper::CLTypeEnum::I32;

  auto tuple2_type = createTuple2(elem1, elem2);

  TEST_ASSERT(threeWayCompare(tuple2_type));
}

/// Tuple2<U256, List<Any>>
void clType_tuple2_recursiveTest() {
  auto elem1 = Casper::CLTypeEnum::U256;
  auto elem2 = createList(Casper::CLTypeEnum::Any);

  auto tuple2_type = createTuple2(elem1, elem2);

  TEST_ASSERT(threeWayCompare(tuple2_type));
}

void clType_tuple3_test() {
  auto elem1 = Casper::CLTypeEnum::I32;
  auto elem2 = Casper::CLTypeEnum::String;
  auto elem3 = createOption(Casper::CLTypeEnum::U512);

  auto tuple3_type = createTuple3(elem1, elem2, elem3);

  TEST_ASSERT(threeWayCompare(tuple3_type));
}

void clTypeParsed_test() {
  std::string json_str = R"(
  {
    "bytes": "06da6662305f01",
    "parsed": "1508345276122",
    "cl_type": "U512"
  }
  )";

  nlohmann::json j = nlohmann::json::parse(json_str);
  std::cout << std::endl << j.dump(2) << std::endl;
  Casper::CLValue cl;
  Casper::from_json(j, cl);

  TEST_ASSERT(rva::get<Casper::CLTypeEnum>(cl.cl_type.type) ==
              Casper::CLTypeEnum::U512);

  nlohmann::json j2;
  Casper::to_json(j2, cl);
  std::cout << std::endl << j2.dump(2) << std::endl;

  TEST_ASSERT(j2.dump() == j.dump());
}

/// test the serialization of a CLValue using a json file as input
void clValue_with_jsonFile(std::string file_name) {
  // read the json file
  nlohmann::json input_json;
  try {
    std::string file_path = __FILE__;
    std::string dir_path = file_path.substr(0, file_path.rfind("/"));
    std::cout << dir_path << std::endl;
    std::string file_path_name = dir_path + "/data/CLValue/" + file_name;
    std::ifstream ifs(file_path_name);
    input_json = nlohmann::json::parse(ifs);
  } catch (std::exception& e) {
    std::cout << "clValue_with_jsonFile: " << e.what() << std::endl;
  }

  std::cout << std::endl << input_json.dump(2) << std::endl;

  // create a CLValue from the json
  Casper::CLValue generated_obj;
  Casper::from_json(input_json, generated_obj);

  // create a new json from the generated CLValue
  nlohmann::json generated_json;
  Casper::to_json(generated_json, generated_obj);

  std::cout << std::endl << generated_json.dump(2) << std::endl;

  // compare the final parsed json with the initial json
  TEST_ASSERT(generated_json.dump() == input_json.dump());
}
void clValue_with_boolTrueTest() { clValue_with_jsonFile("Bool-True.json"); }

void clValue_with_boolFalseTest() { clValue_with_jsonFile("Bool-False.json"); }

void clValue_with_I32Test() { clValue_with_jsonFile("I32.json"); }

void clValue_with_I64Test() { clValue_with_jsonFile("I64.json"); }

void clValue_with_U8Test() { clValue_with_jsonFile("U8.json"); }

void clValue_with_U32Test() { clValue_with_jsonFile("U32.json"); }

void clValue_with_U64Test() { clValue_with_jsonFile("U64.json"); }

void clValue_with_U128Test() { clValue_with_jsonFile("U128.json"); }

void clValue_with_U256Test() { clValue_with_jsonFile("U256.json"); }

void clValue_with_U256_2Test() { clValue_with_jsonFile("U256-2.json"); }

void clValue_with_U512Test() { clValue_with_jsonFile("U512.json"); }

void clValue_with_U512_0Test() { clValue_with_jsonFile("U512-0.json"); }

void clValue_with_UnitTest() { clValue_with_jsonFile("Unit.json"); }

void clValue_with_StringTest() { clValue_with_jsonFile("String.json"); }

void clValue_with_URefTest() { clValue_with_jsonFile("URef.json"); }

// void clValue_with_KeyTest() { clValue_with_jsonFile("Key.json"); }

void clValue_with_PublicKeyTest() { clValue_with_jsonFile("PublicKey.json"); }

void clValue_with_OptionTest() { clValue_with_jsonFile("Option.json"); }

void clValue_with_OptionListKeyNULLTest() {
  clValue_with_jsonFile("OptionListKey-NULL.json");
}

void clValue_with_OptionU64NULLTest() {
  clValue_with_jsonFile("OptionU64-NULL.json");
}

void clValue_with_OptionU64Test() { clValue_with_jsonFile("OptionU64.json"); }

void clValue_with_ListTest() { clValue_with_jsonFile("List.json"); }

void clValue_with_ListByteArray32Test() {
  clValue_with_jsonFile("ListByteArray32.json");
}

void clValue_with_ListOptionStringTest() {
  clValue_with_jsonFile("ListOptionString.json");
}

void clValue_with_ListU8Test() { clValue_with_jsonFile("ListU8.json"); }

void clValue_with_ListU256Test() { clValue_with_jsonFile("ListU256.json"); }

void clValue_with_ByteArrayTest() { clValue_with_jsonFile("ByteArray.json"); }

// void clValue_with_ResultOkTest() { clValue_with_jsonFile("ResultOk.json");
// }

// void clValue_with_ResultErrTest() {
// clValue_with_jsonFile("ResultErr.json");

void clValue_with_MapTest() { clValue_with_jsonFile("Map.json"); }

void clValue_with_Tuple1Test() { clValue_with_jsonFile("Tuple1.json"); }

void clValue_with_Tuple2Test() { clValue_with_jsonFile("Tuple2.json"); }

void clValue_with_Tuple3Test() { clValue_with_jsonFile("Tuple3.json"); }

void clValue_with_AnyTest() { clValue_with_jsonFile("Any.json"); }

#define RPC_TEST 1
#define SER_DE_TEST 1
#define CL_TYPE_TEST 1
#define CL_VALUE_TEST 1

TEST_LIST = {

#if RPC_TEST == 1
    {"infoGetPeers checks node list size", infoGetPeers_Test},
    {"chainGetStateRootHash using Block height parameter",
     chainGetStateRootHash_with_blockHeightTest},
    {"chainGetStateRootHash using invalid Block height parameter",
     chainGetStateRootHash_with_invalidBlockHeightTest},
    {"chainGetStateRootHash using Block hash parameter",
     chainGetStateRootHash_with_blockHashTest},
    {"chainGetStateRootHash using empty Block hash parameter ",
     chainGetStateRootHash_with_emptyParameterTest},
    {"infoGetDeploy using Deploy hash parameter",
     infoGetDeploy_with_deployHashTest},
    {"infoGetDeploy using invalid Deploy hash parameter",
     infoGetDeploy_with_invalidDeployHashTest},
    {"infoGetStatus compares with a reference value",
     infoGetStatus_with_emptyParameterTest},
    {"chainGetBlockTransfers using Block hash parameter",
     chainGetBlockTransfers_with_blockHashTest},
    {"chainGetBlock using Block hash parameter",
     chainGetBlock_with_blockHashTest},
    {"chainGetEraInfoBySwitchBlock using Block hash parameter ",
     chainGetEraInfoBySwitchBlock_with_blockHashTest},
    {"stateGetItem using state root hash and key parameters ",
     stateGetItem_with_keyTest},
    {"stateGetItem using invalid state root hash and key parameters ",
     stateGetItem_with_invalidKeyTest},
    {"stateGetDictionaryItem using state root hash and dictionary key "
     " parameters ",
     stateGetDictionaryItem_with_keyTest},
    {"stateGetBalance compares with a reference value",
     stateGetBalance_with_urefTest},
    {"stateGetBalance using invalid URef and state root hash parameters ",
     stateGetBalance_with_invalidUrefTest},
    {"stateGetAuctionInfo using Block hash parameter (may take a while)",
     stateGetAuctionInfo_with_blockHashTest},
    {"toLower checks internal lower case converter", stringUtil_toLowerTest},
    {"getAccountHash checks internal PublicKey to AccountHash converter",
     publicKey_getAccountHashTest},
#endif

#if SER_DE_TEST == 1
    {"Serialize - Bool", serializeBoolTest},
    {"Serialize - I32", serializeI32Test},
    {"Serialize - I64", serializeI64Test},
    {"Serialize - String", serializeStringTest},
    {"Serialize - U8", serializeU8Test},
    {"Serialize - U32", serializeU32Test},
    {"Serialize - U64", serializeU64Test},
    {"Serialize - U128", serializeU128Test},
    {"Serialize - U256", serializeU256Test},
    {"Serialize - U512", serializeU512Test},
    {"Serialize - ByteArray", serializeByteArrayTest},
    {"Serialize - Key", serializeKeyTest},
    {"Serialize - PublicKey", serializePublicKeyTest},
#endif

#if CL_TYPE_TEST == 1

    {"CLType", cltype_test},
    {"CLType json", cltype_json_test},
    {"CLType List<String>", cltype_str_list_test},
    {"CLValue parsed", clTypeParsed_test},

    {"CLType Tuple1", clType_tuple1_test},
    {"CLType Tuple2", clType_tuple2_test},
    {"CLType Tuple2 recursive", clType_tuple2_recursiveTest},
    {"CLType Tuple3", clType_tuple3_test},
    {"CLType Option", clType_option_test},
    {"CLType Option recursive", clType_option_recursiveTest},
#endif

#if CL_VALUE_TEST == 1
    {"CLValue using Bool expected false", clValue_with_boolFalseTest},
    {"CLValue using Bool expected true", clValue_with_boolTrueTest},
    {"CLValue using I32", clValue_with_I32Test},
    {"CLValue using I64", clValue_with_I64Test},
    {"CLValue using U8", clValue_with_U8Test},
    {"CLValue using U32", clValue_with_U32Test},
    {"CLValue using U64", clValue_with_U64Test},
    {"CLValue using U128", clValue_with_U128Test},
    {"CLValue using U256", clValue_with_U256Test},
    {"CLValue using U256-2", clValue_with_U256_2Test},
    {"CLValue using U512", clValue_with_U512Test},
    {"CLValue using U512-0", clValue_with_U512_0Test},
    {"CLValue using Unit", clValue_with_UnitTest},
    {"CLValue using String", clValue_with_StringTest},
    {"CLValue using URef", clValue_with_URefTest},
    // {"CLValue using Key", clValue_with_KeyTest},
    // {"CLValue using Account Key", clValue_with_accountKeyTest},
    // {"CLValue using Hash Key", clValue_with_hashKeyTest},
    {"CLValue using PublicKey", clValue_with_PublicKeyTest},
    {"CLValue using Option", clValue_with_OptionTest},
    {"CLValue using Option<List<Key>> = NULL",
     clValue_with_OptionListKeyNULLTest},
    {"CLValue using Option<U64> = NULL", clValue_with_OptionU64NULLTest},
    {"CLValue using Option<U64>", clValue_with_OptionU64Test},
    {"CLValue using List", clValue_with_ListTest},
    {"CLValue using List<ByteArray:32>", clValue_with_ListByteArray32Test},
    {"CLValue using List<Option<String>>", clValue_with_ListOptionStringTest},
    {"CLValue using List<U8>", clValue_with_ListU8Test},
    {"CLValue using List<U256>", clValue_with_ListU256Test},
    {"CLValue using ByteArray", clValue_with_ByteArrayTest},
    // {"CLValue using ResultOk", clValue_with_ResultOkTest},
    // {"CLValue using ResultErr", clValue_with_ResultErrTest},
    {"CLValue using Map", clValue_with_MapTest},
    {"CLValue using Tuple1", clValue_with_Tuple1Test},
    {"CLValue using Tuple2", clValue_with_Tuple2Test},
    {"CLValue using Tuple3", clValue_with_Tuple3Test},
    {"CLValue using Any", clValue_with_AnyTest},

#endif

    {NULL, NULL}};

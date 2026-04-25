// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2013-2026 The Electron Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>
#include <limits>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * Electron Genesis Block:
 * CBlock(hash=00000063044a..., ver=112, hashPrevBlock=00000000000000, hashMerkleRoot=6a1105, nTime=1400888888, nBits=1e00ffff, nNonce=159133830, vtx=1)
 *   CTransaction(hash=6a1105, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001e010445...)
 *     CTxOut(nValue=5.00000000, scriptPubKey=...)
 *   vMerkleTree: 6a1105
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    // Electron legacy genesis block parameters.
    const char* pszTimestamp = "The US VA should use a blockchain to keep track of their patients";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        // Electron reward logic is height-tiered in validation.cpp.
        consensus.nSubsidyHalvingInterval = std::numeric_limits<int>::max();
        // Forward-activate the legacy 0.8 ladder at this height. Below it the
        // 0.15.21 chain pays 50 ELT flat (preserving recently-mined history);
        // at and after it the legacy ladder applies (long-tail tier = 5 ELT).
        // Set tip + 250 blocks (~4h at 60s spacing) past tip 6,175,679 to give
        // miners and operators an upgrade window before the cliff.
        consensus.nSubsidyLadderActivationHeight = 6175929;
        // Keep the legacy version checks disabled for historical blocks.
        consensus.BIP34Height = 100000000; // Disabled - far in future
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 100000000; // Disabled - far in future
        consensus.BIP66Height = 100000000; // Disabled - far in future
        // Electron mainnet uses a 1-minute block cadence with a 30-minute retarget.
        consensus.powLimit = uint256S("0x000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 30 * 60;
        consensus.nPowTargetSpacing = 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.fStrictChainId = true;
        consensus.nAuxpowChainId = 0x0007;
        // Preserve the legacy nominal AuxPow activation height from the 0.8.x
        // Electron tree. Historical sync compatibility is handled in
        // validation.cpp by accepting pre-start AuxPow-bearing blocks, matching
        // legacy behavior during bootstrap import and IBD.
        consensus.nAuxpowStartHeight = 500000;
        consensus.nRuleChangeActivationThreshold = 27;
        consensus.nMinerConfirmationWindow = 30;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Mainnet SegWit signaling starts on May 11, 2026 00:00:00 UTC and
        // times out on May 11, 2027 00:00:00 UTC.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1778457600;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1809993600;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xf9;
        pchMessageStart[1] = 0xbc;
        pchMessageStart[2] = 0xac;
        pchMessageStart[3] = 0xc4;
        nDefaultPort = 6853;
        nPruneAfterHeight = 100000;

        // Electron genesis block.
        genesis = CreateGenesisBlock(1400888888, 159133830, 0x1e00ffff, 112, 5 * COIN);
        consensus.hashGenesisBlock = uint256S("0x00000063044a9b6cad9015b289de8a86a654d5a916fcaaeeefb5b9408448df40");
        assert(genesis.hashMerkleRoot == uint256S("0x6a1105401133793ebc124b837824144b64058bd0b89408cd17201012972f3ec8"));
        assert(genesis.GetHash() == consensus.hashGenesisBlock);

        // BlakeStream ecosystem DNS seeds — shared across all 6 coins
        vSeeds.emplace_back("blakestream.io", "seed.blakestream.io", false);
        vSeeds.emplace_back("blakecoin.org", "seed.blakecoin.org", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,92);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,7);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
        bech32_hrp = "elt";

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {0,       uint256S("0x00000063044a9b6cad9015b289de8a86a654d5a916fcaaeeefb5b9408448df40")},
                {25550,   uint256S("0xe8b6e0cfda9a65717ac300d5e1222bbbe45439d13cb9b681c77b775f77d8ec09")},
                {107785,  uint256S("0x0c57a0c979d324a667563d417eabc0a72d3710a70b92fb5541f776f48e50766b")},
                {194686,  uint256S("0x591461dfbaedcb23a96d2b8ce5b202ebb501bae203817c060bc44dafd563cbba")},
                {268428,  uint256S("0x0000000000052546b3b71130b7795cfe200d9a3d2b61fcd3fcc3e3cb5b668789")},
                {371000,  uint256S("0x000000000000c18f5062e45073251ede9f9ebffbd4d1fdd09e3c33553b3b93da")},
                {415000,  uint256S("0x00000000000b69fca351b7ea2f766170faf87897d81b1a34fb67f0bd0dbdd50e")},
                {580000,  uint256S("0x00000000000e0231682f8235d0207b5e8dee5a54d9ce7d13a607667a60779214")},
                {628199,  uint256S("0xca8c5ff1d964ee60e0d1168e7929d067d26953183248e6d95585c7c2d0a09193")},
                {804089,  uint256S("0xd2e44c14b00e0e93081d53a164659a24f806ccfdfda2e2fa82e1498f6e0a5dbe")},
                {813123,  uint256S("0x14cc1a6c4173e09db94934f2996e2e34792e356d58c767c79acec08260323826")},
                {1850000, uint256S("0xa1ee97c7f3a9c1771e7d65808b9b3469a597d7f5e400b579c859c886de90dc6e")},
                {3255451, uint256S("0xf1d51594b36be58004948cb7242367b41882368a5892814d5cb755b328d61f0d")},
                {3700000, uint256S("0x591fd4f3c632162d759a1a9938c3ccb7334e7d4bf44c49fa62dc21e999b410e5")},
                {4014588, uint256S("0x66e1610c8ae6bcf0b50c48d01d32eabb3680ec3d8f387506baf1c19ae62706f1")},
            }
        };

        chainTxData = ChainTxData{
            1626480852,
            4684035,
            9000.0 / (24 * 60 * 60)
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = std::numeric_limits<int>::max();
        consensus.BIP34Height = 100000000; // Disabled - far in future
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 100000000; // Disabled - far in future
        consensus.BIP66Height = 100000000; // Disabled - far in future
        consensus.powLimit = uint256S("0x000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 30 * 60;
        consensus.nPowTargetSpacing = 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.fStrictChainId = false;
        consensus.nAuxpowChainId = 0x0007;
        consensus.nAuxpowStartHeight = 0;
        consensus.nRuleChangeActivationThreshold = 27;
        consensus.nMinerConfirmationWindow = 30;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x15;
        pchMessageStart[3] = 0x10;
        nDefaultPort = 16853;
        nPruneAfterHeight = 1000;

        // Historical Electron testnet genesis is unresolved, so keep a safe
        // local placeholder chain here instead of inventing historical data.
        genesis = CreateGenesisBlock(1400888888, 159133830, 0x1e00ffff, 112, 5 * COIN);
        consensus.hashGenesisBlock = uint256S("0x00000063044a9b6cad9015b289de8a86a654d5a916fcaaeeefb5b9408448df40");
        assert(genesis.hashMerkleRoot == uint256S("0x6a1105401133793ebc124b837824144b64058bd0b89408cd17201012972f3ec8"));

        vFixedSeeds.clear();
        vSeeds.clear();
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,142);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,170);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
        bech32_hrp = "telt";

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0x00000063044a9b6cad9015b289de8a86a654d5a916fcaaeeefb5b9408448df40")},
            }
        };

        chainTxData = ChainTxData{
            1400888888,
            0,
            0.0
        };

    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = std::numeric_limits<int>::max();
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 30 * 60; // Keep regtest aligned with Electron timing while remaining easy to mine.
        consensus.nPowTargetSpacing = 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.fStrictChainId = false;
        consensus.nAuxpowChainId = 0x0007;
        consensus.nAuxpowStartHeight = 0;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Keep the standard regtest window for functional tests.
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        // Safe local regtest chain uses the Electron mainnet genesis payload.
        genesis = CreateGenesisBlock(1400888888, 159133830, 0x1e00ffff, 112, 5 * COIN);
        consensus.hashGenesisBlock = uint256S("0x00000063044a9b6cad9015b289de8a86a654d5a916fcaaeeefb5b9408448df40");
        assert(genesis.hashMerkleRoot == uint256S("0x6a1105401133793ebc124b837824144b64058bd0b89408cd17201012972f3ec8"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData){
            {
                {0, uint256S("0x00000063044a9b6cad9015b289de8a86a654d5a916fcaaeeefb5b9408448df40")}
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,92);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,7);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};
        bech32_hrp = "relt";
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}

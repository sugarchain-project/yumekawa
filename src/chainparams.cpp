// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <hash.h> // for signet block challenge hash
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <versionbitsinfo.h>

#include <assert.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <arith_uint256.h> /* SugarShield */

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
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "The Times 17/July/2019 Bitcoin falls after senators call Facebook delusional over libra";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/* SugarShield */
const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = CBaseChainParams::MAIN;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = 12500000;
        consensus.BIP16Exception = uint256{};
        consensus.BIP34Height = 17;
        consensus.BIP34Hash = uint256S("0x72e36f3fcdf98d3625dfe03f28a914c513b913231e479d53fc22e5e46cf5b585"); // getblockhash 17
        consensus.BIP65Height = 0;  // Always on
        consensus.BIP66Height = 0;  // Always on
        consensus.CSVHeight = 0;    // Always on
        consensus.SegwitHeight = 0; // Always on
        consensus.MinBIP9WarningHeight = 12240; // segwit activation height + miner confirmation window
        consensus.powLimit = uint256S("003fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 61200; // 17 hours = 17*60*60 = 61200
        consensus.nPowTargetSpacing = 5;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 9180; // 75% of nMinerConfirmationWindow = 61200/5*0.75 = 9180 (was 1916 = 1209600/600*0.95+0.8) // 9180/510 = 18 cycles of DigiShieldZEC
        consensus.nMinerConfirmationWindow = 12240; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of Taproot (BIPs 340-342)
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = 1230767999; // December 31, 2008

        /* Sugarchain Settings */
        assert(MAX_MONEY == pow(2,30) * COIN);
        assert(consensus.nSubsidyHalvingInterval == pow(5,8) * 32);
        assert(consensus.nPowTargetSpacing == 10 * 60 / 120);

        /* SugarShield */
        consensus.nPowAveragingWindow = 510; // 2550/nPowTargetSpacing(5) = 510
        consensus.nPowMaxAdjustDown = 32; // 32% adjustment down
        consensus.nPowMaxAdjustUp = 16; // 16% adjustment up
        assert(maxUint/UintToArith256(consensus.powLimit) == 1024); // 0x0000000000000000000000000000000000000000000000000000000000000400 == 1024
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow); // 1024 >= 510
        assert(maxUint/UintToArith256(consensus.powLimit) == consensus.nPowAveragingWindow + 514); // 1024 − 510 = 514

        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000003f23ef34da28"); // getblockhash 6513497 && "chainwork"
        consensus.defaultAssumeValid = uint256S("0x855f0c66238bc0246c8ca25cf958283fd49b9fb4b217ddeb518e5ea9f5071b9e"); // getblockhash 6513497 && "hash"

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x9f;
        pchMessageStart[1] = 0xeb;
        pchMessageStart[2] = 0x4b;
        pchMessageStart[3] = 0x9d;
        nDefaultPort = 34230;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 6;
        m_assumed_chain_state_size = 1;

        genesis = CreateGenesisBlock(1565881200, 247, 0x1f3fffff, 1, 42.94967296 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(genesis.GetPoWHash() == uint256S("0x0031205acedcc69a9c18f79b84790179d68fb90588bedee6587ff701bdde04eb"));
        assert(consensus.hashGenesisBlock == uint256S("0x7d5eaec2dbb75f99feadfa524c78b7cabc1d8c8204f79d4f3a83381b811b0adc"));
        assert(genesis.hashMerkleRoot == uint256S("0x7677ce2a579cb0411d1c9e6b1e9072b8f537f1e59cb387dacac2daac56e150b0"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as an addrfetch if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("1seed.sugarchain.info"); // cryptozeny
        vSeeds.emplace_back("2seed.sugarchain.info"); // cryptozeny
        vSeeds.emplace_back("seed.sugarchain.site"); // ROZ

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,63); // legacy: starting with "S" (upper)
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,125); // p2sh-segwit: starting with "s" (lower)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "sugar"; // bech32: starting with "sugar1q..."

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                { 0, uint256S("0x7d5eaec2dbb75f99feadfa524c78b7cabc1d8c8204f79d4f3a83381b811b0adc")},
            }
        };

        chainTxData = ChainTxData{
            // Data from RPC: getchaintxstats 4096 855f0c66238bc0246c8ca25cf958283fd49b9fb4b217ddeb518e5ea9f5071b9e (6513497)
            /* nTime    */ 1599181736,
            /* nTxCount */ 6858263,
            /* dTxRate  */ 0.2053689306146399
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = CBaseChainParams::TESTNET;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = 12500000;
        consensus.BIP16Exception = uint256{};
        consensus.BIP34Height = 17;
        consensus.BIP34Hash = uint256S("0xc7c210c9267ebc195100c71691336c7048cbdd37761b4bd20c9b1f32caf4fb9c"); // getblockhash 17
        consensus.BIP65Height = 0;  // Always on
        consensus.BIP66Height = 0;  // Always on
        consensus.CSVHeight = 0;    // Always on
        consensus.SegwitHeight = 0; // Always on
        consensus.MinBIP9WarningHeight = 12240; // segwit activation height + miner confirmation window
        consensus.powLimit = uint256S("003fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 61200; // 17 hours = 17*60*60 = 61200
        consensus.nPowTargetSpacing = 5;
        consensus.fPowAllowMinDifficultyBlocks = false; // Sugarchain: DAA SugarShield-N510
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 9180; // 75% of nMinerConfirmationWindow = 61200/5*0.75 = 9180 (was 1916 = 1209600/600*0.95+0.8) // 9180/510 = 18 cycles of DigiShieldZEC
        consensus.nMinerConfirmationWindow = 12240; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of Taproot (BIPs 340-342)
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = 1230767999; // December 31, 2008

        /* Sugarchain Settings */
        assert(MAX_MONEY == pow(2,30) * COIN);
        assert(consensus.nSubsidyHalvingInterval == pow(5,8) * 32);
        assert(consensus.nPowTargetSpacing == 10 * 60 / 120);

        /* SugarShield */
        consensus.nPowAveragingWindow = 510; // 2550/nPowTargetSpacing(5) = 510
        consensus.nPowMaxAdjustDown = 32; // 32% adjustment down
        consensus.nPowMaxAdjustUp = 16; // 16% adjustment up
        assert(maxUint/UintToArith256(consensus.powLimit) == 1024); // 0x0000000000000000000000000000000000000000000000000000000000000400 == 1024
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow); // 1024 >= 510
        assert(maxUint/UintToArith256(consensus.powLimit) == consensus.nPowAveragingWindow + 514); // 1024 − 510 = 514

        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000000014d9bf048"); // getblockhash 4000000 && "chainwork" (testnet)
        consensus.defaultAssumeValid = uint256S("0xbc05c2d5e81785f287cd58a798b64467cff35c8ef2bbe8062d8420eeb86f4056"); // getblockhash 4000000 && "hash" (testnet)

        pchMessageStart[0] = 0xb0;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x90;
        pchMessageStart[3] = 0x70;
        nDefaultPort = 44230;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 1;
        m_assumed_chain_state_size = 1;

        genesis = CreateGenesisBlock(1565913601, 490, 0x1f3fffff, 1, 42.94967296 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(genesis.GetPoWHash() == uint256S("0x0032f49a73e00fc182e08d5ede75c1418c7833092d663e43a5463c1dbd096f28"));
        assert(consensus.hashGenesisBlock == uint256S("0xe0e0e42e493ba7b15f7b0fe1a7e66f73b7fd8b3e6e6a7b0e821a6b95040d3826"));
        assert(genesis.hashMerkleRoot == uint256S("0x7677ce2a579cb0411d1c9e6b1e9072b8f537f1e59cb387dacac2daac56e150b0"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("1seed-testnet.cryptozeny.com"); // cryptozeny

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,66); // legacy: starting with "T" (upper)
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,128); // p2sh-segwit: starting with "t" (lower)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tugar"; // bech32: starting with "tugar1q..."

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {0, uint256S("0xe0e0e42e493ba7b15f7b0fe1a7e66f73b7fd8b3e6e6a7b0e821a6b95040d3826")},
            }
        };

        chainTxData = ChainTxData{
            // Data from RPC: getchaintxstats 4096 bd83debccee1bef17340539beff64ad3feab03c25e5d91969cf5418b8e2fe5a7 (780811)
            /* nTime    */ 1598648664,
            /* nTxCount */ 4033793,
            /* dTxRate  */ 0.1692345821801809
        };
    }
};

/**
 * Signet
 */
class SigNetParams : public CChainParams {
public:
    explicit SigNetParams(const ArgsManager& args) {
        std::vector<uint8_t> bin;
        vSeeds.clear();

        if (!args.IsArgSet("-signetchallenge")) {
            bin = ParseHex("512103ad5e0edad18cb1f0fc0d28a3d4f1f3e445640337489abb10404f2d1e086be430210359ef5021964fe22d6f8e05b2463c9540ce96883fe3b278760f048f5189f2e6c452ae");
            // TODO: signet not launched yet
            /*
            vSeeds.emplace_back("178.128.221.177");
            vSeeds.emplace_back("2a01:7c8:d005:390::5");
            vSeeds.emplace_back("ntv3mtqw5wt63red.onion:54230");
            */

            consensus.nMinimumChainWork = uint256S(""); // TODO: signet not launched yet
            consensus.defaultAssumeValid = uint256S(""); // TODO: signet not launched yet
            m_assumed_blockchain_size = 0; // TODO: signet not launched yet
            m_assumed_chain_state_size = 0; // TODO: signet not launched yet
            chainTxData = ChainTxData{ // TODO: signet not launched yet
                // Data from RPC: getchaintxstats 4096 00000128807d9175c494e24d805fc7854f7d79aa965cbb128342ad8b70cecfa5
                /* nTime    */ 0,
                /* nTxCount */ 0,
                /* dTxRate  */ 0,
            };
        } else {
            const auto signet_challenge = args.GetArgs("-signetchallenge");
            if (signet_challenge.size() != 1) {
                throw std::runtime_error(strprintf("%s: -signetchallenge cannot be multiple values.", __func__));
            }
            bin = ParseHex(signet_challenge[0]);

            consensus.nMinimumChainWork = uint256{}; // a new signet clean
            consensus.defaultAssumeValid = uint256{}; // a new signet clean
            m_assumed_blockchain_size = 0; // a new signet clean
            m_assumed_chain_state_size = 0; // a new signet clean
            chainTxData = ChainTxData{ // a new signet clean
                0,
                0,
                0,
            };
            LogPrintf("Signet with challenge %s\n", signet_challenge[0]);
        }

        if (args.IsArgSet("-signetseednode")) {
            vSeeds = args.GetArgs("-signetseednode");
        }

        strNetworkID = CBaseChainParams::SIGNET;
        consensus.signet_blocks = true;
        consensus.signet_challenge.assign(bin.begin(), bin.end());
        consensus.nSubsidyHalvingInterval = 12500000;
        consensus.BIP16Exception = uint256{};
        consensus.BIP34Height = 1; // TODO: signet not launched yet
        consensus.BIP34Hash = uint256{};
        consensus.BIP65Height = 1; // TODO: signet not launched yet
        consensus.BIP66Height = 1; // TODO: signet not launched yet
        consensus.CSVHeight = 1; // TODO: signet not launched yet
        consensus.SegwitHeight = 1; // TODO: signet not launched yet
        consensus.nPowTargetTimespan = 61200; // 17 hours = 17*60*60 = 61200
        consensus.nPowTargetSpacing = 5;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 9180; // 75% of nMinerConfirmationWindow = 61200/5*0.75 = 9180 (was 1916 = 1209600/600*0.95+0.8) // 9180/510 = 18 cycles of DigiShieldZEC
        consensus.nMinerConfirmationWindow = 12240; // nPowTargetTimespan / nPowTargetSpacing
        consensus.MinBIP9WarningHeight = 12240; // segwit activation height + miner confirmation window
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Activation of Taproot (BIPs 340-342)
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        /* Sugarchain Settings */
        assert(MAX_MONEY == pow(2,30) * COIN);
        assert(consensus.nSubsidyHalvingInterval == pow(5,8) * 32);
        assert(consensus.nPowTargetSpacing == 10 * 60 / 120);

        /* SugarShield */
        consensus.nPowAveragingWindow = 510; // 2550/nPowTargetSpacing(5) = 510
        consensus.nPowMaxAdjustDown = 32; // 32% adjustment down
        consensus.nPowMaxAdjustUp = 16; // 16% adjustment up
        assert(maxUint/UintToArith256(consensus.powLimit) == 512); // 0x0000000000000000000000000000000000000000000000000000000000000200 == 512
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow); // 512 >= 510
        assert(maxUint/UintToArith256(consensus.powLimit) == consensus.nPowAveragingWindow + 2); // 512 − 510 = 2

        // message start is defined as the first 4 bytes of the sha256d of the block script
        CHashWriter h(SER_DISK, 0);
        h << consensus.signet_challenge;
        uint256 hash = h.GetHash();
        memcpy(pchMessageStart, hash.begin(), 4);

        nDefaultPort = 54230;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1602302400, 862, 0x1f7fffff, 1, 42.94967296 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(genesis.GetPoWHash() == uint256S("0x0011df7a28e271dc914cc43b498c0f7107bd549e0d6630055f7c10bb0471bc6b"));
        assert(consensus.hashGenesisBlock == uint256S("0x223231facc4c2337baedba62921cf0ada7f867a869194ce9b3697eefd9d54c59"));
        assert(genesis.hashMerkleRoot == uint256S("0x7677ce2a579cb0411d1c9e6b1e9072b8f537f1e59cb387dacac2daac56e150b0"));

        vFixedSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,66); // legacy: starting with "T" (upper)
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,128); // p2sh-segwit: starting with "t" (lower)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tugar"; // bech32: starting with "tugar1q..."

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = true;
        m_is_mockable_chain = false;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID =  CBaseChainParams::REGTEST;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = 150; // Sugarchain: Settings Part 1 // Special halvings rule only for regtest
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 500; // BIP34 activated on regtest (Used in functional tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in functional tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in functional tests)
        consensus.CSVHeight = 432; // CSV activated on regtest (Used in rpc activation tests)
        consensus.SegwitHeight = 0; // SEGWIT is always activated on regtest unless overridden
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f");
        consensus.nPowTargetTimespan = 61200; // 17 hours = 17*60*60 = 61200
        consensus.nPowTargetSpacing = 5;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        /* Sugarchain Settings */
        assert(MAX_MONEY == pow(2,30) * COIN);
        assert(consensus.nSubsidyHalvingInterval == pow(5,8) * 32 / 1000 * 0.012);
        assert(consensus.nPowTargetSpacing == 10 * 60 / 120);

        /* SugarShield */
        consensus.nPowAveragingWindow = 17; // 85/nPowTargetSpacing(5) = 17
        consensus.nPowMaxAdjustDown = 0; // Turn off adjustment down
        consensus.nPowMaxAdjustUp = 0; // Turn off adjustment up
        assert(maxUint/UintToArith256(consensus.powLimit) == 17); // 0x0000000000000000000000000000000000000000000000000000000000000011 == 17
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow); // 17 >= 17
        assert(maxUint/UintToArith256(consensus.powLimit) == consensus.nPowAveragingWindow + 0); // 17 − 17 = 0

        consensus.nMinimumChainWork = uint256{}; // regtest clean
        consensus.defaultAssumeValid = uint256{}; // regtest clean

        pchMessageStart[0] = 0xaf;
        pchMessageStart[1] = 0xfb;
        pchMessageStart[2] = 0x5b;
        pchMessageStart[3] = 0xad;
        nDefaultPort = 45340;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0; // regtest clean
        m_assumed_chain_state_size = 0; // regtest clean

        UpdateActivationParametersFromArgs(args);

        genesis = CreateGenesisBlock(1565913602, 9, 0x200f0f0f, 1, 42.94967296 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(genesis.GetPoWHash() == uint256S("0x0d144c097fa9c4cf7482fde74f95da8045b516490741af0ee9b1ac1f1dd6c914"));
        assert(consensus.hashGenesisBlock == uint256S("0xd567a9c891c7a47e6dd03f8006cb65b0d6406b5dc7b2c86d7a904815c394e1f1"));
        assert(genesis.hashMerkleRoot == uint256S("0x7677ce2a579cb0411d1c9e6b1e9072b8f537f1e59cb387dacac2daac56e150b0"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = true;
        m_is_test_chain = true;
        m_is_mockable_chain = true;

        checkpointData = {
            {
                {0, uint256S("0xd567a9c891c7a47e6dd03f8006cb65b0d6406b5dc7b2c86d7a904815c394e1f1")},
            }
        };

        chainTxData = ChainTxData{ // regtest clean
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,61); // legacy: starting with "R" (upper)
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,123); // p2sh-segwit: starting with "r" (lower)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "rugar"; // bech32: starting with "rugar1q..."
    }

    /**
     * Allows modifying the Version Bits regtest parameters.
     */
    void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
    void UpdateActivationParametersFromArgs(const ArgsManager& args);
};

void CRegTestParams::UpdateActivationParametersFromArgs(const ArgsManager& args)
{
    if (args.IsArgSet("-segwitheight")) {
        int64_t height = args.GetArg("-segwitheight", consensus.SegwitHeight);
        if (height < -1 || height >= std::numeric_limits<int>::max()) {
            throw std::runtime_error(strprintf("Activation height %ld for segwit is out of valid range. Use -1 to disable segwit.", height));
        } else if (height == -1) {
            LogPrintf("Segwit disabled for testing\n");
            height = std::numeric_limits<int>::max();
        }
        consensus.SegwitHeight = static_cast<int>(height);
    }

    if (!args.IsArgSet("-vbparams")) return;

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> vDeploymentParams;
        boost::split(vDeploymentParams, strDeployment, boost::is_any_of(":"));
        if (vDeploymentParams.size() != 3) {
            throw std::runtime_error("Version bits parameters malformed, expecting deployment:start:end");
        }
        int64_t nStartTime, nTimeout;
        if (!ParseInt64(vDeploymentParams[1], &nStartTime)) {
            throw std::runtime_error(strprintf("Invalid nStartTime (%s)", vDeploymentParams[1]));
        }
        if (!ParseInt64(vDeploymentParams[2], &nTimeout)) {
            throw std::runtime_error(strprintf("Invalid nTimeout (%s)", vDeploymentParams[2]));
        }
        bool found = false;
        for (int j=0; j < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; ++j) {
            if (vDeploymentParams[0] == VersionBitsDeploymentInfo[j].name) {
                UpdateVersionBitsParameters(Consensus::DeploymentPos(j), nStartTime, nTimeout);
                found = true;
                LogPrintf("Setting version bits activation parameters for %s to start=%ld, timeout=%ld\n", vDeploymentParams[0], nStartTime, nTimeout);
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(strprintf("Invalid deployment (%s)", vDeploymentParams[0]));
        }
    }
}

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const ArgsManager& args, const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN) {
        return std::unique_ptr<CChainParams>(new CMainParams());
    } else if (chain == CBaseChainParams::TESTNET) {
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    } else if (chain == CBaseChainParams::SIGNET) {
        return std::unique_ptr<CChainParams>(new SigNetParams(args));
    } else if (chain == CBaseChainParams::REGTEST) {
        return std::unique_ptr<CChainParams>(new CRegTestParams(args));
    }
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(gArgs, network);
}

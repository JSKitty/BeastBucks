// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <deploymentinfo.h>
#include <hash.h> // for signet block challenge hash
#include <util/system.h>

#include <assert.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTimeTx, uint32_t nTimeBlock, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(9999) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;
    txNew.nTime = nTimeTx;

    CBlock genesis;
    genesis.nTime    = nTimeBlock;
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
static CBlock CreateGenesisBlock(uint32_t nTimeTx, uint32_t nTimeBlock, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Do What Thou Wilt shall be the whole of the Law. Love is the law, love under will. 4 6 3 8 A B K 2 4 A L G M O R 3 Y X 24 89 R P S T O V A L ☉ in 3º Scorpio : ☽ in 8º Aries : dies Jovis : Anno Vix æræ legis";
    const CScript genesisOutputScript = CScript();
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTimeTx, nTimeBlock, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network on which people trade goods and services.
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = CBaseChainParams::MAIN;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        //consensus.BIP16Height = 0;
        consensus.BIP34Height = 10;
        consensus.BIP34Hash = uint256S("0x0");
        consensus.powLimit =            uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 32;
        consensus.bnInitialHashTarget = uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 40;

        consensus.nTargetTimespan = 7 * 24 * 60 * 60;  // one week
        consensus.nStakeTargetSpacing = 60; // 1-minute: block spacing
        consensus.nTargetSpacingWorkMax = 12 * consensus.nStakeTargetSpacing; // 2-hour
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;
        consensus.nStakeMinAge = 60; // 1-minute: minimum age for coin age // BeastBucksTODO
        consensus.nStakeMaxAge = 120;// BeastBucksTODO
        consensus.nModifierInterval = 6 * 60 * 60; // Modifier interval: time to elapse before new modifier is computed // BeastBucksTODO
        consensus.nCoinbaseMaturity = 25; // BeastBucksTODO

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1815; // 90% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing

        consensus.SegwitHeight = 10;

        consensus.nMinimumChainWork = uint256S("0x0"); // 650000
        consensus.defaultAssumeValid = uint256S("0x0");  // 650000

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xe6;
        pchMessageStart[1] = 0xe8;
        pchMessageStart[2] = 0xe9;
        pchMessageStart[3] = 0xe6;
        nDefaultPort = 9393;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 2;

        genesis = CreateGenesisBlock(1345083810, 1345084287, 3522847152u, 0x1d00ffff, 1, 50000000);
        consensus.hashGenesisBlock = genesis.GetHash();
        printf("--- MAINNET ---\nGenesis block hash: %s\nHash Merkle Root: %s\n", consensus.hashGenesisBlock.ToString().c_str(), genesis.hashMerkleRoot.ToString().c_str());
        assert(consensus.hashGenesisBlock == uint256S("0x16ff29bfc2ce6522d57cec54fdff6ccbfe533228513b7883321d53cb8eb0656c"));
        assert(genesis.hashMerkleRoot == uint256S("0x6e48064304b372e3a19bd776cc6ef1745e98bc5f52c55fe7da8b2d0154877a35"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as an addrfetch if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        //vSeeds.emplace_back("future_seeders_go_here");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 25);  // BeastBucks: addresses begin with 'B'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 117); // peercoin: addresses begin with 'p' (JSKitty: Wtf Peercoin docs, this can't be a P!)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1, 183);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        // human readable prefix to bench32 address
        bech32_hrp = "beast";

        vFixedSeeds = std::vector<uint8_t>(std::begin(chainparams_seed_main), std::end(chainparams_seed_main));

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {     0, uint256S("0x0")},
            }
        };

        m_assumeutxo_data = MapAssumeutxo{
         // TODO to be specified in a future patch.
        };

        chainTxData = ChainTxData{
            // Data as of block 3125be5493e80431952593cce42b160019671108103735509238830939e1c9a3 (height 650000).
            1666903503, // * UNIX timestamp of last known number of transactions
            1,    // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the ChainStateFlushed debug.log lines)
            0.006673443 // * estimated number of transactions per second after that timestamp
                        //   2145533/(1666903503-1345400356) = 0.006673443
        };
    }
};

/**
 * Testnet (v3): public test network which is reset from time to time.
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = CBaseChainParams::TESTNET;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        //consensus.BIP16Height = 0;
        consensus.BIP34Height = 293368;
        consensus.BIP34Hash = uint256S("00000002c0b976c7a5c9878f1cec63fb4d88d68d614aedeaf8158c42d904795e");
        consensus.powLimit =            uint256S("0000000fffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 28;
        consensus.bnInitialHashTarget = uint256S("00000007ffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 29;

        consensus.nTargetTimespan = 7 * 24 * 60 * 60;  // one week
        consensus.nStakeTargetSpacing = 10 * 60;  // 10-minute block spacing
        consensus.nTargetSpacingWorkMax = 12 * consensus.nStakeTargetSpacing; // 2-hour
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;
        consensus.nStakeMinAge = 60 * 60 * 24; // test net min age is 1 day
        consensus.nStakeMaxAge = 60 * 60 * 24 * 90;
        consensus.nModifierInterval = 60 * 20; // Modifier interval: time to elapse before new modifier is computed
        consensus.nCoinbaseMaturity = 60;

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing

        consensus.SegwitHeight = 394215;

        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000a39348f70f067a");  // 500000
        consensus.defaultAssumeValid = uint256S("0xa40f64181ee4a3bedda2eae0107d9da0e049fe285b6e6e2a7f1f11697f22c7ed"); // 500000

        pchMessageStart[0] = 0xcb;
        pchMessageStart[1] = 0xf2;
        pchMessageStart[2] = 0xc0;
        pchMessageStart[3] = 0xef;
        nDefaultPort = 9903;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 1;

        genesis = CreateGenesisBlock(1345083810, 1345090000, 122894938, 0x1d0fffff, 1, 50000000);
        consensus.hashGenesisBlock = genesis.GetHash();
        printf("--- TESTNET ---\nGenesis block hash: %s\nHash Merkle Root: %s\n", consensus.hashGenesisBlock.ToString().c_str(), genesis.hashMerkleRoot.ToString().c_str());
        assert(consensus.hashGenesisBlock == uint256S("0x73d6a90acc07b22248dd0e2d8985fdafc14543532dd97dbf803ab1db1770a7c1"));
        assert(genesis.hashMerkleRoot == uint256S("0x6e48064304b372e3a19bd776cc6ef1745e98bc5f52c55fe7da8b2d0154877a35"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // human readable prefix to bench32 address
        bech32_hrp = "tpc";

        vFixedSeeds = std::vector<uint8_t>(std::begin(chainparams_seed_test), std::end(chainparams_seed_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {     0, uint256S("0x0")},
            }
        };

        m_assumeutxo_data = MapAssumeutxo{
            // TODO to be specified in a future patch.
        };

        chainTxData = ChainTxData{
            // Data as of block a40f64181ee4a3bedda2eae0107d9da0e049fe285b6e6e2a7f1f11697f22c7ed (height 500000)
            1664282408, // * UNIX timestamp of last known number of transactions
            980877,     // * total number of transactions between genesis and that timestamp

                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.003082068 // * estimated number of transactions per second after that timestamp
                        //   980877/(1664282408-1346029522) = 0.003082068

        };
    }
};

/**
 * Signet: test network with an additional consensus parameter (see BIP325).
 */
class SigNetParams : public CChainParams {
public:
    explicit SigNetParams(const ArgsManager& args) {
        std::vector<uint8_t> bin;
        vSeeds.clear();

        if (!args.IsArgSet("-signetchallenge")) {
            bin = ParseHex("512103ad5e0edad18cb1f0fc0d28a3d4f1f3e445640337489abb10404f2d1e086be430210359ef5021964fe22d6f8e05b2463c9540ce96883fe3b278760f048f5189f2e6c452ae");
            vSeeds.emplace_back("seed.signet.bitcoin.sprovoost.nl.");

            // Hardcoded nodes can be removed once there are more DNS seeds
            vSeeds.emplace_back("178.128.221.177");
            vSeeds.emplace_back("v7ajjeirttkbnt32wpy3c6w3emwnfr3fkla7hpxcfokr3ysd3kqtzmqd.onion:38333");

            consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000000de26b0e471");
            consensus.defaultAssumeValid = uint256S("0x00000112852484b5fe3451572368f93cfd2723279af3464e478aee35115256ef"); // 78788
            m_assumed_blockchain_size = 1;
            chainTxData = ChainTxData{
                // Data from RPC: getchaintxstats 4096 0000003d9144c56ac110ae04a0c271a0acce2f14f426b39fdf0d938c96d2eb09
                /* nTime    */ 1645631279,
                /* nTxCount */ 1257429,
                /* dTxRate  */ 0.1389638742514995,
            };
        } else {
            const auto signet_challenge = args.GetArgs("-signetchallenge");
            if (signet_challenge.size() != 1) {
                throw std::runtime_error(strprintf("%s: -signetchallenge cannot be multiple values.", __func__));
            }
            bin = ParseHex(signet_challenge[0]);

            consensus.nMinimumChainWork = uint256{};
            consensus.defaultAssumeValid = uint256{};
            m_assumed_blockchain_size = 0;
            chainTxData = ChainTxData{
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
        consensus.BIP16Exception = uint256{};
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256{};
        consensus.CSVHeight = 1;
        consensus.SegwitHeight = 1;
//        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1815; // 90% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("00000377ae000000000000000000000000000000000000000000000000000000");
/*
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0; // No activation delay

        // Activation of Taproot (BIPs 340-342)
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0; // No activation delay
*/
        // message start is defined as the first 4 bytes of the sha256d of the block script
        CHashWriter h(SER_DISK, 0);
        h << consensus.signet_challenge;
        uint256 hash = h.GetHash();
        memcpy(pchMessageStart, hash.begin(), 4);

        nDefaultPort = 38333;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1345083810, 1345090000, 122894938, 0x1d0fffff, 1, 50000000);
        consensus.hashGenesisBlock = genesis.GetHash();
        printf("--- SIGNET ---\nGenesis block hash: %s\nHash Merkle Root: %s\n", consensus.hashGenesisBlock.ToString().c_str(), genesis.hashMerkleRoot.ToString().c_str());
        assert(consensus.hashGenesisBlock == uint256S("0x73d6a90acc07b22248dd0e2d8985fdafc14543532dd97dbf803ab1db1770a7c1"));
        assert(genesis.hashMerkleRoot == uint256S("0x6e48064304b372e3a19bd776cc6ef1745e98bc5f52c55fe7da8b2d0154877a35"));

        vFixedSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tb";

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = true;
        m_is_mockable_chain = false;
    }
};

/**
 * Regression test: intended for private networks only. Has minimal difficulty to ensure that
 * blocks can be found instantly.
 */

class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID =  CBaseChainParams::REGTEST;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 1; // Always active unless overridden
        consensus.BIP34Hash = uint256();
        consensus.powLimit =            uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 28;
        consensus.bnInitialHashTarget = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 29;

        consensus.nTargetTimespan = 7 * 24 * 60 * 60; // two weeks
        consensus.nStakeTargetSpacing = 10 * 60; // 10-minute block spacing
        consensus.nTargetSpacingWorkMax = 12 * consensus.nStakeTargetSpacing; // 2-hour
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;

        consensus.nStakeMinAge = 60 * 60 * 24; // test net min age is 1 day
        consensus.nStakeMaxAge = 60 * 60 * 24 * 90;
        consensus.nModifierInterval = 60 * 20; // Modifier interval: time to elapse before new modifier is computed
        consensus.nCoinbaseMaturity = 60;

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
/*
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0; // No activation delay

        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0; // No activation delay
*/
        consensus.nMinimumChainWork = uint256{};
        consensus.defaultAssumeValid = uint256{};

        pchMessageStart[0] = 0xcb;
        pchMessageStart[1] = 0xf2;
        pchMessageStart[2] = 0xc0;
        pchMessageStart[3] = 0xef;
        nDefaultPort = 9903;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;

        genesis = CreateGenesisBlock(1345083810, 1345090000, 122894938, 0x1d0fffff, 1, 50000000);

        consensus.hashGenesisBlock = genesis.GetHash();
        printf("--- REGNET ---\nGenesis block hash: %s\nHash Merkle Root: %s\n", consensus.hashGenesisBlock.ToString().c_str(), genesis.hashMerkleRoot.ToString().c_str());
        assert(consensus.hashGenesisBlock == uint256S("0x73d6a90acc07b22248dd0e2d8985fdafc14543532dd97dbf803ab1db1770a7c1"));
        assert(genesis.hashMerkleRoot == uint256S("0x6e48064304b372e3a19bd776cc6ef1745e98bc5f52c55fe7da8b2d0154877a35"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();
        vSeeds.emplace_back("dummySeed.invalid.");

        m_is_test_chain = true;
        m_is_mockable_chain = true;
        checkpointData = {
            {
                {0, uint256S("0x0")},
            }
        };

        m_assumeutxo_data = MapAssumeutxo{

        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "pcrt";
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        //fMineBlocksOnDemand = true;
    }
    void UpdateActivationParametersFromArgs(const ArgsManager& args);
};

static void MaybeUpdateHeights(const ArgsManager& args, Consensus::Params& consensus)
{
    for (const std::string& arg : args.GetArgs("-testactivationheight")) {
        const auto found{arg.find('@')};
        if (found == std::string::npos) {
            throw std::runtime_error(strprintf("Invalid format (%s) for -testactivationheight=name@height.", arg));
        }
        const auto name{arg.substr(0, found)};
        const auto value{arg.substr(found + 1)};
        int32_t height;
        if (!ParseInt32(value, &height) || height < 0 || height >= std::numeric_limits<int>::max()) {
            throw std::runtime_error(strprintf("Invalid height value (%s) for -testactivationheight=name@height.", arg));
        }
        if (name == "segwit") {
            consensus.SegwitHeight = int{height};
        } else if (name == "bip34") {
            consensus.BIP34Height = int{height};
        } else if (name == "dersig") {
            //consensus.BIP66Height = int{height};
        } else if (name == "cltv") {
            //consensus.BIP65Height = int{height};
        } else if (name == "csv") {
            consensus.CSVHeight = int{height};
        } else {
            throw std::runtime_error(strprintf("Invalid name (%s) for -testactivationheight=name@height.", arg));
        }
    }
}

void CRegTestParams::UpdateActivationParametersFromArgs(const ArgsManager& args)
{
    MaybeUpdateHeights(args, consensus);
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

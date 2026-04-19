// Copyright (c) 2014-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "validation.h"
#include "net.h"

#include "test/test_bitcoin.h"

#include <boost/signals2/signal.hpp>
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(main_tests, TestingSetup)

static void TestElectronSubsidySchedule(const Consensus::Params& consensusParams)
{
    BOOST_CHECK_EQUAL(GetBlockSubsidy(0, consensusParams), 5 * COIN);
    BOOST_CHECK_EQUAL(GetBlockSubsidy(1, consensusParams), 20 * COIN);
    BOOST_CHECK_EQUAL(GetBlockSubsidy(525600, consensusParams), 20 * COIN);
    BOOST_CHECK_EQUAL(GetBlockSubsidy(525601, consensusParams), 10 * COIN);
    BOOST_CHECK_EQUAL(GetBlockSubsidy(1051200, consensusParams), 10 * COIN);
    BOOST_CHECK_EQUAL(GetBlockSubsidy(1051201, consensusParams), 5 * COIN);
}

BOOST_AUTO_TEST_CASE(block_subsidy_test)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    TestElectronSubsidySchedule(chainParams->GetConsensus());
}

BOOST_AUTO_TEST_CASE(subsidy_limit_test)
{
    const auto chainParams = CreateChainParams(CBaseChainParams::MAIN);
    for (int nHeight : {0, 1, 525600, 525601, 1051200, 1051201, 5000000}) {
        CAmount nSubsidy = GetBlockSubsidy(nHeight, chainParams->GetConsensus());
        BOOST_CHECK(nSubsidy <= 20 * COIN);
        BOOST_CHECK(MoneyRange(nSubsidy));
    }
}

bool ReturnFalse() { return false; }
bool ReturnTrue() { return true; }

BOOST_AUTO_TEST_CASE(test_combiner_all)
{
    boost::signals2::signal<bool (), CombinerAll> Test;
    BOOST_CHECK(Test());
    Test.connect(&ReturnFalse);
    BOOST_CHECK(!Test());
    Test.connect(&ReturnTrue);
    BOOST_CHECK(!Test());
    Test.disconnect(&ReturnFalse);
    BOOST_CHECK(Test());
    Test.disconnect(&ReturnTrue);
    BOOST_CHECK(Test());
}
BOOST_AUTO_TEST_SUITE_END()

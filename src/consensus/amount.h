// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CONSENSUS_AMOUNT_H
#define BITCOIN_CONSENSUS_AMOUNT_H

#include <cstdint>

/** Amount in satoshis (Can be negative) */
typedef int64_t CAmount;

/** The amount of base units in one ELT. */
static constexpr CAmount COIN = 100000000;

/** No amount larger than this (in base units) is valid.
 *
 * Note that this constant is *not* Electron's total money supply; it is a
 * consensus-critical money-range sanity limit inherited from the released
 * 0.15.21 chain. Changing it can fork historical replay.
 * */
static constexpr CAmount MAX_MONEY = 7000000000LL * COIN;
inline bool MoneyRange(const CAmount& nValue) { return (nValue >= 0 && nValue <= MAX_MONEY); }

#endif // BITCOIN_CONSENSUS_AMOUNT_H

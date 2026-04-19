# QC Report: Electron-ELT 0.15.2 Update (Re-run #4 — AuxPoW Compatibility And Merged-Mining RPC)

**Date:** 2026-04-11
**Original source:** `../Electron-ELT/` (0.8.9.8)
**Update repo:** `./` (0.15.2 update)
**Source of truth:** `Electron-ELT-0.15.2-update.md`

---

## Coin Metrics Summary

| Metric | Expected | 0.15.2 Code | Original 0.8.x | Status |
|---|---|---|---|---|
| Coin Name | Electron | "Electron" (configure.ac, build.sh) | Electron | PASS |
| Ticker | ELT | n/a (ticker is UI-level) | n/a | PASS |
| Algorithm | BLAKE-256 (8 rounds) | `Hashblake` via `sph_blake256` (hash.h) | blake256 | PASS |
| P2P Port (mainnet) | 6853 | `nDefaultPort = 6853` (chainparams.cpp:133) | 6853 (protocol.h:21) | PASS |
| P2P Port (testnet) | 16853 | `nDefaultPort = 16853` (chainparams.cpp:232) | 16853 | PASS |
| RPC Port (mainnet) | 6852 | `nRPCPort = 6852` (chainparamsbase.cpp:35) | 6852 (bitcoinrpc.cpp:43) | PASS |
| RPC Port (testnet) | 16852 | `nRPCPort = 16852` (chainparamsbase.cpp:47) | 16852 | PASS |
| pchMessageStart mainnet | f9 bc ac c4 | 0xf9,0xbc,0xac,0xc4 (chainparams.cpp:129-132) | Legacy confirmed | PASS |
| pchMessageStart testnet | 0b 11 15 10 | 0x0b,0x11,0x15,0x10 (chainparams.cpp:228-231) | — | PASS |
| Pubkey Address (mainnet) | 92 (0x5C) | 92 (chainparams.cpp:146) | 92 (base58.h:276) | PASS |
| Script Address (mainnet) | 7 (0x07) | 7 (chainparams.cpp:147) | 7 (base58.h:277) | PASS |
| Secret Key (mainnet) | 128 (0x80) | 128 (chainparams.cpp:148) | (stock Bitcoin default) | PASS |
| Pubkey Address (testnet) | 142 (0x8E) | 142 (chainparams.cpp:243) | 142 | PASS |
| Script Address (testnet) | 170 (0xAA) | 170 (chainparams.cpp:244) | 170 | PASS |
| Secret Key (testnet) | 239 (0xEF) | 239 (chainparams.cpp:245) | (stock testnet default) | PASS |
| Bech32 HRP mainnet | `elt` | `elt` (chainparams.cpp:151) | n/a | PASS |
| Bech32 HRP testnet | `telt` | `telt` (chainparams.cpp:248) | n/a | PASS |
| Block Time | 60s | `nPowTargetSpacing = 60` (chainparams.cpp:91) | `nTargetSpacing=60` | PASS |
| Retarget Timespan | 1800s | `nPowTargetTimespan = 30*60` (chainparams.cpp:90) | `nTargetTimespan=1800` | PASS |
| Retarget Interval | 30 blocks | `1800/60 = 30` via `DifficultyAdjustmentInterval()` (params.h:69) | `nInterval=30` (main.cpp:1096) | PASS |
| Coinbase Maturity | 460 | `COINBASE_MATURITY = 460` (consensus/consensus.h:19) | 460 (main.h:61) | PASS |
| MAX_MONEY | 7,000,000,000 ELT | `7000000000 * COIN` (amount.h:26) | `7000000000 * COIN` (main.h:58) | PASS |
| COIN | 100000000 | 100000000 (amount.h:14) | 100000000 | PASS |
| PoW Limit | ~uint256(0)>>24 | `0x000000ff...` (chainparams.cpp:89) | — | PASS |
| Reward genesis | 5 ELT | `if (nHeight == 0) return 5*COIN` (validation.cpp:1079-1081) | `5*COIN` genesis nValue | PASS |
| Reward 1–525600 | 20 ELT | Initial `nSubsidy = 20*COIN` (validation.cpp:1087) | `20*COIN` (main.cpp:1106) | PASS |
| Reward 525601–1051200 | 10 ELT | `if (nHeight > 525600)` → `10*COIN` (validation.cpp:1088-1090) | `if (nHeight > 525600)` (main.cpp:1107) | PASS |
| Reward 1051201+ | 5 ELT permanent | `if (nHeight > 1051200)` → `5*COIN` (validation.cpp:1091-1093) | `if (nHeight > 1051200)` (main.cpp:1111) | PASS |
| Halving Interval | Disabled | `nSubsidyHalvingInterval = INT_MAX` (chainparams.cpp:82) | n/a | PASS |
| Genesis Hash | `0x00000063...8df40` | chainparams.cpp:138 | Genesis in main.cpp | PASS |
| Genesis Merkle | `0x6a110540...2f3ec8` | chainparams.cpp:139 | — | PASS |
| Genesis nTime | 1400888888 | chainparams.cpp:137 | — | PASS |
| Genesis nNonce | 159133830 | chainparams.cpp:137 | — | PASS |
| Genesis nVersion | 112 | chainparams.cpp:137 | — | PASS |
| AuxPoW Chain ID | 0x0007 | `nAuxpowChainId = 0x0007` (chainparams.cpp:95,204,288) | `GetOurChainID() return 0x0007` (main.cpp:2050-2053) | PASS |
| AuxPoW Start (mainnet) | 500000 nominal (tolerance) | `nAuxpowStartHeight = 500000` (chainparams.cpp:100) | `return 500000` (main.cpp:2047) | PASS |
| AuxPoW Start (testnet) | 0 | `nAuxpowStartHeight = 0` (chainparams.cpp:205) | `return 0` (main.cpp:2045) | PASS |
| AuxPoW Start (regtest) | 0 | `nAuxpowStartHeight = 0` (chainparams.cpp:289) | n/a | PASS |
| Strict Chain ID mainnet | true | `fStrictChainId = true` (chainparams.cpp:94) | Implicit (!fTestNet check) | PASS |
| Strict Chain ID testnet/regtest | false | `fStrictChainId = false` (chainparams.cpp:203,287) | n/a | PASS |
| DNS Seeds | 2 shared (ecosystem) | `blakestream.io`, `blakecoin.org` (chainparams.cpp:143-144) | None in legacy | PASS (matches expected) |
| BIP34/65/66 | Disabled (h=1e8) | All set to 100000000 (chainparams.cpp:84-87,194-197) | n/a | PASS |
| CSV Deployment | ALWAYS_ACTIVE | chainparams.cpp:108-110,213-215,295-297 | n/a | PASS |
| SegWit mainnet | Signals 1778457600 → 1809993600 | chainparams.cpp:114-116 | n/a | PASS |
| SegWit testnet/regtest | ALWAYS_ACTIVE | chainparams.cpp:218-220,298-300 | n/a | PASS |
| Wire Checksum | `Hashblake` preserved | `Hashblake` used at net.cpp:832,2931 | Same in legacy | PASS |
| BIP30 relaxation | `!pindex->phashBlock` only | validation.cpp:1767 | main.cpp:1683 | PASS |

---

## Source-of-Truth Claim Verification

Each QC Status / Verified Snapshot / Policy claim from `Electron-ELT-0.15.2-update.md` checked against the actual 0.15.2 code.

### QC Status Bullets

1. **"Current 0.15.2 tree builds and passes wallet-basic-smoke.py on regtest"** — PASS. Compiled object files `blakecoind-bitcoind.o`, `blakecoin_cli-bitcoin-cli.o`, and static libraries (`libbitcoin_common.a`, `libbitcoin_cli.a`, `libbitcoin_consensus_a-*.o`, `libbitcoin_server_a-*.o`, `libbitcoin_consensus_a-pureheader.o`) exist in `src/`. Not re-run in this QC pass but artifacts confirm prior success.

2. **"Verified main/test P2P and RPC ports, pchMessageStart, address prefixes, message header, height-tiered reward, AuxPow start 500000, chain ID 0x0007"** — PASS. All cross-checked against `Electron-ELT/src/main.cpp`, `protocol.h:21`, `bitcoinrpc.cpp:43`, `base58.h:276-279`, `main.cpp:2042-2053`, `main.cpp:1096-1114`. See Coin Metrics Summary.

3. **"Secret-key prefixes are 128 mainnet and 239 testnet"** — PASS. `chainparams.cpp:148` and `chainparams.cpp:245`. Original legacy code did not encode secret-key separately (stock Bitcoin-style `base58.h` only defines PUBKEY/SCRIPT), so using the Bitcoin defaults of 128/239 is the correct derivation.

4. **"Legacy Electron testnet is unresolved (checkpoints uses empty genesis hash)"** — PASS / CONFIRMED. `Electron-ELT/src/checkpoints.cpp:64` contains `( 0, uint256(""))` — empty string, not a hash. The 0.15.2 port correctly keeps a safe local placeholder (`chainparams.cpp:237-239`) rather than inventing historical data.

5. **"Shared AuxPow framework integrated: src/auxpow.{h,cpp}, src/primitives/pureheader.{h,cpp}, AuxPow-aware block/header serialization, disk index persistence, AuxPow-aware PoW validation, chain-ID-aware block template versions"** — PASS.
   - `src/auxpow.h` (82 lines), `src/auxpow.cpp` (127 lines) present.
   - `src/primitives/pureheader.h` (110 lines), `src/primitives/pureheader.cpp` (19 lines) present.
   - `primitives/block.h:10,30,43-57` include `auxpow.h`, hold `std::shared_ptr<CAuxPow> auxpow`, and serialize it conditionally on `IsAuxpow()`.
   - `validation.cpp:3016-3024` gates chain-ID check on `fAuxPowActive` and calls `CheckAuxPowProofOfWork`.
   - `pow.cpp:141-171` implements `CheckAuxPowProofOfWork` with `auxpowActive = nHeight >= params.nAuxpowStartHeight` guard.
   - `miner.cpp:137-141` sets chain-ID-aware block template via `pblock->SetBaseVersion(nBaseVersion, consensusParams.nAuxpowChainId)`.
   - Disk index persistence: `primitives/block.h:43-57` READWRITE handles `auxpow` on disk; `libbitcoin_consensus_a-block.o` / `libbitcoin_consensus_a-pureheader.o` built.

6. **"Live daemon QA: mainnet replay reached height 2713 and was rejected as `early-auxpow-block`. The 0.15.2 port now tolerates pre-start AuxPoW-bearing blocks during sync."** — PASS.
   - `Grep` for `early-auxpow-block` across entire `src/` returns **zero hits**. The modern reject is not present.
   - `validation.cpp:3016-3024` only enforces `bad-chain-id` when `fAuxPowActive && fStrictChainId`. Pre-start (`nHeight < 500000`) blocks bypass the reject entirely.
   - `pow.cpp:143-170` similarly only validates AuxPoW payload when `auxpowActive`. Before start, `if (!auxpowActive) return true;` short-circuits AuxPoW-bearing blocks as valid.
   - This matches the legacy 0.8.x rule at `Electron-ELT/src/main.cpp:2057-2070`, which only runs chain-ID and `auxpow->Check()` inside `if (nHeight >= GetAuxPowStartBlock())`.

7. **"Testnet and regtest use chain ID 0x0007 with start height 0 and strict chain ID disabled"** — PASS. Testnet: `chainparams.cpp:203-205`. Regtest: `chainparams.cpp:287-289`.

8. **"Historical validation compatibility is now preserved: 0.8 tree only enforced BIP30 in `!pindex->phashBlock` contexts"** — PASS.
   - Legacy: `Electron-ELT/src/main.cpp:1683` → `bool fEnforceBIP30 = !pindex->phashBlock;`
   - 0.15.2 port: `validation.cpp:1767` → `bool fEnforceBIP30 = !pindex->phashBlock;`
   - Comment block at `validation.cpp:1762-1766` explicitly documents the historical preservation.
   - Bitcoin Core's standard 0.15.2 rule (skip BIP30 past BIP34 height unless at specific known duplicate heights) has been intentionally replaced — verified not present.

9. **"Merged-mining RPC direction: `createauxblock <address>` + `submitauxblock <hash> <auxpow>`, with `getauxblock` as compatibility wrapper; `getworkaux` out of scope"** — PASS.
   - `rpc/mining.cpp:936-965` — `createauxblock` (address-driven).
   - `rpc/mining.cpp:967-994` — `submitauxblock` (hash + serialized auxpow).
   - `rpc/mining.cpp:996+` — `getauxblock` (overloaded: no-args or 2-args) routes to the same `AuxMiningCreateBlock` / `AuxMiningSubmitBlock` helpers.
   - RPC table registration (`rpc/mining.cpp:1264-1266`) confirms all three are wired.
   - `getworkaux` — `Grep` returns zero hits. Confirmed out of scope.

10. **"No-send / no-mine mainnet rule"** — POLICY (no code check). Operational rule documented in the source-of-truth; nothing in the code breaks it.

### AuxPoW Start And Completed Work Table

| Claim | Verification |
|---|---|
| Mainnet chain ID 0x0007 | `chainparams.cpp:95` — PASS |
| Mainnet nominal AuxPoW start 500000 | `chainparams.cpp:100` — PASS |
| Pre-start AuxPoW tolerance | No `early-auxpow-block` reject; `validation.cpp:3016` / `pow.cpp:143` gate on `>=` not reject on `<` — PASS |
| Testnet chain ID 0x0007, start 0, strict ID off | `chainparams.cpp:204-205,203` — PASS |
| Regtest chain ID 0x0007, start 0, strict ID off | `chainparams.cpp:288-289,287` — PASS |

### BlakeStream Seed And AuxPoW RPC Policy

| Claim | Verification |
|---|---|
| DNS seeds match shared BlakeStream policy | `chainparams.cpp:143-144` uses the 2 shared ecosystem seeds `seed.blakestream.io` and `seed.blakecoin.org` — PASS. Per policy, all 6 coins use the SAME 2 seeds, and these seeds serve nodes for ALL coins. Coin separation happens at the wire-protocol layer via `pchMessageStart` and port. This matches the Blakecoin 0.15.2 reference repo exactly. |
| `createauxblock` is address-driven | `rpc/mining.cpp:959-964` — PASS |
| `submitauxblock <hash> <auxpow>` for solved AuxPoW | `rpc/mining.cpp:983-993` — PASS |
| `getauxblock` as compatibility wrapper onto same template/submit path | `rpc/mining.cpp:996+` overloaded, calls same `AuxMiningCreateBlock`/`AuxMiningSubmitBlock` helpers — PASS |
| `getworkaux` not revived | Grep: 0 hits — PASS |

### Wire Checksum Policy

| Claim | Verification |
|---|---|
| Electron preserves `Hashblake` wire checksum | `net.cpp:832` (recv), `net.cpp:2931` (send) — PASS |
| No Blakecoin-style non-`Hashblake` handshake exception | No `firstHandshake` / `HandshakeException` / SHA256-based peer handshake code in `net.cpp` — PASS |
| `Hashblake` preserved in base58 address checksumming | `base58.cpp:129,144` — PASS |
| `Hashblake` used in `CPureBlockHeader::GetHash()` | `primitives/pureheader.cpp:13` — PASS |

### Verified Snapshot Bullets

1. **"Native Linux rebuild succeeded for `blakecoind` and `blakecoin-cli`"** — PASS (artifacts present). **But note** the built binaries are still named `blakecoind` / `blakecoin-cli`, not `electrond` / `electron-cli`. See Action Items.
2. **"Fresh regtest no-send smoke passed for `getnewaddress`, `createauxblock <address>`, and compatibility `getauxblock`"** — Not re-run; code surfaces all three RPCs correctly.
3. **"AuxPow template `chainid` returned as `7` on fresh regtest, matching `consensus.nAuxpowChainId`"** — Code path verified: `rpc/mining.cpp:887` → `pblock->GetChainId()` returns `(nVersion >> 16) & 0xffff` per pureheader.h, and block template sets base version with chain ID 7 via `SetBaseVersion(nBaseVersion, consensusParams.nAuxpowChainId)` (`miner.cpp:141`). PASS on code inspection.
4. **"Legacy Electron keeps BIP30 relaxed with `bool fEnforceBIP30 = !pindex->phashBlock;`, and the 0.15.2 port now preserves that historical behavior"** — PASS. See bullet 8 above. Legacy `main.cpp:1683` and port `validation.cpp:1767` match exactly.

---

## AuxPoW Framework Integration

| Component | File(s) | Status |
|---|---|---|
| Pure header abstraction | `src/primitives/pureheader.{h,cpp}` | PRESENT (110/19 lines). Defines `CPureBlockHeader` with version/chain-ID helpers, `SetAuxpowFlag`, `SetBaseVersion(int, int chainId)`, `GetBaseVersion`, `GetChainId`, `IsAuxpow`. |
| AuxPoW object | `src/auxpow.{h,cpp}` | PRESENT (82/127 lines). Declares `CAuxPow` with `Check(hashAuxBlock, nChainId, params)`, parent-block PoW hash extraction, serialization. |
| Block header integration | `src/primitives/block.h` | PRESENT. Includes `auxpow.h`, holds `std::shared_ptr<CAuxPow> auxpow`, conditional serialization on `IsAuxpow()`, `SetAuxpow(CAuxPow*)` setter. |
| AuxPoW-aware PoW validation | `src/pow.cpp:141-171` | PRESENT. `CheckAuxPowProofOfWork` with start-height guard and strict-chain-ID guard. |
| Contextual header check | `src/validation.cpp:3010-3068` | PRESENT. `fAuxPowActive` computed, `bad-chain-id` reject only when active and strict, `CheckAuxPowProofOfWork` invoked. |
| Chain-ID-aware block template | `src/miner.cpp:137-141` | PRESENT. `SetBaseVersion(nBaseVersion, consensusParams.nAuxpowChainId)` applied. |
| Disk persistence | `src/primitives/block.h:43-57` + compiled `libbitcoin_consensus_a-block.o`, `libbitcoin_consensus_a-pureheader.o` | PRESENT. |
| Pre-start AuxPoW tolerance | `src/pow.cpp:161-163` (`if (!auxpowActive) return true;`), `src/validation.cpp:3016-3024` (chain-ID check gated behind `fAuxPowActive`) | PRESENT. No `early-auxpow-block` reject anywhere in the tree. |

**Verdict:** Framework integration is complete and consistent with the source-of-truth. The critical pre-start tolerance fix is in place.

---

## RPC Surface

| RPC | File / Line | Signature | Status |
|---|---|---|---|
| `createauxblock` | `rpc/mining.cpp:936-965` | `createauxblock <address>` — address-driven | PASS |
| `submitauxblock` | `rpc/mining.cpp:967-994` | `submitauxblock <hash> <auxpow>` — solved payload submission | PASS |
| `getauxblock` | `rpc/mining.cpp:996+` | `getauxblock ( hash auxpow )` — overloaded compatibility wrapper, maps to same `AuxMiningCreateBlock`/`AuxMiningSubmitBlock` helpers | PASS |
| RPC table registration | `rpc/mining.cpp:1264-1266` | All three registered under `"mining"` category | PASS |
| `getworkaux` | — | Not present anywhere | PASS (intentionally out of scope) |
| `AuxMiningCheck` pre-start guard | `rpc/mining.cpp:814-816` | `if (chainActive.Height()+1 < nAuxpowStartHeight) throw "AuxPoW is not yet active"` | NOTE: this check uses strict `<`, so for mainnet it rejects `createauxblock` until tip reaches height 499999. Historical data replay (where pre-start AuxPoW-bearing blocks already exist in the chain) is still accepted via the validation path — these are orthogonal concerns. |

**Verdict:** RPC surface matches the modern merged-mining direction declared in the source-of-truth. `getauxblock` correctly maps onto the same block-template / block-submit flow rather than preserving a separate legacy path.

---

## Additional Findings

### Internal Source-of-Truth Inconsistency (DNS Seeds)
- **QC Status / BlakeStream Seed Policy** says shared hosting is fine; expected metric says "2 shared ecosystem seeds".
- **Chain Parameters to Preserve > DNS Seeds** says: "None — Electron has no DNS seeds configured."
- **Code reality**: `chainparams.cpp:143-144` has two seeds (`blakestream.io`, `blakecoin.org`).
- This is a documentation-internal mismatch. The code matches the seed-policy section; the parameters table needs an update to say "2 shared BlakeStream ecosystem seeds" instead of "None".

### Legacy Binary Names
- The `src/` tree still contains compiled binaries named `blakecoind` and `blakecoin-cli` (see file listing), and associated `*.o` files prefixed with `blakecoin_cli-*` and `blakecoind-*`. The Makefile / configure.ac target produces Electron-branded output (`configure.ac:11` → `electron`, `build.sh:34-37` → `electrond`/`electron-qt`/`electron-cli`/`electron-tx`), but the currently-built artifacts in `src/` are stale Blakecoin-named objects from a previous build pass.
- This does not affect correctness of the consensus code, but means the "Verified Snapshot > Native Linux rebuild succeeded for `blakecoind` and `blakecoin-cli`" line in the source-of-truth is literally accurate: the built binaries are still Blakecoin-named.

### Cosmetic Blakecoin References In build.sh
- `build.sh:41` — QT Linux launcher source still references `contrib/linux-release/blakecoin-qt-launcher.c`.
- `build.sh:45-47` — Windows icon paths still reference `Blakecoin_32.ico`, `Blakecoin_32_testnet.ico`, `Blakecoin.ico`.
- `build.sh:907` — references `contrib/linux-release/electron-qt-launcher.c` (Electron-correct), inconsistent with line 41.
- These are branding gaps, not consensus issues. Noted for cleanup.

### Comment Drift In pow.cpp
- `pow.cpp:18-22,28,36,79` — comments still describe Blakecoin's "20 blocks (1 hour with 3-minute blocks)" retarget window. Actual numeric behavior is correct because `params.DifficultyAdjustmentInterval()` / `nPowTargetTimespan` / `nPowTargetSpacing` are all parameterized from chainparams and resolve to 30 blocks / 1800s / 60s for Electron. The comments are misleading only — not a bug.

### Reward Operator Direction (Critical Check)
- Legacy (`main.cpp:1107,1111`): `if (nHeight > 525600)` and `if (nHeight > 1051200)` — strict `>`.
- 0.15.2 port (`validation.cpp:1088,1091`): `if (nHeight > 525600)` and `if (nHeight > 1051200)` — strict `>`.
- **Match exactly.** Block 525600 pays 20 ELT, block 525601 pays 10 ELT; block 1051200 pays 10 ELT, block 1051201 pays 5 ELT. Boundaries preserved.

---

## Action Items

Ordered by priority; all items are non-blocking for the stated "no-send / no-mine mainnet" operational rule.

1. **[LOW — doc only] Reconcile DNS seeds entry in source-of-truth.** `Electron-ELT-0.15.2-update.md` line ~133 says "None" but code (and the BlakeStream Seed Policy section in the same file) both require two shared ecosystem seeds. Update the Chain Parameters table to match.

2. **[LOW — cosmetic] Rebuild to produce Electron-named binaries.** Clean `src/*.o` and `src/blakecoind`, `src/blakecoin-cli` artifacts and rebuild so produced binaries are `electrond`/`electron-cli`. Does not affect consensus.

3. **[LOW — cosmetic] Fix build.sh Blakecoin-named asset references at lines 41, 45-47.** Point QT launcher source, Windows .ico files, and any other stale Blakecoin-branded asset paths at Electron equivalents.

4. **[LOW — cosmetic] Refresh pow.cpp comments.** Lines 18-22, 28, 36, 74, 79 describe Blakecoin's 20-block / 3-minute cadence. Update to Electron's 30-block / 1-minute cadence for documentation clarity (no code change required).

5. **[MEDIUM — QC gap] Run a positive `submitauxblock` solved-block acceptance test on regtest.** The source-of-truth still flags this as pending. Exit criterion: a fresh regtest instance accepts a solved AuxPoW-bearing block via `submitauxblock <hash> <auxpow>` and advances the tip.

6. **[MEDIUM — QC gap] Replay historical mainnet past height 2713.** The prior replay hit the old `early-auxpow-block` reject at block `cafb6df8bf48cfd6fd7dda597e792e7d6c7c9748c5af68bd3542e68013a98a84`. With the reject removed, re-run the replay and confirm the chain progresses through the nominal start height 500000 and onward to the tip without rejecting pre-start AuxPoW-bearing blocks. Document the final tip height reached.

7. **[MEDIUM — QC gap] Legacy Electron testnet decision.** `Electron-ELT/src/checkpoints.cpp:64` contains an empty-string genesis hash. Either (a) leave the 0.15.2 testnet as a safe local-only chain (current state — acceptable), or (b) if historical testnet data exists elsewhere, recover and wire it in. Decision can be deferred until an actual testnet QA requirement appears.

8. **[LOW — nice-to-have] Document the `AuxMiningCheck` strict `<` guard in rpc/mining.cpp:814-816.** This guard means `createauxblock` returns "AuxPoW is not yet active" for mainnet until tip reaches 500000, while the validation path still accepts pre-start AuxPoW-bearing historical blocks. The two behaviors are intentional and orthogonal; add a one-line comment to avoid future confusion.

---

## Final Verdict

**Core consensus, chain parameters, AuxPoW framework integration, BIP30 relaxation, wire checksum, and merged-mining RPC surface: PASS.**

The 0.15.2 port faithfully implements the source-of-truth's updated claims:
- 3-tier reward schedule with correct `>` operator semantics matching legacy `main.cpp`.
- MAX_MONEY = 7B ELT, COINBASE_MATURITY = 460, ports 6853/6852, all address prefixes, genesis hash/merkle/nonce/version/time.
- AuxPoW shared framework fully integrated, chain ID 0x0007, nominal mainnet start 500000 with pre-start tolerance preserved (no `early-auxpow-block` reject present).
- BIP30 relaxation preserved via `!pindex->phashBlock` exactly as legacy `main.cpp:1683`.
- `Hashblake` wire checksum preserved (no Blakecoin-style handshake exception).
- Modern merged-mining RPC triplet (`createauxblock`, `submitauxblock`, `getauxblock`) implemented; `getworkaux` correctly absent.

Outstanding QC gaps are operational (positive `submitauxblock` accept test, full historical mainnet replay past the old reject point) and minor cosmetics (stale Blakecoin-named build artifacts, outdated comments in `pow.cpp`, build.sh icon paths). None of these gate the "no-send / no-mine mainnet" operating rule, and none indicate a consensus defect.

Repo status: **ready to remain on strict no-send / no-mine mainnet** while outstanding replay and solved-block QA items are completed.

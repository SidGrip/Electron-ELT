# Electron-ELT 0.15.2 Update — Source of Truth

## Overview

Port Electron (ELT) from its current 0.8.9.8 codebase to Bitcoin Core 0.15.2, following the same approach used for the Blakecoin 0.15.2 update (`Blakecoin-0.15.21`).

**Reference codebase:** `../Blakecoin-0.15.21/` — the completed Blakecoin port to 0.15.2
**Original codebase:** `../Electron-ELT/` — current 0.8.9.8 source with all coin-specific parameters

---

## QC Status

- QC is now strong enough to treat the core AuxPow port and the modern merged-mining RPC direction as implemented. Historical-header compatibility is now documented well enough for rollout planning; the production carry-back staging that used to remain is now green.
- The current 0.15.2 tree builds and passes `test/functional/wallet-basic-smoke.py` on regtest with no sends, no funding, and no mining.
- Verified from `../Electron-ELT/src`: main/test P2P and RPC ports, `pchMessageStart`, address prefixes, message header, the height-tiered reward schedule, legacy AuxPow start height `500000`, and chain ID `0x0007`.
- Corrected from source: secret-key prefixes are `128` mainnet and `239` testnet, not `TBD`.
- Legacy Electron testnet is unresolved in the source tree (`checkpoints.cpp` uses an empty genesis hash). The 0.15.2 port should treat testnet/regtest as safe local chains unless that historical data is recovered.
- The shared AuxPow framework is now integrated: `src/auxpow.{h,cpp}`, `src/primitives/pureheader.{h,cpp}`, AuxPow-aware block/header serialization, disk index persistence, AuxPow-aware PoW validation, and chain-ID-aware block template versions.
- Live daemon QA correction: current mainnet bootstrap and peer replay reaches height `2713` and then rejects header `cafb6df8bf48cfd6fd7dda597e792e7d6c7c9748c5af68bd3542e68013a98a84` as `early-auxpow-block` if the port uses a modern pre-start header reject. The 0.15.2 port therefore keeps the legacy nominal AuxPow start height `500000`, but restores the 0.8.x compatibility rule that tolerates AuxPoW-bearing blocks before that height during sync.
- Testnet and regtest use chain ID `0x0007` with start height `0` and strict chain ID disabled for local QA.
- Verified after the fan-out: `make -C src -j4 blakecoind blakecoin-cli` succeeds and `test/functional/test_runner.py --jobs=1 wallet-basic-smoke.py` passes on regtest.
- Historical validation compatibility is now preserved: the legacy Electron 0.8 tree only enforced the BIP30 overwrite rule in `!pindex->phashBlock` contexts, so the 0.15.2 port must not silently restore Bitcoin Core's broader duplicate-txid rejection path.
- Merged-mining RPC direction is now fixed for this port: primary RPCs are `createauxblock <address>` plus `submitauxblock <hash> <auxpow>`, with `getauxblock` kept only as a compatibility wrapper for older pool software. `getworkaux` is intentionally out of scope unless a real dependency is later proven.
- Keep this repo on a strict no-send / no-mine mainnet rule until the final production pool and Electrium carry-back staging are complete.

### BlakeStream Seed And AuxPoW RPC Policy

- BlakeStream DNS seeds (`seed.blakestream.io`, `seed.blakecoin.org`) are shared across all six coins and serve nodes for ALL coins. A single seed lookup returns peer IPs regardless of which coin is asking; coin separation happens at the wire-protocol layer via each coin's unique `pchMessageStart` and default port.
- The production direction for this repo is a modern merged-mining RPC surface: `createauxblock` to build the child-chain template and `submitauxblock` to submit the solved AuxPoW payload.
- `createauxblock` is address-driven on purpose so a pool can choose the child-chain payout script explicitly instead of depending on wallet mining state inside the daemon.
- `getauxblock` remains only as a compatibility mode for older merged-mining software. It should map onto the same block-template / block-submit flow rather than preserving a separate legacy implementation path.
- `getworkaux` is not part of the planned 0.15.2 target. We are not reviving `getwork`-era RPC unless a live pool or deployment proves it is still required.
- The same 2 DNS seeds (`seed.blakestream.io`, `seed.blakecoin.org`) are used by all six coins. This matches the Blakecoin 0.15.2 reference repo exactly.

### Wire Checksum Policy

- Electron should preserve the legacy `Hashblake` P2P message checksum behavior for current network interoperability.
- Do not normalize Electron to Blakecoin's temporary non-`Hashblake` handshake exception.
- Keep Blakecoin documented as the one current exception; Electron stays on `Hashblake` before go-live unless a fresh compatibility review says otherwise.

## AuxPoW Start And Completed Work

| Network | Chain ID | Nominal AuxPoW Start | Observed Pre-Start AuxPoW Evidence In Current QA | Exact Time/Date Status | 0.15.2 Port Rule |
|---------|----------|----------------------|-----------------------------------------------|------------------------|------------------|
| Mainnet | `0x0007` | `500000` | Replay reached height `2713` before hitting the old `early-auxpow-block` reject | Exact timestamp remains archival-only; compatibility rule is already proven by replay | Keep `500000` as the nominal legacy value, but tolerate earlier historical AuxPoW-bearing blocks during bootstrap / IBD |
| Testnet | `0x0007` | `0` | N/A | Local QA only | AuxPoW enabled for local QA; strict chain ID disabled |
| Regtest | `0x0007` | `0` | N/A | Local QA only | AuxPoW enabled for local QA; strict chain ID disabled |

Interpretation note:
`500000` remains the nominal legacy mainnet AuxPoW start preserved in `chainparams.cpp`. The recent fix did not lower that configured height. It changed validation behavior so preserved historical chain data is not incorrectly rejected just because AuxPoW appears earlier than the nominal legacy boundary.

- Completed in this repo:
  - Integrated the shared AuxPoW framework with `src/auxpow.{h,cpp}` and `src/primitives/pureheader.{h,cpp}`.
  - Ported AuxPoW-aware block/header serialization, disk index persistence, block version handling, and PoW validation.
  - Kept Electron's nominal mainnet start height at `500000` while removing the modern `early-auxpow-block` reject that broke historical chain acceptance around height `2713`.
  - Implemented the modern merged-mining RPC direction: `createauxblock <address>` and `submitauxblock <hash> <auxpow>`, with `getauxblock` retained only as a compatibility wrapper.
  - Verified no-send regtest wallet smoke coverage and buildable 0.15.2 daemon/Qt outputs after the AuxPoW compatibility correction.
- Operational rule:
  - Keep the strict no-send / no-mine mainnet rule in place while final production pool and Electrium carry-back staging continue.

---

## Coin Identity

| Parameter | Value |
|-----------|-------|
| Coin Name | Electron |
| Ticker | ELT |
| Algorithm | Blake-256 (8 rounds, single-pass) |
| Merge Mining | Yes (AuxPow; legacy nominal start 500000 with pre-start compatibility preserved) |
| Base Version (current) | 0.8.9.8 (forked from Bitcoin 0.8.5) |
| Target Version | 0.15.2 |

---

## Chain Parameters to Preserve

### Network

| Parameter | Mainnet | Testnet |
|-----------|---------|---------|
| P2P Port | 6853 | 16853 |
| RPC Port | 6852 | 16852 |
| pchMessageStart | 0xf9, 0xbc, 0xac, 0xc4 | 0x0b, 0x11, 0x15, 0x10 |

### Address Prefixes

| Type | Mainnet | Testnet |
|------|---------|---------|
| Pubkey Address | 92 (0x5C) | 142 (0x8E) |
| Script Address | 7 (0x07) | 170 (0xAA) |
| Secret Key | 128 (0x80) | 239 (0xEF) |
| Bech32 HRP | `elt` | `telt` |

### Block Parameters

| Parameter | Value |
|-----------|-------|
| Block Time | 60 seconds (1 minute) |
| Target Timespan | 1,800 seconds (30 minutes) |
| Retarget Interval | 30 blocks |
| Coinbase Maturity | 460 blocks |
| Max Supply | 7,000,000,000 ELT |
| COIN | 100,000,000 satoshis |
| PoW Limit | ~uint256(0) >> 24 |

### Block Reward Schedule (CRITICAL — Non-Standard)

Electron uses a **time-decay reward**, NOT standard halving:

| Block Range | Reward |
|-------------|--------|
| Genesis (block 0) | 5 ELT |
| 1 — 525,600 | 20 ELT |
| 525,601 — 1,051,200 | 10 ELT |
| 1,051,201+ | 5 ELT (permanent) |

**This reward logic is ported exactly to `GetBlockSubsidy()` in `validation.cpp` using the legacy `>` threshold checks.**

### Genesis Block

| Parameter | Value |
|-----------|-------|
| Hash | `0x00000063044a9b6cad9015b289de8a86a654d5a916fcaaeeefb5b9408448df40` |
| nTime | 1400888888 (May 24, 2014) |
| Merkle Root | `0x6a1105401133793ebc124b837824144b64058bd0b89408cd17201012972f3ec8` |
| nNonce | 159,133,830 |
| nVersion | 112 |

### DNS Seeds

- **None** — Electron has no DNS seeds configured. The 0.15.2 port keeps safe local testnet/regtest placeholders only.

### Checkpoints

| Block | Hash |
|-------|------|
| 0 | `0x00000063044a9b6cad9015b289de8a86a654d5a916fcaaeeefb5b9408448df40` |
| 25550 | `0xe8b6e0cfda9a65717ac300d5e1222bbbe45439d13cb9b681c77b775f77d8ec09` |
| 107785 | `0x0c57a0c979d324a667563d417eabc0a72d3710a70b92fb5541f776f48e50766b` |
| 194686 | `0x591461dfbaedcb23a96d2b8ce5b202ebb501bae203817c060bc44dafd563cbba` |

---

## What Needs to Be Done

### Phase 1: Copy & Rebrand the Blakecoin 0.15.2 Base

1. **Copy** the entire `Blakecoin-0.15.21` codebase into this directory
2. **Rename** all Blakecoin references → Electron:
   - Binary names: `electrond`, `electron-qt`, `electron-cli`, `electron-tx`
   - Config file: `electron.conf`, config dir `~/.electron/`
   - URI scheme: `electron://`
   - Desktop entry, icons, window titles
   - `configure.ac`: package name, version
   - Window title: "Electron - Wallet"
   - Application name: "Electron"
   - Signed message header: "Electron Signed Message:\n"

### Phase 2: Apply Coin-Specific Parameters

3. **`src/chainparams.cpp`** — Replace ALL chain parameters:
   - Genesis block (hash, nTime=1400888888, nNonce, coinbase)
   - Network ports (P2P: 6853, RPC: 6852)
   - Message start bytes (0xf9, 0xbc, 0xac, 0xc4)
   - Address prefixes (pubkey: 92, script: 7)
   - Block timing (60s block time, 30-min retarget, 30-block interval)
   - Checkpoints
   - Bech32 HRP
   - **No DNS seeds** — leave empty or add hardcoded nodes

4. **`src/amount.h`** — MAX_MONEY = 7,000,000,000 * COIN (7 billion ELT)

5. **`src/validation.cpp`** — Block reward logic (**CUSTOM — NOT HALVING**):
   ```
   if (height == 0) return 5 * COIN;
   if (height > 1051200) return 5 * COIN;
   if (height > 525600) return 10 * COIN;
   return 20 * COIN;
   ```
   - This replaces the standard `GetBlockSubsidy()` halving logic entirely and is implemented in the 0.15.2 port.
   - **Halving interval should be set to max (disabled)** since rewards are height-range based

6. **`src/consensus/consensus.h`** — COINBASE_MATURITY = 460

7. **`src/qt/`** — Update all GUI branding for Electron

### Phase 3: AuxPow / Merge Mining

8. **AuxPow integration** — Electron supports merge mining
   - Ported from the shared BlakeBitcoin 0.15.2 reference using modern Namecoin/Dogecoin structure and Electron legacy constants
   - `src/auxpow.{h,cpp}` and `src/primitives/pureheader.{h,cpp}` are integrated, with AuxPow-aware block/header serialization, disk index persistence, PoW validation, and block template versions
   - Mainnet keeps the legacy nominal activation height `500000`, but the 0.15.2 port now preserves the 0.8.x rule that tolerated pre-start AuxPoW-bearing blocks during historical sync
   - The modern merged-mining RPC path is now implemented and no-send regtest-smoke verified for `createauxblock <address>` plus compatibility `getauxblock`
   - Historical merged-mined header compatibility is now documented from preserved replay evidence; exact activation dating remains archival-only and is no longer the release blocker

### Phase 4: Build System

9. **`build.sh`** — Update all variables:
   - COIN_NAME: "electron"
   - DAEMON_NAME: "electrond"
   - QT_NAME: "electron-qt"
   - CLI_NAME: "electron-cli"
   - TX_NAME: "electron-tx"
   - VERSION: "0.15.2"
   - RPC_PORT: 6852
   - P2P_PORT: 6853

10. **Docker configs** — Same Docker images as Blakecoin 0.15.2

### Phase 5: SegWit Activation

11. **Mainnet SegWit rollout** — Mainnet versionbits signaling starts on May 11, 2026 00:00:00 UTC (`1778457600`) and times out on May 11, 2027 00:00:00 UTC (`1809993600`).
12. **Activation semantics** — May 11, 2026 is the signaling start date, not guaranteed same-day activation. Actual mainnet SegWit enforcement still depends on miner signaling and BIP9 lock-in.
13. **CSV / test networks** — CSV stays `ALWAYS_ACTIVE`, and testnet/regtest keep `ALWAYS_ACTIVE` SegWit for controlled QA and wallet validation.
14. **BIP34/65/66** — Disable version checks (height 100000000)

---

## Key Differences from Blakecoin

| Aspect | Blakecoin | Electron |
|--------|-----------|----------|
| Block Time | 180s (3 min) | 60s (1 min) |
| Retarget Interval | 20 blocks | 30 blocks |
| Retarget Timespan | 1 hour | 30 minutes |
| Reward Model | Dynamic formula | Time-decay (20→10→5 ELT) |
| Max Supply | 21M | 7 billion |
| Coinbase Maturity | ??? | 460 blocks |
| P2P Port | 8773 | 6853 |
| RPC Port | 8772 | 6852 |
| Pubkey Address | 26 | 92 |
| Genesis Date | Oct 2013 | May 2014 |

---

## Potential Issues & Gotchas

1. **Massive max supply** — 7 billion ELT vs 21M BLC. Ensure `MAX_MONEY` doesn't overflow `CAmount` (int64_t max is ~92 billion * COIN, so 7B * 100M = 7e17 which is fine).
2. **Custom reward schedule** — Implemented exactly. Three tiers based on block height, not halving. The `nSubsidyHalvingInterval` is disabled.
3. **1-minute block time** — Fastest of all the coins. Retarget every 30 blocks (30 minutes). Ensure difficulty adjustment works correctly at this speed.
4. **High coinbase maturity (460)** — Much higher than Bitcoin's 100. Users must wait ~7.7 hours for mined coins to mature.
5. **No DNS seeds** — Network discovery relies entirely on addnode/connect. May want to set up seed nodes.
6. **AuxPow follow-on work** — The shared AuxPow framework is now ported and the modern merged-mining RPC surface is in place. The former production pool / Electrium carry-back staging gate is now green.
7. **Historical BIP30 rule** — The original Electron chain relaxed BIP30 during normal block connection (`!pindex->phashBlock` only). Do not re-enable the stock 0.15.2 BIP30 path without proving the historical chain does not rely on that behavior.
8. **Genesis block merkle root** — Not explicitly stored in 0.8.x code (computed at runtime). Need to extract from a running node or block explorer.

---

## Build & Test Plan

1. Rebuild native Linux after any consensus change and keep `blakecoind` / `blakecoin-cli` green
2. Re-run `test/functional/test_runner.py --jobs=1 wallet-basic-smoke.py` on regtest after consensus-touching edits
3. Verify genesis block hash, address generation, and RPC defaults on port `6852`
4. Re-check reward boundaries at heights `0`, `1`, `525600`, `525601`, `1051200`, and `1051201`
5. Preserve the archived historical replay notes and do not reintroduce the rejected `early-auxpow-block` rule before any mainnet activity
6. Keep positive `submitauxblock` acceptance testing isolated to regtest or dedicated QA environments
7. Build AppImage, Windows, and macOS artifacts after consensus validation is stable

---

## Verified Snapshot

- Native Linux rebuild succeeded for `blakecoind` and `blakecoin-cli`.
- Fresh regtest no-send smoke passed for `getnewaddress`, `createauxblock <address>`, and compatibility `getauxblock`.
- Verified AuxPow template `chainid` returned as `7` on fresh regtest, matching `consensus.nAuxpowChainId`.
- Verified from `../Electron-ELT/src/main.cpp`: legacy Electron keeps BIP30 relaxed with `bool fEnforceBIP30 = !pindex->phashBlock;`, and the 0.15.2 port now preserves that historical validation behavior.
- Direct `createauxblock` plus `submitauxblock` acceptance is now proven in isolated QA. The production carry-back staging that used to block release is now green.

---

## File Reference

| What | Where |
|------|-------|
| Reference (completed) | `../Blakecoin-0.15.21/` |
| Original coin source | `../Electron-ELT/` |
| Original params | `../Electron-ELT/src/main.cpp` |
| Original build script | `../Electron-ELT/build.sh` |
| Qt project file | `../Electron-ELT/Electron-qt.pro` |

## SegWit Activation Test

- Functional test: `test/functional/segwit-activation-smoke.py`
- Build-server wrapper: `/home/sid/Blakestream-Installer/qa/runtime/run-segwit-activation-suite.sh`
- Direct command used by the wrapper:

```bash
BITCOIND=/path/to/electrond BITCOINCLI=/path/to/electron-cli \
python3 ./test/functional/segwit-activation-smoke.py \
  --srcdir="$(pwd)/src" \
  --tmpdir="<artifact_root>/electron-elt/<timestamp>/tmpdir" \
  --nocleanup \
  --loglevel=DEBUG \
  --tracerpc
```

- Expected regtest Bech32 prefix: `relt1`
- Review artifacts:
  `summary.json`, `state-defined.json`, `state-started.json`, `state-locked_in.json`, `state-active.json`, `address-sanity.json`, `combined-regtest.log`, `tmpdir/test_framework.log`, `tmpdir/node*/regtest/debug.log`
- Successful all-six build-server run:
  `/home/sid/Blakestream-Installer/outputs/segwit-activation/20260412T083423Z/run-summary.md`
- Coin artifact directory:
  `/home/sid/Blakestream-Installer/outputs/segwit-activation/20260412T083423Z/electron-elt`
- Harness note:
  the final witness proposal builder now takes the coinbase amount directly from `getblocktemplate()["coinbasevalue"]`, and Electron regtest uses an intentionally easy `powLimit` so the activation proof remains practical without affecting mainnet behavior.
- Safety rule:
  regtest only for activation validation; do not mine or send transactions on mainnet while rollout QA is still in progress.

## AuxPoW Testnet Merged-Mining Verification

- Final successful container-built run:
  `/home/sid/Blakestream-Installer/outputs/auxpow-testnet/20260413T003341Z/run-summary.md`
- Wrapper command:
  `bash /home/sid/Blakestream-Installer/qa/auxpow-testnet/run-auxpow-testnet-suite.sh`
- Parent chain:
  Blakecoin testnet only, fully isolated from public peers.
- Live proof result:
  Electron-ELT accepted `1` merged-mined child block in the final 5-child proof.
- Direct RPC cross-check:
  `createauxblock` plus `submitauxblock` accepted on a fresh Electron-ELT testnet pair. Artifact:
  `/home/sid/Blakestream-Installer/outputs/auxpow-testnet/20260413T003341Z/electron-elt/rpc-crosscheck.json`
- QC note:
  the full 5-child proof required proxy `merkle-size = 16`; `8` was insufficient for the live chain-ID set once Electron-ELT was included.
- Safety rule:
  testnet only for merged-mining QA; do not mine or send transactions on mainnet while AuxPoW rollout validation is still in progress.

## Devnet/Testnet Validation Outcomes

- SegWit activation validation passed on isolated regtest. See:
  `/home/sid/Blakestream-Installer/outputs/segwit-activation/20260412T083423Z/electron-elt`
- AuxPoW merged-mining validation passed on isolated testnet, including direct `createauxblock` plus `submitauxblock` acceptance. See:
  `/home/sid/Blakestream-Installer/outputs/auxpow-testnet/20260413T003341Z/electron-elt`
- Mainnet carry-back audit for the devnet copy lives in:
  `mainnet-carryback-audit-2026-04-18.md`
- Audit result:
  the diff between this repo and the devnet `coins/Electron-ELT` copy stayed limited to devnet `chainparams*` and build cleanup. No new Electron-ELT mainnet wallet, consensus, or RPC carry-back was identified from the devnet copy itself.

## Mainnet Carry-Back Decisions

- SegWit rollout remains scheduled, not forced active.
- Mainnet AuxPoW start height remains `500000` as the chain source of truth.
- Do not port devnet network identity, datadir, test shortcuts, or activation shortcuts back into this repo.
- Pool/runtime carry-back work is tracked in the mainnet Eloipool repo.
- Electrium sync and signing carry-back work is tracked in the Electrium repo.
- Mainnet pool integration now depends on the proven multi-miner aux-child payout path in Eloipool, not the old single active mining-key QA shortcut.

## Staging Hygiene

- Keep the intentional autotools and build-system layer in staging for this repo:
  `Makefile.am`, `Makefile.in`, `aclocal.m4`, `autogen.sh`, `configure*`, `build-aux/*`, and `depends/*`.
- Trim generated build junk before review or promotion:
  `.libs/`, `.deps/`, `autom4te.cache/`, `*.o`, `*.lo`, `*.la`, `config.log`, `config.status`, and similar transient outputs.
- April 19, 2026 staging pass explicitly removed staged libtool and univalue build artifacts while preserving the intentional autotools carry-back set.

## Not Carried Back From Devnet

- `src/chainparams.cpp`, `src/chainparamsbase.cpp`, `src/chainparamsbase.h`
- Any private-testnet `BIP65Height = 1`, `ALWAYS_ACTIVE`, devnet ports, message starts, datadirs, or local-only harness shortcuts
- Pool UI, merged-mine proxy, Electrium, ElectrumX, and builder/runtime scripts

## Pool / Electrium Dependencies

- Mainnet merged-mining now depends on the modern `createauxblock` plus `submitauxblock` direction and the proven multi-miner aux payout model in Eloipool.
- Electrium compatibility now depends on full AuxPoW header support and Blake-family single-SHA signing compatibility.
- Per-coin overlays and branding stay in the Electrium repo and are not folded back into this C++ core tree.

## Safety Rule

- Do not mine on mainnet while carry-back staging is in progress.
- Do not send transactions on mainnet while carry-back staging is in progress.
- Use isolated regtest, testnet, or staging environments until rollout QA is complete.

## April 18, 2026 Devnet Validation Snapshot

- Shared BlakeStream devnet run `20260418T195508Z` proved concurrent multi-miner AuxPoW against the live pool with two mining keys active in the same session.
- Live pooled merged-mined Electron-ELT child block proof is now green:
  height `470` accepted with `tx_count = 3`, and it contained the queued test tx
  `1ddf764639594239e929a61a5bae99e3a073403bbafecf734f87350a42210d2c`.
- Electrium/ElectrumX runtime wiring stayed online during the same proof window.

## Mainnet Carry-Back Snapshot

- Keep Electron-ELT chain identity, AuxPoW start rules, and scheduled SegWit rollout exactly as already documented in this repo.
- Promote only the proven external dependencies:
  mainnet pool multi-miner mining-key payout plumbing and Electrium full AuxPoW-header plus single-SHA signing compatibility.
- Do not carry back any devnet ports, datadirs, private-testnet activation shortcuts, or runtime wrapper behavior into mainnet chain params.

## April 19, 2026 Broader Electrium Staging Closure

- Broader staged packaged-client proof is now green at:
  `/home/sid/Blakestream-Devnet/outputs/electrium-staging/20260419T053030Z/run-summary.md`
- Electron-ELT's packaged Electrium client connected successfully against the
  staged local ElectrumX backend on `127.0.0.1:53001`.
- This run also exposed and closed a shared aux-core startup bug in
  `src/validation.cpp`: height-less disk rereads could treat genesis-like
  headers as regular AuxPoW blocks and fail with a false
  `non-AUX proof of work failed` reject.
- The fix now treats
  `block.GetHash() == consensusParams.hashGenesisBlock || block.hashPrevBlock.IsNull()`
  as genesis-like in the disk-reread path, which keeps standalone staged
  backends honest without relaxing real chained AuxPoW validation.

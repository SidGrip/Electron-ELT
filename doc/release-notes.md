# Electron Core 0.25.2

Electron Core 0.25.2 is a release of the Electron (ELT) full node and wallet,
rebased onto the Bitcoin Core 25.2 codebase. Source and release binaries:

  https://github.com/BlueDragon747/Electron-ELT

Electron is a Blake-256 (8-round) AuxPoW merge-mined coin in the BlakeStream
family. For network parameters and the full 0.25.2 consensus details, see
`README.md`.

## How to upgrade

Shut down the running wallet/node (`electron-qt` or `electrond`) and wait for it
to stop completely, then replace the binaries (`electrond`, `electron-qt`,
`electron-cli`, `electron-tx`, `electron-wallet`) with the 0.25.2 build. Existing
`wallet.dat` and block/chain data are kept.

## Notable changes

- Rebased onto Bitcoin Core 25.2, preserving Electron's network magic, address
  formats, AuxPoW (chain ID `0x0007`) merge-mining, subsidy, and coinbase
  maturity rules.
- Dual wallet support: legacy Berkeley DB `wallet.dat` and descriptor SQLite
  wallets.

## Credits

Electron Core is built on Bitcoin Core. Thanks to the Bitcoin Core developers
and contributors, and to the Electron / BlakeStream contributors.

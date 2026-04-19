# Changelog

Revived is a fork of [LibreVR/Revive](https://github.com/LibreVR/Revive) picking up from the 3.2.0 release (March 2023).

All notable changes to this project will be documented in this file.
The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [3.3.0] - 2026-04-19

### Added

- ReviveXR: Lone Echo II entry in the known-hacks table.
- ReviveInjector: environment snapshot logged at startup for diagnostics.
- hook.cmd: optional `/openxr` flag; prompts for runtime choice at hook time.
- Input: missing touch outputs for right Touch face buttons.

### Changed

- ReviveInjector: OpenXR preferred when no injection flag is given and no OpenVR runtime is detected.
- User-facing strings rebranded to Revived with LibreVR attribution retained.
- BUILDING.md updated to reflect current toolchain (VS2019, Qt 5.15.2).

### Removed

- Oculus.js: `user_heartbeat` call disabled; the token-scraping path it depended on is broken against current Meta runtimes.

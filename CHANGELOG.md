# Changelog

Revived is a fork of [LibreVR/Revive](https://github.com/LibreVR/Revive) picking up from the 3.2.0 release (March 2023).

All notable changes to this project will be documented in this file.
The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [3.4.0] - 2026-04-24

### Added

- Settings page in the overlay dashboard for configuring runtime preference, Meta Horizon and games library paths, and UI colors (accent, border, settings background, dashboard background). Reachable from a Settings button in the dashboard and from the tray icon menu.
- Update checker: polls `update.txt` at the repo root on startup (with a 30-second delay) and via "Check for updates" in the tray menu. Up-to-date and update-available states both surface a balloon notification.
- ReviveManifestController: enumerates every GUID subkey under `HKCU\Software\Oculus VR, LLC\Oculus\Libraries` instead of only the default library, so games installed in secondary libraries are detected.
- ReviveInjector: logs Meta Horizon driver version and GPU driver version in the env snapshot.
- ReviveInjector: blocklist of common non-game processes (browsers, Steam, Discord, Explorer, Roblox clients). Hook attempts against listed processes exit cleanly instead of injecting.
- Installer: writes `HKCU\Software\Revived\DataFolder` based on install mode so the overlay and injector read logs from the matching folder (`%LOCALAPPDATA%\Revive\` for upgrades, `%LOCALAPPDATA%\Revived\` for fresh installs).

### Changed

- ReviveOverlay: multisampling forced off at the Qt surface level, avoiding the black-window rendering issue with default NVIDIA driver settings.

### Fixed

- ReviveInjector: env snapshot now builds and runs against the current OpenVR API.



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

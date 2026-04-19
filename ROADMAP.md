# Roadmap

Planned work for Revived. Dates are targets, not commitments. Scope can shrink but not grow within a version.

> Note: this roadmap may be completed before the targeted time. Dates are upper bounds.

## Codename scheme

Each release gets a codename from PC VR history. Hardware first (in release/reveal order), then companies.

Tags take the form `v3.3.1-dk1`, `v3.4.0-vive`, etc.

**Convention:** test builds and cancelled releases that never ship use the codename `crystal-cove` — the 2014 Oculus prototype that was demoed at CES but never reached customers. If a `crystal-cove` build is later promoted to a real release, it gets renamed to its proper codename at tag time.

### Hardware order

| Version | Codename | Headset | Year |
|---|---|---|---|
| 3.3.1 | dk1 | Oculus Rift DK1 | 2013 |
| 3.3.2 | dk2 | Oculus Rift DK2 | 2014 |
| 3.4.0 | vive | HTC Vive | 2016 |
| 3.4.x | cv1 | Oculus Rift CV1 | 2016 |
| 3.5.0 | rift-s | Oculus Rift S | 2019 |
| 3.5.x | index | Valve Index | 2019 |
| 3.6.0 | reverb | HP Reverb G1 | 2019 |
| 3.6.x | cosmos | HTC Vive Cosmos | 2019 |
| 3.7.0 | odyssey | Samsung Odyssey / Odyssey+ | 2018-2019 |
| 3.7.x | explorer | Lenovo Explorer | 2017 |
| 3.8.0 | g2 | HP Reverb G2 | 2020 |
| 3.8.x | crystal | Pimax Crystal | 2023 |
| 3.9.0 | dream | Pimax Dream Air | 2026 |

### Company order (after hardware exhausted)

| Version | Codename | Company |
|---|---|---|
| 4.0.0 | oculus | Oculus (pre-Meta PCVR era) |
| 4.0.x | htc | HTC |
| 4.1.0 | valve | Valve |
| 4.1.x | hp | HP |
| 4.2.0 | samsung | Samsung |
| 4.2.x | lenovo | Lenovo |
| 4.3.0 | pimax | Pimax |

## 3.3.1 — `dk1` — Hotfix (next ~2 weeks)

Focus: fix anything from 3.3.0 smoke testing or early user reports.

- Fix desktop preview window rendering black (known issue documented in 3.3.0 release notes).
- Regression fixes only. No new features.

## 3.3.2 — `dk2` — Second hotfix (only if needed)

Focus: follow-up patches. May be skipped if `dk1` covers everything.

## 3.4.0 — `vive` — Compatibility pass (~1–2 months)

Focus: restore game compatibility against current Meta Horizon runtime.

- **#1589** — Game detection against current Meta Horizon install layout (`Meta Horizon\Software\` instead of `Oculus\Software\`).
- **#1605** — User-configurable library location.
- **#1867** — Lone Echo audio device routing under Quest Link.
- **#1732** — Population 1 duplicate display fix, if reproducible.

## 3.4.x — `cv1` — Input polish (~2–3 months)

Focus: controller binding correctness across the Oculus Touch family.

- Audit all `Input/*.json` binding files for latent gaps like the right Touch face-button fix in 3.3.0.
- **#1872** — Gorilla Tag hand position via Link.
- Document binding conventions in a new `Input/README.md`.
- Not in scope: Index/Knuckles or WMR binding fixes without a trusted tester.

## 3.5.0 — `rift-s` — Quality of life (~3–4 months)

Focus: diagnostics and installer polish.

- Expand injector environment snapshot with GPU driver version, SteamVR version, per-game launch trace.
- Installer: in-place upgrade flow that preserves user hook.cmd registrations across reinstall.

## 3.5.x — `index` — Settings UI (tentative)

Focus: user-facing configuration.

- Overlay UI for runtime preference (OpenVR vs OpenXR) without editing command-line flags.
- Tentative scope — may split or move to backlog depending on complexity.

## 3.6.0 — `reverb` — Community feedback (~6 months)

Focus: whatever's surfaced from real users between releases. Specific scope TBD closer to the date.

## Backlog (no timeline)

- Knuckles / Index binding corrections (needs hardware or trusted tester).
- WMR-specific fixes (platform EOL; low priority).
- Asgard's Wrath 1 stutter investigation (#1354). Large, underspecified, engine-family issue.
- Linux port (#1571). Blocked by Oculus SDK being Windows-only.

## Non-goals

- New game support beyond what LibreVR already enumerates.
- UI redesign of the dashboard overlay.
- Automated CI/release pipeline (manual releases are fine for a hobby fork).
- Forking Oculus SDK itself.
- Telemetry or ads of any kind.

## Principles

- Every release ships only what's tested on CV1 + Quest 3 via Link.
- Inherit upstream fixes if LibreVR ever resumes work.
- Keep LibreVR attribution visible in all user-facing text.

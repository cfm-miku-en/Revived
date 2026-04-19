# Roadmap

Planned work for Revived, organized by theme. Dates are targets. Scope can shrink but not grow within a milestone.

> Note: this roadmap may be completed before the targeted time. Dates are upper bounds. Milestones may merge, split, or reorder as work progresses.

## Codename scheme

Each release gets a codename from PC VR history. Tags take the form `v3.3.1-vive`, `v3.4.0-rift-s`, etc.

**Hardware order:** vive, cv1, rift-s, index, reverb, cosmos, odyssey, explorer, g2, crystal, dream.

**Companies (after hardware exhausted):** oculus, htc, valve, hp, samsung, lenovo, pimax.

**Beta/development tags** use `dk1` and `dk2` as suffixes — the original Oculus development kits. Example: `v3.4.0-dk1-beta1`, `v3.4.0-dk2-beta1`. Final releases drop the `-dk*-beta*` suffix.

**Cancelled or test builds** that never ship publicly get the codename `crystal-cove` — the 2014 Oculus prototype that was demoed but never reached customers. If a `crystal-cove` build is later promoted to a real release, it's renamed to its proper codename at tag time.

When the codename list exhausts, the cycle continues with numeric suffixes: `vive-2`, `cv1-2`, etc.

---

## Milestone 1 — Hotfix (~2 weeks)

Focus: fix anything surfaced from 3.3.0 smoke testing or early user reports.

- Fix desktop preview window rendering black (known issue documented in 3.3.0 release notes).
- Regression fixes only. No new features.

## Milestone 2 — Compatibility pass (~1–2 months)

Focus: restore game compatibility against current Meta Horizon runtime.

- **#1589** — Game detection against current Meta Horizon install layout (`Meta Horizon\Software\` instead of `Oculus\Software\`).
- **#1605** — User-configurable library location.
- **#1867** — Lone Echo audio device routing under Quest Link.
- **#1732** — Population 1 duplicate display fix, if reproducible.

## Milestone 3 — Input polish (~2–3 months)

Focus: controller binding correctness across the Oculus Touch family.

- Audit all `Input/*.json` binding files for latent gaps like the right Touch face-button fix in 3.3.0.
- **#1872** — Gorilla Tag hand position via Link.
- Document binding conventions in a new `Input/README.md`.
- Not in scope: Index/Knuckles or WMR binding fixes without a trusted tester.

## Milestone 4 — Quality of life (~3–4 months)

Focus: diagnostics and installer polish.

- Expand injector environment snapshot with GPU driver version, SteamVR version, per-game launch trace.
- Installer: in-place upgrade flow that preserves user hook.cmd registrations across reinstall.

## Milestone 5 — Settings UI (tentative)

Focus: user-facing configuration.

- Overlay UI for runtime preference (OpenVR vs OpenXR) without editing command-line flags.
- Tentative scope — may split or move to backlog depending on complexity.

## Milestone 6 — Community feedback (~6 months)

Focus: whatever's surfaced from real users between releases. Specific scope TBD closer to the date.

---

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

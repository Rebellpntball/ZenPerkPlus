# ZenPerkPlus

Optional add-on for [ZenSkills](https://github.com/ZenarchistCode/ZenSkills) **main `c1a9255` (2026-06-04)** / 1.29.

Adds three **Zen-style** skill trees. Progression (EXP spend, max perks, reset, refund) stays in ZenSkills.

| Role | Skill key | Focus |
|------|-----------|--------|
| **Gunner** | `firearms` | Run & gun / marksman, jam & wear |
| **Operator** | `combat_ops` | Scout stamina / field medic, radio |
| **Wheelman** | `driver` | Distance EXP, crash control |

## Requirements

- DayZ 1.29+
- **ZenModCore** + **ZenSkills** (requiredAddons)
- Optional: Expansion AI (`EXPANSIONMODAI`)

Load **after** ZenSkills in `-mod=`.

## GUI

- **I** (rebind under ZEN) opens the PerkPlus tree.
- **U** stays vanilla Zen (survival/crafting/hunting/gathering). Add-on keys are yanked during that Init so missing widgets cannot NRE.
- Nodes reuse Zen tree background + hunting/gathering/crafting perk `.paa` until you copy/rename files (see `data/gui/images/README.md` and `UseCustomPerkIcons`).

Unlock/reset use `RPC_ServerReceive_PerkUnlock` / `PerkReset`. The PerkPlus menu extends `ZenSkillsGUIBase` so the plugin can refresh it.

## Config

`profiles/ZenPerkPlus/` (synced). New: `UseCustomPerkIcons` (default false).

Effect magnitudes (jam, stamina, bandage, crash) live here. EXP **cost** lives in ZenSkills JSON.

## Data

Additive DB migration only. Existing Zen progress is kept.

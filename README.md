# ZenPerkPlus

Optional add-on for [ZenSkills](https://github.com/ZenarchistCode/ZenSkills) **main `c1a9255` (2026-06-04)** / 1.29.

Adds three **Zen-style** skill trees. Progression (EXP spend, max perks, reset, refund) stays in ZenSkills.

| Role | Skill key | Focus |
|------|-----------|--------|
| **Gunner** | `firearms` | Less sway/recoil, fewer jams, dry-mag sidearm ping, shallow Deadeye zoom |
| **Operator** | `combat_ops` | Stamina, quieter steps, faster bandage/splint, shock regen, radio static when a player or AI is nearby |
| **Wheelman** | `driver` | Distance EXP, crash control, fuel/battery, engine repair EXP |

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

Effect magnitudes live here. EXP **cost** stays in ZenSkills JSON.

Radio static needs the Radio Discipline perk and a powered radio. It says someone or hostile chatter is close. No direction, no distance, no infected. `CombatRadioDetectPlayers` and `CombatRadioDetectExpansionAI` toggle who counts. Presence range/cooldown are `AIContactPingRangeMeters` (80) and `AIContactPingCooldownSeconds` (40). If an older profile JSON already exists, set those by hand. Vehicle speed boost stays off.

## Data

Additive DB migration only. Existing Zen progress is kept.

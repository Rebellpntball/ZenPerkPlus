# ZenPerkPlus

Optional add-on for [ZenSkills](https://github.com/ZenarchistCode/ZenSkills) that adds three broad skill trees:

| Skill | Focus |
|-------|--------|
| **firearms** | Weapon handling, jam/wear reduction, sidearm awareness, tactical zoom |
| **combat_ops** | Kill EXP (infected / players / Expansion AI), combat radio static, recovery perks |
| **driver** | Distance EXP, crash control, fuel/battery care (hooks partially reserved for future work) |

## Requirements

- DayZ server with **ZenModCore** and **ZenSkills**
- Optional: Expansion AI (`EXPANSIONMODAI`) for AI kill EXP and radio detection

## Installation

1. Copy the `ZenPerkPlus` folder into your server mods directory.
2. Add `@ZenPerkPlus` (or your packed PBO name) to the server `-mod=` list **after** ZenSkills.
3. Restart the server. Config is generated under `profiles/ZenPerkPlus/` and synced to clients.

## Config highlights

`ZenPerkPlus` JSON (server + client sync) exposes toggles and values for:

- Skill enable flags (`EnableFirearmsSkill`, `EnableCombatOpsSkill`, `EnableDriverSkill`)
- Firearm type lists (SMG / rifle / sniper / shotgun / pistol substrings)
- Shot / kill EXP, jam & wear reduction
- Combat radio range, cooldown, powered-radio requirement
- Kill EXP amounts (infected, player, Expansion AI, animal)
- Driver distance EXP and experimental vehicle toggles (many default off)

## Data migration

Player DBs are updated **additively**: missing skill and perk entries for the three new trees are created without wiping existing progress.

## GUI note

ZenPerkPlus does **not** change ZenSkills layouts or images. New skills inject into data/EXP systems; full skill-button UI for add-on trees needs either an upstream ZenSkills UI hook or a separate ZenPerkPlus menu. See `docs/ZenSkillsGUICompatibility.md`.

## License / authorship

Originally structured as a PR against ZenSkills-style packaging. Runtime code depends on ZenSkills / ZenModCore APIs (`ZenConfigBase`, `GetZenSkillsPlugin`, `EEKilledZen`, etc.).

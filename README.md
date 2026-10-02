# ZenPerkPlus

Optional add-on for [ZenSkills](https://github.com/ZenarchistCode/ZenSkills). Adds three **Zen-style** skill trees for a survivalist squad:

| Role | Skill key | Focus |
|------|-----------|--------|
| **Gunner** | `firearms` | Run & gun (left) / marksman (right), jam & wear, sidearm ready |
| **Operator** | `combat_ops` | Scout stamina & presence (left) / field medic speed (right), radio |
| **Wheelman** | `driver` | Distance EXP, crash control, fuel/battery, getaway |

Progression **stays in ZenSkills** (EXP spend, max perks, reset, refund). ZenPerkPlus only injects defs, awards EXP from actions, and applies gameplay effects. See `docs/DESIGN.md`.

## Requirements

- DayZ with **ZenModCore** and **ZenSkills**
- Optional: Expansion AI (`EXPANSIONMODAI`) for AI kill EXP and radio detection

## Installation

1. Copy `ZenPerkPlus` into your server mods folder.
2. Add it to `-mod=` **after** ZenSkills.
3. Restart. Config: `profiles/ZenPerkPlus/` (synced to clients).

## Config

`ZenPerkPlus` JSON controls **add-on** tuning only:

- Enable each role tree
- Firearm category lists, shot/kill EXP, jam/wear toggles
- Combat radio range/cooldown, kill EXP amounts
- Driver distance EXP and vehicle-related toggles

Zen’s own JSON still controls perk cost, refund on reset, and global skill rules.

## Data migration

Additive: missing skill/perk entries for the three trees are created; existing Zen progress is not wiped.

## GUI

Zen’s stock menu does not list add-on skill buttons. Plan is a **PerkPlus tree UI** that reuses Zen tree images and the same slot grid. Until that ships, data and hooks still run; players need the future menu (or debug) to spend perks on these trees. See `docs/ZenSkillsGUICompatibility.md` and `docs/DESIGN.md`.

## License / authorship

Depends on ZenSkills / ZenModCore APIs. Pack and load as a separate mod after ZenSkills.

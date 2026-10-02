# ZenPerkPlus design (v1)

## Goal

Three **Zen-style** skill trees for a survivalist squad. Basic but noticeable. Not a full RPG.

| Role (UI name) | Skill key | Party job |
|----------------|-----------|-----------|
| **Gunner** | `firearms` | Wins fights |
| **Operator** | `combat_ops` | Moves / patches / radio |
| **Wheelman** | `driver` | Travel and extract |

Hunter / tracker fantasy stays on **ZenSkills Hunting**. PerkPlus does not replace it.

## Progression (must stay Zen)

- EXP, perk spend, tier gates, **MaxAllowedPerks**, **reset / refund** → **ZenSkills**
- Injected defs use the same slot IDs as Zen (`1_1` … `4_2`)
- Default `MaxAllowedPerks = 8` on add-on trees so players specialize (left vs right)
- Server can still raise costs via Zen skill JSON after load

## Config split

| Owner | Responsibility |
|-------|----------------|
| **ZenSkills** | Unlock cost, refund, reset, player DB |
| **ZenPerkPlusConfig** | Enable flags, EXP award amounts, effect magnitudes, radio/driver toggles |

## Tree layout (reuse Zen tree art)

Same grid as Zen. Own menu later should clone Zen layout and reuse `skill_background_tree` / node chrome.

### Gunner (`firearms`)

```
4  [ Gunfighter ]     [ Deadeye ]        ← signatures (RG | MM)
3  [ Controlled Burst][ Clean Chamber ][ Field Maintenance ]
2  [ Close Quarters ] [ Sidearm Ready ]
1  [ Weapon Familiar][ Hip Ready ]    [ Steady Grip ]
```

- **Left:** Run & Gun  
- **Right:** Marksman  
- **Shared:** Familiar, Sidearm Ready, Field Maintenance  

### Operator (`combat_ops`)

```
4  [ Ghost Pace ]     [ Combat Medic ]
3  [ Long Push ]      [ Radio Discipline ][ Stay With Me ]
2  [ Light Step ]     [ Field Splint ]
1  [ Ops Training ]   [ Second Wind ]     [ Quick Wrap ]
```

- **Left:** Scout (stamina / quiet — not infinite sprint)  
- **Right:** Field Medic (faster care — not full medical overhaul)  

### Wheelman (`driver`)

Linear: Training → Soft Hands / Road Sense → Iron Chassis / Fuel Saver → Crash Control / Mechanic / Battery → Convoy → **Getaway**.

## UI

- Stock Zen GUI **cannot** show new skill tabs (hardcoded buttons)
- Plan: **ZenPerkPlus menu** that reuses Zen tree images and the same perk grid widgets
- Reset button must call **Zen** reset for that skill key (no second economy)

## Out of scope (v1)

- Double-jump / unlimited sprint  
- Damage multipliers that break vanilla guns  
- Engineer as a fourth tree  
- Full footstep ESP / animals permanently ignore player  

## Implementation status

| Piece | Status |
|-------|--------|
| Config + sync | Done |
| Skill/EXP injection (role names) | Updated this pass |
| DB migration | Done |
| Weapon / kill / radio hooks | Partial (legacy perk IDs still valid via aliases) |
| Own tree UI + reused art | Next |
| Scout stamina / medic action speed / driver crash feel | Next gameplay pass |

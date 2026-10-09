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
- Baseline `1_1` nodes (Weapon Familiar / Ops Training / Driver Training) use Zen's built-in perk EXP boost

## Config split

| Owner | Responsibility |
|-------|----------------|
| **ZenSkills** | Unlock cost, refund, reset, player DB |
| **ZenPerkPlusConfig** | Enable flags, EXP award amounts, effect magnitudes, radio/driver toggles |

## Tree layout

Same grid as Zen. Own menu (`I`) clones the tree and reuses `skill_background_tree` plus hunting / gathering / crafting node art until `UseCustomPerkIcons`.

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

- **Left:** Scout (stamina — not infinite sprint)
- **Right:** Field Medic (faster bandage — not a medical overhaul)

### Wheelman (`driver`)

Linear: Training → Soft Hands / Road Sense → Iron Chassis / Fuel Saver → Crash Control / Mechanic / Battery → Convoy → **Getaway**.

## UI

- Stock Zen GUI **cannot** show new skill tabs (hardcoded buttons)
- **ZenPerkPlus menu** (default **I**) reuses Zen tree art and the same perk grid
- Unlock / reset call Zen `RPC_ServerReceive_PerkUnlock` / `PerkReset`
- Add-on keys are yanked during stock U + highscores Init so missing widgets cannot NRE

## Out of scope (v1)

- Double-jump / unlimited sprint
- Damage multipliers that break vanilla guns
- Engineer as a fourth tree
- Full footstep ESP / animals permanently ignore player
- ADS sway, recoil climb, footstep volume, shock regen
- Fuel saver, battery care, vehicle speed, auto sidearm swap (config flags exist, not hooked)
- Field-mechanic repair EXP (action key exists, no repair hook yet)
- Deadeye does not add a real zoom; sprint-zoom stays off unless rewritten later

## What actually plays in v1

| Feel | Hook |
|------|------|
| Shot / firearm kill EXP | `Weapon_Base.EEFired`, kill handler |
| Fewer jams, slower wear | `GetChanceToJam`, small durability refund |
| Sidearm ping | Only when the perk is owned **and** the gun is dry |
| Kill EXP + radio static | Player / infected / animal / Expansion AI `EEKilledZen`, powered radio |
| Scout stamina | `DepleteStaminaEx` drain cut + short post-kill return |
| Faster bandages | `ActionBandageSelfCB` / `TargetCB` |
| Drive distance EXP | Driver seat distance accumulator |
| Softer crashes | Vehicle HP refund while you drive |

## Implementation status

Aligned to ZenSkills **main `c1a9255` (2026-06-04)**. No newer ZenSkills commit since that.

| Piece | Status |
|-------|--------|
| Config + sync (v4, `UseCustomPerkIcons`) | Done |
| Skill / EXP injection, MaxAllowedPerks 8 | Done |
| Additive DB migration | Done |
| Own tree UI + Zen RPCs + GUI yank | Done |
| Jam, wear, stamina, bandage, crash, radio, distance EXP | Done |
| Animal kill EXP | Done |
| Custom perk art | Optional (`UseCustomPerkIcons`) |
| Sway / noise / fuel / battery / repair EXP / real zoom | Not in v1 |

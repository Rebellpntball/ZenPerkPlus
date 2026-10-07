# ZenSkills GUI compatibility (aligned to ZenSkills `c1a9255`, 2026-06-04)

Target: **ZenSkills main @ c1a9255** (“Fixed ZenModCore compatibility”) + 1.29 (`5d30f5d`).

## What latest Zen changed (PerkPlus must match)

| Change | PerkPlus handling |
|--------|-------------------|
| `ZenSkillsGUI extends ZenSkillsGUIBase` | Our menu **extends ZenSkillsGUIBase** and implements `ForceUpdateFromServer(int)` so unlock RPCs refresh the open PerkPlus page |
| `ZenMissionFunctions.Freeze/UnfreezePlayerControls()` | Used in PerkPlus `OnShow` / `OnHide` |
| `GetZenModCoreConfig` → `GetZenCoreConfig` | Not used by PerkPlus |
| GUI still loops **all** `db.Skills` and `LoadImageFile` with **no null check** | **Yank** `firearms` / `combat_ops` / `driver` during stock U + highscores `Init`, then restore. `UpdateSkillPerkLabels` skips add-on keys |
| Perk icons `ZenSkills/data/gui/images/<skillKey>/<slot>_<level>.paa` | PerkPlus `LoadImageFile`s the same pattern. Default **reuses** Zen `hunting` / `gathering` / `crafting` folders |
| Tab `skill_<FirstLetterUppercase>.edds` | Mapped to `skill_Hunting` / `Gathering` / `Crafting` until custom icons exist |
| HUD `skill_<skillKey>.edds` (lowercase) | HUD `ZenShowPerk` remaps add-on keys |

## Image rename (when you ship custom art)

Copy Zen’s slot files, **keep filenames**, change folder names:

```
ZenSkills/data/gui/images/hunting/1_1_0.paa … 4_2_3.paa
  -> ZenPerkPlus/data/gui/images/firearms/

ZenSkills/data/gui/images/gathering/…
  -> ZenPerkPlus/data/gui/images/combat_ops/

ZenSkills/data/gui/images/crafting/…
  -> ZenPerkPlus/data/gui/images/driver/
```

Tab / HUD (both casings — Zen is inconsistent):

```
skill_Firearms.edds  (U-style FirstLetterUppercase)
skill_firearms.edds  (HUD lowercase)
skill_Combat_ops.edds / skill_combat_ops.edds
skill_Driver.edds / skill_driver.edds
```

Then set `UseCustomPerkIcons = true` in the synced ZenPerkPlus JSON.

Until that flag is on, nodes load Zen’s hunting/gathering/crafting `.paa` so the tree is not blank.

## Stock U menu

PerkPlus skills stay **off** the U page on purpose (no `FirearmsButtonImage` widgets). Spend them on the PerkPlus tree (default **I**). Reset/unlock still go through Zen RPCs.

## 1.29 gameplay hooks

- Stamina: `StaminaHandler.DepleteStaminaEx(EStaminaModifiers, dT, coeff)`
- Bandage: `ActionBandageSelfCB` / `ActionBandageTargetCB` `CAContinuousTime`
- Vehicle driver: `CarScript.CrewMember(0)`

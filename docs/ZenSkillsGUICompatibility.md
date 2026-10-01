# ZenSkills GUI compatibility notes

ZenPerkPlus does not modify ZenSkills layout or image files.

Inspection findings:

- Skill button widgets are fixed in `ZenSkills/data/gui/layouts/zen_skills_menu.layout` and currently exist for the original ZenSkills skills only: `Survival`, `Crafting`, `Hunting`, and `Gathering`.
- `ZenSkillsGUI.OnInit()` loops over every entry in the player DB and derives widget names from the skill key with `ZenSkillFunctions.FirstLetterUppercase(skillKey)`, then looks up `<SkillName>ButtonImage`, `<SkillName>Label`, `<SkillName>ButtonPerks`, and `<SkillName>Progress`.
- Skill icon paths are derived from that same generated label as `ZenSkills/data/gui/images/skill_<SkillName>.edds`.
- Perk icon paths are derived from the selected skill key and slot/level as `ZenSkills/data/gui/images/<skillKey>/<slot>_<level>.paa`.
- Perk node widgets themselves are generic fixed widgets (`PerkIcon1_1` through `PerkIcon4_2`), but the image files loaded into them are skill-key-specific.

Impact for injected skills:

- A new skill key such as `radio_operator` would look for widgets like `Radio_operatorButtonImage`, which do not exist in the stock ZenSkills layout.
- Adding a ZenPerkPlus imageset alone cannot make those missing widgets exist or change ZenSkills' hardcoded `ZenSkills/data/gui/images/...` load paths.
- Until ZenSkills exposes a UI extension/fallback hook, ZenPerkPlus should leave existing ZenSkills image/layout files intact. The add-on can safely inject data definitions and migrate DBs, but a full visible button grid for the added skills needs either an upstream ZenSkills UI extension point or a separate ZenPerkPlus-owned UI.

Future compatible options:

1. Add an upstream ZenSkills GUI fallback hook that lets add-ons provide skill buttons and icon paths.
2. Add a separate ZenPerkPlus UI page/menu for add-on skills.
3. Add ZenPerkPlus-owned layout and imageset assets only if ZenSkills can load them without editing ZenSkills source files. For v1, do not register a ZenPerkPlus imageset because the current ZenSkills GUI does not consume add-on image sets for skill buttons or perk icon path resolution.

## Deprecated test skill keys

Earlier ZenPerkPlus test builds injected eight narrow skill keys (`radio_operator`, `field_medic`, `engineer`, `tactics`, `smg`, `rifle`, `sniper`, and `shotgun`). Current v1 builds only create the broad Zen-style trees `firearms`, `combat_ops`, and `driver`.

ZenPerkPlus intentionally does not wipe generated ZenSkills configs or player DBs. If a test server already generated a ZenSkills config containing the deprecated narrow test skills, remove those deprecated skill definitions manually or regenerate the test ZenSkills config before release. Existing player DB migration remains additive only and does not erase progress.

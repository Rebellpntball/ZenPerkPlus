# Perk / tab images

ZenSkills loads:

```
<data/gui/images>/<skillKey>/<slot>_<level>.paa
skill_<Name>.edds
```

PerkPlus default (`UseCustomPerkIcons = false`) **reuses packed Zen art**:

| Skill key | Zen folder reused |
|-----------|-------------------|
| `firearms` | `hunting` |
| `combat_ops` | `gathering` |
| `driver` | `crafting` |

To use renamed copies instead:

1. Copy those `.paa` sets into this directory as `firearms/`, `combat_ops/`, `driver/` (same `1_1_0.paa` … `4_2_3.paa` names).
2. Add `skill_Firearms.edds` and `skill_firearms.edds` (and the combat_ops / driver pair).
3. Set `UseCustomPerkIcons` true in `profiles/ZenPerkPlus`.

Do not rename `1_1_0` to perk titles — Zen’s GUI language is **slot_level**.

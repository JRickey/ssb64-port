# Mushroom Kingdom (Inishie) crash — `dEFGroundDatas[]` short by one entry — 2026-05-02

**Status:** RESOLVED.

## Symptom

Selecting **Mushroom Kingdom** in VS Mode immediately crashed at stage-load
during `scVSBattleStartBattle` → `grCommonSetupInitAll` → `efGroundMakeAppearActor`
→ `efGroundSetupRandomWeights`. SIGSEGV with the watchdog dump:

```
fault_addr=0x10000000a
pc=…  efGroundSetupRandomWeights + 176
1   efGroundMakeAppearActor + 336
2   grCommonSetupInitAll + 184
3   scVSBattleStartBattle + 312
```

The fault address `0x10000000a` has the “small-region” shape — high bits
nearly zero, low byte `0xa` — typical of a pointer that was never a real
heap address in the first place.

## Root cause

`src/ef/efground.c:991` declares the per-stage ambient-effect table:

```c
EFGroundData dEFGroundDatas[/* */] =
{
    /* Castle, Sector, Jungle, Zebes, Hyrule (NULL stub),
       Yoster, Pupupu, Yamabuki */
};
```

Eight entries — one per `nGRKindCastle`…`nGRKindYamabuki` (indices 0–7).

`grdef.h` defines `nGRKindInishie = 8` and includes Inishie in the VS-stage
range (`nGRKindBattleEnd = nGRKindInishie`), so the existing guard in
`efGroundMakeAppearActor` lets gkind 8 through:

```c
if ((gSCManagerBattleState->gkind <= nGRKindBattleEnd) && …
    (dEFGroundDatas[gSCManagerBattleState->gkind].effect_params != NULL))
```

For Inishie this reads `dEFGroundDatas[8]` — past the end of an 8-element
array. On the LP64 build the trailing bytes happen to look like a valid
non-NULL `effect_params` pointer, the `!= NULL` check passes, and
`efGroundSetupRandomWeights` then dereferences the bogus pointer:

```c
param = sEFGroundActor.effect_data->effect_params;   // bogus
for (i = 0; i < sEFGroundActor.effect_data->params_num; i++)
    j += (param + i)->effect_weight;                  // SIGSEGV
```

Mushroom Kingdom has no rain/snow/falling-leaf style ambient ground
effects on the original cart — it should have used the same NULL stub
entry that Hyrule has at index 4. The decomp shipped with the array
truncated; on N64 the trailing zero-bss could have masked it (a NULL
`effect_params` would have failed the guard cleanly), but in our LP64
binary the layout differs and the next bytes are non-NULL.

## Fix

Append a NULL stub entry for Inishie, mirroring the Hyrule shape:

```c
// Mushroom Kingdom — no ambient ground effects (same pattern as Hyrule).
{ 0, NULL, 0x0, NULL }
```

`src/ef/efground.c:1056`. With the entry present, the guard at
`efground.c:1580` now sees `effect_params == NULL` for gkind 8 and skips
the actor-creation path entirely.

## Class / audit hook

“Per-gkind table missing the unlockable stage’s slot.” Inishie was added
to `GRKind` after the original ambient-effects pass and several decomp
tables were never extended. When a new bug presents as
*“selecting Mushroom Kingdom crashes inside a `gr*` / `ef*` setup function
with a small/garbage fault_addr”*, grep for `[nGRKindBattleEnd]` /
`[nGRKindCastle..nGRKindYamabuki]`-shaped tables in `src/gr/` and
`src/ef/` and verify each has 9 entries (Castle through Inishie) instead
of 8.

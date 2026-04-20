# Item `map_coll.bottom` sign inversion — Resolved (2026-04-20)

## Symptom

In VS battles, dropped items (capsules, eggs, pokéballs, crates, etc.) visibly embedded into stage platforms instead of resting on top. Fighter collision on the same platforms was correct. First observable after the battle-start hang fix (`c2a536c`) made matches run long enough to see item physics.

Representative trace from a diagnostic `port_log` inside `mpProcessSetLandingFloor`:

```
LANDFLOOR map_coll.bot=280.0 pre_y=655.801 probe_y=935.801 floor_dist=86.199 post_y=742.000 line=4
```

Floor line at Y=1022, item translate.y settled at Y=742 — origin `map_coll.bottom` (280) units *below* the floor, which is what the shared landing math (`translate.y += floor_y − (translate.y + map_coll.bottom)`) produces.

## Root cause

The ROM's `ITAttributes.map_coll_bottom` is a **positive magnitude** for attribute-driven items (capsule=+60, pokéball=+113, tomato=+180, egg=+280, …). `itManagerMakeItem` copies it straight to the runtime `MPObjectColl`:

```c
ip->coll_data.map_coll.bottom = attr->map_coll_bottom;
```

The shared collision code (`mpProcessSetLandingFloor`, `itMapCheckCollideAllRebound`, …) treats `map_coll.bottom` as a *signed* Y offset applied additively: `object_pos.y = translate.y + map_coll.bottom`. For a floor probe below the origin that offset must be **negative**. Every item that hand-sets its own diamond — `itmsbomb`, `ittaru`, `ittarubomb`, `itnbumper`, `itkamex`, `wpsamuschargeshot` — explicitly writes `-COLL_SIZE`:

```c
ip->coll_data.map_coll.bottom = -ITMSBOMB_COLL_SIZE;  // itmsbomb.c:254
ip->coll_data.map_coll.bottom = -ip->coll_data.map_coll.width;  // ittaru.c:294
```

So the runtime convention unambiguously wants a negative value. The attribute-driven path (and the upstream decomp at `VetriTheRetri/ssb-decomp-re`) never negates. Verified the byte-swap + halfswap fixup is correct end-to-end by dumping raw u32s pre- and post-`portFixupStructU16`: the values read are the ROM's actual BE-intended s16s, not a byte-order artefact. The ROM genuinely stores them as positive magnitudes, and the decomp misses the sign flip.

## Fix

Negate on load in `itManagerMakeItem` (`src/it/itmanager.c`), guarded by `#ifdef PORT` and annotated so the non-PORT branch stays byte-identical to the decomp:

```c
#ifdef PORT
    ip->coll_data.map_coll.top      = attr->map_coll_top;
    ip->coll_data.map_coll.center   = attr->map_coll_center;
    ip->coll_data.map_coll.bottom   = -attr->map_coll_bottom;
    ip->coll_data.map_coll.width    = attr->map_coll_width;
#else
    ip->coll_data.map_coll.top      = attr->map_coll_top;
    ip->coll_data.map_coll.center   = attr->map_coll_center;
    ip->coll_data.map_coll.bottom   = attr->map_coll_bottom;
    ip->coll_data.map_coll.width    = attr->map_coll_width;
#endif
```

`top`, `center`, `width` do not need flipping — `top` is 0 for every observed item, `center` is already negative in ROM (consistent with "Y-offset from origin" convention), and `width` is a non-directional magnitude.

After the fix, a capsule/egg/pokéball dropped from the air rests flat on the platform surface, and fighter-pickup works normally.

## Files

- `src/it/itmanager.c` — `itManagerMakeItem` map_coll copy.

## Class-of-bug note

This is a **deliberate PORT deviation from the decomp** (per CLAUDE.md rule "preserve behavior, not byte-matching"). The upstream decomp's direct copy only works in-situ on the N64 ROM for reasons we could not identify from static analysis — possibly the N64 build ingested a different revision of `gITManagerCommonData` that stored the field as negative, or there's an intermediate sign flip in a code path we haven't traced. Either way, the runtime convention is well-established by the hand-set overrides listed above, and negating on load gives the correct visible behavior. If future work identifies the true upstream-compatible mechanism, the `#ifdef PORT` branch can be retired.

**Weapons (`wpmanager.c`) were checked and do not need the same fix**: the `samus_charge_shot_hit_detection_2026-04-13` dumps show `WPAttributes.map_coll_bottom = -10` (already negative) for Fox Blaster; weapon-path attributes appear to be stored with the correct sign.

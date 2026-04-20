# 1P character-select re-entry segfault: stale `sMNPlayers1PGameTimeGObj` — Resolved (2026-04-20)

## Symptom

Selecting "1P" from the mode menu after having visited any other game mode (e.g. VS) crashed the game immediately on entering 1P character select (`nSCKind1PGamePlayers`, scene 17). First entry to scene 17 in a session worked fine; the crash only manifested on re-entry.

`ssb64.log` captured it as a SIGSEGV inside `gcEndProcessAll` with a sub-4GiB fault address:

```
SSB64: ftManagerMakeFighter - return fkind=0
SSB64: gcEjectGObj ENTER gobj=0x104dbaff8 id=4294967295 kind=255 link_id=36 dl_link_id=0 gpr_head=0xffe40024 obj=0x300000000 link_next=0x10fff50024 link_prev=0xff82788c00960069
SSB64: gcEndProcessAll SUSPECT proc=0xffe40024 gobj=0x104dbaff8 (low addr or unaligned)
SSB64: !!!! CRASH SIGSEGV fault_addr=0xffe40024
3   ssb64                               0x00000001046fbec8 gcEndProcessAll + 152
```

Note the fields of the "gobj" being ejected:
- `id=0xFFFFFFFF`, `kind=0xFF` — sentinel-looking
- `link_id=36` — out of range (max is `GC_COMMON_MAX_LINKS = 33`)
- `link_prev=0xff82788c00960069` — wild
- `gpr_head=0xffe40024` — sub-4GiB, what gets dereferenced and faults

Also note the address `0x104dbaff8` happens to land at offset `0x60` inside the freshly-created `fighter_gobj` at `0x104dbaf98`. On LP64 `sizeof(GObj) == 232` (`0xE8`), so `0x104dbaff8` is **inside** that fighter — its first 4 bytes are the fighter's `camera_tag` field (set to `~0` by `gcAddGObjDisplay`), which is exactly the bogus `id=0xFFFFFFFF` printed above. That explained why the bytes "looked structured" rather than random — they were valid fields of the wrong object.

## Root cause

`src/mn/mnplayers/mnplayers1pgame.c` keeps five static `GObj*` slots populated lazily:

```c
GObj *sMNPlayers1PGameTimeGObj;       // mnplayers1pgame.c:68
GObj *sMNPlayers1PGameHiScoreGObj;    // mnplayers1pgame.c:101
GObj *sMNPlayers1PGameBonusesGObj;    // mnplayers1pgame.c:104
GObj *sMNPlayers1PGameLevelGObj;      // mnplayers1pgame.c:113
GObj *sMNPlayers1PGameStockGObj;      // mnplayers1pgame.c:116
```

Each follows the same lifecycle pattern — "if a previous instance exists, eject it; allocate a new one":

```c
if (sMNPlayers1PGameTimeGObj != NULL)
{
    gcEjectGObj(sMNPlayers1PGameTimeGObj);
}
gobj = lbCommonMakeSpriteGObj(...);
sMNPlayers1PGameTimeGObj = gobj;
```

That pattern is correct *within* a single scene lifetime. But the per-scene heap (`syTaskmanInitGeneralHeap`) is reset wholesale on every scene transition — so any pointer surviving that reset is dangling, and the `!= NULL` check happily lets it through.

`mnPlayers1PGameInitVars` (called from `mnPlayers1PGameFuncStart` on every scene-17 entry) explicitly nulls four of the five slots — but missed `sMNPlayers1PGameTimeGObj`:

```c
sMNPlayers1PGameHiScoreGObj = NULL;
sMNPlayers1PGameBonusesGObj = NULL;
sMNPlayers1PGameLevelGObj = NULL;
sMNPlayers1PGameStockGObj = NULL;
// sMNPlayers1PGameTimeGObj — MISSING
```

Re-entering scene 17 thus reaches `mnPlayers1PGameMakeLabels` → `mnPlayers1PGameMakeTimeSelect` with a stale pointer, the `!= NULL` guard passes, and `gcEjectGObj` reads garbage out of the new heap layout.

## Fix

Add `sMNPlayers1PGameTimeGObj = NULL;` to `mnPlayers1PGameInitVars` alongside the other four resets.

```c
sMNPlayers1PGameHiScoreGObj = NULL;
sMNPlayers1PGameBonusesGObj = NULL;
sMNPlayers1PGameLevelGObj = NULL;
sMNPlayers1PGameStockGObj = NULL;
sMNPlayers1PGameTimeGObj = NULL;   // ← added
```

This is a decomp-source change, not a `#ifdef PORT` workaround: the original N64 code presumably worked because IDO's BSS-init or the in-game scene-transition cleanup cleared the slot for them, but on the port the static keeps its value across heap resets and the missed reset becomes load-bearing.

## Class of bug / where to look next

"Static `GObj*` slot not nulled at scene start" is the class. The pattern to grep for is:

```
if (sFOO != NULL) { gcEjectGObj(sFOO); }
... sFOO = gobj;
```

Anywhere this exists, confirm the static is also explicitly nulled in the scene's init function. Two scenes are particularly suspicious because they hold many such slots between menu re-entries: 1P character select (this file) and VS character select (`mnplayersvs.c`).

This is *not* the same root cause as `item_arrow_gobj_implicit_int_2026-04-20.md` (LP64 truncation from missing prototype) — both produce sub-4GiB fault addresses that look superficially similar in the SIGSEGV log, but here the pointer was a real heap address that got *invalidated* by a heap reset, not a 64→32-bit truncation. Tell them apart by checking whether the upper 32 bits of the bogus pointer are clean zeros (truncation) or arbitrary garbage (stale).

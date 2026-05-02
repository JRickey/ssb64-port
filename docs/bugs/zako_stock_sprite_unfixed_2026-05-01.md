# Polygon-team stock sprite missing portFixupSprite — 2026-05-01

## Symptom

Issue #53. After the polygon-Pikachu LP64 fix landed and stage 12 (Fighting
Polygon Team) loaded, the renderer flooded ~280k
`RelocPointerTable: invalid/stale token` errors per run from
`lbcommon.c:2427` and the polygon stock icons rendered at the wrong on-screen
position. Each error is a synchronous PORT_RESOLVE call from the render path,
which is what produces the visible lag.

## Diagnostic

Per-frame log of `lbCommonPrepSObjDraw`:

```
PrepDraw sobj=0x_a2b08 nbm=36 bmtok=0x8300166b bm_resolved=0x_a29c0 attr=0x0201 w=8 h=8
```

The same offending sobj at allocation time:

```
gcAddSObj sobj=0x_a2b08 ... src_nbm=1 src_bmtok=0x830000db sobj_nbm=1 attr=0x0201
```

So the embedded `sobj->sprite` was correct at allocation but reported
`nbitmaps=36` at draw time. The renderer then walks 36 Bitmap structs of
junk (PORT_RESOLVE'd to memory adjacent to the SObj), each failing the
PORT_RESOLVE generation check.

Cluster identified: `gobj=0x_a2a20` had **30 SObjs** linked off it — exactly
`SC1PGAME_STAGE_MAX_TEAM_COUNT`. Search showed `sc1PGameInitTeamStockDisplay`
allocates 30 placeholder sobjs for the polygon kill counter, and
`sc1PGameTeamStockDisplayProcDisplay` reassigns each one's sprite every
frame from `sSC1PGameZakoStockFile`'s `llFTStocksZakoSprite`.

## Root cause

`sc1pgame.c:1727` reads the polygon stock sprite directly out of the file:

```c
case nSC1PGameStageZako:
    sobj->sprite = *lbRelocGetFileData(Sprite*, sSC1PGameZakoStockFile,
                                       llFTStocksZakoSprite);
    break;
```

Other sprite paths (`lbCommonMakeSObjForGObj`, `FTSprites` loaders, the
training-mode resolver) all run `portFixupSprite` on the source first.
This per-frame copy did not — so every `{s16,s16}` field pair in the
Sprite struct was still byte-pair-swapped from pass1 BSWAP32. Reading
`sprite->nbitmaps` at offset `0x28` actually returned the original
`ndisplist` value (`36`), and the renderer then walked 36 Bitmap entries
of garbage in memory adjacent to the SObj allocation block.

`portFixupSprite` is idempotent on pointer key (`sStructU16Fixups`), so
fixing the source sprite once at file-load time also fixes every
subsequent per-frame copy.

## Fix

`sc1PGameInitTeamStockDisplay` calls `portFixupSprite` plus
`portFixupBitmapArray` + `portFixupSpriteBitmapData` on the Zako stock
sprite right after `lbRelocGetFileData`, mirroring what
`lbCommonMakeSObjForGObj` does for normal sprite-creation paths.

Files:

- `src/sc/sc1pmode/sc1pgame.c` — port-side fixup at the Zako case in
  `sc1PGameInitTeamStockDisplay`; also added the three `portFixup*`
  externs at the top of the TU.

Also drive-by: `gcAddSObjForGObj` now `bzero`'s the embedded Sprite when
the caller passes `NULL` for `sprite`. Previously the freelist slot's
prior occupant's `nbitmaps`/`bitmap` token leaked through any path that
relies on `attr = SP_HIDDEN` to silence the SObj (e.g.
`ifcommon.c:1003`'s damage-emblem placeholder); this wasn't the
root-cause of #53's lag but is the same shape of bug and showed up in
the diagnostic walk.

## Repro

Play 1P Game through to the Fighting Polygon Team stage.

Before fix: ~280k `RelocPointerTable: invalid/stale token` errors per
session at `lbcommon.c:2427`, polygon stock icons render at the wrong
on-screen position, frame rate drops noticeably for the stage.
After fix: 0 token errors, 0 NULL-bitmap logs, frames advance cleanly.

Stage 1 (VS Link) regression-checked clean — same code path is reached
on every stage; only the Zako case ever loaded a sprite without going
through the existing fixup chain.

## Class

Same family as the documented "trace consumers, not decomp comments"
feedback: any place that passes a `Sprite*` to the renderer needs to be
the result of a `portFixupSprite` chain (either through
`lbCommonMakeSObjForGObj` or an explicit one-off call). The Yoshi/Kirby
team stocks at the same call site (lines 1708/1718) work because
`sSC1PGameEnemyTeamSprites->stock_sprite` was already fixed up via the
fighter-attribute load path before this proc runs; only the Zako-team
path loaded its sprite from a separate one-off file with no fixup hook.

Audit hook for future investigations: search for `lbRelocGetFileData(Sprite*`
call sites and confirm each goes through a fixup before being used.

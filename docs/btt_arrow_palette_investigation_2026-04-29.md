# BTT/BTP Arrow Pink Outline — Investigation Handoff (2026-04-29)

**Issue:** [#4 Arrow texture in Break The Targets/Board The Platforms
renders weird depending on character](https://github.com/JRickey/BattleShip/issues/4) —
arrow outline is pink/magenta where it should be black, and the
inner-fill color shifts per character (Fox brownish, Yoshi green,
Pikachu yellow, Samus orange). Link's stage renders the arrow with a
proper black outline.

**Status:** Not fixed. Strong fingerprint of a CI-texture palette
sharing/staleness problem, but the per-character variance is itself a
clue I need a running build to track. Documenting the leading theory
and the trace data the reporter provided.

## What the fingerprint tells us

A CI4/CI8 texture rendered with the *wrong* TLUT will have:
- Stable shape (palette indices unchanged → silhouette correct)
- Color that shifts to whatever the currently-staged palette holds at
  those indices

That matches the report exactly: arrow shape is the same per character,
inner-fill color tracks the active character palette, outline (which
should index palette-index "black") is rendering as the
character-palette's pink/magenta entry.

The fact that Link is the *only* character whose BTT arrow renders
correctly is the strongest hint: Link's character palette likely has a
black entry at the same palette index the arrow's outline polys
reference, so the wrong-LUT path happens to produce visually-correct
output. Every other character's palette has a non-black color at that
index.

So the bug is: **the BTT/BTP arrow's own palette is never loaded into
TMEM before the arrow draw**, and the arrow inherits whatever LUT was
last staged — which on stage entry is the player's fighter palette.

## Where to look next

1. Find the arrow's draw site. The arrows are stage decorations in
   the BTT/BTP layouts. They aren't in `src/gr/grbonus/grbonus3.c`
   (which I checked — no sprite/arrow refs), so they're likely drawn
   from the per-character bonus stage display routines or as part of
   a packed stage geometry display list.
2. Capture the trace's CI texture activity in the frame the arrows
   appear. The reporter's trace
   (`https://files.catbox.moe/xse2xg.gbi`, ~20 MB) covers Fox / Yoshi /
   Link BTT entries. Filter for `G_LOADTLUT` events that *don't*
   precede the arrow's `G_VTX` / triangle commands — the gap is the
   bug.
3. Cross-check whether the arrow texture's reloc file is loaded by
   `lbRelocLoadFilesListed(dSC1PBonusStageFileIDs, ...)`. If not, the
   arrow file's pass2 fixup never runs, and the arrow's bitmap is
   pulled out of an unrelated heap region whose pass1 BSWAP32 state
   matches whichever fighter happened to load there.

## Why this isn't being fixed in this pass

I can't repro the bug without running the game. Patching blind risks
false fixes that mask the real issue (e.g., forcing a black-LUT
preload would cover the symptom for every character but would also
hide whatever resource-loading order bug is actually producing it).
The reporter included `ssb64.log` and a GBI trace with three character
entries — the next session can lean on those rather than go in cold.

## Caveat

The "Link works" data point is consistent with the wrong-LUT theory
but not exclusive to it. An alternate theory — that the arrow uses an
RGBA texture which is being read out of the wrong heap address — would
also produce per-character variance, but would NOT preserve the
arrow's silhouette as cleanly across characters. The reporter's
screenshots show identical shapes across Fox / Yoshi / Fox-BTP, so the
silhouette stability is real and CI-with-wrong-LUT is the most
parsimonious explanation.

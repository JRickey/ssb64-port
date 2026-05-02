# ADPCM codebook stale slots cause Sector Z strong-hit SFX corruption

**Date:** 2026-05-02
**Status:** Fix shipped. Verified: user could no longer reproduce after fix; 120 s instrumented WAV dump shows zero windows with the corruption signature, vs. 1 window with 43/48 jumps in the buggy WAV.
**Class:** N64 RSP microcode op reimplemented as CPU memcpy; the spec-faithful "copy `count` bytes" lost an incidental property of the original microcode (the unused tail of a shared global buffer was always overwritten by the wave's own surrounding metadata, so re-reads of stale slots were deterministic-per-wave). On the port, the unused tail keeps the **previous voice's** book bytes — which differs across voice scheduling.

## Symptom

User on Sector Z reported a specific reproducible audio glitch: when an Arwing flies through, simultaneous "strongest" hit SFX (smashes / KO hits — PunchL, KickL, BurnL, ShockL, SlashL, BatHit) sound corrupted. Weak/medium variants of the same FGMs are clean. Other stages are unaffected because they don't have a long-running loud ambient voice constantly cycling pvoice slots.

WAV-dump signature (instrumented build, `SSB64_AUDIO_DUMP=1`):

| Window  | Total jumps (\|Δsample\| > 8000 in 100 ms) | Jumps at `idx % 16 == 15` |
|---------|------------------------------------------|----------------------------|
| t=40.70s (corrupted strong hit) | 48 | **43 (90 %)** |
| t=7.0 / 22.0 / 25.9 / 33.9 (clean peaks) | 0–7 | 0 |

`idx % 16 == 15` is the last sample of an ADPCM frame — i.e. every discontinuity sits at the boundary between two consecutive 16-sample ADPCM frames. Mean abs sample-to-sample diff at position 15 within the corrupted window was 2× the diff at all other positions (3555 vs ~1400), with max diff 18982 vs ~7000.

## Root cause

The audio mixer (`port/audio/mixer.c`) keeps a single global 256-byte ADPCM codebook buffer:

```c
static struct {
    …
    int16_t  adpcm_table[8][2][8];   /* 8 predictors × 2 rows × 8 coeffs = 256 B */
    …
} rspa;
```

Each voice issues `aLoadADPCM(count, book_addr)` before its `aADPCMdec` chain. The implementation was a literal "copy `count` bytes":

```c
void aLoadADPCMImpl(int count, uintptr_t book_addr) {
    if (book_addr == 0) return;
    memcpy(rspa.adpcm_table, (const void*)book_addr, count);
}
```

SSB64's books are uniformly `order=2`, `npredictors=4` → `count = 2*order*npredictors*ADPCMVSIZE = 2*2*4*8 = 128` bytes (verified by `SSB64_AUDIO_BOOK_LOG=1` showing every load at `count=128`). That fills `rspa.adpcm_table[0..3]`; slots `[4..7]` retain whatever the **previous** voice's book left in those bytes.

ADPCM frame headers encode `table_index = byte & 0xf`. With a 4-predictor book, valid indices are 0..3. If a wave's data ever encodes a frame whose `table_index ∈ {4,5,6,7}` (likely a tooling/encoder quirk that the original microcode tolerated incidentally — see "Why this is port-specific" below), the decoder reads predictor coefficients that belong to **another voice's wave**. The math becomes:

```
acc = wrong_tbl[0][j] * prev2 + wrong_tbl[1][j] * prev1 + (ins[j] << 11) + Σ wrong_tbl[1][...] * ins[k]
```

`prev1`/`prev2` are correct (the last 2 samples of this voice's previous frame), but the predictor coefficients are from a completely unrelated sample. The first sample of the affected frame jumps by ~10 000–15 000, then samples within the frame chain forward from the bad first sample, then the next frame either uses a valid index (clean continuation) or repeats the pattern.

Result: a sustained 100 ms burst of 16-sample-period discontinuities clustering at frame boundaries — exactly what we see in the WAV.

Why **strong** hits specifically: they're loud, they're allocated reactively when the action lands, and they tend to land on a pvoice slot that was just servicing the long-running Arwing whoosh (which keeps its own 128-byte book in `rspa.adpcm_table[0..3]`, leaving its leftovers in `[4..7]`). Weak/medium hits are short enough that voice scheduling rarely places them on a pvoice with the right kind of stale bytes — and they may not happen to encode any out-of-range `table_index`.

Why **Sector Z** specifically: the Arwing whoosh (`nSYAudioFGMSectorAmbient1`, fired in `grsector.c:409` on patrol entry) is one of the few stage ambients that holds a voice for ~2 s of continuous decoding while character-action SFX cycle pvoice slots in and out.

## Fix

`port/audio/mixer.c::aLoadADPCMImpl`:

```c
void aLoadADPCMImpl(int count, uintptr_t book_addr) {
    if (book_addr == 0) return;

    /* PORT: zero the table before copying so unused predictor slots
     * default to zero coefficients instead of retaining a previous
     * voice's book.  Also clamp count to the table's size so a
     * malformed wave (npredictors > 8) cannot overrun rspa into
     * adjacent fields (polef_coefs, dry/wet accumulators). */
    if (count < 0) count = 0;
    if ((size_t)count > sizeof(rspa.adpcm_table)) {
        count = (int)sizeof(rspa.adpcm_table);
    }
    memset(rspa.adpcm_table, 0, sizeof(rspa.adpcm_table));
    memcpy(rspa.adpcm_table, (const void*)book_addr, count);
}
```

If decoder data ever references `table_index >= npredictors` after the fix, it gets an all-zero predictor — `acc = 0 * prev2 + 0 * prev1 + (ins[j] << 11)` — i.e. just the residual. That's a quiet, benign output instead of a wild jump.

Cost: one 256-byte `memset` per voice per audio frame — ≤ tens of nanoseconds, negligible against the surrounding mixer work.

## Why this is port-specific

On N64, `aLoadADPCM` is not a C function — it's an RSP microcode handler that runs on the audio coprocessor. The reference Nintendo n_audio microcode loads the codebook into a fixed RSP DMEM region, and (per inspection of similar microcodes) the load was effectively reading whole DMA chunks of book material from RDRAM. Because the encoder lays out each wave's bookblock **contiguously with adjacent metadata in ROM**, the bytes past the book's nominal `count` were the same per wave, every time the wave loaded — they were that wave's own following data. The mixer's unused table slots therefore behaved as though they held wave-specific (deterministic) garbage that the wave's data never addressed.

Reimplementing the op as a CPU `memcpy(rspa.adpcm_table, addr, count)` faithfully copies "exactly `count` bytes" — but the unused tail of `rspa.adpcm_table` is now populated by the **previous voice's** book, not by this wave's neighbouring metadata. That makes the unused slots non-deterministic across voice scheduling. Any data path that incidentally relied on "the same garbage every time, never read anyway" now reads someone else's coefficients.

This is the same bug-class as `audio_alparam_variant_overrun_2026-04-24.md` (struct-overlay where layout assumptions silently held on N64 but break on LP64) and `audio_mixer_macro_clobber_2026-04-24.md` (header include order silently held on N64 but breaks on PORT-side macro replacement). The pattern: any port reimplementation of an N64 microcode/macro that "copies what it asks for" should also defensively normalize the rest of any shared global buffer the original microcode incidentally populated.

## Audit hook

Any PORT-side reimplementation of an N64 RSP microcode op (`aSetBuffer`, `aLoadADPCM`, `aLoadBuffer`, `aSetLoop`, `aResample`, `aADPCMdec`, `aMix`, `aEnvMixer`, `aClearBuffer`) that writes into a **shared global buffer** (`rspa.*`) needs to consider: does the op's spec say "read N bytes" but the real RSP routinely overwrites the entire buffer? If so, a faithful CPU `memcpy(buf, src, N)` is **not** equivalent — it leaves the tail in a state that depends on prior voice scheduling, where the original ucode left it in a state that depended only on ROM layout.

When in doubt, zero the buffer first and copy what you have. The cost is negligible; the protection is broad.

## Diagnostic that nailed it

Built with env-gated voice/book logging, repro'd, then analyzed the WAV with NumPy:

```python
diffs_by_pos = [[] for _ in range(16)]
for i, d in enumerate(np.abs(np.diff(seg))):
    diffs_by_pos[i % 16].append(d)
# pos 15 mean=3555, all others ~1400
```

Position-15 (frame boundary) systematically being 2× the other positions immediately localized the bug to "predictor seed wrong at every new ADPCM frame", which narrowed the search to per-frame state read by the decoder — `rspa.adpcm_table` and `dc_state`. `dc_state` was already being reset on `SET_WAVETABLE` (line 836 in `n_env.c`); `rspa.adpcm_table` was not.

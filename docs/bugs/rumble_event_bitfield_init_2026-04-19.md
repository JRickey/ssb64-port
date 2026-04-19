# Rumble event bitfield initializers inverted on LE (2026-04-19)

## Symptom

Hang with macOS beach ball when starting a battle (1P or VS). Watchdog reports `active_tid=5(game)` with frozen `yield_count`, and the SIGUSR1 backtrace shows the game coroutine spinning inside `gmRumbleActorProcUpdate` (src/gm/gmrumble.c:401). Timing: the last successful activity was a fighter entering the arena in its `AppearR`/`AppearL` status — the stage is loaded, battle UI is up, then the game stops yielding.

## Root cause

`GMRumbleEventDefault` (src/gm/gmtypes.h:348) declares its bitfields in endian-dependent order so the 3-bit opcode always sits in the low bits of the 16-bit word:

```c
struct GMRumbleEventDefault {
#if IS_BIG_ENDIAN
    u16 opcode : 3;
    u16 param  : 13;
#else
    u16 param  : 13;
    u16 opcode : 3;
#endif
};
```

The event tables in `gmrumble.c` (`dGMRumbleEvent0` … `dGMRumbleEvent11`) used **positional** initializers, which bind by field declaration order:

```c
{ nGMRumbleEventStartRumble, 8000 }
```

On BE the first field is `opcode`, so this yields `opcode=StartRumble, param=8000` — correct. On LE the first field is `param`, so the literal is stored as `param=StartRumble=1, opcode=(8000 & 7)=0=End`.

`gmRumbleUpdateEventExecute` (gmrumble.c:185) runs `while (rumble_status == 0) switch (opcode)`. With `opcode == End` it resets `p_event` back to the start of the list and re-enters the loop. `rumble_status` never advances off 0, so the coroutine spins forever and starves every other task.

The hang is triggered at battle start because fighter spawn (`AppearR`/`AppearL`) queues a rumble event, which `gcRunGObjProcess` → `gmRumbleActorProcUpdate` → `gmRumbleUpdateEventExecute` tries to execute.

## Fix

Define a `PORT`-guarded macro that wraps positional init on BE and designated init on LE, then use it in every `dGMRumbleEventN` table:

```c
#ifdef PORT
#define GMRUMBLE_EV(op, p) { .opcode = (op), .param = (p) }
#else
#define GMRUMBLE_EV(op, p) { (op), (p) }
#endif
```

Designated initializers bind by field name, so they produce the same bit layout regardless of bitfield declaration order. The original IDO-matching positional form is preserved on non-PORT builds.

## Files touched

- `src/gm/gmrumble.c` — macro + all `dGMRumbleEventN` initializers.

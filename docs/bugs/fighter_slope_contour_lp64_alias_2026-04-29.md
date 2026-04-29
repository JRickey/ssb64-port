# Fighter Slope Contour LP64 Alias (issue #5) — Resolved (2026-04-29)

**Symptoms:** Fighters such as Kirby, Samus, and Donkey Kong did not contour their feet correctly on sloped ground. On Peach's Castle's center roof, the visible result was one foot floating while the other clipped into the slope instead of following the ground contour.

**Root cause:** `func_ovl2_800EBC0C` computes the slope-contour IK bend for foot joints. The decomp preserves an N64 layout alias:

```c
attach_dobj = dobj->child->user_data.p;
sp38.x = attach_dobj->rotate.vec.f.x;
sp38.y = attach_dobj->rotate.vec.f.y;
sp38.z = attach_dobj->rotate.vec.f.z;
```

For fighter joints, `user_data.p` is actually an `FTParts *`, not a `DObj *`. On N64, this works because `DObj::rotate.vec.f` at offset `0x30` aliases the cached transform basis in `FTParts::unk_dobjtrans_0x10[2]`. On LP64, `DObj` grows because its pointers widen to 8 bytes, so `DObj::rotate.vec.f` no longer lands at offset `0x30`. The foot IK solver then reads the wrong vector, producing bad contour rotations while the collision normal and floor-line data are otherwise sane.

**Fix:** Keep the original alias for non-port builds, but under `PORT` read the intended cached transform row explicitly:

```c
FTParts *attach_parts = ftGetParts(dobj->child);
sp38.x = attach_parts->unk_dobjtrans_0x10[2][0];
sp38.y = attach_parts->unk_dobjtrans_0x10[2][1];
sp38.z = attach_parts->unk_dobjtrans_0x10[2][2];
```

This preserves the original N64 semantics without depending on incompatible host `DObj` layout.

**Files changed:** `src/ft/ftparam.c`

**Verification:** `cmake --build /Users/jackrickey/Dev/ssb64-port/build -j8` rebuilt `src/ft/ftparam.c.o` and relinked `BattleShip` successfully.

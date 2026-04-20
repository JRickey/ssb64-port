# Known Bugs & Resolved Issues

This directory documents significant bugs encountered during the port, their symptoms, root causes, and fixes. When fixing a new bug, add an entry here so future sessions can recognize the same class of issue.

## Index

| Date | Slug | Summary |
|------|------|---------|
| 2026-04-20 | [wpattributes_bitfield_padding](wpattributes_bitfield_padding_2026-04-20.md) | **RESOLVED** — Mario 64% damage / Fox 0 damage / Samus pass-through all one bug: port's `WPAttributes` bitfield positions didn't match IDO BE's physical layout. IDO packs `angle:10` into the u16 pad at 0x26, and `knockback_weight` belongs to Word 1 not Word 2. Verified by disassembling IDO-compiled reader. See [docs/debug_ido_bitfield_layout.md](../debug_ido_bitfield_layout.md) for the audit method |
| 2026-04-20 | [itattributes_type_field_offset](itattributes_type_field_offset_2026-04-20.md) | Item pickup (pokeball, capsule) SIGBUS: decomp's `ITAttributes.type:4` declared in wrong bitfield word; real field is bits 13-10 of Word B2 at offset `0x3C`, not bits 28-31 of Word B4 at offset `0x44`. Likely copy-paste from WPAttributes which has no `type` field |
| 2026-04-20 | [item_map_coll_bottom_sign](item_map_coll_bottom_sign_2026-04-20.md) | Items embed into stage platforms: ROM stores `ITAttributes.map_coll_bottom` as positive magnitude, shared collision code expects negative signed offset — negate on load in `itManagerMakeItem` |
| 2026-04-20 | [item_arrow_gobj_implicit_int](item_arrow_gobj_implicit_int_2026-04-20.md) | Dropped-item despawn segfault: `ifCommonItemArrowMakeInterface` called without prototype → implicit-int rule truncates the 64-bit `GObj*` return to 32 bits on LP64 |
| 2026-04-20 | [mnplayers1pgame_timegobj_stale](mnplayers1pgame_timegobj_stale_2026-04-20.md) | 1P character-select crash on re-entry from VS: `sMNPlayers1PGameTimeGObj` static slot was missing from `mnPlayers1PGameInitVars`'s reset list, so its stale value survived the per-scene heap reset and `gcEjectGObj` then dereferenced freed memory |
| 2026-04-19 | [rumble_event_bitfield_init](rumble_event_bitfield_init_2026-04-19.md) | Battle-start hang: positional initializers on endian-conditional bitfield stored opcode as param; gmRumbleUpdateEventExecute spins on phantom End |
| 2026-04-18 | [aobjevent32_halfswap](aobjevent32_halfswap_2026-04-18.md) | Fighter figatree u16-halfswap corrupts AObjEvent32 command bitfields; lazy per-stream un-halfswap walker at EVENT32 reader entry |
| 2026-04-14 | [title_border_right_edge_slice](title_border_right_edge_slice_2026-04-14.md) | ce89700's zero-fill of sprite trailing columns dimmed the right edge of the title-screen border sprite; fix replicates edge pixel instead |
| 2026-04-13 | [samus_charge_shot_hit_detection](samus_charge_shot_hit_detection_2026-04-13.md) | **SUPERSEDED** by the 04-20 `WPAttributes` bitfield fix: `attack_count` was being decoded from the wrong bits (read 0 instead of the true 1), so the hit loop never iterated. The bypass-path theory was a red herring |
| 2026-04-11 | [mpvertex_byte_swap](mpvertex_byte_swap_2026-04-11.md) | MPVertexData / MPVertexIDs byte-swap deferral broke Jungle floor collision |
| 2026-04-11 | [controller_motorevt_lp64](controller_motorevt_lp64_2026-04-11.md) | Unk80045268 ContMotorEvt struct pun faults on LP64 |
| 2026-04-11 | [fighter_tile_masks_wrap](fighter_tile_masks_wrap_2026-04-11.md) | Missing SetTile masks/maskt clamp → black squares on fighter body parts |
| 2026-04-11 | [loadtlut_cross_boundary](loadtlut_cross_boundary_2026-04-11.md) | LOADTLUT spillover across 256-byte palette boundary dropped entries |
| 2026-04-11 | [osmesg_union](osmesg_union_2026-04-11.md) | OSMesg `void*` vs union type split produced uninitialized upper bytes |
| 2026-04-11 | [bzero_recursion_arm64](bzero_recursion_arm64_2026-04-11.md) | Stubbed bzero on macOS/arm64 recursed infinitely via memset lowering |
| 2026-04-10 | [sprite_texel_tmem_swizzle](sprite_texel_tmem_swizzle_2026-04-10.md) | Sprite byteswap deferral + N64 RDP TMEM line swizzle left sprites sheared |
| 2026-04-08 | [segment_0e_gdl](segment_0e_gdl_2026-04-08.md) | Segment 0x0E G_DL resolution exception branched into the graphics heap |
| 2026-04-08 | [particle_bank_dma](particle_bank_dma_2026-04-08.md) | efParticleGetLoadBankID read stale heap bytes for particle bank descriptor |
| 2026-04-08 | [ground_geometry_collision](ground_geometry_collision_2026-04-08.md) | PORT_RESOLVE + all-u16 collision struct byte-swap fixes |
| 2026-04-07 | [dl_normalization_guard](dl_normalization_guard_2026-04-07.md) | Heuristic guard mis-detected packed DLs starting with w1=0 sync commands |
| 2026-04-06 | [display_list_widening](display_list_widening_2026-04-06.md) | Packed 8-byte resource DLs needed widening to 16-byte native format |

#include <lb/library.h>
#include <sys/matrix.h>
#include <ef/efdef.h>

extern u16 gSYSinTable[0x800];

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x800D5D50
u8 dLBParticleColorDitherMode = G_CD_MAGICSQ;

// 0x800D5D54
u8 dLBParticleAlphaDitherMode = G_AD_PATTERN;

// 0x800D5D58
u16 dLBParticleCurrentGeneratorID = 0;

// 0x800D5D5C
u8 dLBParticleCurrentTransformID = 123;

// // // // // // // // // // // //
//                               //
//   GLOBAL / STATIC VARIABLES   //
//                               //
// // // // // // // // // // // //

// 0x800D6350
LBParticle *sLBParticleStructsAllocFree;

// 0x800D6354
s32 sLBParticlePad0x800D6354;

// 0x800D6358
LBParticle *sLBParticleStructsAllocLinks[16];

// 0x800D6398
LBGenerator *sLBParticleGeneratorsAllocFree;

// 0x800D639C
LBGenerator *sLBParticleGeneratorsQueued;

// 0x800D63A0
DObj *sLBParticleAttachDObjs[LBPARTICLE_ATTACH_DOBJ_NUM_MAX];

// 0x800D63C0
s32 sLBParticleScriptBanksNum[LBPARTICLE_BANKS_NUM_MAX];

// 0x800D63E0
s32 sLBParticleTextureBanksNum[LBPARTICLE_BANKS_NUM_MAX];

// 0x800D6400
LBScript **sLBParticleScriptBanks[LBPARTICLE_BANKS_NUM_MAX];

// 0x800D6420
LBTexture **sLBParticleTextureBanks[LBPARTICLE_BANKS_NUM_MAX];

// 0x800D6440
void (*sLBParticleGeneratorFuncDefault)(LBGenerator*, Vec3f*);

// 0x800D6444
void (*sLBParticleGeneratorFuncSetup)(LBGenerator*);

// 0x800D6448
u16 gLBParticleStructsUsedNum;

// 0x800D644A
u16 gLBParticleGeneratorsUsedNum;

// 0x800D644C
u16 gLBParticleTransformsUsedNum;

// 0x800D644E
u16 D_ovl0_800D644E;

// 0x800D6450
u16 D_ovl0_800D6450;

// 0x800D6452
u16 D_ovl0_800D6452;

// 0x800D6454
LBTransform *sLBParticleTransformsAllocFree;

// 0x800D6458
LBGenerator *sLBParticleGeneratorsLastProcessed;

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x800CE040
s32 lbParticleAllocTransforms(s32 num, size_t size)
{
	LBTransform *xf;
	s32 i;

	sLBParticleTransformsAllocFree = NULL;
	
	for (i = 0; i < num; i++)
	{
		xf = syTaskmanMalloc(size, 0x4);

		if (xf == NULL)
		{
			return i;
		}
		xf->next = sLBParticleTransformsAllocFree;
		sLBParticleTransformsAllocFree = xf;
	}
	gLBParticleTransformsUsedNum = 0;
	D_ovl0_800D6452 = 0;

	return i;
}

// 0x800CE0D8
LBTransform* lbParticleGetTransform(u8 status, u16 generator_id)
{
	LBTransform *xf = sLBParticleTransformsAllocFree;

	if (xf != NULL)
	{
        sLBParticleTransformsAllocFree = xf->next;
        xf->users_num = 1;
        xf->transform_id = dLBParticleCurrentTransformID;
        xf->proc_dead = NULL;

		xf->translate.x = xf->translate.y = xf->translate.z = 0.0F;
		xf->rotate.x = xf->rotate.y = xf->rotate.z = 0.0F;
		xf->scale.x = xf->scale.y = xf->scale.z = 1.0F;

		xf->transform_status = status;
		xf->generator_id = generator_id;

		gLBParticleTransformsUsedNum++;

		if (D_ovl0_800D6452 < gLBParticleTransformsUsedNum)
		{
			D_ovl0_800D6452 = gLBParticleTransformsUsedNum;
		}
	}
	return xf;
}

// 0x800CE188
void lbParticleEjectTransform(LBTransform *xf)
{
	if (xf->proc_dead != NULL)
	{
		xf->proc_dead(xf);
	}
	xf->next = sLBParticleTransformsAllocFree;
	sLBParticleTransformsAllocFree = xf;

	gLBParticleTransformsUsedNum--;
}

// 0x800CE1DC
LBTransform* lbParticleAddTransformForStruct(LBParticle *pc, u8 status)
{
    pc->xf = lbParticleGetTransform(status, pc->generator_id);

	return pc->xf;
}

// 0x800CE218
LBTransform* lbParticleAddTransformForGenerator(LBGenerator *gn, u8 status)
{
	gn->xf = lbParticleGetTransform(status, gn->generator_id);

	return gn->xf;
}

// 0x800CE254
void lbParticleSetupBankID(s32 bank_id, LBScriptDesc *script_desc, LBTextureDesc *texture_desc)
{
	s32 i, j;

	if (bank_id >= LBPARTICLE_BANKS_NUM_MAX)
	{
		return;
	}
	sLBParticleScriptBanksNum[bank_id] = script_desc->scripts_num;
	sLBParticleTextureBanksNum[bank_id] = texture_desc->textures_num;

	sLBParticleScriptBanks[bank_id] = script_desc->scripts;
	sLBParticleTextureBanks[bank_id] = texture_desc->textures;

	for (i = 1; i <= sLBParticleScriptBanksNum[bank_id]; i++)
	{
		/* 
         * By default, the scripts array is populated with the offsets of the scripts
		 * in their respective file, so this is essentially making them into valid RAM pointers.
		 */
		script_desc->scripts[i - 1] = lbRelocGetFileData(LBScript*, script_desc, script_desc->scripts[i - 1]);
	}
	for (i = 1; i <= sLBParticleTextureBanksNum[bank_id]; i++)
	{
		// Much like scripts, textures from the file are also being "pointerized" here.
		texture_desc->textures[i - 1] = lbRelocGetFileData(LBTexture*, texture_desc, texture_desc->textures[i - 1]);
	}
	for (i = 0; i < sLBParticleTextureBanksNum[bank_id]; i++)
	{
		for (j = 0; j < sLBParticleTextureBanks[bank_id][i]->count; j++)
		{
			sLBParticleTextureBanks[bank_id][i]->data[j] = lbRelocGetFileData(void*, texture_desc, sLBParticleTextureBanks[bank_id][i]->data[j]);
		}
		if (sLBParticleTextureBanks[bank_id][i]->fmt == G_IM_FMT_CI)
		{
			if (sLBParticleTextureBanks[bank_id][i]->flags & 1)
			{
				// Single palette after the images
				j = sLBParticleTextureBanks[bank_id][i]->count;

				sLBParticleTextureBanks[bank_id][i]->data[j] = lbRelocGetFileData(void*, texture_desc, sLBParticleTextureBanks[bank_id][i]->data[j]);
			}
			else for (j = sLBParticleTextureBanks[bank_id][i]->count; j < sLBParticleTextureBanks[bank_id][i]->count * 2; j++)
			{
                // One palette per image
				sLBParticleTextureBanks[bank_id][i]->data[j] = lbRelocGetFileData(void*, texture_desc, sLBParticleTextureBanks[bank_id][i]->data[j]);
			}
		}
	}
}

// 0x800CE418
GObj* lbParticleAllocStructs(s32 num)
{
	s32 i;
	LBParticle *pc;

	sLBParticleStructsAllocFree = NULL;

	for (i = 0; i < ARRAY_COUNT(sLBParticleStructsAllocLinks); i++)
	{
		sLBParticleStructsAllocLinks[i] = NULL;
	}
	for (i = num - 1; i >= 0; i--)
	{
		pc = syTaskmanMalloc(sizeof(*pc), 0x4);

		if (pc == NULL)
		{
			return NULL;
		}
		pc->next = sLBParticleStructsAllocFree;
		sLBParticleStructsAllocFree = pc;
	}
	gLBParticleStructsUsedNum = 0;
	D_ovl0_800D644E = 0;

	if (gcFindGObjByID(~0x5) != NULL)
	{
		return NULL;
	}
	return gcMakeGObjSPAfter(~0x5, lbParticleStructFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
}

// 0x800CE4E4
LBParticle* lbParticleMakeStruct
(
	LBParticle *this_pc,
	s32 bank_id,
	u32 flags,
	u16 texture_id,
	u8 *bytecode,
	s32 lifetime,
	f32 pos_x,
	f32 pos_y,
	f32 pos_z,
	f32 vel_x,
	f32 vel_y,
	f32 vel_z,
	f32 size,
	f32 gravity,
	f32 friction,
	u32 texture_flags,
	LBGenerator *gn
)
{
	LBParticle *new_pc;
	s32 i;

    new_pc = sLBParticleStructsAllocFree;
    
    if (new_pc == NULL)
    {
        return NULL;
    }
    gLBParticleStructsUsedNum++;
    
    if (D_ovl0_800D644E < gLBParticleStructsUsedNum) 
    {
        D_ovl0_800D644E = gLBParticleStructsUsedNum;
    }
    if (gn != NULL)
    {
        new_pc->generator_id = gn->generator_id;
    } 
    else new_pc->generator_id = ++dLBParticleCurrentGeneratorID;

    if (gn != NULL)
    {
        new_pc->xf = gn->xf;
        
        if (new_pc->xf != NULL)
        {
            new_pc->xf->users_num++;
        }
    }
    else new_pc->xf = NULL;
    
    sLBParticleStructsAllocFree = new_pc->next;
    
    if (this_pc == NULL) 
    {
        new_pc->next = sLBParticleStructsAllocLinks[bank_id >> 3];
        sLBParticleStructsAllocLinks[bank_id >> 3] = new_pc;
    } 
    else 
    {
        new_pc->next = this_pc->next;
        this_pc->next = new_pc;
    }
    new_pc->bank_id = bank_id;
    new_pc->flags = flags;
    new_pc->texture_id = texture_id;

	new_pc->pos.x = pos_x;
	new_pc->pos.y = pos_y;
	new_pc->pos.z = pos_z;

    new_pc->vel.x = vel_x;
    new_pc->vel.y = vel_y;
    new_pc->vel.z = vel_z;
    
    new_pc->size = size;
    new_pc->gravity = gravity;
    new_pc->friction = friction;

    new_pc->lifetime = lifetime + 1;
    new_pc->bytecode_csr = new_pc->return_ptr = 0;
    
    new_pc->bytecode = bytecode;

    if (texture_flags != 0)
    { 
        new_pc->flags |= LBPARTICLE_FLAG_SHAREDPAL;
    }
    new_pc->bytecode_timer = (bytecode != NULL) ? 1 : 0;
    
    new_pc->frame_id = 0;
    
    new_pc->primcolor.r = new_pc->primcolor.g = new_pc->primcolor.b = new_pc->primcolor.a = 0xFF;
    new_pc->envcolor.r = new_pc->envcolor.g = new_pc->envcolor.b = new_pc->envcolor.a = 0x00;
    
    new_pc->size_target_length = new_pc->primcolor_target_length = new_pc->envcolor_target_length = 0;
    
    new_pc->gn = gn;

	return new_pc;
}

// 0x800CE6B8
LBParticle* lbParticleMakeChildScriptID(LBParticle *pc, s32 bank_id, s32 script_id)
{
	LBScript *script;
	s32 id = bank_id & 7;

	if (id >= LBPARTICLE_BANKS_NUM_MAX)
	{
		return NULL;
	}
	if (script_id >= sLBParticleScriptBanksNum[id])
	{
		return NULL;
	}
	script = sLBParticleScriptBanks[id][script_id];

	return lbParticleMakeStruct
	(
		pc,
		bank_id,
		script->flags,
		script->texture_id,
		script->bytecode,
		script->particle_lifetime,
		0.0F, 0.0F, 0.0F,
		script->vel.x, script->vel.y, script->vel.z,
		script->size,
		script->gravity,
		script->friction,
		sLBParticleTextureBanks[id][script->texture_id]->flags,
		NULL
	);
}

// 0x800CE7A8
LBParticle* lbParticleMakeParam
(
	s32 bank_id,
	u32 flags,
	u16 texture_id,
	u8 *bytecode,
	s32 lifetime,
	f32 pos_x,
	f32 pos_y,
	f32 pos_z,
	f32 vel_x,
	f32 vel_y,
	f32 vel_z,
	f32 size,
	f32 gravity,
	f32 friction,
	u32 texture_flags,
	LBGenerator *gn
)
{
	LBParticle *pc = lbParticleMakeStruct
	(
		NULL,
		bank_id,
		flags,
		texture_id,
		bytecode,
		lifetime,
		pos_x, pos_y, pos_z,
		vel_x, vel_y, vel_z,
		size,
		gravity,
		friction,
		texture_flags,
		gn
	);
	if (pc != NULL)
	{
		lbParticleUpdateStruct(pc, 0, bank_id >> 3);
	}
	return pc;
}

// 0x800CE870
LBParticle* lbParticleMakeCommon(s32 bank_id, s32 script_id)
{
	LBParticle *pc = lbParticleMakeChildScriptID(NULL, bank_id, script_id);

	if (pc != NULL)
	{
		lbParticleUpdateStruct(pc, NULL, bank_id >> 3);
	}
	return pc;
}

// 0x800CE8C0
LBParticle* lbParticleMakePosVel(s32 bank_id, s32 script_id, f32 pos_x, f32 pos_y, f32 pos_z, f32 vel_x, f32 vel_y, f32 vel_z)
{
	LBParticle *pc;
	LBScript *script;
	s32 id = bank_id & 7;

	if (id >= LBPARTICLE_BANKS_NUM_MAX)
	{
		return NULL;
	}
	if (script_id >= sLBParticleScriptBanksNum[id])
	{
		return NULL;
	}
	script = sLBParticleScriptBanks[id][script_id];

	pc = lbParticleMakeStruct
	(
		NULL,
		bank_id,
		script->flags,
		script->texture_id,
		script->bytecode,
		script->particle_lifetime,
		pos_x, pos_y, pos_z,
		vel_x, vel_y, vel_z,
		script->size,
		script->gravity,
		script->friction,
		sLBParticleTextureBanks[id][script->texture_id]->flags,
		NULL
	);
	if (pc != NULL)
	{
		lbParticleUpdateStruct(pc, 0, bank_id >> 3);
	}
	return pc;
}

// 0x800CE9E8
LBParticle* lbParticleMakeScriptID(s32 bank_id, s32 script_id)
{
	return lbParticleMakeChildScriptID(NULL, bank_id, script_id);
}

// 0x800CEA14
void LBParticleProcessStruct(LBParticle *pc)
{
	if (pc != NULL)
	{
		lbParticleUpdateStruct(pc, NULL, pc->bank_id >> 3);
	}
}

// 0x800CEA40
void lbParticleEjectStruct(LBParticle *this_pc)
{
	LBParticle *prev_pc, *current_pc;
	LBGenerator *gn;
	s32 bank_id;

	bank_id = this_pc->bank_id >> 3;
	current_pc = sLBParticleStructsAllocLinks[bank_id];
	prev_pc = NULL;

	while (current_pc != NULL)
	{
		if (current_pc == this_pc)
		{
			if (prev_pc == NULL)
			{
				sLBParticleStructsAllocLinks[bank_id] = current_pc->next;
			}
			else prev_pc->next = current_pc->next;

			gn = this_pc->gn;

			if ((gn != NULL) && (this_pc->flags & LBPARTICLE_FLAG_VORTEX) && (gn->kind == nLBParticleKindVortex))
			{
				gn->generator_vars.vortex.lifetime--;
			}
			if (this_pc->xf != NULL)
			{
				this_pc->xf->users_num--;

				if (this_pc->xf->users_num == 0)
				{
					lbParticleEjectTransform(this_pc->xf);
				}
			}
			current_pc->next = sLBParticleStructsAllocFree;
			sLBParticleStructsAllocFree = current_pc;
			gLBParticleStructsUsedNum--;

			break;
		}
		prev_pc = current_pc;
		current_pc = current_pc->next;
	}
}

// 0x800CEB50 - unused? Eject all LBParticle structs
void lbParticleEjectStructAll(void)
{
	LBParticle *current_pc, *next_pc;
	s32 i;

	for (i = 0; i < ARRAY_COUNT(sLBParticleStructsAllocLinks); i++)
	{
		current_pc = sLBParticleStructsAllocLinks[i];

		while (current_pc != NULL)
		{
			next_pc = current_pc->next;

			lbParticleEjectStruct(current_pc);

			current_pc = next_pc;
		}
	}
}

// 0x800CEBC0 - WARNING: Big Endian only!
u8* lbParticleReadFloatBigEnd(u8 *csr, f32 *f)
{
	u8 bytes[4];

	bytes[0] = *csr++;
	bytes[1] = *csr++;
	bytes[2] = *csr++;
	bytes[3] = *csr++;

	*f = *(f32*)bytes;

	return csr;
}

// 0x800CEBF8
u8* lbParticleReadUShort(u8 *csr, u16 *s)
{
	u16 value = *csr++;

	if (value & 0x80)
	{
		value = ((value & 0x7F) << 8) + *csr++;
	}
	*s = value + 1;

	return csr;
}

// 0x800CEC34
void lbParticleRotateVel(LBParticle *pc, f32 angle)
{
	Vec3f vel;
	f32 sin_angle;
	f32 magnitude;
	f32 pitch;
	f32 yaw;
	f32 sin_pitch;
	f32 cos_pitch;
	f32 sin_yaw;
	f32 cos_yaw;
	f32 cos_angle;
	
	vel.x = pc->vel.x;
	vel.y = pc->vel.y;
	vel.z = pc->vel.z;
	
	pitch = syUtilsArcTan2(vel.y, vel.z);
	sin_pitch = __sinf(pitch);
	cos_pitch = __cosf(pitch);
	
	yaw = syUtilsArcTan2(vel.x, (vel.y * sin_pitch) + (vel.z * cos_pitch));
	sin_yaw = __sinf(yaw);
	cos_yaw = __cosf(yaw);
	
	magnitude = sqrtf(SQUARE(vel.x) + SQUARE(vel.y) + SQUARE(vel.z));
	
	vel.y = syUtilsRandFloat() * F_CST_DTOR32(360.0F);
	
	sin_angle = __sinf(angle) * magnitude;
	
	vel.z = sin_yaw;
	vel.x = __cosf(vel.y) * sin_angle;
	vel.y = __sinf(vel.y) * sin_angle;
	
	cos_angle = __cosf(angle) * magnitude;
	
	pc->vel.x = (vel.x * cos_yaw) + (cos_angle * sin_yaw);
	pc->vel.y = (((-vel.x * sin_pitch) * sin_yaw) + (vel.y * cos_pitch)) + ((cos_angle * sin_pitch) * cos_yaw);
	pc->vel.z = (((-vel.x * cos_pitch) * vel.z) - (vel.y * sin_pitch)) + ((cos_angle * cos_pitch) * cos_yaw);
}

// 0x800CEDBC
void lbParticleSetDistVelDObj(LBParticle *pc, DObj *dobj)
{
	f32 dx, dy, dz, dist;

	if (dobj == NULL)
	{
		return;
	}
	dx = dobj->translate.vec.f.x - pc->pos.x;
	dy = dobj->translate.vec.f.y - pc->pos.y;
	dz = dobj->translate.vec.f.z - pc->pos.z;

	dist = sqrtf(SQUARE(pc->vel.x) + SQUARE(pc->vel.y) + SQUARE(pc->vel.z));

	if ((SQUARE(dx) + SQUARE(dy) + SQUARE(dz)) != 0.0F)
	{
		dist /= sqrtf(SQUARE(dx) + SQUARE(dy) + SQUARE(dz));

		pc->vel.x = dx * dist;
		pc->vel.y = dy * dist;
		pc->vel.z = dz * dist;
	}
}

// 0x800CEEB8
void lbParticleAddDistVelMagDObj(LBParticle *pc, DObj *dobj, f32 magnitude)
{
	f32 dx, dy, dz, dist;

	if (dobj == NULL)
	{
		return;
	}
	dx = dobj->translate.vec.f.x - pc->pos.x;
	dy = dobj->translate.vec.f.y - pc->pos.y;
	dz = dobj->translate.vec.f.z - pc->pos.z;

	dist = SQUARE(dx) + SQUARE(dy) + SQUARE(dz);

	if (dist != 0.0F)
	{
		dist = magnitude / dist;

		pc->vel.x += dist * dx;
		pc->vel.y += dist * dy;
		pc->vel.z += dist * dz;
	}
}

#ifdef NON_MATCHING
// 0x800CEF4C - NONMATCHING: v0 VS v1 regswap in switch statement initialization + floats are a mess near the end of the function
LBParticle* lbParticleUpdateStruct(LBParticle *this_pc, LBParticle *other_pc, s32 bank_id)
{
    LBParticle *current_pc;
    LBParticle *next_pc;
    LBGenerator *gn;
    u8 *csr; // s1
    u8 command; // s0
    u16 bytecode_timer;
    u16 angle_id;
    u16 bytecode_csr;
    s32 svar1;  // sp88?
    s32 svar2;
    u8 opcode;
    f32 fvar1; // s3
    f32 fvar2;
    f32 unused1[2];
    f32 sp70;
    f32 unused2[2];
    f32 f0, f1;
    f32 sx1, sx2; // sp5C, sp58
    f32 cx1, cx2; // f16=>sp54, f12=>sp50
    u16 angle_id_2;
    f32 sx4, cx4; 
    f32 sx3, cx3; // f18, f0=>sp44
    
    if (this_pc->flags & LBPARTICLE_FLAG_PAUSE)
    {
        return this_pc->next;
    }
    if (this_pc->bytecode_timer != 0)
    {
        this_pc->bytecode_timer--;
        
        if (this_pc->bytecode_timer == 0)
        {
            csr = &this_pc->bytecode[this_pc->bytecode_csr];
            
            do
            {
                command = *csr++;
                
                if (command >= 0x80)
                {
                    bytecode_timer = 0;

                    opcode = (command & 0xF8);
                    
                    if (opcode > 0x98)
                    {
                        opcode = (command & 0xF0);

                        if ((opcode != 0xC0) && (opcode != 0xD0))
                        {
                            opcode = command;
                        }
                    }
                    // Even if you DO manage to fix the floats near the end, how do you deal with the v0 vs v1 regswap up here!?

                    switch (opcode)
                    {
                    case LBPARTICLE_OPCODE_SETPOS:
                        if (command & 1) 
                        { 
                            csr = lbParticleReadFloatBigEnd(csr, &this_pc->pos.x);
                        }
                        if (command & 2) 
                        { 
                            csr = lbParticleReadFloatBigEnd(csr, &this_pc->pos.y);
                        }
                        if (command & 4)
                        { 
                            csr = lbParticleReadFloatBigEnd(csr, &this_pc->pos.z);
                        }
                        break;
                        
                    case LBPARTICLE_OPCODE_ADDPOS:
                        if (command & 1)
                        {
                            csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                            this_pc->pos.x += fvar1;
                        }
                        if (command & 2)
                        {
                            csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                            this_pc->pos.y += fvar1;
                        }
                        if (command & 4)
                        {
                            csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                            this_pc->pos.z += fvar1;
                        }
                        break;
                        
                    case LBPARTICLE_OPCODE_SETVEL:
                        if (command & 1)
                        {
                            csr = lbParticleReadFloatBigEnd(csr, &this_pc->vel.x);
                        }
                        if (command & 2)
                        {
                            csr = lbParticleReadFloatBigEnd(csr, &this_pc->vel.y);
                        }
                        if (command & 4)
                        {
                            csr = lbParticleReadFloatBigEnd(csr, &this_pc->vel.z);
                        }
                        break;
                        
                    case LBPARTICLE_OPCODE_ADDVEL:
                        if (command & 1)
                        {
                            csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                            this_pc->vel.x += fvar1;
                        }
                        if (command & 2)
                        {
                            csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                            this_pc->vel.y += fvar1;
                        }
                        if (command & 4)
                        {
                            csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                            this_pc->vel.z += fvar1;
                        }
                        break;
                        
                    case LBPARTICLE_OPCODE_SETSIZELERP:
                        csr = lbParticleReadUShort(csr, &this_pc->size_target_length);
                        csr = lbParticleReadFloatBigEnd(csr, &this_pc->size_target);

                        if (this_pc->size_target_length == 1)
                        {
                            this_pc->size = this_pc->size_target;
                            this_pc->size_target_length = 0;
                        }
                        break;
                        
                    case LBPARTICLE_OPCODE_SETFLAG:
                        this_pc->flags = *csr++;
                        break;
                        
                    case LBPARTICLE_OPCODE_SETGRAVITY:
                        csr = lbParticleReadFloatBigEnd(csr, &this_pc->gravity);
                        
                        if (this_pc->gravity == 0.0F)
                        {
                            this_pc->flags &= ~LBPARTICLE_FLAG_GRAVITY;
                        }
                        else this_pc->flags |= LBPARTICLE_FLAG_GRAVITY;
                        break;
                        
                    case LBPARTICLE_OPCODE_SETFRICTION:
                        csr = lbParticleReadFloatBigEnd(csr, &this_pc->friction);
                            
                        if (this_pc->friction == 1.0F)
                        {
                            this_pc->flags &= ~LBPARTICLE_FLAG_FRICTION;
                        }
                        else this_pc->flags |= LBPARTICLE_FLAG_FRICTION;
                        break;
                        
                    case LBPARTICLE_OPCODE_MAKESCRIPT:
                        svar1 = *csr++;
                        svar1 <<= 8;
                        svar1 += *csr++;

                        current_pc = lbParticleMakeChildScriptID(this_pc, this_pc->bank_id, svar1);
                            
                        if (current_pc != NULL)
                        {
                            current_pc->pos.x = this_pc->pos.x;
                            current_pc->pos.y = this_pc->pos.y;
                            current_pc->pos.z = this_pc->pos.z;
                            current_pc->generator_id = this_pc->generator_id;
                            current_pc->gn = this_pc->gn;
                            current_pc->xf = this_pc->xf;
                                
                            if (current_pc->xf != NULL)
                            {
                                current_pc->xf->users_num++;
                            }
                            lbParticleUpdateStruct(current_pc, this_pc, this_pc->bank_id >> 3);
                        }  
                        break;
                
                    case LBPARTICLE_OPCODE_MAKEGENERATOR:
                        svar1 = *csr++;
                        svar1 <<= 8;
                        svar1 += *csr++;

                        gn = lbParticleMakeGenerator(this_pc->bank_id, svar1);
                            
                        if (gn != NULL)
                        {
                            gn->pos.x = this_pc->pos.x;
                            gn->pos.y = this_pc->pos.y;
                            gn->pos.z = this_pc->pos.z;
                            gn->generator_id = this_pc->generator_id;
                            gn->xf = this_pc->xf;
                                
                            if (gn->xf != NULL)
                            {
                                gn->xf->users_num++;
                            }
                        }    
                        break;
                        
                    case LBPARTICLE_OPCODE_SETLIFERAND:
                        svar1 = *csr++;
                        svar1 <<= 8;
                        svar1 += *csr++;

                        svar2 = *csr++;
                        svar2 <<= 8;
                        svar2 += *csr++;

                        this_pc->lifetime = svar1 + (s32) (svar2 * syUtilsRandFloat());
                        break;
                        
                    case LBPARTICLE_OPCODE_TRYDEADRAND:
                        svar1 = *csr++;
                        svar2 = syUtilsRandFloat() * 100.0F;

                        if (svar1 < svar2)
                        {
                            break;
                        }
                        else this_pc->lifetime = 1;
                        goto loop_break;
                        
                    case LBPARTICLE_OPCODE_ADDVELRAND:    
                        csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                        this_pc->pos.x += fvar1 * syUtilsRandFloat();

                        csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                        this_pc->pos.y += fvar1 * syUtilsRandFloat();

                        csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                        this_pc->pos.z += fvar1 * syUtilsRandFloat();
                        break;
                        
                    case LBPARTICLE_OPCODE_SETVELANGLE:
                        csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                        lbParticleRotateVel(this_pc, fvar1);
                        break;
                        
                    case LBPARTICLE_OPCODE_MAKERAND:                        
                        svar1 = *csr++;
                        svar1 <<= 8;
                        svar1 += *csr++;

                        svar2 = *csr++;
                        svar2 <<= 8;
                        svar2 += *csr++;

                        svar1 += (s32) (svar2 * syUtilsRandFloat());

                        current_pc = lbParticleMakeChildScriptID(this_pc, this_pc->bank_id, svar1);
                            
                        if (current_pc != NULL)
                        {
                            current_pc->pos.x = this_pc->pos.x;
                            current_pc->pos.y = this_pc->pos.y;
                            current_pc->pos.z = this_pc->pos.z;
                            current_pc->generator_id = this_pc->generator_id;
                            current_pc->gn = this_pc->gn;
                            current_pc->xf = this_pc->xf;

                            if (current_pc->xf != NULL)
                            {
                                current_pc->xf->users_num++;
                            }
                            lbParticleUpdateStruct(current_pc, this_pc, this_pc->bank_id >> 3);
                        }
                        break;
                        
                    case LBPARTICLE_OPCODE_MULVELUFORM: 
                        csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                            
                        this_pc->vel.x *= fvar1;
                        this_pc->vel.y *= fvar1;
                        this_pc->vel.z *= fvar1;
                        break;
                        
                    case LBPARTICLE_OPCODE_SETSIZERAND:    
                        csr = lbParticleReadUShort(csr, &this_pc->size_target_length);
                        csr = lbParticleReadFloatBigEnd(csr, &this_pc->size_target);
                        csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                            
                        this_pc->size_target += fvar1 * syUtilsRandFloat();
                            
                        if (this_pc->size_target_length == 1) 
                        {
                            this_pc->size = this_pc->size_target;
                            this_pc->size_target_length = 0;
                        }
                        break;
                        
                    case LBPARTICLE_OPCODE_ENVCOLOR:
                        this_pc->flags |= LBPARTICLE_FLAG_ENVCOLOR;
                        break;
                        
                    case LBPARTICLE_OPCODE_NOMASKST:
                        this_pc->flags &= ~(LBPARTICLE_FLAG_MASKT | LBPARTICLE_FLAG_MASKS);
                        break;
                        
                    case LBPARTICLE_OPCODE_MASKS:            
                        this_pc->flags &= ~LBPARTICLE_FLAG_MASKT;
                        this_pc->flags |= LBPARTICLE_FLAG_MASKS;
                        break;
                        
                    case LBPARTICLE_OPCODE_MASKT:
                        this_pc->flags &= ~LBPARTICLE_FLAG_MASKS;
                        this_pc->flags |= LBPARTICLE_FLAG_MASKT;
                        break;
                            
                    case LBPARTICLE_OPCODE_MASKST:
                        this_pc->flags |= (LBPARTICLE_FLAG_MASKT | LBPARTICLE_FLAG_MASKS);
                        break;
                        
                    case LBPARTICLE_OPCODE_ALPHABLEND:
                        this_pc->flags |= LBPARTICLE_FLAG_ALPHABLEND;
                        break;
                        
                    case LBPARTICLE_OPCODE_NODITHER:
                        this_pc->flags &= ~LBPARTICLE_FLAG_DITHER;
                        break;
                        
                    case LBPARTICLE_OPCODE_DITHER:
                        this_pc->flags |= LBPARTICLE_FLAG_DITHER;
                        break;
                    
                    case LBPARTICLE_OPCODE_NONOISE:
                        this_pc->flags |= LBPARTICLE_FLAG_NOISE;
                        break;
                        
                    case LBPARTICLE_OPCODE_NOISE:
                        this_pc->flags &= ~LBPARTICLE_FLAG_NOISE;
                        break;
                        
                    case LBPARTICLE_OPCODE_SETDISTVEL:
                        svar1 = *csr++;
                        lbParticleSetDistVelDObj(this_pc, sLBParticleAttachDObjs[svar1 - 1]);
                        break;
                        
                    case LBPARTICLE_OPCODE_ADDDISTVELMAG:
                        svar1 = *csr++;
                        csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                        lbParticleAddDistVelMagDObj(this_pc, sLBParticleAttachDObjs[svar1 - 1], fvar1);
                        break;
                        
                    case LBPARTICLE_OPCODE_MAKEID:
                        svar1 = *csr++;
                        svar1 <<= 8;
                        svar1 += *csr++;

                        current_pc = lbParticleMakeChildScriptID(this_pc, this_pc->bank_id, svar1);

                        if (current_pc != NULL)
                        {
                            current_pc->pos.x = this_pc->pos.x;
                            current_pc->pos.y = this_pc->pos.y;
                            current_pc->pos.z = this_pc->pos.z;
                            current_pc->vel.x = this_pc->vel.x;
                            current_pc->vel.y = this_pc->vel.y;
                            current_pc->vel.z = this_pc->vel.z;
                            current_pc->generator_id = this_pc->generator_id;
                            current_pc->gn = this_pc->gn;
                            current_pc->xf = this_pc->xf;
                                
                            if (current_pc->xf != NULL)
                            {
                                current_pc->xf->users_num++;
                            }
                            lbParticleUpdateStruct(current_pc, this_pc, this_pc->bank_id >> 3);
                        }
                        break;
                        
                    case LBPARTICLE_OPCODE_PRIMBLENDRAND:
                        fvar1 = *csr++;
                        this_pc->target_primcolor.r += fvar1 * syUtilsRandFloat();
                        fvar1 = *csr++;
                        this_pc->target_primcolor.g += fvar1 * syUtilsRandFloat();
                        fvar1 = *csr++;
                        this_pc->target_primcolor.b += fvar1 * syUtilsRandFloat();
                        fvar1 = *csr++;
                        this_pc->target_primcolor.a += fvar1 * syUtilsRandFloat();
                            
                        if (this_pc->primcolor_target_length == 0)
                        {
                            // this has lwl and lwr, so maybe it's a struct?
                            this_pc->primcolor = this_pc->target_primcolor;
                        }
                        break;
                        
                    case LBPARTICLE_OPCODE_ENVBLENDRAND:
                        fvar1 = *csr++;
                        this_pc->target_envcolor.r += fvar1 * syUtilsRandFloat();
                        fvar1 = *csr++;
                        this_pc->target_envcolor.g += fvar1 * syUtilsRandFloat();
                        fvar1 = *csr++;
                        this_pc->target_envcolor.b += fvar1 * syUtilsRandFloat();
                        fvar1 = *csr++;
                        this_pc->target_envcolor.a += fvar1 * syUtilsRandFloat();

                        if (this_pc->envcolor_target_length == 0)
                        {
                            // this has lwl and lwr, so maybe it's a struct?
                            this_pc->envcolor = this_pc->target_envcolor;
                        }
                        break;
                        
                    case 0xBC:    
                        this_pc->frame_id = *csr++;
                        fvar1 = *csr++;
                        this_pc->frame_id += fvar1 * syUtilsRandFloat();
                        break;
                        
                    case LBPARTICLE_OPCODE_SETVELMAG:
                        csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                        csr = lbParticleReadFloatBigEnd(csr, &fvar2);

                        fvar1 += fvar2 * syUtilsRandFloat();

                        fvar2 = sqrtf(SQUARE(this_pc->vel.x) + SQUARE(this_pc->vel.y) + SQUARE(this_pc->vel.z));
                        
                        fvar1 /= fvar2;

                        this_pc->vel.x *= fvar1;
                        this_pc->vel.y *= fvar1;
                        this_pc->vel.z *= fvar1;
                        break;
                        
                    case LBPARTICLE_OPCODE_MULVELAXIS:    
                        csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                        this_pc->vel.x *= fvar1;
                        csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                        this_pc->vel.y *= fvar1;
                        csr = lbParticleReadFloatBigEnd(csr, &fvar1);
                        this_pc->vel.z *= fvar1;
                        break;
                        
                        case LBPARTICLE_OPCODE_SETATTACHID:
                        svar1 = *csr++ - 1;
                        this_pc->flags |= LBPARTICLE_SET_ATTACH_ID(svar1);
                        break;
                        
                    case LBPARTICLE_OPCODE_SETPRIMBLEND:
                        csr  = lbParticleReadUShort(csr, &this_pc->primcolor_target_length);
                        this_pc->target_primcolor = this_pc->primcolor;
                            
                        if (command & 1)
                        {
                            this_pc->target_primcolor.r = *csr++;
                        }
                        if (command & 2)
                        {
                            this_pc->target_primcolor.g = *csr++;
                        }
                        if (command & 4)
                        {
                            this_pc->target_primcolor.b = *csr++;
                        }
                        if (command & 8)
                        {
                            this_pc->target_primcolor.a = *csr++;
                        }
                        if (this_pc->primcolor_target_length == 1)
                        {
                            this_pc->primcolor = this_pc->target_primcolor;
                            
                            this_pc->primcolor_target_length = 0;
                        }
                        break;
                        
                    case LBPARTICLE_OPCODE_SETENVBLEND:
                        csr = lbParticleReadUShort(csr, &this_pc->envcolor_target_length);
                        this_pc->target_envcolor = this_pc->envcolor;
                            
                        if (command & 1)
                        {
                            this_pc->target_envcolor.r = *csr++;
                        }
                        if (command & 2)
                        {
                            this_pc->target_envcolor.g = *csr++;
                        }
                        if (command & 4)
                        {
                            this_pc->target_envcolor.b = *csr++;
                        }
                        if (command & 8)
                        {
                            this_pc->target_envcolor.a = *csr++;
                        }
                        if (this_pc->envcolor_target_length == 1)
                        {
                            this_pc->envcolor = this_pc->target_envcolor;
                            this_pc->envcolor_target_length = 0;
                        }
                        break;
                        
                    case LBPARTICLE_OPCODE_SETLOOP:    
                        this_pc->loop_count = *csr++;
                        this_pc->loop_ptr = (u16) ((uintptr_t)csr - (uintptr_t)this_pc->bytecode);
                        break;
                        
                    case LBPARTICLE_OPCODE_LOOP:    
                        this_pc->loop_count--;
                            
                        if (this_pc->loop_count != 0)
                        {
                            csr = (u8*) (this_pc->bytecode + this_pc->loop_ptr);
                        }
                        break;
                        
                    case LBPARTICLE_OPCODE_SETRETURN:            
                        this_pc->return_ptr = (u16) ((uintptr_t)csr - (uintptr_t)this_pc->bytecode);
                        break;
                        
                    case LBPARTICLE_OPCODE_RETURN:
                        csr = (u8*) (this_pc->bytecode + this_pc->return_ptr);
                        break;
                        
                    case LBPARTICLE_OPCODE_DEAD:
                    case LBPARTICLE_OPCODE_END:
                        this_pc->lifetime = 1;
                        goto loop_break;
                    }
                }
                else
                {
                    bytecode_timer = command & 0x1F;
                    
                    if (command & 0x20)
                    {
                        bytecode_timer = *csr++ + (bytecode_timer << 8);
                    }
                    if ((command & 0xC0) && ((command & 0xC0) == 0x40))
                    {
                        // Advance 
                        this_pc->frame_id = *csr++;
                    }
                }
            }
            while (bytecode_timer == 0);
            
        loop_break:
            bytecode_csr = (u8*) ((uintptr_t)csr - (uintptr_t)this_pc->bytecode);
            
            this_pc->bytecode_csr = bytecode_csr;
            this_pc->bytecode_timer = bytecode_timer;
        }
    }
    if (this_pc->size_target_length)
    {
        this_pc->size += (this_pc->size_target - this_pc->size) / this_pc->size_target_length;

        this_pc->size_target_length--;
    }
    if (this_pc->primcolor_target_length) 
    {
        this_pc->primcolor.r =
        ((this_pc->primcolor.r << 16) + ((this_pc->target_primcolor.r - this_pc->primcolor.r) * (65536 / this_pc->primcolor_target_length))) >> 16;

        this_pc->primcolor.g =
        ((this_pc->primcolor.g << 16) + ((this_pc->target_primcolor.g - this_pc->primcolor.g) * (65536 / this_pc->primcolor_target_length))) >> 16;

        this_pc->primcolor.b =
        ((this_pc->primcolor.b << 16) + ((this_pc->target_primcolor.b - this_pc->primcolor.b) * (65536 / this_pc->primcolor_target_length))) >> 16;

        this_pc->primcolor.a =
        ((this_pc->primcolor.a << 16) + ((this_pc->target_primcolor.a - this_pc->primcolor.a) * (65536 / this_pc->primcolor_target_length))) >> 16;

        this_pc->primcolor_target_length--;
    }
    if (this_pc->envcolor_target_length) 
    {
        this_pc->envcolor.r =
        ((this_pc->envcolor.r << 16) + ((this_pc->target_envcolor.r - this_pc->envcolor.r) * (65536 / this_pc->envcolor_target_length))) >> 16;

        this_pc->envcolor.g =
        ((this_pc->envcolor.g << 16) + ((this_pc->target_envcolor.g - this_pc->envcolor.g) * (65536 / this_pc->envcolor_target_length))) >> 16;

        this_pc->envcolor.b =
        ((this_pc->envcolor.b << 16) + ((this_pc->target_envcolor.b - this_pc->envcolor.b) * (65536 / this_pc->envcolor_target_length))) >> 16;

        this_pc->envcolor.a =
        ((this_pc->envcolor.a << 16) + ((this_pc->target_envcolor.a - this_pc->envcolor.a) * (65536 / this_pc->envcolor_target_length))) >> 16;
            
        this_pc->envcolor_target_length--;        
    }
    this_pc->lifetime--;
    
    if (this_pc->lifetime == 0)
    {
        if (other_pc == NULL)
        {
            sLBParticleStructsAllocLinks[bank_id] = this_pc->next;
        }
        else other_pc->next = this_pc->next;
        
        next_pc = this_pc->next;
        
        if ((this_pc->gn != NULL) && (this_pc->flags & LBPARTICLE_FLAG_VORTEX) && (this_pc->gn->kind == nLBParticleKindVortex))
        {
            this_pc->gn->generator_vars.vortex.lifetime--;
        }
        if (this_pc->xf != NULL) 
        {
            this_pc->xf->users_num--;
            
            if (this_pc->xf->users_num == 0) 
            {
                lbParticleEjectTransform(this_pc->xf);

                if (other_pc == NULL) 
                {
                    if (next_pc != sLBParticleStructsAllocLinks[bank_id])
                    {
                        next_pc = sLBParticleStructsAllocLinks[bank_id];
                    }
                }
            }
        }
        this_pc->next = sLBParticleStructsAllocFree;
        sLBParticleStructsAllocFree = this_pc;
        gLBParticleStructsUsedNum--;

        return next_pc;
    }
    if (this_pc->flags & LBPARTICLE_FLAG_VORTEX)
    {
        gn = this_pc->gn;

        syGetSinCosUShort(sx1, cx1, this_pc->gravity, angle_id);
        syGetSinCosUShort(sx2, cx2, this_pc->friction, angle_id);
        
        sx1 *= (1.0F / 32768.0F);
        cx1 *= (1.0F / 32768.0F);
        sx2 *= (1.0F / 32768.0F);
        cx2 *= (1.0F / 32768.0F);

        this_pc->vel.z += gn->generator_vars.vortex.f;

        sp70 = ABSF(gn->unk_gn_0x38);
        
        syGetSinCosUShort(sx3, cx3, ABSF(gn->unk_gn_0x3C), angle_id_2);
        
        sp70 += this_pc->vel.z * (sx3 / cx3);
        sp70 *= this_pc->vel.y;
        
        this_pc->vel.x += gn->gravity;

        syGetSinCosUShort(sx4, cx4, this_pc->vel.x, angle_id);
        
        sp70 *= (1.0F / 32768.0F);

        /*
        f32 sp6C;     // f2
        f32 sx1, sx2; // sp5C, sp58
        f32 cx1, cx2; // f16=>sp54, f12=>sp50 
        f32 sx3, cx3; // f18, f0=>sp44
        */

        // Lots of trouble here...

        f0 = sp70 * cx4;
        f1 = sp70 * sx4;
        
        this_pc->pos.x = ((f0 * cx2) + (this_pc->vel.z * sx2)) + gn->pos.x;
        this_pc->pos.y = ((-f0 * sx1 * sx2) + (f1 * cx1)) + (this_pc->vel.z * sx1 * cx2) + gn->pos.y;
        this_pc->pos.z = ((-f0 * cx1 * sx2) - (f1 * sx1)) + (this_pc->vel.z * cx1 * cx2) + gn->pos.z;
    }
    else
    {
        if (this_pc->flags & LBPARTICLE_FLAG_GRAVITY)
        {
            this_pc->vel.y -= this_pc->gravity;
        }
        if (this_pc->flags & LBPARTICLE_FLAG_FRICTION)
        {
            this_pc->vel.x *= this_pc->friction;
            this_pc->vel.y *= this_pc->friction;
            this_pc->vel.z *= this_pc->friction;
        }
        this_pc->pos.x += this_pc->vel.x;
        this_pc->pos.y += this_pc->vel.y;
        this_pc->pos.z += this_pc->vel.z;
    }
    if (this_pc->flags & LBPARTICLE_FLAG_ATTACH)
    {
        svar1 = LBPARTICLE_GET_ATTACH_ID(this_pc->flags);

        if (sLBParticleAttachDObjs[svar1] != NULL)
        {
            sLBParticleAttachDObjs[svar1]->translate.vec.f.x = this_pc->pos.x;
            sLBParticleAttachDObjs[svar1]->translate.vec.f.y = this_pc->pos.y;
            sLBParticleAttachDObjs[svar1]->translate.vec.f.z = this_pc->pos.z;
        }
    }
    return this_pc->next;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/lb/lbparticle/lbParticleUpdateStruct.s")
#endif /* NON_MATCHING */

// 0x800D0C74
void lbParticleStructFuncRun(GObj *gobj)
{
	u32 flags = gobj->flags;
	s32 i;
	LBParticle *prev_pc;
	LBParticle *current_pc;
	LBParticle *next_pc;

	for (i = 0; i < ARRAY_COUNT(sLBParticleStructsAllocLinks); i++, flags >>= 1)
	{
		if (flags & 0x10000)
		{
			continue;
		}
		prev_pc = NULL;

		current_pc = sLBParticleStructsAllocLinks[i];

		while (current_pc != NULL)
		{
			next_pc = lbParticleUpdateStruct(current_pc, prev_pc, i);

			if (current_pc->next == next_pc)
			{
				prev_pc = current_pc;
				current_pc = next_pc;
			}
			else current_pc = next_pc;
		}
	}
}

#ifdef NON_MATCHING
/* 
 * NONMATCHING: xh, xl, yh, yl, pos_x, pos_y and pos_z float reg and/or order swaps
 * This is likely related to control flow, because xl and yl are supposed to go into saved regs,
 * but they are instead placed in fv0 and fv1 (f0 and f2 respectively). xh and yh are however
 * correctly placed into the registers they belong in. The empty brackets are also suspicious.
 */

// 0x800D0D34
void lbParticleDrawTextures(GObj *gobj)
{
    LBParticle *pc;
    void *prev_image, *prev_palette;
    s32 prev_ac, prev_alpha;
    s32 tlut;
    Mtx44f look_at_f;
    Mtx44f projection_f;
    u8 ac, alpha;
    s32 bank_id;
    void **p_palette;
    f32 pos_x, pos_y, pos_z;
    f32 tx, ty, tz;
    f32 pc0_magnitude;  // var_f14?
    f32 pc1_magnitude;  // var_f16?
    f32 tm;
    CObj *cobj;
    s32 s;              // sp210
    s32 t;              // sp20C
    f32 vscale0;        // sp208
    f32 vtrans0;        // sp204
    f32 vscale1;        // sp200
    f32 vtrans1;        // sp1FC
    f32 vscale2;        // sp1F8
    f32 vtrans2;        // sp1F4    
    s32 dsdx;           // sp1F0
    s32 dtdy;           // sp1EC
    s32 cms, cmt;
    s32 fmt, siz;
    s32 i;              // s2
    u32 j;              // sp1D4
    s32 width, height;
    f32 xh, yh;
    f32 xl, yl;
    void *image, *palette;
    u8 masks, maskt;
    f32 mx, my;

    cobj = CObjGetStruct(gGCCurrentCamera);

    for (i = 0; i < cobj->xobjs_num; i++)
    {
        switch (cobj->xobjs[i]->kind)
        {
            case nGCMatrixKindPerspFastF:
                syMatrixPerspFastF
                (
                    &projection_f,
                    NULL,
                    cobj->projection.persp.fovy,
                    cobj->projection.persp.aspect,
                    cobj->projection.persp.near,
                    cobj->projection.persp.far,
                    cobj->projection.persp.scale
                );
                break;
                
            case nGCMatrixKindPerspF:
                syMatrixPerspF
                (
                    &projection_f,
                    NULL,
                    cobj->projection.persp.fovy,
                    cobj->projection.persp.aspect,
                    cobj->projection.persp.near,
                    cobj->projection.persp.far,
                    cobj->projection.persp.scale
                );
                break;

			case nGCMatrixKindOrtho:
				syMatrixOrthoF
                (
                    &projection_f,
                    cobj->projection.ortho.l,
                    cobj->projection.ortho.r,
                    cobj->projection.ortho.b,
                    cobj->projection.ortho.t,
                    cobj->projection.ortho.n,
                    cobj->projection.ortho.f,
                    cobj->projection.ortho.scale
                );
                break;
                
            case 6:
            case 7:
            case 12:
            case 13:
                syMatrixLookAtF
                (
                    &look_at_f,
                    cobj->vec.eye.x,
                    cobj->vec.eye.y,
                    cobj->vec.eye.z,
                    cobj->vec.at.x,
                    cobj->vec.at.y,
                    cobj->vec.at.z,
                    cobj->vec.up.x,
                    cobj->vec.up.y,
                    cobj->vec.up.z
                );
                break;
                
            case 8:
            case 9:
            case 14:
            case 15:
                syMatrixModLookAtF
                (
                    &look_at_f,
                    cobj->vec.eye.x,
                    cobj->vec.eye.y,
                    cobj->vec.eye.z,
                    cobj->vec.at.x,
                    cobj->vec.at.y,
                    cobj->vec.at.z,
                    cobj->vec.up.x,
                    0.0F,
                    1.0F,
                    0.0F
                );
                break;
                
            case 10:
            case 11:
            case 16:
            case 17:
                syMatrixModLookAtF
                (
                    &look_at_f,
                    cobj->vec.eye.x,
                    cobj->vec.eye.y,
                    cobj->vec.eye.z,
                    cobj->vec.at.x,
                    cobj->vec.at.y,
                    cobj->vec.at.z,
                    cobj->vec.up.x,
                    0.0F,
                    0.0F,
                    1.0F
                );
                break;
                
            default:
                syMatrixPerspFastF
                (
                    &projection_f,
                    NULL,
                    cobj->projection.persp.fovy,
                    cobj->projection.persp.aspect,
                    cobj->projection.persp.near,
                    cobj->projection.persp.far,
                    cobj->projection.persp.scale
                );
                syMatrixLookAtF
                (
                    &look_at_f,
                    cobj->vec.eye.x,
                    cobj->vec.eye.y,
                    cobj->vec.eye.z,
                    cobj->vec.at.x,
                    cobj->vec.at.y,
                    cobj->vec.at.z,
                    cobj->vec.up.x,
                    cobj->vec.up.y,
                    cobj->vec.up.z
                );
                break;
        }
    }
    if (cobj->xobjs_num != 0)
    {
        guMtxCatF(look_at_f, projection_f, projection_f);
        
        vscale0 = cobj->viewport.vp.vscale[0];
        vscale1 = -cobj->viewport.vp.vscale[1];
        vscale2 = cobj->viewport.vp.vscale[2];
        
        vtrans0 = cobj->viewport.vp.vtrans[0];
        vtrans1 = cobj->viewport.vp.vtrans[1];
        vtrans2 = cobj->viewport.vp.vtrans[2];
    }
    else
    {
        vscale0 = cobj->viewport.vp.vscale[0];
        vscale1 = -cobj->viewport.vp.vscale[1];
        vscale2 = cobj->viewport.vp.vscale[2];
        
        vtrans0 = cobj->viewport.vp.vtrans[0];
        vtrans1 = cobj->viewport.vp.vtrans[1];
        vtrans2 = cobj->viewport.vp.vtrans[2];
        
        guMtxIdentF(projection_f);

        projection_f[0][0] = 1.0F / vscale0;
        projection_f[1][1] = 1.0F / vscale1;
        projection_f[2][2] = -1.0F / vscale2;

        projection_f[3][0] = -vtrans0 / vscale0;
        projection_f[3][1] = -vtrans1 / vscale1;
        projection_f[3][2] = vtrans2 / vscale2;
    }
    pc0_magnitude = sqrtf(SQUARE(projection_f[0][0]) + SQUARE(projection_f[1][0]) + SQUARE(projection_f[2][0]));
    pc1_magnitude = sqrtf(SQUARE(projection_f[0][1]) + SQUARE(projection_f[1][1]) + SQUARE(projection_f[2][1]));
    
    gDPPipeSync(gSYTaskmanDLHeads[0]++);
    gDPSetTexturePersp(gSYTaskmanDLHeads[0]++, G_TP_NONE);
    gDPSetDepthSource(gSYTaskmanDLHeads[0]++, G_ZS_PRIM);
    gDPSetColorDither(gSYTaskmanDLHeads[0]++, G_CD_MAGICSQ | dLBParticleColorDitherMode);
    gDPSetAlphaDither(gSYTaskmanDLHeads[0]++, G_AD_PATTERN | dLBParticleAlphaDitherMode);

    prev_image = NULL;
    prev_palette = NULL;

    prev_ac = -1;
    prev_alpha = -1;
    tlut = -1;
    
    dLBParticleCurrentTransformID++;
    
    for (j = 0; j < ARRAY_COUNT(sLBParticleStructsAllocLinks); j++)
    {
        if (gobj->camera_mask & (1 << j))
        {
            for (pc = sLBParticleStructsAllocLinks[j]; pc != NULL; pc = pc->next)
            {
                if (pc->size != 0.0F)
                {
                    pos_x = pc->pos.x;
                    pos_y = pc->pos.y;
                    pos_z = pc->pos.z;
                    
                    if (pc->xf != NULL)
                    {
                        if (pc->xf->transform_id != dLBParticleCurrentTransformID)
                        {
							if (pc->xf->transform_status != nLBTransformStatusFinished)
							{
                                syMatrixTraRotRpyRScaF
                                (
                                    &pc->xf->affine,
                                    pc->xf->translate.x,
                                    pc->xf->translate.y,
                                    pc->xf->translate.z,
                                    pc->xf->rotate.x,
                                    pc->xf->rotate.y,
                                    pc->xf->rotate.z,
                                    pc->xf->scale.x,
                                    pc->xf->scale.y,
                                    pc->xf->scale.z
                                );
                            }
							if (pc->xf->transform_status == nLBTransformStatusReady)
							{
								pc->xf->transform_status = nLBTransformStatusFinished;
							}
                            guMtxCatF(pc->xf->affine, projection_f, pc->xf->projection);
                            
                            pc->xf->pc0_magnitude = sqrtf
                            (
                                SQUARE(pc->xf->projection[0][0]) +
                                SQUARE(pc->xf->projection[1][0]) +
                                SQUARE(pc->xf->projection[2][0])
                            );
                            pc->xf->pc1_magnitude = sqrtf
                            (
                                SQUARE(pc->xf->projection[0][1]) +
                                SQUARE(pc->xf->projection[1][1]) +
                                SQUARE(pc->xf->projection[2][1])
                            );
                            pc->xf->transform_id = dLBParticleCurrentTransformID;
                        }
                        s = (pc->xf->affine[0][0] >= 0.0F) ? 0 : -1;
                        t = (pc->xf->affine[1][1] >= 0.0F) ? 0 : -1;
                        
                        mx = pc->xf->pc0_magnitude;
                        my = pc->xf->pc1_magnitude;
                        
                        tx = ((pc->xf->projection[0][0] * pos_x) + (pc->xf->projection[1][0] * pos_y) + (pc->xf->projection[2][0] * pos_z)) + pc->xf->projection[3][0];
                        ty = ((pc->xf->projection[0][1] * pos_x) + (pc->xf->projection[1][1] * pos_y) + (pc->xf->projection[2][1] * pos_z)) + pc->xf->projection[3][1];
                        tz = ((pc->xf->projection[0][2] * pos_x) + (pc->xf->projection[1][2] * pos_y) + (pc->xf->projection[2][2] * pos_z)) + pc->xf->projection[3][2];
                        tm = ((pc->xf->projection[0][3] * pos_x) + (pc->xf->projection[1][3] * pos_y) + (pc->xf->projection[2][3] * pos_z)) + pc->xf->projection[3][3];
                    }
                    else
                    {
                        s = t = 0;
                        
                        mx = pc0_magnitude;
                        my = pc1_magnitude;
                        
                        tx = ((projection_f[0][0] * pos_x) + (projection_f[1][0] * pos_y) + (projection_f[2][0] * pos_z)) + projection_f[3][0];
                        ty = ((projection_f[0][1] * pos_x) + (projection_f[1][1] * pos_y) + (projection_f[2][1] * pos_z)) + projection_f[3][1];
                        tz = ((projection_f[0][2] * pos_x) + (projection_f[1][2] * pos_y) + (projection_f[2][2] * pos_z)) + projection_f[3][2];
                        tm = ((projection_f[0][3] * pos_x) + (projection_f[1][3] * pos_y) + (projection_f[2][3] * pos_z)) + projection_f[3][3];
                    }
                    if (tm == 0.0F)
                    {
                        // uh huh
                    }
                    else
                    {
                        tm = 1.0F / tm;
                
                        tx *= tm;
                        ty *= tm;
                        tz *= tm;
                        
                        if
                        (
                            (tx < -1.0F) || (tx > 1.0F) ||
                            (ty < -1.0F) || (ty > 1.0F) ||
                            (tz < +0.0F) || (tz > 1.0F)
                        )
                        {
                            // uh huh
                        }
                        else
                        {
                            tm *= pc->size;

                            mx = tm * mx;
                            xh = mx + tx;
                            
                            my = tm * my;
                            yh = my + ty;

                            tx = tx * vscale0 + vtrans0;
                            xh = xh * vscale0 + vtrans0;
                            
                            if (tx < xh)
                            {
                                xh = xh;
                                xl = tx - (xh - tx);
                            }
                            else
                            {
                                xl = xh;
                                xh = tx - (xh - tx);
                            }
                            ty = ty * vscale1 + vtrans1;
                            yh = yh * vscale1 + vtrans1;
                            
                            if (ty < yh)
                            {
                                yh = yh;
                                yl = ty - (yh - ty);
                            }
                            else
                            {
                                yl = yh;
                                yh = ty - (yh - ty);
                            }
                            tz = tz * vscale2 + vtrans2;
                            
                            bank_id = pc->bank_id & 7;

                            fmt = sLBParticleTextureBanks[bank_id][pc->texture_id]->fmt;
                            siz = sLBParticleTextureBanks[bank_id][pc->texture_id]->siz;
                            
                            width = sLBParticleTextureBanks[bank_id][pc->texture_id]->width;
                            height = sLBParticleTextureBanks[bank_id][pc->texture_id]->height;
                            
                            image = sLBParticleTextureBanks[bank_id][pc->texture_id]->data[pc->frame_id];
                            
                            if (fmt == G_IM_FMT_CI)
                            {
                                p_palette = &sLBParticleTextureBanks[bank_id][pc->texture_id]->data[sLBParticleTextureBanks[bank_id][pc->texture_id]->count];
                                
                                palette = (!(pc->flags & LBPARTICLE_FLAG_SHAREDPAL)) ? p_palette[pc->frame_id] : p_palette[0];
                            }
                            dsdx = (width * 4096.0F) / (xh - xl);
                            dtdy = (height * 4096.0F) / (yh - yl);
                
                            if (s < 0)
                            {
                                s = width * 32;
                                dsdx = -dsdx;
                            }
                            if (t < 0)
                            {
                                t = height * 32;
                                dtdy = -dtdy;
                            }
							if (pc->flags & LBPARTICLE_FLAG_MASKS)
							{
                                dsdx *= 2;
                                cms = 1;
                                
                                switch (width)
                                {
                                case 2:
                                    masks = 1;
                                    break;
                                    
                                case 4:
                                    masks = 2;
                                    break;
                                    
                                case 8:
                                    masks = 3;
                                    break;
                                    
                                case 16:
                                    masks = 4;
                                    break;
                                    
                                case 32:
                                    masks = 5;
                                    break;
                                    
                                case 64:
                                    masks = 6;
                                    break;
                                    
                                case 128:
                                    masks = 7;
                                    break;
                                    
                                case 256:
                                    masks = 8;
                                    break;

                                default:
                                    masks = 0;
                                    break;
                                }
                            }
                            else
                            {
                                cms = 2;
                                masks = 0;
                            }
                            if (pc->flags & LBPARTICLE_FLAG_MASKT)
                            {
                                dtdy *= 2;
                                cmt = 1;
                                
                                switch (height)
                                {
                                case 2:
                                    maskt = 1;
                                    break;
                                    
                                case 4:
                                    maskt = 2;
                                    break;
                                    
                                case 8:
                                    maskt = 3;
                                    break;
                                    
                                case 16:
                                    maskt = 4;
                                    break;
                                    
                                case 32:
                                    maskt = 5;
                                    break;
                                    
                                case 64:
                                    maskt = 6;
                                    break;
                                    
                                case 128:
                                    maskt = 7;
                                    break;
                                    
                                case 256:
                                    maskt = 8;
                                    break;
                                    
                                default: 
                                    maskt = 0;
                                    break;
                                }
                            }
                            else
                            {
                                cmt = 2;
                                maskt = 0;
                            }
                            if (fmt == G_IM_FMT_CI)
                            {
                                if (palette != prev_palette)
                                {
                                    gDPLoadTLUT_pal256(gSYTaskmanDLHeads[0]++, palette);
                                    
                                    prev_palette = palette;
                                }
                                if (tlut != 1)
                                {
                                    gDPSetTextureLUT(gSYTaskmanDLHeads[0]++, G_TT_RGBA16);
                                    
                                    tlut = 1;
                                }
                            }
                            else if (tlut != 0)
                            {
                                gDPSetTextureLUT(gSYTaskmanDLHeads[0]++, G_TT_NONE);
                                
                                tlut = 0;
                            }
                            if (image != prev_image)
                            {
                                switch (siz)
                                {
                                case G_IM_SIZ_4b:
                                    gDPLoadTextureBlock_4b
                                    (
                                        gSYTaskmanDLHeads[0]++,
                                        image,
                                        fmt,
                                        width,
                                        height,
                                        0,
                                        cms,
                                        cmt,
                                        masks,
                                        maskt,
                                        G_TX_NOLOD,
                                        G_TX_NOLOD
                                    );
                                    if ((width * height) >= 4096)
                                    {
                                        prev_palette = NULL;
                                    }
                                    break;
                                    
                                case G_IM_SIZ_8b:
                                    gDPLoadTextureBlock
                                    (
                                        gSYTaskmanDLHeads[0]++,
                                        image,
                                        fmt,
                                        G_IM_SIZ_8b,
                                        width,
                                        height,
                                        0,
                                        cms,
                                        cmt,
                                        masks,
                                        maskt,
                                        G_TX_NOLOD,
                                        G_TX_NOLOD
                                    );
                                    if ((width * height) >= 2048)
                                    {
                                        prev_palette = NULL;
                                    }
                                    break;
                                    
                                case G_IM_SIZ_16b:
                                    gDPLoadTextureBlock
                                    (
                                        gSYTaskmanDLHeads[0]++,
                                        image,
                                        fmt,
                                        G_IM_SIZ_16b,
                                        width,
                                        height,
                                        0,
                                        cms,
                                        cmt,
                                        masks,
                                        maskt,
                                        G_TX_NOLOD,
                                        G_TX_NOLOD
                                    );
                                    if ((width * height) >= 1024)
                                    {
                                        prev_palette = NULL;
                                    }
                                    break;
                                    
                                case G_IM_SIZ_32b:
                                    gDPLoadTextureBlock
                                    (
                                        gSYTaskmanDLHeads[0]++,
                                        image,
                                        fmt,
                                        G_IM_SIZ_32b,
                                        width,
                                        height,
                                        0,
                                        cms,
                                        cmt,
                                        masks,
                                        maskt,
                                        G_TX_NOLOD,
                                        G_TX_NOLOD
                                    );
                                    if ((width * height) >= 512)
                                    {
                                        prev_palette = NULL;
                                    }
                                    break;
                                    
                                default:
                                    break;
                                }
                                prev_image = image;
                            }
                            gDPSetPrimColor(gSYTaskmanDLHeads[0]++, 0, 0, pc->primcolor.r, pc->primcolor.g, pc->primcolor.b, pc->primcolor.a);
                
                            if (pc->flags & LBPARTICLE_FLAG_ENVCOLOR)
                            {
                                gDPSetEnvColor(gSYTaskmanDLHeads[0]++, pc->envcolor.r, pc->envcolor.g, pc->envcolor.b, pc->envcolor.a);
                                
                                gDPSetCombineLERP
                                (
                                    gSYTaskmanDLHeads[0]++,
                                    PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT,
                                    PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT
                                );
                            }
                            else if (pc->flags & LBPARTICLE_FLAG_NOISE)
                            {
                                gDPSetCombineLERP
                                (
                                    gSYTaskmanDLHeads[0]++,
                                    NOISE, 0, TEXEL0, 0, TEXEL0, 0, PRIMITIVE, 0,
                                    NOISE, 0, TEXEL0, 0, TEXEL0, 0, PRIMITIVE, 0
                                );
                            }
                            else gDPSetCombineMode(gSYTaskmanDLHeads[0]++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
                            
                            if (pc->flags & LBPARTICLE_FLAG_DITHER)
                            {
                                ac = G_AC_DITHER;
                            }
                            else
                            {
                                ac = G_AC_THRESHOLD;

								if (pc->flags & LBPARTICLE_FLAG_ALPHABLEND)
								{
                                    alpha = pc->envcolor.a;
                                }
                                else alpha = 0x08;
                                
                                if (prev_alpha != alpha)
                                {
                                    gDPSetBlendColor(gSYTaskmanDLHeads[0]++, 0x00, 0x00, 0x00, alpha);
                
                                    prev_alpha = alpha;
                                }
                            }
                            if (prev_ac != ac)
                            {
                                gDPSetAlphaCompare(gSYTaskmanDLHeads[0]++, ac);
                                
                                prev_ac = ac;
                            }
                            gDPSetPrimDepth(gSYTaskmanDLHeads[0]++, (s32) (tz * 32.0F), 0);
                            gSPScisTextureRectangle(gSYTaskmanDLHeads[0]++, xl, yl, xh, yh, G_TX_RENDERTILE, s, t, dsdx, dtdy);
                        }
                    }
                }
            }
        }
    }
    if (tlut != 0)
    {
        gDPSetTextureLUT(gSYTaskmanDLHeads[0]++, G_TT_NONE);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/lb/lbparticle/lbParticleDrawTextures.s")
#endif /* NON_MATCHING */

// 0x800D2720
void lbParticleAddAttachDObj(s32 bank_id, DObj *dobj)
{
	if ((bank_id >= 1) && (bank_id <= ARRAY_COUNT(sLBParticleAttachDObjs)))
	{
		sLBParticleAttachDObjs[bank_id - 1] = dobj;
	}
}

// 0x800D2744
void lbParticleSetDitherModes(s32 colordither_mode, s32 alphadither_mode)
{
	dLBParticleColorDitherMode = colordither_mode;
	dLBParticleAlphaDitherMode = alphadither_mode;
}

// 0x800D2758
GObj* lbParticleAllocGenerators(s32 num)
{
	s32 i;

	sLBParticleGeneratorsAllocFree = sLBParticleGeneratorsQueued = NULL;

	for (i = num - 1; i >= 0; i--)
	{
		LBGenerator *gn = syTaskmanMalloc(sizeof(*gn), 0x4);

		if (gn == NULL)
		{
			return NULL;
		}
		gn->next = sLBParticleGeneratorsAllocFree;

		sLBParticleGeneratorsAllocFree = gn;
	}
	gLBParticleGeneratorsUsedNum = 0;
	D_ovl0_800D6450 = 0;

	return gcMakeGObjSPAfter(~0x6, lbParticleGeneratorFuncRun, 0, GOBJ_PRIORITY_DEFAULT);
}

// 0x800D27F8
void lbParticleGetPosVelDObj(Vec3f *pos, Vec3f *vel, DObj *dobj)
{
	Mtx44f dst;
	Mtx44f tmp;
	f32 x, y, z;

	guMtxIdentF(dst);

	do
	{
		if ((dobj->scale.vec.f.x != 1.0F) || (dobj->scale.vec.f.y != 1.0F) || (dobj->scale.vec.f.z != 1.0F))
		{
			syMatrixScaF(&tmp, dobj->scale.vec.f.x, dobj->scale.vec.f.y, dobj->scale.vec.f.z);
			guMtxCatF(dst, tmp, dst);
		}
		if ((dobj->rotate.vec.f.x != 0.0F) || (dobj->rotate.vec.f.y != 0.0F) || (dobj->rotate.vec.f.z != 0.0F))
		{
			syMatrixRotRpyRF(&tmp, dobj->rotate.vec.f.x, dobj->rotate.vec.f.y, dobj->rotate.vec.f.z);
			guMtxCatF(dst, tmp, dst);
		}
		if ((dobj->translate.vec.f.x != 0.0F) || (dobj->translate.vec.f.y != 0.0F) || (dobj->translate.vec.f.z != 0.0F))
		{
			syMatrixTraF(&tmp, dobj->translate.vec.f.x, dobj->translate.vec.f.y, dobj->translate.vec.f.z);
			guMtxCatF(dst, tmp, dst);
		}
		if (dobj->vec != NULL)
		{
			GCTranslate *translate = NULL;
			GCRotate *rotate = NULL;
			GCScale *scale = NULL;
			uintptr_t csr = (uintptr_t)dobj->vec->data;
			s32 i;

			for (i = 0; i < ARRAY_COUNT(dobj->vec->kinds); i++)
			{
				switch (dobj->vec->kinds[i])
				{
				case nGCDrawVectorKindNone:
                    break;

				case nGCDrawVectorKindTranslate:
					translate = (GCTranslate*)csr;
					csr += sizeof(*translate);
					break;

				case nGCDrawVectorKindRotate:
					rotate = (GCRotate*)csr;
					csr += sizeof(*rotate);
					break;

				case nGCDrawVectorKindScale:
					scale = (GCScale*)csr;
					csr += sizeof(*scale);
					break;
				}
			}
			if (scale != NULL)
			{
				if ((scale->vec.f.x != 1.0F) || (scale->vec.f.y != 1.0F) || (scale->vec.f.z != 1.0F))
				{
					syMatrixScaF(&tmp, scale->vec.f.x, scale->vec.f.y, scale->vec.f.z);
					guMtxCatF(dst, tmp, dst);
				}
			}
			if (rotate != NULL)
			{
				if ((rotate->vec.f.x != 0.0F) || (rotate->vec.f.y != 0.0F) || (rotate->vec.f.z != 0.0F))
				{
					syMatrixRotRpyRF(&tmp, rotate->vec.f.x, rotate->vec.f.y, rotate->vec.f.z);
					guMtxCatF(dst, tmp, dst);
				}
			}
			if (translate != NULL)
			{
				if ((translate->vec.f.x != 0.0F) || (translate->vec.f.y != 0.0F) || (translate->vec.f.z != 0.0F))
				{
					syMatrixTraF(&tmp, translate->vec.f.x, translate->vec.f.y, translate->vec.f.z);
					guMtxCatF(dst, tmp, dst);
				}
			}
		}
		dobj = dobj->parent;
	}
    while (dobj != DOBJ_PARENT_NULL);

	pos->x = dst[3][0];
	pos->y = dst[3][1];
	pos->z = dst[3][2];

	x = vel->x, y = vel->y, z = vel->z;

	guNormalize(&dst[0][0], &dst[1][0], &dst[2][0]);
	guNormalize(&dst[0][1], &dst[1][1], &dst[2][1]);
	guNormalize(&dst[0][2], &dst[1][2], &dst[2][2]);

	vel->x = (dst[0][0] * x) + (dst[1][0] * y) + (dst[2][0] * z);
	vel->y = (dst[0][1] * x) + (dst[1][1] * y) + (dst[2][1] * z);
	vel->z = (dst[0][2] * x) + (dst[1][2] * y) + (dst[2][2] * z);
}

#ifdef NON_MATCHING
/* 
 * NONMATCHING: move $a0, $s0 and move $a1, $s1 order swap around sLBParticleGeneratorFuncDefault
 */

// 0x800D2C4C
void lbParticleGeneratorFuncRun(GObj *gobj)
{
    LBGenerator *gn, *next_gn;
    f32 pos_random;
    Vec3f pos;
    Vec3f vel;
    f32 zero;
    f32 temp_vel_x;
    f32 temp_vel_y;
    f32 temp_vel_z;
    f32 magnitude;
    f32 angle1;
    f32 angle2;
    f32 sin_angle1;
    f32 cos_angle1;
    f32 sin_angle2;
    f32 cos_angle2;
    f32 spEC;
    f32 vel_x;
    f32 vel_y;
    f32 vel_z;
    f32 pv0;
    s32 unused;
    f32 temp_f26;
    f32 pos_x;
    f32 pos_y;
    f32 pos_z;
    f32 pm1;
    f32 var_f20;
    f32 pv1;
    f32 spB8;
    f32 vmag;
    
    gn = sLBParticleGeneratorsQueued;
    sLBParticleGeneratorsLastProcessed = NULL;
    
    while (gn != NULL)
    {
        if (gobj->flags & (1 << ((gn->bank_id >> 3) + 0x10)))
        {
            sLBParticleGeneratorsLastProcessed = gn;

            gn = gn->next;

            continue;
        }
        if (gn->flags & LBPARTICLE_FLAG_PAUSE)
        {
            sLBParticleGeneratorsLastProcessed = gn;

            gn = gn->next;

            continue;
        }
        if (gn->update_rate < 0.0F)
        {
            gn->frame -= gn->update_rate;
        }
        else gn->frame += (syUtilsRandFloat() * gn->update_rate);

        if (gn->frame >= 1.0F)
        {
            vel.x = gn->vel.x;
            vel.y = gn->vel.y;
            vel.z = gn->vel.z;

            if (gn->dobj != NULL)
            {
                lbParticleGetPosVelDObj(&pos, &vel, gn->dobj);
                    
                gn->pos.x = pos.x;
                gn->pos.y = pos.y;
                gn->pos.z = pos.z;
            }
            switch (gn->kind)
            {
            case 0:
            case 3:
            case 4:
                pv0 = gn->generator_vars.rotate.base + (syUtilsRandFloat() * (gn->generator_vars.rotate.target - gn->generator_vars.rotate.base));
                spB8 = (gn->generator_vars.rotate.target - gn->generator_vars.rotate.base) / (s32) gn->frame;
                break;

            default:
                pv0 = F_CST_DTOR32(360.0F) * syUtilsRandFloat();
                spB8 = F_CST_DTOR32(360.0F) / (s32) gn->frame;
                break;
            }
        }
        while (gn->frame >= 1.0F)
        {
            switch (gn->kind)
            {
            case 0:
            case 3:
            case 4:
                vel_x = vel.x;
                vel_y = vel.y;
                vel_z = vel.z;
                        
                angle1 = syUtilsArcTan2(vel_y, vel_z);
                        
                sin_angle1 = __sinf(angle1); // spFC
                cos_angle1 = __cosf(angle1); // spF8

                angle2 = syUtilsArcTan2(vel_x, (vel_y * sin_angle1) + (vel_z * cos_angle1));
                        
                sin_angle2 = __sinf(angle2); // spF4
                cos_angle2 = __cosf(angle2); // spF0
                // spFC = sin_angle1;
                magnitude = sqrtf(SQUARE(vel_x) + SQUARE(vel_y) + SQUARE(vel_z)); // sp108
                        
                if (gn->unk_gn_0x38 < 0.0F)
                {
                    vmag = pv1 = 1.0F;
                    var_f20 = -gn->unk_gn_0x38;
                }
                else
                {
                    pv1 = vmag = syUtilsRandFloat();

                    if (gn->kind != 0)
                    {
                        vmag = pv1 = sqrtf(pv1);
                    }
                    var_f20 = gn->unk_gn_0x38 * pv1;
                }
                if (gn->unk_gn_0x3C < 0.0F)
                {
                    pv0 += spB8;
                    pv1 = -gn->unk_gn_0x3C;
                }
                else
                {
                    pv0 = gn->generator_vars.rotate.base + (syUtilsRandFloat() * (gn->generator_vars.rotate.target - gn->generator_vars.rotate.base));
                    pv1 *= gn->unk_gn_0x3C;
                }
                zero = 0.0F;
                        
                spEC = __cosf(pv0) * var_f20;
                temp_f26 = __sinf(pv0) * var_f20;
                pm1 = __sinf(pv1) * magnitude;

                vel_x = __cosf(pv0) * pm1;
                vel_y = __sinf(pv0) * pm1;
                vel_z = __cosf(pv1) * magnitude;

                pos_x = (spEC * cos_angle2) + zero + gn->pos.x;
                pos_y = (-spEC * sin_angle1 * sin_angle2) + (temp_f26 * cos_angle1) + zero + gn->pos.y;
                pos_z = (-spEC * cos_angle1 * sin_angle2) - (temp_f26 * sin_angle1) + zero + gn->pos.z;

                temp_vel_x = (vel_x * cos_angle2) + (vel_z * sin_angle2);
                temp_vel_y = (-vel_x * sin_angle1 * sin_angle2) + (vel_y * cos_angle1) + (vel_z * sin_angle1 * cos_angle2);
                temp_vel_z = (-vel_x * cos_angle1 * sin_angle2) - (vel_y * sin_angle1) + (vel_z * cos_angle1 * cos_angle2);

                if (gn->kind == 3)
                {
                    temp_vel_x *= vmag;
                    temp_vel_y *= vmag;
                    temp_vel_z *= vmag;
                }
                lbParticleMakeParam
                (
                    gn->bank_id,
                    gn->flags,
                    gn->texture_id,
                    gn->bytecode,
                    gn->particle_lifetime,
                    pos_x,
                    pos_y,
                    pos_z,
                    temp_vel_x,
                    temp_vel_y,
                    temp_vel_z,
                    gn->size,
                    gn->gravity,
                    gn->friction,
                    0,
                    gn
                );
                break;
                        
            case 1:
                vel_x = vel.x;
                vel_y = vel.y;
                vel_z = vel.z;
                        
                pos_random = syUtilsRandFloat();

                pos_x = gn->pos.x + (pos_random * (gn->generator_vars.move.x - gn->pos.x));
                pos_y = gn->pos.y + (pos_random * (gn->generator_vars.move.y - gn->pos.y));
                pos_z = gn->pos.z + (pos_random * (gn->generator_vars.move.z - gn->pos.z));
                        
                lbParticleMakeParam
                (
                    gn->bank_id,
                    gn->flags,
                    gn->texture_id,
                    gn->bytecode,
                    gn->particle_lifetime,
                    pos_x,
                    pos_y,
                    pos_z,
                    vel.x,
                    vel.y,
                    vel.z,
                    gn->size,
                    gn->gravity,
                    gn->friction,
                    0,
                    gn
                );
                break;
                        
            case 2:
                vel_x = vel.x;
                vel_y = vel.y;
                vel_z = vel.z;
                        
                angle1 = syUtilsArcTan2(vel_y, vel_z);
                sin_angle1 = __sinf(angle1);
                cos_angle1 = __cosf(angle1);
                // spF8 = cos_angle1;
                angle2 = syUtilsArcTan2(vel_x, (vel_y * sin_angle1) + (vel_z * cos_angle1));
                        
                magnitude = sqrtf(SQUARE(vel_x) + SQUARE(vel_y) + SQUARE(vel_z));
                        
                pv1 = (gn->unk_gn_0x38 < 0.0F) ? 1.0F : syUtilsRandFloat();
                pv0 = (gn->unk_gn_0x3C < 0.0F) ? pv0 + spB8 : syUtilsRandFloat() * F_CST_DTOR32(360.0F);

                gn->generator_vars.vortex.f = magnitude;

                if
                (
                    lbParticleMakeParam
                    (
                        gn->bank_id,
                        gn->flags | LBPARTICLE_FLAG_VORTEX,
                        gn->texture_id,
                        gn->bytecode,
                        gn->particle_lifetime,
                        0,
                        0,
                        0,
                        pv0,
                        pv1,
                        0,
                        gn->size,
                        angle1,
                        angle2,
                        0,
                        gn
                    ) != NULL
                )
                {
                    gn->generator_vars.vortex.lifetime++;
                }
                break;
                        
            default:
                // OK seriously this order swap is ridiculous
                if (sLBParticleGeneratorFuncDefault != NULL)
                {
                    sLBParticleGeneratorFuncDefault(gn, &vel);
                }
                break;
            }
            gn->frame -= 1.0F;
        }
        if (gn->generator_lifetime != 0)
        {
            gn->generator_lifetime--;

            if (gn->generator_vars.rotate.target); // bruh
    
            if (gn->generator_lifetime == 0)
            {
                if ((gn->kind == nLBParticleKindVortex) && (gn->generator_vars.vortex.lifetime != 0))
                {
                    gn->update_rate = 0.0F;
                    gn->generator_lifetime = 1;
                }
                else
                {
                    if (sLBParticleGeneratorsLastProcessed == NULL)
                    {
                        sLBParticleGeneratorsQueued = gn->next;
                    }
                    else sLBParticleGeneratorsLastProcessed->next = gn->next;
                
                    next_gn = gn->next;
                
                    if (gn->xf != NULL)
                    {
                        gn->xf->users_num--;
                
                        if (gn->xf->users_num == 0)
                        {
                            lbParticleEjectTransform(gn->xf);
                        }
                    }
                    gn->next = sLBParticleGeneratorsAllocFree;
                    sLBParticleGeneratorsAllocFree = gn;
                
                    gn = next_gn;
                
                    gLBParticleGeneratorsUsedNum--;

                    continue;
                }
            }
        }
        sLBParticleGeneratorsLastProcessed = gn;

        gn = gn->next;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/lb/lbparticle/lbParticleGeneratorFuncRun.s")
#endif /* NON_MATCHING */

// 0x800D353C
LBGenerator* lbParticleGetGenerator(void)
{
    LBGenerator *gn = sLBParticleGeneratorsAllocFree;
    
    if (sLBParticleGeneratorsAllocFree == NULL)
    {
        return NULL;
    }
    gLBParticleGeneratorsUsedNum++;
    
    if (D_ovl0_800D6450 < gLBParticleGeneratorsUsedNum)
    {
        D_ovl0_800D6450 = gLBParticleGeneratorsUsedNum;
    }
    sLBParticleGeneratorsAllocFree = gn->next;
    gn->next = sLBParticleGeneratorsQueued;
    sLBParticleGeneratorsQueued = gn;
    
    if (sLBParticleGeneratorsLastProcessed == NULL)
    {
        sLBParticleGeneratorsLastProcessed = gn;
    }
    gn->generator_id = ++dLBParticleCurrentGeneratorID;
    gn->xf = NULL;
    
    return gn;
}

// 0x800D35DC
LBGenerator* lbParticleMakeGenerator(s32 bank_id, s32 script_id)
{
    LBGenerator *gn;
    s32 id = bank_id & 7;  
    s32 unused;
    
    if (id >= LBPARTICLE_BANKS_NUM_MAX)
    {
        return NULL;
    }
    if (script_id >= sLBParticleScriptBanksNum[id])
    {
        return NULL;
    }
    gn = lbParticleGetGenerator();

    if (gn != NULL)
    {   
        gn->kind = sLBParticleScriptBanks[id][script_id]->kind;
        gn->bank_id = bank_id;
        gn->flags = sLBParticleScriptBanks[id][script_id]->flags;
        gn->texture_id = sLBParticleScriptBanks[id][script_id]->texture_id;
        gn->particle_lifetime = sLBParticleScriptBanks[id][script_id]->particle_lifetime;
        gn->generator_lifetime = sLBParticleScriptBanks[id][script_id]->generator_lifetime;
        
        gn->pos.x = 0.0F;
        gn->pos.y = 0.0F;
        gn->pos.z = 0.0F;
        
        gn->vel.x = sLBParticleScriptBanks[id][script_id]->vel.x;
        gn->vel.y = sLBParticleScriptBanks[id][script_id]->vel.y;
        gn->vel.z = sLBParticleScriptBanks[id][script_id]->vel.z;
        
        gn->gravity = sLBParticleScriptBanks[id][script_id]->gravity;
        gn->friction = sLBParticleScriptBanks[id][script_id]->friction;
        gn->size = sLBParticleScriptBanks[id][script_id]->size;
        
        gn->bytecode = sLBParticleScriptBanks[id][script_id]->bytecode;
        
        gn->unk_gn_0x38 = sLBParticleScriptBanks[id][script_id]->unk_script_0x20;
        gn->unk_gn_0x3C = sLBParticleScriptBanks[id][script_id]->unk_script_0x24;
        gn->update_rate = sLBParticleScriptBanks[id][script_id]->update_rate;
        
        gn->frame = 0.0F;
        
        if (sLBParticleTextureBanks[id][sLBParticleScriptBanks[id][script_id]->texture_id]->flags != 0)
        {
            gn->flags |= LBPARTICLE_FLAG_SHAREDPAL;
        }
        gn->dobj = NULL;
        
        switch (gn->kind)
        {
        case 0:
        case 3:
        case 4:
            gn->generator_vars.rotate.base = 0;
            gn->generator_vars.rotate.target = F_CST_DTOR32(360.0F);
            break;
            
        case 1:
            gn->generator_vars.move.x = gn->pos.x + gn->vel.x;
            gn->generator_vars.move.y = gn->pos.y + gn->vel.y;
            gn->generator_vars.move.z = gn->pos.z + gn->vel.z;
            break;
            
        case 2:
            gn->generator_vars.vortex.lifetime = 0;
            break;
            
        default:
            if (sLBParticleGeneratorFuncSetup != NULL)
            {
                sLBParticleGeneratorFuncSetup(gn);
            }
            break;
        }
    }
    return gn;
}

// 0x800D3884
void lbParticleEjectGenerator(LBGenerator *this_gn)
{
    LBGenerator *prev_gn = NULL, *current_gn = sLBParticleGeneratorsQueued;
    
    while (current_gn != NULL)
    {
        if (current_gn == this_gn)
        {
            if ((this_gn->kind == nLBParticleKindVortex) && (this_gn->generator_vars.vortex.lifetime != 0))
            {
                this_gn->update_rate = 0.0F;
                this_gn->generator_lifetime = 1;
                
                break;
            }
            if (prev_gn == NULL)
            {
                sLBParticleGeneratorsQueued = current_gn->next;
            }
            else prev_gn->next = current_gn->next;
            
            if (this_gn->xf != NULL)
            {
                this_gn->xf->users_num--;
                
                if (this_gn->xf->users_num == 0)
                {
                    lbParticleEjectTransform(this_gn->xf);
                }
            }
            current_gn->next = sLBParticleGeneratorsAllocFree;
            sLBParticleGeneratorsAllocFree = current_gn;
            gLBParticleGeneratorsUsedNum--;
            
            break;
        }
        prev_gn = current_gn;
        current_gn = current_gn->next;
    }
}

// 0x800D3978
void lbParticleEjectGeneratorAll(void)
{
	LBGenerator *current_gn = sLBParticleGeneratorsQueued;

	while (current_gn != NULL)
	{
		LBGenerator *next_gn = current_gn->next;

		lbParticleEjectGenerator(current_gn);

		current_gn = next_gn;
	}
}

// 0x800D39C0
void lbParticleSetGeneratorFuncs(void (*func_setup)(LBGenerator*), void (*func_default)(LBGenerator*, Vec3f*))
{
	sLBParticleGeneratorFuncSetup = func_setup;
	sLBParticleGeneratorFuncDefault = func_default;
}

// 0x800D39D4
void lbParticleEjectStructID(u16 generator_id, s32 link_id)
{
    LBParticle *next_pc;
    LBParticle *current_pc;
    LBParticle *prev_pc;

    LBGenerator *next_gn;
    LBGenerator *current_gn;
    LBGenerator *prev_gn;

    prev_pc = NULL;
    current_pc = sLBParticleStructsAllocLinks[link_id];
    
    while (current_pc != NULL)
    {
        next_pc = current_pc->next;
            
        if (current_pc->generator_id == generator_id)
        {
            if (prev_pc == NULL)
            {
                sLBParticleStructsAllocLinks[link_id] = current_pc->next;
            }
            else prev_pc->next = current_pc->next;
                
            if ((current_pc->gn != NULL) && (current_pc->flags & LBPARTICLE_FLAG_VORTEX) && (current_pc->gn->kind == nLBParticleKindVortex))
            {
                current_pc->gn->generator_vars.vortex.lifetime--;
            }
            if (current_pc->xf != NULL)
            {
                current_pc->xf->users_num--;
                    
                if (current_pc->xf->users_num == 0)
                {
                    lbParticleEjectTransform(current_pc->xf);
                }
            }
            current_pc->next = sLBParticleStructsAllocFree;
            sLBParticleStructsAllocFree = current_pc;
            gLBParticleStructsUsedNum--;
        }
        else prev_pc = current_pc;
            
        current_pc = next_pc;
    }
    prev_gn = NULL;
    current_gn = sLBParticleGeneratorsQueued;
    
    while (current_gn != NULL)
    {
        next_gn = current_gn->next;
        
        if (current_gn->generator_id == generator_id)
        {
            if ((current_gn->kind == nLBParticleKindVortex) && (current_gn->generator_vars.vortex.lifetime != 0))
            {
                current_gn->update_rate = 0.0F;
                current_gn->generator_lifetime = 1;
                
                prev_gn = current_gn;
            }
            else
            {
                if (prev_gn == NULL)
                {
                    sLBParticleGeneratorsQueued = current_gn->next;
                }
                else prev_gn->next = current_gn->next;
            
                if (current_gn->xf != NULL)
                {
                    current_gn->xf->users_num--;
                
                    if (current_gn->xf->users_num == 0)
                    {
                        lbParticleEjectTransform(current_gn->xf);
                    }
                }
                current_gn->next = sLBParticleGeneratorsAllocFree;
                sLBParticleGeneratorsAllocFree = current_gn;
                gLBParticleGeneratorsUsedNum--;
            }
        }
        else prev_gn = current_gn;
        
        current_gn = next_gn;
    }
}

// 0x800D3BFC
void lbParticleEjectStructSelf(LBParticle *pc)
{
	lbParticleEjectStructID(pc->generator_id, pc->bank_id >> 3);
}

// 0x800D3C28
void lbParticleEjectStructGenerator(LBGenerator *gn)
{
	lbParticleEjectStructID(gn->generator_id, gn->bank_id >> 3);
}

// 0x800D3C54
void lbParticleEjectGeneratorDObj(GObj *gobj)
{
	DObj *dobj;
	LBGenerator *current_gn, *next_gn;

	if (gobj->obj_kind == nGCCommonAppendDObj)
	{
		for (dobj = DObjGetStruct(gobj); dobj != NULL; dobj = gcGetTreeDObjNext(dobj))
		{
			for (current_gn = sLBParticleGeneratorsQueued; current_gn != NULL; current_gn = next_gn)
			{
				next_gn = current_gn->next;

				if (current_gn->dobj == dobj)
				{
					lbParticleEjectGenerator(current_gn);
				}
			}
		}
	}
}

// 0x800D3CE0
void lbParticleSetStructPosAll(f32 pos_x, f32 pos_y, f32 pos_z)
{
    LBGenerator *gn;
    s32 i;
    
    for (i = 0; i < ARRAY_COUNT(sLBParticleStructsAllocLinks); i++)
    {
        LBParticle *pc = sLBParticleStructsAllocLinks[i];
        
        while (pc != NULL)
        {
            pc->pos.x = pos_x;
            pc->pos.y = pos_y;
            pc->pos.z = pos_z;

            pc = pc->next;
        }
    }
    gn = sLBParticleGeneratorsQueued;
    
    while (gn != NULL)
    {
        gn->pos.x = pos_x;
        gn->pos.y = pos_y;
        gn->pos.z = pos_z;

        gn = gn->next;
    }
}

// 0x800D3D64
void lbParticlePauseAllID(u16 generator_id, s32 link_id)
{
	LBParticle *pc;
	LBGenerator *gn;

	for (pc = sLBParticleStructsAllocLinks[link_id]; pc != NULL; pc = pc->next)
	{
		if (pc->generator_id == generator_id)
		{
			pc->flags |= LBPARTICLE_FLAG_PAUSE;
		}
	}
	for (gn = sLBParticleGeneratorsQueued; gn != NULL; gn = gn->next)
	{
		if (gn->generator_id == generator_id)
		{
			gn->flags |= LBPARTICLE_FLAG_PAUSE;
		}
	}
}

// 0x800D3DE8
void lbParticleResumeAllID(u16 generator_id, s32 link_id)
{
	LBParticle *pc;
	LBGenerator *gn;

	for (pc = sLBParticleStructsAllocLinks[link_id]; pc != NULL; pc = pc->next)
	{
		if (pc->generator_id == generator_id)
		{
			pc->flags &= ~LBPARTICLE_FLAG_PAUSE;
		}
	}
	for (gn = sLBParticleGeneratorsQueued; gn != NULL; gn = gn->next)
	{
		if (gn->generator_id == generator_id)
		{
			gn->flags &= ~LBPARTICLE_FLAG_PAUSE;
		}
	}
}

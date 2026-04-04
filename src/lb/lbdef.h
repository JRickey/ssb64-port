#ifndef _LBDEF_H_
#define _LBDEF_H_

#define LBRELOC_CACHE_ALIGN(x) 			(((x) + 0xF) & ~0xF)

#define LBPARTICLE_BANKS_NUM_MAX        8
#define LBPARTICLE_ATTACH_DOBJ_NUM_MAX  8

// Flag bitfield is 16-bits; 1 bit used for toggling attach DObj, 3-bits for attach DObj ID and the remaining is for other flags

#define LBPARTICLE_FLAG_GRAVITY         0x1         // Particle has gravity?
#define LBPARTICLE_FLAG_FRICTION        0x2         // Particle has friction?
#define LBPARTICLE_FLAG_VORTEX          0x4         // This description might be wrong... Toggled off, this causes Hyrule tornado particles to stay on the floor like a puddle
#define LBPARTICLE_FLAG_SHAREDPAL		0x10		// All images in texture series share the same palette
#define LBPARTICLE_FLAG_MASKS           0x20        // Use S-Axis mask
#define LBPARTICLE_FLAG_MASKT           0x40        // Use T-Axis mask
#define LBPARTICLE_FLAG_ENVCOLOR        0x80        // Use environment color
#define LBPARTICLE_FLAG_NOISE           0x100       // Use noise as source in color combiner?
#define LBPARTICLE_FLAG_ALPHABLEND      0x200       // Use environment color's alpha value for blending
#define LBPARTICLE_FLAG_DITHER          0x400       // Use dithering
#define LBPARTICLE_FLAG_PAUSE           0x800       // Skip parsing this particle's bytecode?
#define LBPARTICLE_FLAG_ATTACH          0x8000      // Particle is fixed to a DObj?

#define LBPARTICLE_SET_ATTACH_ID(id)    (LBPARTICLE_FLAG_ATTACH | ((id) << 0xC))
#define LBPARTICLE_GET_ATTACH_ID(id)    (((id) & 0x7000) >> 0xC)

#define LBPARTICLE_OPCODE_SETPOS        0x80        // Set particle position
#define LBPARTICLE_OPCODE_ADDPOS        0x88        // Add to particle position
#define LBPARTICLE_OPCODE_SETVEL        0x90        // Set particle velocity
#define LBPARTICLE_OPCODE_ADDVEL        0x98        // Add to particle velocity

#define LBPARTICLE_OPCODE_SETSIZELERP	0xA0		// Set target size and interpolation length
#define LBPARTICLE_OPCODE_SETFLAG       0xA1        // Set particle flag, only 8-bit though...?
#define LBPARTICLE_OPCODE_SETGRAVITY    0xA2        // Set gravity and particle flag based on whether gravity is 0.0 or not
#define LBPARTICLE_OPCODE_SETFRICTION   0xA3        // Set friction and particle flag based on whether friction is 1.0 or not
#define LBPARTICLE_OPCODE_MAKESCRIPT    0xA4        // Make new particle from script with spawning particle as parent?'
#define LBPARTICLE_OPCODE_MAKEGENERATOR 0xA5        // Make new generator
#define LBPARTICLE_OPCODE_SETLIFERAND   0xA6        // Set fixed minimum lifetime + randomized maximum lifetime
#define LBPARTICLE_OPCODE_TRYDEADRAND   0xA7        // Check given % value against random chance, destroy if value >= random chance
#define LBPARTICLE_OPCODE_ADDVELRAND    0xA8        // Add base velocity * random velocity to each position axis
#define LBPARTICLE_OPCODE_SETVELANGLE   0xA9        // Set new angle for particle's velocity
#define LBPARTICLE_OPCODE_MAKERAND      0xAA        // Make particle from script between base ID and random ID
#define LBPARTICLE_OPCODE_MULVELUFORM   0xAB        // Multiply each velocity axis by one shared, uniform multiplier
#define LBPARTICLE_OPCODE_SETSIZERAND	0xAC		// Set random target size and interpolation length
#define LBPARTICLE_OPCODE_ENVCOLOR      0xAD        // Set LBPARTICLE_FLAG_ENVCOLOR flag
#define LBPARTICLE_OPCODE_NOMASKST      0xAE        // Clear LBPARTICLE_FLAG_MASKS and LBPARTICLE_FLAG_MASKT flags
#define LBPARTICLE_OPCODE_MASKS         0xAF        // Clear LBPARTICLE_FLAG_MASKT and set LBPARTICLE_FLAG_MASKS flags
#define LBPARTICLE_OPCODE_MASKT         0xB0        // Clear LBPARTICLE_FLAG_MASKS and set LBPARTICLE_FLAG_MASKT flags
#define LBPARTICLE_OPCODE_MASKST        0xB1        // Set LBPARTICLE_FLAG_MASKS and LBPARTICLE_FLAG_MASKT flags
#define LBPARTICLE_OPCODE_ALPHABLEND    0xB2        // Set LBPARTICLE_FLAG_ALPHABLEND flag
#define LBPARTICLE_OPCODE_NODITHER      0xB3        // Clear LBPARTICLE_FLAG_DITHER flag
#define LBPARTICLE_OPCODE_DITHER        0xB4        // Set LBPARTICLE_FLAG_DITHER flag
#define LBPARTICLE_OPCODE_NONOISE       0xB5        // Clear LBPARTICLE_FLAG_NOISE flag
#define LBPARTICLE_OPCODE_NOISE         0xB6        // Set LBPARTICLE_FLAG_NOISE flag
#define LBPARTICLE_OPCODE_SETDISTVEL    0xB7        // Set particle velocity to magnitude of distance between particle and target DObj?
#define LBPARTICLE_OPCODE_ADDDISTVELMAG 0xB8        // Add magnitude of distance between particle and target DObj to particle velocity?
#define LBPARTICLE_OPCODE_MAKEID        0xB9        // Make particle of script ID
#define LBPARTICLE_OPCODE_PRIMBLENDRAND 0xBA        // Set primitive blend color values to base * random
#define LBPARTICLE_OPCODE_ENVBLENDRAND  0xBB        // Set environment blend color values to base * random
#define LBPARTICLE_OPCODE_SETVELMAG     0xBD        // Set velocity to base value + random value in range, then clamp to current velocity's magnitude and apply
#define LBPARTICLE_OPCODE_MULVELAXIS    0xBE        // Multiply each velocity axis by a unique multiplier for each
#define LBPARTICLE_OPCODE_SETATTACHID   0xBF        // Set attach DObj array ID

#define LBPARTICLE_OPCODE_SETPRIMBLEND  0xC0        // Set primitive blend color and length; immediately set primitive color to blend color struct if length is 1
#define LBPARTICLE_OPCODE_SETENVBLEND   0xD0        // Set environment blend color and length; immediately set primitive color to blend color struct if length is 1

#define LBPARTICLE_OPCODE_SETLOOP       0xFA        // Initialize loop
#define LBPARTICLE_OPCODE_LOOP          0xFB        // Decrement loop and set bytecode cursor to loop start if not over yet
#define LBPARTICLE_OPCODE_SETRETURN     0xFC        // Initialize current bytecode position as anchor to jump back to
#define LBPARTICLE_OPCODE_RETURN        0xFD        // Return to bytecode position anchor
#define LBPARTICLE_OPCODE_DEAD          0xFE        // Destroy particle
#define LBPARTICLE_OPCODE_END           0xFF        // Same as LBPARTICLE_OPCODE_DEAD

/*
 * Used to define a compound int where lowest 3 bits = bank ID,
 * and the rest are a mask where each bit is the array index of
 * an LBGenerator linked list to be skipped over if
 * the corresponding GObj flag is toggled?
 * 
 * OR this with a particle bank ID
 */
#define LBPARTICLE_MASK_GENLINK(id)				\
(((id) + 1) * 8)

#define syGetSinCosUShort(sin, cos, angle, id) 	\
{                                               \
    id = SINTABLE_RAD_TO_ID(angle) & 0xFFF;     \
                                                \
    sin = gSYSinTable[id & 0x7FF];     			\
                                                \
    if (id & 0x800)                             \
    {                                           \
        sin = -sin;                             \
    }                                          	\
    id += 0x400;                               	\
                                    			\
    cos = gSYSinTable[id & 0x7FF];     			\
                                                \
    if (id & 0x800)                             \
    {                                           \
        cos = -cos;                             \
    }                                           \
}

#define LBBACKUP_MASK_FIGHTER(kind) (1 << (kind))
#define LBBACKUP_MASK_STAGE(kind) (1 << (kind))

#define LBBACKUP_CHARACTER_MASK_ALL				\
(									 			\
	LBBACKUP_MASK_FIGHTER(nFTKindMario) 	|	\
	LBBACKUP_MASK_FIGHTER(nFTKindFox) 		|	\
	LBBACKUP_MASK_FIGHTER(nFTKindDonkey)	|	\
	LBBACKUP_MASK_FIGHTER(nFTKindSamus) 	|	\
	LBBACKUP_MASK_FIGHTER(nFTKindLuigi) 	|	\
	LBBACKUP_MASK_FIGHTER(nFTKindLink) 		|	\
	LBBACKUP_MASK_FIGHTER(nFTKindYoshi) 	|	\
	LBBACKUP_MASK_FIGHTER(nFTKindCaptain)	|	\
	LBBACKUP_MASK_FIGHTER(nFTKindKirby)		|	\
	LBBACKUP_MASK_FIGHTER(nFTKindPikachu)	|	\
	LBBACKUP_MASK_FIGHTER(nFTKindPurin) 	|	\
	LBBACKUP_MASK_FIGHTER(nFTKindNess)			\
)

#define LBBACKUP_CHARACTER_MASK_UNLOCK			\
(												\
	LBBACKUP_MASK_FIGHTER(nFTKindNess) 		|	\
	LBBACKUP_MASK_FIGHTER(nFTKindPurin) 	|	\
	LBBACKUP_MASK_FIGHTER(nFTKindCaptain)	| 	\
	LBBACKUP_MASK_FIGHTER(nFTKindLuigi)			\
)

#define LBBACKUP_CHARACTER_MASK_STARTER (LBBACKUP_CHARACTER_MASK_ALL & ~LBBACKUP_CHARACTER_MASK_UNLOCK)

#define LBBACKUP_UNLOCK_MASK_LUIGI (1 << nLBBackupUnlockLuigi)
#define LBBACKUP_UNLOCK_MASK_NESS (1 << nLBBackupUnlockNess)
#define LBBACKUP_UNLOCK_MASK_CAPTAIN (1 << nLBBackupUnlockCaptain)
#define LBBACKUP_UNLOCK_MASK_PURIN (1 << nLBBackupUnlockPurin)
#define LBBACKUP_UNLOCK_MASK_INISHIE (1 << nLBBackupUnlockInishie)
#define LBBACKUP_UNLOCK_MASK_SOUNDTEST (1 << nLBBackupUnlockSoundTest)
#define LBBACKUP_UNLOCK_MASK_ITEMSWITCH (1 << nLBBackupUnlockItemSwitch)

#define LBBACKUP_UNLOCK_MASK_ALL 		\
(										\
	LBBACKUP_UNLOCK_MASK_ITEMSWITCH | 	\
	LBBACKUP_UNLOCK_MASK_SOUNDTEST 	| 	\
	LBBACKUP_UNLOCK_MASK_INISHIE   	|	\
	LBBACKUP_UNLOCK_MASK_PURIN 		| 	\
	LBBACKUP_UNLOCK_MASK_CAPTAIN 	| 	\
	LBBACKUP_UNLOCK_MASK_NESS 		| 	\
	LBBACKUP_UNLOCK_MASK_LUIGI			\
)

#define LBBACKUP_UNLOCK_MASK_NEWCOMERS (LBBACKUP_UNLOCK_MASK_LUIGI | LBBACKUP_UNLOCK_MASK_PURIN | LBBACKUP_UNLOCK_MASK_CAPTAIN | LBBACKUP_UNLOCK_MASK_NESS)
#define LBBACKUP_UNLOCK_MASK_PRIZE (LBBACKUP_UNLOCK_MASK_ALL & ~LBBACKUP_UNLOCK_MASK_NEWCOMERS)

#define LBBACKUP_GROUND_MASK_ALL 			\
(											\
	LBBACKUP_MASK_STAGE(nGRKindCastle) 	| 	\
	LBBACKUP_MASK_STAGE(nGRKindSector) 	|	\
	LBBACKUP_MASK_STAGE(nGRKindJungle) 	|	\
	LBBACKUP_MASK_STAGE(nGRKindZebes) 	|	\
	LBBACKUP_MASK_STAGE(nGRKindHyrule) 	|	\
	LBBACKUP_MASK_STAGE(nGRKindYoster) 	|	\
	LBBACKUP_MASK_STAGE(nGRKindPupupu) 	| 	\
	LBBACKUP_MASK_STAGE(nGRKindYamabuki)	\
)

#define LBBACKUP_ERROR_RANDOMKNOCKBACK (1 << nLBBackupErrorRandomKnockback)		// 0x1 - Random knockback between 0.1 (?) and 1.0 x 200u
#define LBBACKUP_ERROR_HALFSTICKRANGE  (1 << nLBBackupErrorHalfStickRange)		// 0x2 - Halves control stick input range
#define LBBACKUP_ERROR_1PGAMEMARIO 	(1 << nLBBackupError1PGameMario) 			// 0x4 - Forces Mario in 1P Game
#define LBBACKUP_ERROR_VSBATTLECASTLE (1 << nLBBackupErrorVSBattleCastle) 	    // 0x8 - Forces Peach's Castle in VS Mode

typedef enum LBParticleKind
{
	nLBParticleKindVortex = 2

} LBParticleKind;

typedef enum LBFileLocation
{
    nLBFileLocationExtern,
    nLBFileLocationDefault,
    nLBFileLocationForce,
	nLBFileLocationEnumCount

} LBFileLocation;

typedef enum LBBackupUnlock
{
	nLBBackupUnlockLuigi,	 			    // Luigi
	nLBBackupUnlockNess,				    // Ness
	nLBBackupUnlockCaptain,	 			    // Captain Falcon
	nLBBackupUnlockPurin,	 			    // Jigglypuff
	nLBBackupUnlockInishie,	 			    // Mushroom Kingdom
	nLBBackupUnlockSoundTest, 			    // Sound Test menu
	nLBBackupUnlockItemSwitch, 			    // Item Switch menu
	nLBBackupUnlockEnumCount

} LBBackupUnlock;

typedef enum LBBackupErrors
{
	nLBBackupErrorRandomKnockback,
	nLBBackupErrorHalfStickRange,
	nLBBackupError1PGameMario,
	nLBBackupErrorVSBattleCastle

} LBBackupErrors;

typedef struct LBRelocSetup					LBRelocSetup;
typedef union  LBRelocDesc					LBRelocDesc;
typedef struct LBInternBuffer					LBInternBuffer;
typedef struct LBFileNode					LBFileNode;
typedef struct LBTableEntry 				LBTableEntry;
typedef struct LBScript                     LBScript;
typedef struct LBScriptDesc                 LBScriptDesc;
typedef struct LBTexture                    LBTexture;
typedef struct LBTextureDesc                LBTextureDesc;
typedef struct LBTransform		            LBTransform;
typedef struct LBGenerator		            LBGenerator;
typedef struct LBParticle		            LBParticle;
typedef struct LBTransitionDesc				LBTransitionDesc;
typedef struct LBBackup1PRecord             LBBackup1PRecord;
typedef struct LBBackupVSRecord         LBBackupVSRecord;
typedef struct LBBackupData                 LBBackupData;

#endif

#ifndef _ITVARS_H_
#define _ITVARS_H_

#include <ssb_types.h>
#include <sys/obj.h>
#include <it/itdef.h>
#include <ef/efdef.h>

#define ITBOX_EXPLODE_FRAME_END 8
#define ITBOX_HEALTH_MAX 15		 // Maximum damage box can resist before breaking
#define ITBOX_EXPLODE_SCALE 1.4F // Size of explosion graphics
#define ITBOX_GRAVITY 4.0F
#define ITBOX_TVEL 120.0F
#define ITBOX_MAP_REBOUND_COMMON 0.2F
#define ITBOX_MAP_REBOUND_GROUND 0.5F

#define ITCAPSULE_EXPLODE_FRAME_END 6
#define ITCAPSULE_GRAVITY 1.2F
#define ITCAPSULE_TVEL 100.0F
#define ITCAPSULE_MAP_REBOUND_COMMON 0.2F
#define ITCAPSULE_MAP_REBOUND_GROUND 0.4F
#define ITCAPSULE_EXPLODE_SCALE 1.4F // Size of explosion graphics

#define ITTOMATO_DAMAGE_HEAL 100
#define ITTOMATO_GRAVITY 1.2F
#define ITTOMATO_TVEL 100.0F
#define ITTOMATO_MAP_REBOUND_COMMON 0.3F
#define ITTOMATO_MAP_REBOUND_GROUND 0.5F

#define ITHEART_DAMAGE_HEAL 999
#define ITHEART_GRAVITY 0.25F
#define ITHEART_TVEL 30.0F
#define ITHEART_MAP_REBOUND_COMMON 0.1F
#define ITHEART_MAP_REBOUND_GROUND 0.0F

#define ITSTAR_COLANIM_ID 0x4A
#define ITSTAR_COLANIM_LENGTH 0
#define ITSTAR_BGM_ID 0x2D
#define ITSTAR_BGM_DURATION 10
#define ITSTAR_INVINCIBLE_TIME 600
#define ITSTAR_WARN_BEGIN_FRAME (ITSTAR_INVINCIBLE_TIME - 480)
#define ITSTAR_INTERACT_DELAY 16 // Wait this many frames before becoming tangible
#define ITSTAR_GRAVITY 1.2F
#define ITSTAR_TVEL 100.0F
#define ITSTAR_MAP_REBOUND_COMMON 1.0F
#define ITSTAR_VEL_X 8.0F
#define ITSTAR_BOUNCE_Y 50.0F // Y-velocity added when Star Man touches the ground

#define ITSWORD_GRAVITY 1.2F
#define ITSWORD_TVEL 100.0F
#define ITSWORD_MAP_REBOUND_COMMON 0.2F
#define ITSWORD_MAP_REBOUND_GROUND 0.5F

#define ITBAT_GRAVITY 1.5F
#define ITBAT_TVEL 120.0F
#define ITBAT_MAP_REBOUND_COMMON 0.2F
#define ITBAT_MAP_REBOUND_GROUND 0.5F

#define ITHARISEN_GRAVITY 1.0F
#define ITHARISEN_TVEL 80.0F
#define ITHARISEN_MAP_REBOUND_COMMON 0.0F
#define ITHARISEN_MAP_REBOUND_GROUND 0.3F

#define ITLGUN_AMMO_MAX 16
#define ITLGUN_GRAVITY 1.5F
#define ITLGUN_TVEL 130.0F
#define ITLGUN_MAP_REBOUND_COMMON 0.2F
#define ITLGUN_MAP_REBOUND_GROUND 0.1F
#define ITLGUN_AMMO_VEL_X 300.0F
#define ITLGUN_AMMO_STEP_SCALE_X 10.0F
#define ITLGUN_AMMO_CLAMP_SCALE_X (160.0F / 3.0F)

#define ITFFLOWER_AMMO_MAX 60
#define ITFFLOWER_AMMO_LIFETIME 30
#define ITFFLOWER_AMMO_VEL 30.0F
#define ITFFLOWER_GRAVITY 1.2F
#define ITFFLOWER_TVEL 100.0F
#define ITFFLOWER_MAP_REBOUND_COMMON 0.0F
#define ITFFLOWER_MAP_REBOUND_GROUND 0.5F

#define ITHAMMER_TIME 720
#define ITHAMMER_BGM_ID 0x2E
#define ITHAMMER_BGM_DURATION 20
#define ITHAMMER_WEAR_COLANIM_ID 0x4E // Color animation applied to hammer when time is almost up
#define ITHAMMER_WEAR_COLANIM_LENGTH 0
#define ITHAMMER_WARN_BEGIN_FRAME (ITHAMMER_TIME - 600)
#define ITHAMMER_GRAVITY 1.5F
#define ITHAMMER_TVEL 120.0F
#define ITHAMMER_MAP_REBOUND_COMMON 0.5F
#define ITHAMMER_MAP_REBOUND_GROUND 0.2F

#define ITMSBOMB_EXPLODE_LIFETIME 16
#define ITMSBOMB_DETECT_FIGHTER_DELAY 100
#define ITMSBOMB_DETECT_FIGHTER_RADIUS                                                                                 \
	SQUARE(400.0F) // Radius is actually 400 units, but it is not sqrt'd in the
				   // detection function, so it ends up checking 160000.0F
#define ITMSBOMB_GRAVITY 1.5F
#define ITMSBOMB_TVEL 80.0F
#define ITMSBOMB_MAP_REBOUND_COMMON 0.4F
#define ITMSBOMB_MAP_REBOUND_GROUND 0.3F
#define ITMSBOMB_COLL_SIZE 30.0F	// Size of Motion Sensor Bomb collision box
#define ITMSBOMB_EXPLODE_SCALE 1.2F // Size of explosion graphics

#define ITBOMBHEI_EXPLODE_COLANIM_ID 0x4D
#define ITBOMBHEI_EXPLODE_COLANIM_DURATION 90
#define ITBOMBHEI_EXPLODE_LIFETIME 6
#define ITBOMBHEI_WALK_WAIT 180	 // Number of frames required to pass for Bob-Omb to begin walking
#define ITBOMBHEI_FLASH_WAIT 480 // Number of frames required to pass for Bob-Omb to begin flashing
#define ITBOMBHEI_SMOKE_WAIT 4	 // Delay between smoke GFX?
#define ITBOMBHEI_EXPLODE_WAIT                                                                                         \
	90.0F // Number of frames required to pass for Bob-Omb to explode after
		  // flashing? Also a float for some reason
#define ITBOMBHEI_WALK_VEL_X 24.0F
#define ITBOMBHEI_GRAVITY 1.2F
#define ITBOMBHEI_TVEL 100.0F
#define ITBOMBHEI_MAP_REBOUND_COMMON 0.4F
#define ITBOMBHEI_MAP_REBOUND_GROUND 0.3F
#define ITBOMBHEI_EXPLODE_SCALE 1.4F

#define ITSTARROD_AMMO_MAX 20
#define ITSTARROD_GRAVITY 1.2F
#define ITSTARROD_TVEL 100.0F
#define ITSTARROD_MAP_REBOUND_COMMON 0.2F
#define ITSTARROD_MAP_REBOUND_GROUND 0.5F
#define ITSTARROD_AMMO_SMASH_VEL_X 120.0F
#define ITSTARROD_AMMO_TILTVEL_X 80.0F
#define ITSTARROD_AMMO_SMASH_LIFETIME 30.0F
#define ITSTARROD_AMMO_TILT_LIFETIME 30.0F

#define ITGSHELL_LIFETIME 240
#define ITGSHELL_HEALTH_MAX 4		// Set every time shell hits a target
#define ITGSHELL_EFFECT_SPAWN_INT 8	// Interval between dust GFX
#define ITGSHELL_DAMAGE_ALL_WAIT 32 // Wait this many frames before shell can damage its owner and teammates
#define ITGSHELL_CLAMP_VEL_X 90.0F	// Limit maximum X-velocity to this value
#define ITGSHELL_REBOUND_MUL_X                                                                                         \
	0.125F							 // Multiplies negated X-velocity; actually division, but optimized
									 // into multiplication if possible
#define ITGSHELL_REBOUND_VEL_Y 38.0F // Y-velocity gained when recoiling from hitting a target
#define ITGSHELL_STOP_VEL_X 12.0F	 // Halt all horizontal velocity if less than this value
#define ITGSHELL_DAMAGE_MUL_NORMAL                                                                                     \
	8.0F // Multiplies damage taken and turns it into horizontal velocity; run
		 // when Shell is hit out of its "wait" state
#define ITGSHELL_DAMAGE_MUL_ADD                                                                                        \
	3.0F // Multiplies damage taken and adds it to horizontal velocity; run whe
		 // Shell is hit out of its "slide" state
#define ITGSHELL_GRAVITY 1.2F
#define ITGSHELL_TVEL 100.0F
#define ITGSHELL_MAP_REBOUND_COMMON 0.2F
#define ITGSHELL_MAP_REBOUND_GROUND 0.5F

#define ITCONTAINER_EFFECT_COUNT                                                                                          \
	7								// Number of pieces Barrel and Crate containers burst into when smashed
									// open
#define ITCONTAINER_GFX_LIFETIME 90 // Smashed container pieces despawn once this runs out

#define ITTARU_LIFETIME 360
#define ITTARU_EXPLODE_LIFETIME 8
#define ITTARU_DESPAWN_FLASH_START 60 // Begin flashing when below this value
#define ITTARU_HEALTH_MAX 10
#define ITTARU_MUL_VEL_X 1.4F
#define ITTARU_VEL_MIN 0.1F		   // Decrement lifetime if Barrel's XY velocity is less than this value
#define ITTARU_ROLL_ROTATE_MUL 0.0045F // Multiplies rotation of barrel
#define ITTARU_EXPLODE_SCALE 1.4F	   // Size of explosion graphics
#define ITTARU_GRAVITY 4.0F
#define ITTARU_TVEL 90.0F
#define ITTARU_MAP_REBOUND_COMMON 0.5F
#define ITTARU_MAP_REBOUND_GROUND 0.2F

#define ITRSHELL_INTERACT_MAX 24
#define ITRSHELL_LIFETIME 480
#define ITRSHELL_HEALTH_MAX 4
#define ITRSHELL_EFFECT_SPAWN_INT 8
#define ITRSHELL_DAMAGE_ALL_WAIT 16
#define ITRSHELL_CLAMP_VEL_X 70.0F
#define ITRSHELL_CLAMP_AIR_X                                                                                           \
	90.0F // Leftover from Green Shell? This is checked only when the ground
		  // beneath the Red Shell collapses
#define ITRSHELL_HIT_INITVEL_X                                                                                        \
	8.0F // Initialize hitbox if it is currently inactive and velocity is less
		 // than this value
#define ITRSHELL_MUL_VEL_X 1.2F
#define ITRSHELL_STOP_VEL_X                                                                                            \
	8.0F							  // Halt all horizontal velocity if less than this value OR disable
									  // hitbox
#define ITRSHELL_ADD_VEL_X 60.0F	  // Added when Red Shell is reflected
#define ITRSHELL_RECOIL_VEL_X (-8.0F) // Applied when shell is attacked
#define ITRSHELL_RECOIL_MUL_X 0.7F	  // Multiplies recoil velocity
#define ITRSHELL_DAMAGE_MUL_NORMAL                                                                                     \
	10.0F // Multiplies damage taken and turns it into horizontal velocity; run
		  // when Shell is hit out of its "wait" state
#define ITRSHELL_GRAVITY 1.2F
#define ITRSHELL_TVEL 100.0F
#define ITRSHELL_MAP_REBOUND_COMMON 0.25F
#define ITRSHELL_MAP_REBOUND_GROUND 0.5F

#define ITBUMPER_CASTLE_KNOCKBACK 300 // Weight-Dependent Set Knockback of Bumper on Peach's Castle
#define ITBUMPER_CASTLE_ANGLE 361	  // Knockback angle of Bumper on Peach's Castle
#define ITBUMPER_LIFETIME 360
#define ITBUMPER_DESPAWN_TIMER                                                                                         \
	60 // Bumper's lifetime is reset to this value; there is no hitbox during
	   // this state, and it vanishes for good once it runs out
#define ITBUMPER_STOPVEL_WAIT                                                                                       \
	4 // Bumper's X-velocity is reset to 0.0 when multi is less than this
	  // value
#define ITBUMPER_DAMAGE_ALL_WAIT 16
#define ITBUMPER_HIT_SCALE 10	   // Initial of bumper when hitting a target while airborne
#define ITBUMPER_HIT_ANIM_LENGTH 3 // Number of frames bumper's airborne hit texture lasts
#define ITBUMPER_COLL_SIZE 120.0F
#define ITBUMPER_REBOUND_VEL_X (-100.0F) // Applied when bumper hits a target while grounded
#define ITBUMPER_REBOUND_AIR_X (-400.0F) // Applied when bumper hits a target while airborne
#define ITBUMPER_REBOUND_AIR_Y 200.0F	 // Applied when bumper hits a target while airborne
#define ITBUMPER_GRAVITY_NORMAL 1.4F
#define ITBUMPER_GRAVITY_HIT 4.0F
#define ITBUMPER_TVEL 80.0F
#define ITBUMPER_MAP_REBOUND_COMMON 0.8F
#define ITBUMPER_MAP_REBOUND_GROUND 0.8F

#define ITGLUCKY_EGG_SPAWN_COUNT                                                                                      \
	1									 // Wait this many frames before spawning egg (read only when egg spawn
										 // function is called)
#define ITGLUCKY_EGG_SPAWN_OFF_X 200.0F // X-offset of egg spawn
#define ITGLUCKY_EGG_SPAWN_OFF_Y 200.0F // Y-offset of egg spawn
#define ITGLUCKY_EGG_SPAWN_MUL 8.0F	 // Multiplies random velocity of egg
#define ITGLUCKY_EGG_SPAWN_ADD_X 8.0F	 // Added to mutliplied random velocity of egg
#define ITGLUCKY_EGG_SPAWN_ADD_Y 30.0F	 // Added to multiplide random velocity of egg
#define ITGLUCKY_EGG_SPAWN_BEGIN 80.0F	 // First frame egg spawn function is called
#define ITGLUCKY_EGG_SPAWN_END 85.0F	 // Last frame egg spawn function is called
#define ITGLUCKY_HIT_ROTATE_Z                                                                                         \
	0.10471976F // Rotation step when Chansey is knocked out of the Pokémon
				// hut? Not actually applied?
#define ITGLUCKY_GRAVITY 1.2F
#define ITGLUCKY_NDAMAGE_KNOCKBACK_MIN 100.0F
#define ITGLUCKY_TVEL 100.0F

#define ITMBALL_SPAWN_WAIT 30 // Wait this many frames before releasing Pokémon once thrown and landed
#define ITMBALL_GRAVITY 1.5F
#define ITMBALL_TVEL 120.0F
#define ITMBALL_MAP_REBOUND_COMMON 0.2F
#define ITMBALL_MAP_REBOUND_GROUND 0.2F

#define ITMONSTER_RISE_STOP_WAIT                                                                                       \
	22 // Universal attribute for Pokémon to stop slowly levitating upward and
	   // to use designated physics this many frames after spawning
#define ITMONSTER_RISE_VEL_Y 16.0F // Y-velocity of "levitate" effect after exiting Poké Ball

#define ITPAKKUN_APPEAR_WAIT 180 // Duration Pirahna Plant waits before coming out of its pipe
#define ITPAKKUN_REBIRTH_WAIT                                                                                          \
	1200 // Duration Pirahna Plant waits before respawning after being knocked
		 // out and hitting one of the blast zones
#define ITPAKKUN_APPEAR_OFF_Y 245.0F
#define ITPAKKUN_CLAMP_OFF_Y 360.0F
#define ITPAKKUN_HURT_SIZE_MUL_Y 0.5F
#define ITPAKKUN_DETECT_SIZE_WIDTH                                                                                     \
	600.0F // Width of Pirahna Plant fighter detection box when checking to
		   // stay inside the pipe
#define ITPAKKUN_DETECT_SIZE_BOTTOM                                                                                    \
	(-300.0F) // Bottom of Pirahna Plant fighter detection box when checking to
			  // stay inside the pipe
#define ITPAKKUN_DETECT_SIZE_TOP                                                                                       \
	700.0F // Top of Pirahna Plant fighter detection box when checking to stay
		   // inside the pipe
#define ITPAKKUN_NDAMAGE_KNOCKBACK_MIN 100.0F
#define ITPAKKUN_GRAVITY 1.5F
#define ITPAKKUN_TVEL 100.0F

#define ITIWARK_FLY_WAIT 30 // Unconfirmed?
#define ITIWARK_MODEL_ROTATE_WAIT                                                                                      \
	6										 // Onix's sprite is flipped every time this timer is up; counts up, reset
											 // to 0 when reaching destination value
#define ITIWARK_ROCK_RUMBLE_WAIT 18			 // Frames to wait between screen shake effects
#define ITIWARK_ROCK_SPAWN_COUNT_RANDOM 9	 // Maximum number of rocks that can be spawned
#define ITIWARK_ROCK_SPAWN_COUNT_MIN 8		 // Added to number of rocks that can be spawned
#define ITIWARK_ROCK_SPAWN_WAIT_MAX 30		 // Maximum random delay between spawned rocks
#define ITIWARK_ROCK_SPAWN_WAIT_MIN 15		 // Added to random delay between spawned rocks
#define ITIWARK_ROCK_SPAWN_OFF_X_MUL 2000.0F // Multiplies random spawn position of rock projectile
#define ITIWARK_ROCK_SPAWN_OFF_X_ADD (-1000.0F) // Added to random spawn position of rock projectile
#define ITIWARK_FLY_VEL_Y 80.0F					// Y-velocity of Onix when flying up
#define ITIWARK_FLY_STOP_Y 200.0F				// Stop this many units away from the upper blast zone
#define ITIWARK_IWARK_ADD_POS_Y (-660.0F)		// Add this to Y-position if user is Onix
#define ITIWARK_OTHER_ADD_POS_Y (-100.0F)		// Add this to Y-position if user is not Onix

#define WPIWARK_ROCK_RANDOM_VEL_MAX 3		 // Maximum number of random initial Y-velocities possible
#define WPIWARK_ROCK_GRAVITY 2.0F			 // Gravity of rock projectile
#define WPIWARK_ROCK_TVEL 200.0F			 // Terminal velocity of rock projectile
#define WPIWARK_ROCK_ROTATE_STEP (-0.5F)	 // Z-rotation step of rock projectile
#define WPIWARK_ROCK_VEL_Y_START_A (-100.0F) // One of three random initial Y-velocities possible
#define WPIWARK_ROCK_VEL_Y_START_B (-50.0F)	 // One of three random initial Y-velocities possible
#define WPIWARK_ROCK_VEL_Y_START_C 0.0F		 // One of three random initial Y-velocities possible
#define WPIWARK_ROCK_COLLIDE_MUL_VEL_Y                                                                                 \
	0.1F										 // Assume this is what slows the rocks down so much when they hit the
												 // ground
#define WPIWARK_ROCK_COLLIDE_ADD_VEL_Y (-150.0F) // Added when rock collides with ground

#define ITKABIGON_EFFECT_SPAWN_INT 4
#define ITKABIGON_DROP_WAIT                                                                                            \
	60 // Wait this many frames before dropping after reaching the top of the
	   // screen
#define ITKABIGON_RUMBLE_WAIT 18			// Wait this many frames between screen shake effects
#define ITKABIGON_DROP_VEL_Y (-220.0F)		// Velocity of downward drop
#define ITKABIGON_DROP_OFF_X_MUL 2000.0F	// Multiplies random drop X-position
#define ITKABIGON_DROP_OFF_X_ADD (-1000.0F) // Added to multiplied random drop X-position
#define ITKABIGON_DROP_SIZE_KABIGON 4.0F	// Size of Snorlax's drop model and hitbox
#define ITKABIGON_DROP_SIZE_OTHER 5.2F		// Size of non-Snorlax drop model and hitbox (Clefairy)
#define ITKABIGON_JUMP_VEL_Y 80.0F			// Y-velocity of Snorlax's jump
#define ITKABIGON_JUMP_GFX_MUL_OFF 200.0F	// Multiplies random spawn position of dust GFX when Snorlax jumps
#define ITKABIGON_JUMP_GFX_SUB_OFF                                                                                     \
	100.0F // Subtracted from random spawn position of dust GFX when Snorlax
		   // jumps
#define ITKABIGON_MAP_OFF_Y                                                                                            \
	200.0F // Halt jump this many units away from top blastonze / despawn if
		   // Y-position is less than bottom map_bound + this value

#define ITTOSAKINTO_LIFETIME 360
#define ITTOSAKINTO_FLAP_VEL_X 10.0F // Semi-constant horizontal velocity that may randomly be inverted
#define ITTOSAKINTO_FLAP_VEL_Y 60.0F // Vertical velocity gained from touching the ground
#define ITTOSAKINTO_GRAVITY 6.0F
#define ITTOSAKINTO_TVEL 90.0F

#define ITMEW_LIFETIME 480
#define ITMEW_EFFECT_SPAWN_INT 3
#define ITMEW_STARTVEL_X 8.0F	 // Mew's initial X-velocity
#define ITMEW_STARTVEL_Y -20.0F // Mew's initial Y-velocity
#define ITMEW_FLY_ADD_VEL_Y 0.8F // Added to Mew's Y-velocity every frame

#define ITNYARS_LIFETIME 240
#define ITNYARS_MODEL_ROTATE_WAIT 30 // Flip Meowth's model every time this timer runs out; reset at 0
#define ITNYARS_COIN_SPAWN_MAX 4	 // Spawn this many coins simultaneously
#define ITNYARS_COIN_LIFETIME 10
#define ITNYARS_COIN_SPAWN_WAIT 8	  // Spawn a new set of coins at this frequency (frames)
#define ITNYARS_COIN_ANGLE_STEP 13.0F // Multiplies coin rotation step, actually angle in degrees
#define ITNYARS_COIN_ANGLE_DIFF 90.0F // Angle difference between each simultaneously spawned coin
#define ITNYARS_COIN_VEL_X 130.0F

#define ITLIZARDON_LIFETIME 480
#define ITLIZARDON_FLAME_LIFETIME 30
#define ITLIZARDON_FLAME_SPAWN_WAIT 8 // Wait this many frames before spawning another flame projectile
#define ITLIZARDON_TURN_WAIT 26		  // Wait this many frames before flipping Charizard's sprite
#define ITLIZARDON_FLAME_ANGLE F_CLC_DTOR32(-15.0F) // Angle at which flame projectile spreads (-0.2617994F)
#define ITLIZARDON_FLAME_VEL 50.0F
#define ITLIZARDON_LIZARDON_FLAME_OFF_X 180.0F // X-offset of Charizard's flame spawn position
#define ITLIZARDON_LIZARDON_FLAME_OFF_Y 120.0F // Y-offset of Charizard's flame spawn position
#define ITLIZARDON_OTHER_FLAME_OFF_X 100.0F	   // X-offset of other flame spawn position (Clefairy)
#define ITLIZARDON_DUST_OFF_X (-400.0F)	   // X-offset of dust GFX when turning around
#define ITLIZARDON_GRAVITY 1.0F
#define ITLIZARDON_TVEL 40.0F
#define ITLIZARDON_MAP_REBOUND_COMMON 0.2F
#define ITLIZARDON_MAP_REBOUND_GROUND 1.0F

#define ITSPEAR_SPAWN_COUNT 16			  // Swarm will have this many member Beedrill
#define ITSPEAR_SPAWN_WAIT_CONST 12		  // Minimum number of frames between Beedrill swarm member spawns
#define ITSPEAR_SPAWN_WAIT_RANDOM 9		  // Added to minimum number of frames between Beedrill swarm member spawns
#define ITSPEAR_SPAWN_OFF_Y_MUL 1800.0F	  // Multiplies random vertical spawn position
#define ITSPEAR_SPAWN_OFF_Y_ADD (-800.0F) // Added to random vertical spawn position
#define ITSPEAR_SWARM_FLY_VEL_X 130.0F	  // X-velocity of Beedrill swarm members
#define ITSPEAR_SWARM_CALL_VEL_X                                                                                       \
	6.0F							   // Leader Beedrill's X-velocity when flying off to call the swarm,
									   // added each frame until called
#define ITSPEAR_SWARM_CALL_VEL_Y 60.0F // ???
#define ITSPEAR_SWARM_CALL_OFF_X                                                                                       \
	500.0F // Begin swarm when leader is this many units away from the side
		   // map_bound
#define ITSPEAR_SWARM_CALL_WAIT                                                                                        \
	51.0F // Fly off into the distance and call Beedrill swarm on this frame of
		  // the leader's wait animation
#define ITSPEAR_GRAVITY 1.0F
#define ITSPEAR_TVEL 90.0F

#define ITKAMEX_LIFETIME 360
#define ITKAMEX_HYDRO_LIFETIME 20
#define ITKAMEX_HYDRO_SPAWN_WAIT_CONST 30	   // Constant wait frames to spawn Hydro Pump
#define ITKAMEX_HYDRO_SPAWN_WAIT_RANDOM 1	   // Maximum random wait frames to spawn Hydro Pump
#define ITKAMEX_KAMEX_HYDRO_SPAWN_OFF_X 360.0F // X-offset of Blastoise Hydro Pump spawn position
#define ITKAMEX_KAMEX_HYDRO_SPAWN_OFF_Y 100.0F // Y-offset of Blastoise Hydro Pump spawn position
#define ITKAMEX_OTHER_HYDRO_SPAWN_OFF_X 100.0F // X-offset of other Hydro Pump spawn position (Clefairy)
#define ITKAMEX_DUST_SPAWN_OFF_X (-150.0F)	   // X-offset of dust GFX
#define ITKAMEX_COLL_SIZE 341.0F
#define ITKAMEX_PUSH_VEL_X 2.3F
#define ITKAMEX_CONSTVEL_X 38.0F // Constant velocity after Blastoise lands
#define ITKAMEX_GRAVITY 1.0F
#define ITKAMEX_TVEL 40.0F

#define ITMLUCKY_LIFETIME 90		   // Used only after Chansey has spawned all of its eggs
#define ITMLUCKY_EGG_SPAWN_WAIT_ADD 4 // Added when Chansey gets hit
#define ITMLUCKY_EGG_SPAWN_COUNT 3
#define ITMLUCKY_EGG_SPAWN_WAIT_CONST 30
#define ITMLUCKY_EGG_SPAWN_BASE_VEL 8.0F	// Multiplies random velocity
#define ITMLUCKY_EGG_SPAWN_ADD_VEL_X 7.0F	// Added to multiplied random X-velocity
#define ITMLUCKY_EGG_SPAWN_ADD_VEL_Y 40.0F // Added to multiplide random Y-velocity
#define ITMLUCKY_GRAVITY 1.0F
#define ITMLUCKY_TVEL 40.0F

#define ITEGG_EXPLODE_EFFECT_WAIT 8	 // Wait this many frames before spawning eggshell break GFX on explosion
#define ITEGG_EXPLODE_EFFECT_SCALE 1.4F // Scale of explosion GFX
#define ITEGG_GRAVITY 1.2F
#define ITEGG_MAP_REBOUND_COMMON 0.2F
#define ITEGG_MAP_REBOUND_GROUND 0.5F
#define ITEGG_TVEL 100.0F

#define ITSTARMIE_LIFETIME 240
#define ITSTARMIE_SWIFT_LIFETIME 30
#define ITSTARMIE_SWIFT_SPAWN_WAIT_CONST 12 // Constant wait frames to spawn Hydro Pump
#define ITSTARMIE_SWIFT_SPAWN_WAIT_RANDOM 1 // Maximum random wait frames to spawn Swift
#define ITSTARMIE_SWIFTVEL_X 150.0F
#define ITSTARMIE_STARMIE_SWIFT_SPAWN_OFF_X 200.0F // X-offset of Starmie Swift spawn position
#define ITSTARMIE_STARMIE_SWIFT_SPAWN_OFF_Y 100.0F // Y-offset of Starmie Swift spawn position
#define ITSTARMIE_OTHER_SWIFT_SPAWN_OFF_X 100.0F   // X-offset of other Swift spawn position (Clefairy)
#define ITSTARMIE_TARGET_POS_OFF_X 400.0F // Added to Starmie's target travel position + fighter's collision box
#define ITSTARMIE_TARGET_POS_OFF_Y 250.0F // Added to Starmie's target travel position + fighter's collision box
#define ITSTARMIE_FOLLOW_VEL_X 20.0F	  // X-velocity at which Starmie follows its victim
#define ITSTARMIE_ADD_VEL_X 10.0F
#define ITSTARMIE_PUSH_VEL_X 70.0F // Set every time Starmie fires Swift

#define ITSAWAMURA_LIFETIME 600
#define ITSAWAMURA_KICK_WAIT 40
#define ITSAWAMURA_TARGET_POS_OFF_Y                                                                                    \
	500.0F // Added to Hitmonlee's target travel position + fighter's collision
		   // box
#define ITSAWAMURA_DESPAWN_OFF_X                                                                                       \
	500.0F							// Despawn if Hitmonlee is within this many units of the side
									// map_bounds
#define ITSAWAMURA_KICK_SIZE 300.0F // Size of kick hitbox
#define ITSAWAMURA_KICK_VEL_X 400.0F
#define ITSAWAMURA_GRAVITY 2.4F
#define ITSAWAMURA_TVEL 100.0F

#define ITDOGAS_DESPAWN_WAIT 90 // Wait this many frames after running out of gas before despawning
#define ITDOGAS_SMOG_SPAWN_WAIT 8
#define ITDOGAS_SMOG_SPAWN_COUNT 32 // Spawn this many Smog clouds
#define ITDOGAS_SMOG_LIFETIME 30
#define ITDOGAS_SMOG_VEL 18.0F
#define ITDOGAS_SMOG_MUL_OFF_X 400.0F // Multiply random spawn position
#define ITDOGAS_SMOG_SUB_OFF_X 200.0F // Subtracted from multiplied random spawn position
#define ITDOGAS_SMOG_MUL_OFF_Y 800.0F // Multiply random spawn position
#define ITDGOAS_SMOG_SUB_OFF_Y 400.0F // Subtracted from multiplied random spawn position

#define ITMARUMINE_EXPLODE_LIFETIME 6
#define ITMARUMINE_EXPLODE_EFFECT_SCALE 1.4F

#define ITPORYGON_SHAKE_STOP_WAIT 32 // Stop vertical shake after this many frames have passed?

#define ITHITOKAGE_FLAME_LIFETIME 20
#define ITHITOKAGE_FLAME_SPAWN_WAIT 8
#define ITHITOKAGE_FLAME_SPAWN_ANGLE F_CLC_DTOR32(-12.0F) // -0.20943952F
#define ITHITOKAGE_FLAME_VEL_BASE 45.0F
#define ITHITOKAGE_FLAME_SPAWN_BEGIN 40.0F		   // Charmander can begin firing Flamethrower this frame onward
#define ITHITOKAGE_FLAME_SPAWN_END 120.0F		   // Charmander can no longer fire Flamethrower this frame onward
#define ITHITOKAGE_FLAME_SPAWN_OFF_X (-250.0F)	   // X-offset of flame spawn
#define ITHITOKAGE_HIT_ROTATE_Z F_CLC_DTOR32(6.0F) // 0.10471976F
#define ITHITOKAGE_NDAMAGE_KNOCKBACK_MIN 100.0F	   // Minimum knockback required to send Charmander into damage state
#define ITHITOKAGE_GRAVITY 1.2F
#define ITHITOKAGE_TVEL 100.0F

#define ITFUSHIGIBANA_RETURN_WAIT 128 // Wait this many frames before deactivating hitbox?
#define ITFUSHIGIBANA_RAZOR_LIFETIME 24
#define ITFUSHIGIBANA_RAZOR_SPAWN_WAIT 16
#define ITFUSHIGIBANA_RAZOR_VEL_X (-100.0F)
#define ITFUSHIGIBANA_RAZOR_ADD_VEL_X 5.0F
#define ITFUSHIGIBANA_RAZOR_SPAWN_BEGIN 40.0F	  // Venusaur can begin shooting Razor Leaf this frame onward
#define ITFUSHIGIBANA_RAZOR_SPAWN_END 120.0F	  // Venusaur can no longer shoot Razor Leaf this frame onward
#define ITFUSHIGIBANA_RAZOR_SPAWN_OFF_X (-540.0F) // X-offset of leaf spawn

#define ITTARUBOMB_EFFECT_COUNT 7		// Number of pieces RTTF Bomb bursts into when smashed open
#define ITTARUBOMB_GFX_LIFETIME 90 // Smashed RTTF Bomb pieces despawn once this runs out

#define ITTARUBOMB_HEALTH_MAX 10
#define ITTARUBOMB_EXPLODE_LIFETIME 6
#define ITTARUBOMB_EXPLODE_EFFECT_SCALE 1.4F
#define ITTARUBOMB_MUL_VEL_X 1.4F
#define ITTARUBOMB_VEL_MIN 0.1F			// Decrement lifetime if Barrel's XY velocity is less than this value
#define ITTARUBOMB_ROLL_ROTATE_MUL 0.0045F // Multiplies rotation of barrel
#define ITTARUBOMB_GRAVITY 4.0F
#define ITTARUBOMB_TVEL 90.0F
#define ITTARUBOMB_MAP_REBOUND_COMMON 0.5F

#define ITPKFIRE_LIFETIME 100
#define ITPKFIRE_HURT_DAMAGE_MUL 3 // Multiplies damage taken, result gets subtracted from lifetime
#if defined(REGION_US)
#define ITPKFIRE_GRAVITY 0.45F
#define ITPKFIRE_TVEL 55.0F
#else
#define ITPKFIRE_GRAVITY 0.4F
#define ITPKFIRE_TVEL 50.0F
#endif
#define ITPKFIRE_MAP_REBOUND_COMMON 0.2F
#define ITPKFIRE_MAP_REBOUND_GROUND 0.5F

#define ITLINKBOMB_HEALTH 7
#define ITLINKBOMB_LIFETIME 300
#define ITLINKBOMB_EXPLODE_LIFETIME 6
#define ITLINKBOMB_BLOAT_COLANIM_ID 0x4F   // Apply this color animation when Bomb begins to bloat
#define ITLINKBOMB_BLOAT_COLANIM_LENGTH 96 // Duration of bloat color animation
#define ITLINKBOMB_SCALE_INDEX_MAX 10	   // Maximum number of scale floats in array?
#define ITLINKBOMB_SCALE_INDEX_REWIND (ITLINKBOMB_SCALE_INDEX_MAX / 2) // When index reaches this value, rewind
#define ITLINKBOMB_SCALE_INT                                                                                           \
	4 // Interval between Bomb inflate/deflate animation updates; effectively
	  // animation speed
#define ITLINKBOMB_DAMAGE_RECOIL_VEL_X                                                                                 \
	20.0F // Horizontal knockback taken from hits dealing less than
		  // ITLINKBOMB_HEALTH can sustain
#define ITLINKBOMB_DAMAGE_RECOIL_VEL_Y                                                                                 \
	18.0F // Vertical knockback taken from hits dealing less than
		  // ITLINKBOMB_HEALTH can sustain
#define ITLINKBOMB_EXPLODE_THRESHOLD_VEL_X                                                                             \
	36.0F // If X-velocity is greater than this value when hitting a target,
		  // explode
#define ITLINKBOMB_EXPLODE_THRESHOLD_VEL_Y                                                                             \
	25.0F // If Y-velocity is greater than this value when hitting a target,
		  // explode
#define ITLINKBOMB_BLOAT_BEGIN 96.0F	  // Bomb begins bloating when lifetime is equal or less than this value
#define ITLINKBOMB_HIT_RECOIL_VEL_X 8.0F  // X-velocity upon hitting a target without exploding
#define ITLINKBOMB_HIT_RECOIL_VEL_Y 20.0F // Y-velocity upon hitting a target without exploding
#define ITLINKBOMB_EXPLODE_EFFECT_SCALE 1.3F
#define ITLINKBOMB_GRAVITY 1.2F
#define ITLINKBOMB_TVEL 100.0F
#define ITLINKBOMB_MAP_REBOUND_COMMON 0.4F
#define ITLINKBOMB_MAP_REBOUND_GROUND 0.3F

typedef struct ITCommonItemVarsBombHei
{
	u16 smoke_delay; // Frames before smoke effect is created when Bob-Omb
					 // begins walking?

} ITCommonItemVarsBombHei;

typedef struct ITCommonItemVarsShell
{
	u8 damage_all_delay; // Shell can hit owner and teammates once this frame
						 // timer reaches -1
	u8 dust_effect_int;	 // Delay between dust GFX
	u8 health;			 // Appears to determine whether Shell will despawn after hittin
						 // a target, shell can have up to 4 HP
	ub8 is_damage;		 // Shell can damage players
	ub8 is_setup_vars;
	u8 interact; // Decremented each time the shell is attacked, reflected, or
				 // it hits something; despawns shell at 0
	f32 vel_x;

} ITCommonItemVarsShell;

typedef struct ITCommonItemVarsTaru
{
	f32 roll_rotate_step;

} ITCommonItemVarsTaru;

typedef struct ITCommonItemVarsBumper
{
	u16 hit_anim_length; // Number of frames "hit" animation lasts after
						 // bouncing off a target in midair
	u16 unk_0x2;
	u16 damage_all_delay; // Bumper is able to hit its owner (and teammates?)
						  // once this timer runs out

} ITCommonItemVarsBumper;

typedef struct ITGroundItemVarsGLucky
{
	Vec3f pos;
	u16 egg_spawn_count;

} ITGroundItemVarsGLucky;

typedef struct ITCommonItemVarsMBall
{
	ub16 is_rebound;   // Set to TRUE when Poké Ball bounces back off a target
	GObj* owner_gobj;  // While Poké Balls can be reflected, ownership is not
					   // transfered; it is retained by the player who
					   // originally picked it up
	GObj* effect_gobj; // Poké Ball ray scatter GFX

} ITCommonItemVarsMBall;

typedef struct ITGroundItemVarsPakkun
{
	Vec3f pos;
	ub8 is_wait_fighter; // TRUE if a fighter is occupying the pipe

} ITGroundItemVarsPakkun;

typedef struct ITMonsterItemVarsIwark
{
	u16 rock_spawn_remain;
	s32 rock_spawn_wait; // Delay between rock projectiles
	u16 rock_spawn_max;
	u16 rumble_frame; // Used as a boolean, but not really a bool? This only
					  // checks if the variable is non-zero, but it keeps
					  // counting up.
	u16 rumble_wait;
	u16 rock_spawn_count; // Number of rocks to spawn?

} ITMonsterItemVarsIwark;

typedef struct ITMonsterItemVarsKabigon
{
	s32 dust_effect_int;
	s32 rumble_wait;

} ITMonsterItemVarsKabigon;

typedef struct ITMonsterItemVarsTosakinto
{
	Vec3f pos;

} ITMonsterItemVarsTosakinto;

typedef struct ITMonsterItemVarsMew
{
	s32 esper_gfx_int;

} ITMonsterItemVarsMew;

typedef struct ITMonsterItemVarsNyars
{
	u16 coin_spawn_wait;
	u16 coin_rotate_step;
	u16 model_rotate_wait;

} ITMonsterItemVarsNyars;

typedef struct ITMonsterItemVarsLizardon
{
	Vec3f pos;
	u16 turn_wait;
	u16 flame_spawn_wait;

} ITMonsterItemVarsLizardon;

typedef struct ITMonsterItemVarsSpear
{
	u16 spear_spawn_count;
	s32 spear_spawn_wait;
	f32 spear_spawn_pos_y;

} ITMonsterItemVarsSpear;

typedef struct ITMonsterItemVarsKamex
{
	s32 hydro_spawn_wait;
	f32 hydro_push_vel_x; // Added to Blastoise's velocity if is_apply_push is
						  // TRUE
	sb32 is_apply_push;	  // Blastoise will be pushed forward after each Hydro
						  // Pump stream if enabled

} ITMonsterItemVarsKamex;

typedef struct ITMonsterItemVarsMLucky
{
	u16 egg_spawn_wait;
	u16 lifetime;

} ITMonsterItemVarsMLucky;

typedef struct ITMonsterItemVarsStarmie
{
	s32 unk_0x0;
	s32 swift_spawn_wait;
	Vec3f target_pos; // Position Starmie is set to travel when released from
					  // its Poké Ball
	Vec3f victim_pos; // Position of Starmie's selected victim
	f32 add_vel_x;

} ITMonsterItemVarsStarmie;

typedef struct ITMonsterItemVarsDogas
{
	Vec3f pos;
	s32 smog_spawn_wait;

} ITMonsterItemVarsDogas;

typedef struct ITGroundItemVarsMarumine
{
	Vec3f offset;

} ITGroundItemVarsMarumine;

typedef struct ITGroundItemVarsPorygon
{
	Vec3f offset;

} ITGroundItemVarsPorygon;

typedef struct ITGroundItemVarsHitokage
{
	Vec3f offset;
	u16 flags;
	u16 flame_spawn_wait;

} ITGroundItemVarsHitokage;

typedef struct ITGroundItemVarsFushigibana
{
	Vec3f offset;
	u16 flags;
	u16 razor_spawn_wait;

} ITGroundItemVarsFushigibana;

typedef struct ITGroundItemVarsTaruBomb
{
	f32 roll_rotate_step;

} ITGroundItemVarsTaruBomb;

typedef struct ITFighterItemVarsPKFire
{
	LBTransform *xf;

} ITFighterItemVarsPKFire;

typedef struct ITFighterItemVarsLinkBomb
{
	u16 unk_0x0;
	u16 drop_update_wait; // Wait frames when dropping bomb, must pass before
						  // ProcUpdate, ProcHit and ProcDamage can be checked?
	u16 scale_id;
	u16 scale_int; // Interval between Bomb inflate/deflate animation updates;
				   // effectively animation speed

} ITFighterItemVarsLinkBomb;

#endif

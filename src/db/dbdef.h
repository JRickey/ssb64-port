#ifndef _DBDEF_H_
#define _DBDEF_H_

typedef enum DBMenuOptionKind
{
	nDBMenuOptionKindExitLabel,        // Label which exits menu when selected
	nDBMenuOptionKindNumeric,          // Number, 4 bytes
	nDBMenuOptionKindDouble,           // Double
	nDBMenuOptionKindString,           // String
	nDBMenuOptionKindLabel,            // Label
	nDBMenuOptionKindNumericByte,      // Number, 1 byte
	nDBMenuOptionKindStringByte        // String, 1 byte

} DBMenuOptionKind;

typedef enum DBBattleScene
{
	nDBBattleSceneVSMode,
	nDBBattleScene1PMode,
	nDBBattleSceneStaffroll,
	nDBBattleSceneExplain,
	nDBBattleSceneAutoDemo,
	nDBBattleSceneCongra

} DBBattleScene;

typedef struct DBFontPadding			DBFontPadding;
typedef struct DBFont					DBFont;
typedef struct DBMenu		            DBMenu;
typedef struct DBMenuOption             DBMenuOption;
typedef struct DBMenuPosition           DBMenuPosition;
typedef struct DBFighter                DBFighter;
typedef struct DBTextBuffer				DBTextBuffer;

#endif

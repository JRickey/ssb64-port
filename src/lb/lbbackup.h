#ifndef _LBBACKUP_H_
#define _LBBACKUP_H_

#include <ssb_types.h>
#include <sys/objdef.h>
#include <lb/lbdef.h>

extern s32 lbBackupCreateChecksum(LBBackupData *backup);
extern sb32 lbBackupIsChecksumValid(void);
extern void lbBackupWrite(void);
extern sb32 lbBackupIsSramValid(void);
extern void lbBackupApplyOptions(void);
extern void lbBackupCorrectErrors(void);
extern void lbBackupClearNewcomers(void);
extern void lbBackupClear1PHighScore(void);
extern void lbBackupClearVSRecord(void);
extern void lbBackupClearBonusStageTime(void);
extern void lbBackupClearPrize(void);
extern void lbBackupClearAllData(void);

#endif

#include "common.h"
#include <n_audio/n_libaudio.h>

// 0x8002F6C0
s16 n_alSeqpGetVol(N_ALSeqPlayer* seqp)
{
    return (seqp->vol * seqp->masterVol) >> 7;
}
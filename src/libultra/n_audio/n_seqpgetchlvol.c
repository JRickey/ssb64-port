#include "common.h"
#include <n_audio/n_libaudio.h>

// 0x8002FDE0
u8 n_alSeqpGetChlVol(N_ALSeqPlayer* seqp, u8 chan)
{
    return (seqp->chanState[chan].vol * seqp->chanState[chan].vol2) / 127;
}
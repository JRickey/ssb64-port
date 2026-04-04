/*====================================================================
 *
 * Copyright 1993, Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Silicon Graphics,
 * Inc.; the contents of this file may not be disclosed to third
 * parties, copied or duplicated in any form, in whole or in part,
 * without the prior written permission of Silicon Graphics, Inc.
 *
 * RESTRICTED RIGHTS LEGEND:
 * Use, duplication or disclosure by the Government is subject to
 * restrictions as set forth in subdivision (c)(1)(ii) of the Rights
 * in Technical Data and Computer Software clause at DFARS
 * 252.227-7013, and/or in similar or successor clauses in the FAR,
 * DOD or NASA FAR Supplement. Unpublished - rights reserved under the
 * Copyright Laws of the United States.
 *====================================================================*/

#include <PR/libaudio.h>
#include <n_audio/n_libaudio.h>

// Needs -O3
void n_alCSPSetBank(N_ALCSPlayer *seqp, ALBank *b)
{
	N_ALEvent evt;

	evt.type = AL_SEQP_BANK_EVT;
	evt.msg.spbank.bank = b;
	n_alEvtqPostEvent(&seqp->evtq, &evt, 0);
}

void n_alCSPSetBank_Alt(N_ALCSPlayer *seqp, ALBank *b, u8 arg2)
{
	N_ALEvent evt;

	if (arg2 == 0)
	{
		evt.type = AL_SEQP_BANK_EVT;
		evt.msg.spbank.bank = b;
		n_alEvtqPostEvent(&seqp->evtq, &evt, 0);
	}
	else if (arg2 == 1)
	{
		evt.type = 0x18;
		evt.msg.spbank.bank = b;
		n_alEvtqPostEvent(&seqp->evtq, &evt, 0);
	}
	else if (arg2 == 2)
	{
		evt.type = 0x19;
		evt.msg.spbank.bank = b;
		n_alEvtqPostEvent(&seqp->evtq, &evt, 0);
	}
}
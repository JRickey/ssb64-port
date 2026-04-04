#include <PR/os_internal.h>
#include <n_audio/n_synthInternals.h>

void n_alSynAddPlayer(ALPlayer *client)
{
	OSIntMask mask = osSetIntMask(OS_IM_NONE);

	client->samplesLeft = n_syn->curSamples;

	client->next = n_syn->head;
	n_syn->head = client;

	osSetIntMask(mask);
}

void n_alSynAddSndPlayer(ALPlayer *client)
{
	OSIntMask mask = osSetIntMask(OS_IM_NONE);

	client->samplesLeft = n_syn->curSamples;

	client->next = n_syn->head;
	n_syn->head = client;
    
    if (n_syn->n_sndp == NULL) {
        n_syn->n_sndp = client;
    }

	osSetIntMask(mask);
}

void n_alSynAddSeqPlayer(ALPlayer *client)
{
	OSIntMask mask = osSetIntMask(OS_IM_NONE);

	client->samplesLeft = n_syn->curSamples;

	client->next = n_syn->head;
	n_syn->head = client;

    if (n_syn->n_seqp1 == NULL) {
        n_syn->n_seqp1 = client;
    } else if (n_syn->n_seqp2 == NULL) {
        n_syn->n_seqp2 = client;
    }

	osSetIntMask(mask);
}

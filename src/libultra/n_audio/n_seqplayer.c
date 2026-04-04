#include "common.h"
#include <n_audio/n_libaudio.h>

void __n_seqpStopOsc(N_ALSeqPlayer *seqp, N_ALVoiceState *vs);
void __n_initChanState(N_ALSeqPlayer *seqp);
void __n_postNextSeqEvent(N_ALSeqPlayer *seqp);
void __n_setInstChanState(N_ALSeqPlayer *seqp, ALInstrument *inst, s32 chan);
void __n_resetPerfChanState(N_ALSeqPlayer *seqp, s32 chan);
char __n_voiceNeedsNoteKill (N_ALSeqPlayer *seqp, N_ALVoice *voice, ALMicroTime killTime);
void __n_initFromBank(N_ALSeqPlayer* seqp, ALBank *b);

#define KILL_TIME	50000	/* 50 ms */
#define VOICENEEDSNOTEKILL_DEBUG	_DEBUG_INTERNAL&&0	/* For debugging voiceNeedsNoteKill routine. */

static  ALMicroTime     __n_seqpVoiceHandler(void *node);
static  void            __n_HandleMIDIMsg(N_ALSeqPlayer *seqp, N_ALEvent *event);
static  void            __n_handleMetaMsg(N_ALSeqPlayer *seqp, N_ALEvent *event);
static	void		__n_handleNextSeqEvent(N_ALSeqPlayer *seqp);
static	void		__n_setUsptFromTempo(N_ALSeqPlayer *seqp, f32 tempo);	/* sct 1/8/96 */


// Start csplayer.c?
// 0x8002F51C
void n_alSeqpNew(N_ALSeqPlayer *seqp, ALSeqpConfig *c)
{
    s32                 i;
    N_ALEventListItem     *items;
    N_ALVoiceState        *vs;
    N_ALVoiceState        *voices;
    
    ALHeap *hp = c->heap;    

    /*
     * initialize member variables
     */
    seqp->bank          = 0;
    seqp->unknown0    = 0;
    seqp->unknown1    = 0;
    seqp->target        = NULL;
    seqp->drvr          = n_syn;
    seqp->chanMask      = 0xff;
    seqp->uspt          = 488;
    seqp->nextDelta     = 0;
    seqp->state         = AL_STOPPED;
    seqp->vol           = 0x7FFF;              /* full volume  */
    seqp->debugFlags    = c->debugFlags;
    seqp->frameTime     = AL_USEC_PER_FRAME; /* should get this from driver */
    seqp->curTime    = 0;
    seqp->initOsc       = c->initOsc;     
    seqp->updateOsc     = c->updateOsc;   
    seqp->stopOsc       = c->stopOsc;
    seqp->loopStart   = 0;
    seqp->loopEnd   = 0;
    seqp->loopCount   = 0;

    seqp->nextEvent.type = AL_SEQP_API_EVT;	/* this will start the voice handler "spinning" */

    /*
     * init the channel state
     */
    seqp->maxChannels = c->maxChannels;
    seqp->chanState = alHeapAlloc(hp, c->maxChannels, sizeof(ALChanState) );
    __n_initChanState((N_ALSeqPlayer*)seqp);	/* sct 11/6/95 */
    
    /*
     * init the voice state array
     */
    voices = alHeapAlloc(hp, c->maxVoices, sizeof(N_ALVoiceState));
    seqp->vFreeList = 0;
    for (i = 0; i < c->maxVoices; i++) {
        vs = &voices[i];
        vs->next = seqp->vFreeList;
        seqp->vFreeList = vs;        
    }
    
    seqp->vAllocHead = 0;
    seqp->vAllocTail = 0;    
#if BUILD_VERSION < VERSION_J
#line 109
#endif
    // init the event queue
    items = alHeapAlloc(hp, c->maxEvents, sizeof(N_ALEventListItem));
    n_alEvtqNew(&seqp->evtq, items, c->maxEvents);

    /*
     * add ourselves to the driver
     */
    seqp->node.next       = NULL;
    seqp->node.handler    = __n_seqpVoiceHandler;
    seqp->node.clientData = seqp;
    n_alSynAddSeqPlayer(&seqp->node);  
}

/*
  sct 11/6/95 - Called only when creating a new sequence player.
*/
// 0x8002F4A0
void __n_initChanState(N_ALSeqPlayer *seqp)
{
    int i;

    for (i = 0; i < seqp->maxChannels; i++)
    {
        seqp->chanState[i].instrument = 0;
//////// Only in Smash Bros.
        seqp->chanState[i].unk_0x10 = 0;
////////
	__n_resetPerfChanState (seqp, i);
    }
}


// void func_8002DF1C_2EB1C(void)
// {
// 	return;
// }

// 0x8002DF24
ALMicroTime __n_vsDelta(N_ALVoiceState *vs, ALMicroTime t)
{
	/*
	 * If we are interrupting a previously set envelope segment, we
	 * need to recalculate the segment end time given the current
	 * time. Note: this routine assumes that the voice is currently
	 * playing.
	 */

	s32 delta = vs->envEndTime - t;

	if (delta >= 0)
		return delta;
	else
		return AL_GAIN_CHANGE_TIME;
}

// 0x8002DFF8
void __n_seqpReleaseVoice(N_ALSeqPlayer *seqp, N_ALVoice *voice, ALMicroTime deltaTime)
{
    N_ALEvent                 evt;
    N_ALVoiceState	        *vs = (N_ALVoiceState *)voice->clientPrivate;

    /*
     * if in attack phase, remove all pending volume
     * events for this voice from the queue
     */

    if (vs->envPhase == AL_PHASE_ATTACK) {
    	ALLink              *thisNode;
    	ALLink              *nextNode;
    	N_ALEventListItem     *thisItem, *nextItem;

    	thisNode = seqp->evtq.allocList.next;
    	while( thisNode != 0 ) {
    	    nextNode = thisNode->next;
    	    thisItem = (N_ALEventListItem *)thisNode;
    	    nextItem = (N_ALEventListItem *)nextNode;
    	    if (thisItem->evt.type == AL_SEQP_ENV_EVT) {
				if(thisItem->evt.msg.vol.voice == voice) {
					if( nextItem )
						nextItem->delta += thisItem->delta;
					alUnlink(thisNode);
					alLink(thisNode, &seqp->evtq.freeList);
				}
    	    }
    	    thisNode = nextNode;
    	}
    }

    vs->velocity = 0;
    vs->envPhase = AL_PHASE_RELEASE;
    vs->envGain  = 0;
    vs->envEndTime = seqp->curTime + deltaTime;

    n_alSynSetPriority(voice, 0); /* make candidate for stealing */
    n_alSynSetVol(voice, 0, deltaTime);
    evt.type  = AL_NOTE_END_EVT;
    evt.msg.note.voice = voice;

    n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime);
}

/*
 * __n_vsVol calculates the target volume for the voice based on the
 * note on velocity, envelope, sampleVolume and controller.
 */
// 0x8002DF48
s16 __n_vsVol(N_ALVoiceState *vs, N_ALSeqPlayer *seqp)
{

    u32     t1,t2;

    t1 = (vs->tremelo * vs->velocity * vs->envGain) >> 6;
    t2 = (vs->sound->sampleVolume * n_alSeqpGetVol(seqp) * n_alSeqpGetChlVol(seqp, vs->channel)) >> 14;

    t1 *= t2;
    t1 >>= 15;

    return( (s16)t1 );

}

// 0x8002E1C4
void __n_unmapVoice(N_ALSeqPlayer *seqp, N_ALVoice *voice)
{
	N_ALVoiceState *prev = 0;
	N_ALVoiceState *vs;

	/*
	 * we could use doubly linked lists here and save some code and
	 * execution time, but time spent here in negligible, so it won't
	 * make much difference.
	 */
	for (vs = seqp->vAllocHead; vs != 0; vs = vs->next)
	{
		if (&vs->voice == voice)
		{
			if (prev)
				prev->next = vs->next;
			else
				seqp->vAllocHead = vs->next;

			if (vs == seqp->vAllocTail)
				seqp->vAllocTail = prev;

			vs->next = seqp->vFreeList;
			seqp->vFreeList = vs;
			return;
		}
		prev = vs;
	}
#ifdef _DEBUG
	__osError(ERR_ALSEQPUNMAP, 1, voice);
#endif
}

// 0x8002E234
// -O3: https://decomp.me/scratch/0ProW
// __n_handleMetaMsg?
static void __n_handleMetaMsg(N_ALSeqPlayer* seqp, N_ALEvent* event) 
{
    ALTempoEvent    *tevt = &event->msg.tempo;
    N_ALEvent         evt;
    s32             tempo;
    
    if (event->msg.tempo.status == AL_MIDI_Meta)
    {
        if (event->msg.tempo.type == AL_MIDI_META_TEMPO) 
        {
            tempo = 
                (tevt->byte1 << 16) | 
                (tevt->byte2 <<  8) | 
                (tevt->byte3 <<  0);
            __n_setUsptFromTempo (seqp, (f32)tempo);	/* sct 1/8/96 */
        }
    }
}

// 0x8002E2AC
/*
  sct 11/6/95 - Call this whenever a new instrument gets assigned to a channel
  such as when changing banks or in response to a MIDI program change event.
  Currently also gets called when changing sequences.
*/
// 0x8002E2AC
void __n_setInstChanState_Alt(N_ALSeqPlayer *seqp, ALInstrument *inst, s32 chan)
{
    seqp->chanState[chan].instrument = inst;
    // seqp->chanState[chan].pan = inst->pan;
    // seqp->chanState[chan].vol = inst->volume;
    seqp->chanState[chan].priority = inst->priority;
    seqp->chanState[chan].bendRange = inst->bendRange;
    seqp->chanState[chan].vol2 = inst->volume;
}

// 0x8002E2F8
ALPan __n_vsPan(N_ALVoiceState *vs, N_ALSeqPlayer *seqp)
{
    s32 tmp;

    tmp = seqp->chanState[vs->channel].pan - AL_PAN_CENTER +
        vs->sound->samplePan;
    tmp = MAX(tmp, AL_PAN_LEFT);
    tmp = MIN(tmp, AL_PAN_RIGHT);

    return (ALPan) tmp;
}

// 0x8002E348
N_ALVoiceState *__n_lookupVoice(N_ALSeqPlayer *seqp, u8 key, u8 channel)
{
	N_ALVoiceState  *vs;

	for (vs = seqp->vAllocHead; vs != 0; vs = vs->next)
	{
		if ((vs->key == key) && (vs->channel == channel) &&
			(vs->phase != AL_PHASE_RELEASE) && (vs->phase != AL_PHASE_SUSTREL))
			return vs;
	}

	return 0;
}

// 0x8002E3B8
N_ALVoiceState* __n_mapVoice(N_ALSeqPlayer *seqp, u8 key, u8 vel, u8 channel)
{
	N_ALVoiceState  *vs = seqp->vFreeList;

	if (vs)
	{
		seqp->vFreeList = vs->next;

		vs->next = 0;

		if (!seqp->vAllocHead)
			seqp->vAllocHead = vs;
		else
			seqp->vAllocTail->next = vs;

		seqp->vAllocTail = vs;

		vs->channel             = channel;
		vs->key                 = key;
		vs->velocity            = vel;
		vs->voice.clientPrivate = vs;
	}

	return vs;
}

// 0x8002E41C
// -O3: https://decomp.me/scratch/vkETz
ALSound *__n_lookupSoundQuick(N_ALSeqPlayer *seqp, u8 key, u8 vel, u8 chan)
{
    ALInstrument *inst = seqp->chanState[chan].instrument;
    s32 l = 1;
    s32 r = inst->soundCount;
    s32 i;
    ALKeyMap *keymap;
#if BUILD_VERSION < VERSION_J
#line 885
#endif
    // assert(inst != NULL);       /* sct 10/31/95 - If inst is NULL, then the seqp probably wasn't setup correctly. */


    while (r >= l) {
        i = (l+r)/2;

        keymap = inst->soundArray[i-1]->keyMap;

        if ((key >= keymap->keyMin) && (key <= keymap->keyMax) &&
            (vel >= keymap->velocityMin) && (vel <= keymap->velocityMax)) {
            return inst->soundArray[i-1];
        } else if ((key < keymap->keyMin) ||
                   ((vel < keymap->velocityMin) && (key <= keymap->keyMax))) {
            r = i - 1;
        } else {
            l = i + 1;
        }
    }

    return 0;
}

// 0x8002E520
// __n_HandleMIDIMsg
// static
static void 
__n_HandleMIDIMsg(N_ALSeqPlayer *seqp, N_ALEvent *event)
{
    N_ALVoice             *voice;
    N_ALVoiceState        *vs;
    s32                 status;
    u8                  chan;
    u8                  key;
    u8                  vel;
    u8                  byte1;
    u8                  byte2;
    ALMIDIEvent         *midi = &event->msg.midi;
    s16                 vol;
    N_ALEvent             evt;
    ALMicroTime         deltaTime;
    N_ALVoiceState        *vstate;
    ALPan   		    pan;
    ALFxRef		        fxref;

#ifdef _DEBUG
    /* sct 12/15/95 - Fixed assert to also allow seqp midi event types. */
    assert(event->type == AL_SEQ_MIDI_EVT || event->type == AL_SEQP_MIDI_EVT);
#endif
    
    status = midi->status & AL_MIDI_StatusMask;
    chan = midi->status & AL_MIDI_ChannelMask;
    byte1 = key  = midi->byte1;
    byte2 = vel  = midi->byte2;

    switch (status) {
        case (AL_MIDI_NoteOn):
            
            if (vel != 0) {		/* a real note on */
                ALVoiceConfig   config;
                ALSound         *sound;                    
                s16             cents;
                f32             pitch,oscValue;
                u8              fxmix;
                void            *oscState;
                ALInstrument    *inst;

		/* If we're not playing, don't process note ons. */
		if (seqp->state != AL_PLAYING)
		    break;

                sound = __n_lookupSoundQuick(seqp, key, vel, chan);
                ALFlagFailIf(!sound, seqp->debugFlags & NO_SOUND_ERR_MASK,
			   ERR_ALSEQP_NO_SOUND);
                
                config.priority = seqp->chanState[chan].priority;
                config.fxBus    = 0;
                config.unityPitch = 0;

                vstate = __n_mapVoice(seqp, key, vel, chan);
                ALFlagFailIf(!vstate, seqp->debugFlags & NO_VOICE_ERR_MASK,
			 ERR_ALSEQP_NO_VOICE );

                voice = &vstate->voice;
                
                n_alSynAllocVoice(voice, &config);

                /*
                 * set up the voice state structure
                 */
                vstate->sound = sound;
                vstate->envPhase = AL_PHASE_ATTACK;
                if (seqp->chanState[chan].sustain > AL_SUSTAIN)
                    vstate->phase = AL_PHASE_SUSTAIN;
                else
                    vstate->phase = AL_PHASE_NOTEON;
                
                cents = (key - sound->keyMap->keyBase) * 100
                    + sound->keyMap->detune;
                
                vstate->pitch = alCents2Ratio(cents);
                vstate->envGain = sound->envelope->attackVolume;
                vstate->envEndTime = seqp->curTime +
                    sound->envelope->attackTime;

                /*
                 * setup tremelo and vibrato if active
                 */
                vstate->flags = 0;
                inst = seqp->chanState[chan].instrument;

                oscValue = (f32)AL_VOL_FULL; /* set this as a default */

                if(inst->tremType)
                {
                    if(seqp->initOsc)
                    {
                        deltaTime = (*seqp->initOsc)(&oscState,&oscValue,
                                                     inst->tremType,
                                                     inst->tremRate,
                                                     inst->tremDepth,
                                                     inst->tremDelay);

                        if(deltaTime) /* if deltaTime = zero, don't run osc */
                        {
                            evt.type = AL_TREM_OSC_EVT;
                            evt.msg.osc.vs = vstate;
                            evt.msg.osc.oscState = oscState;
                            n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime);
                            vstate->flags |= 0x01; /* set tremelo flag bit */
                        }
                    }
                }
                /* will default if not changed by initOsc */
                vstate->tremelo = (u8)oscValue;
                
                oscValue = 1.0f; /* set this as a default */
                if(inst->vibType)
                {
                    if(seqp->initOsc)
                    {
                        deltaTime = (*seqp->initOsc)(&oscState,&oscValue,
                                                     inst->vibType,
                                                     inst->vibRate,
                                                     inst->vibDepth,
                                                     inst->vibDelay);

                        if(deltaTime)  /* if deltaTime = zero,don't run osc. */
                        {
                            evt.type = AL_VIB_OSC_EVT;
                            evt.msg.osc.vs = vstate;
                            evt.msg.osc.oscState = oscState;
                            evt.msg.osc.chan = chan;
                            n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime);
                            vstate->flags |= 0x02; /* set the vibrato flag bit */
                        }
                    }
                }
                /* will default if not changed by initOsc */
                vstate->vibrato = oscValue;

                /*
                 * calculate the note on parameters
                 */
                pitch = vstate->pitch * seqp->chanState[chan].pitchBend *
                    vstate->vibrato;
                fxmix = seqp->chanState[chan].fxmix;
                pan   = __n_vsPan(vstate, seqp);
                vol   = __n_vsVol(vstate, seqp);
                deltaTime  = sound->envelope->attackTime;

                n_alSynStartVoiceParams(voice, sound->wavetable,
                                      pitch, vol, pan, fxmix, deltaTime);
                /*
                 * set up callbacks for envelope
                 */
                evt.type          = AL_SEQP_ENV_EVT;
                evt.msg.vol.voice = voice;
                evt.msg.vol.vol   = sound->envelope->decayVolume;
                evt.msg.vol.delta = sound->envelope->decayTime;
                deltaTime = sound->envelope->attackTime;
                
                n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime);

                break;
            }

            /*
             * NOTE: intentional fall-through for note on with zero
             * velocity
             */
            
        case (AL_MIDI_NoteOff):
            vstate = __n_lookupVoice(seqp, key, chan);
            ALFlagFailIf(!vstate, (seqp->debugFlags & NOTE_OFF_ERR_MASK),
		       ERR_ALSEQP_OFF_VOICE );

            if (vstate->phase == AL_PHASE_SUSTAIN)
                vstate->phase = AL_PHASE_SUSTREL;
            else {
                vstate->phase = AL_PHASE_RELEASE;
                __n_seqpReleaseVoice(seqp, &vstate->voice,
				   vstate->sound->envelope->releaseTime);
            }

            break;
            
        case (AL_MIDI_PolyKeyPressure):
	    /*
	     * Aftertouch per key (hardwired to volume). Note that
	     * aftertouch affects only notes that are already
	     * sounding.
             */
	    vstate = __n_lookupVoice(seqp, key, chan);
	    ALFailIf(!vstate,  ERR_ALSEQP_POLY_VOICE );

	    vstate->velocity = byte2;
	    n_alSynSetVol(&vstate->voice, __n_vsVol(vstate, seqp),
			__n_vsDelta(vstate, seqp->curTime));
	    break;
        case (AL_MIDI_ChannelPressure):
#if 0
	    /*
             * Aftertouch per channel (hardwired to volume). Note that
             * aftertouch affects only notes that are already
             * sounding.
             */
	    for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
    		if (vs->channel == chan) {
    		    vs->velocity = byte1;
    		    n_alSynSetVol(&vs->voice, __n_vsVol(vs, seqp),
    				__n_vsDelta(vs, seqp->curTime));
    		}
	    }
#endif
            seqp->chanState[chan].unk_0x10 = byte1;
            break;
            
        case (AL_MIDI_ControlChange):

	    switch (byte1) {
		  
	    case (AL_MIDI_PAN_CTRL):
		seqp->chanState[chan].pan = byte2;
		for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
		    if (vs->channel == chan) {
			pan = __n_vsPan(vs, seqp);
			n_alSynSetPan(&vs->voice, pan);
		    }
		}
		break;

	    case (AL_MIDI_VOLUME_CTRL):
		seqp->chanState[chan].vol = byte2;
		for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
		    if ((vs->channel == chan) &&
			(vs->envPhase != AL_PHASE_RELEASE))
		    {
			vol = __n_vsVol(vs, seqp);
			n_alSynSetVol(&vs->voice, vol,
				    __n_vsDelta(vs, seqp->curTime));
		    }
		}
		break;
            case (AL_MIDI_PRIORITY_CTRL):
		/* leave current voices where they are */
		seqp->chanState[chan].priority = byte2;
		break;
	    case (AL_MIDI_SUSTAIN_CTRL):
		seqp->chanState[chan].sustain = byte2;
		for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
		    if ((vs->channel == chan) &&
			(vs->phase != AL_PHASE_RELEASE)) {
			if ( byte2 > AL_SUSTAIN ) {
			    /*
			     * sustain pedal down
			     */
			    if (vs->phase == AL_PHASE_NOTEON)
				vs->phase = AL_PHASE_SUSTAIN;
			} else {
			    /*
			     * sustain pedal up
			     */
			    if (vs->phase == AL_PHASE_SUSTAIN)
				vs->phase = AL_PHASE_NOTEON;
			    else if(vs->phase == AL_PHASE_SUSTREL) {
				vs->phase = AL_PHASE_RELEASE;
				__n_seqpReleaseVoice(seqp, &vs->voice,
						   vs->sound->envelope->releaseTime);
			    }
			}
		    }
		}
		break;
	    
	    case (AL_MIDI_FX1_CTRL):
		seqp->chanState[chan].fxmix = byte2;
		for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
		    if (vs->channel == chan) {
			n_alSynSetFXMix(&vs->voice, byte2);
		    }
		}
		break;

	    case (AL_MIDI_FX_CTRL_0):
            if (byte2 >= 0x79) {
		        seqp->chanState[chan].bendRange = 0x4B0;
            } else {
		        seqp->chanState[chan].bendRange = byte2 * 10;
            }
            break;
	    case (AL_MIDI_FX_CTRL_1):
            seqp->masterVol = byte2;
            break;
	    case (AL_MIDI_FX_CTRL_2):
	    case (AL_MIDI_FX_CTRL_3):
	    case (AL_MIDI_FX_CTRL_4):
	    case (AL_MIDI_FX_CTRL_5):
	    case (AL_MIDI_FX_CTRL_6):
	    case (AL_MIDI_FX_CTRL_7):
#if 0	/* fx control not implemented */      
		fxref = n_alSynGetFXRef(0, 0);
		if (fxref)
		    n_alSynSetFXParam(fxref, (s16)byte1, (void *)byte2);
		break;
#endif
	    case (AL_MIDI_FX3_CTRL):
	    default:
		break;

	    }
            break;

        case (AL_MIDI_ProgramChange):
	    /* sct 1/16/96 - We must have a valid bank in order to process the program change. */

#ifdef _DEBUG
	    assert(seqp->bank != NULL);
#endif

            if (key < seqp->bank->instCount) {
                ALInstrument *inst = seqp->bank->instArray[key];
                __n_setInstChanState_Alt(seqp, inst, chan);        /* sct 11/6/95 */
            }
#ifdef _DEBUG
	    else
		__osError(ERR_ALSEQPINVALIDPROG, 2, key, seqp->bank->instCount);
#endif                
            break;
            
        case (AL_MIDI_PitchBendChange):
	    {
		s32 bendVal;
		f32 bendRatio;
		s32 cents;
                
		/*
                 * get 14-bit unsigned midi value
                 */
                bendVal = ( (byte2 << 7) + byte1) - 8192;

                /*
                 * calculate pitch bend in cents
                 */
                cents = (seqp->chanState[chan].bendRange * bendVal)/8192;

                /*
                 * calculate the corresponding ratio
                 */
                bendRatio = alCents2Ratio(cents);
                seqp->chanState[chan].pitchBend = bendRatio;
                
                for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                    if (vs->channel == chan) {
                        n_alSynSetPitch(&vs->voice,
                                      vs->pitch * bendRatio * vs->vibrato);
                    }
                }
            }
            break;
            
        default:
#ifdef _DEBUG
	    __osError(ERR_ALSEQPUNKNOWNMIDI, 1, status);
#endif
	    break;
    }        
}

static void __n_setUsptFromTempo (N_ALSeqPlayer *seqp, f32 tempo)
{
    if (seqp->target)
	seqp->uspt = (s32)((f32)tempo * seqp->target->qnpt);
    else
	seqp->uspt = 488;
}

// 0x8002ED54
// __n_seqpVoiceHandler
// static
ALMicroTime 
__n_seqpVoiceHandler(void *node)
{
    N_ALSeqPlayer     *seqp = (N_ALSeqPlayer *) node;
    N_ALEvent         evt;
    N_ALVoice         *voice;
    ALMicroTime     delta;
    N_ALVoiceState    *vs;
    void            *oscState;
    f32		    oscValue;
    u8              chan;
    
    do {

	switch (seqp->nextEvent.type) {

	  case (AL_SEQ_REF_EVT):
	      __n_handleNextSeqEvent(seqp);
              break;

	  case (AL_SEQP_API_EVT):
              evt.type = AL_SEQP_API_EVT;
              n_alEvtqPostEvent(&seqp->evtq, (N_ALEvent *)&evt, seqp->frameTime);
              break;

	  case (AL_NOTE_END_EVT):
              voice = seqp->nextEvent.msg.note.voice;
              n_alSynStopVoice(voice);
              n_alSynFreeVoice(voice);
              vs = (N_ALVoiceState *)voice->clientPrivate;
              if(vs->flags)
                  __n_seqpStopOsc((N_ALSeqPlayer*)seqp,vs);
              __n_unmapVoice(seqp, voice);
              break;

	  case (AL_SEQP_ENV_EVT):
              voice = seqp->nextEvent.msg.vol.voice;
              vs = (N_ALVoiceState *)voice->clientPrivate;
                
              if (vs->envPhase == AL_PHASE_ATTACK)
                  vs->envPhase = AL_PHASE_DECAY;

              delta = seqp->nextEvent.msg.vol.delta;
              vs->envGain = seqp->nextEvent.msg.vol.vol;
              vs->envEndTime = seqp->curTime + delta;
              n_alSynSetVol(voice, __n_vsVol(vs, seqp), delta);
              break;

	  case (AL_TREM_OSC_EVT):
              vs = seqp->nextEvent.msg.osc.vs;
              oscState = seqp->nextEvent.msg.osc.oscState;
              delta = (*seqp->updateOsc)(oscState,&oscValue);
              vs->tremelo = (u8)oscValue;
              n_alSynSetVol(&vs->voice, __n_vsVol(vs,seqp),
                          __n_vsDelta(vs,seqp->curTime));
              evt.type = AL_TREM_OSC_EVT;
              evt.msg.osc.vs = vs;
              evt.msg.osc.oscState = oscState;
              n_alEvtqPostEvent(&seqp->evtq, &evt, delta);
              break;
                
	  case (AL_VIB_OSC_EVT):
              vs = seqp->nextEvent.msg.osc.vs;
              oscState = seqp->nextEvent.msg.osc.oscState;
              chan = seqp->nextEvent.msg.osc.chan;
              delta = (*seqp->updateOsc)(oscState,&oscValue);
              vs->vibrato = oscValue;
              n_alSynSetPitch(&vs->voice, vs->pitch * (((vs->vibrato - 1.0f)
                            * seqp->chanState[chan].unk_0x10) / 127.0f + 1.0f) * seqp->chanState[chan].pitchBend);
              evt.type = AL_VIB_OSC_EVT;
              evt.msg.osc.vs = vs;
              evt.msg.osc.oscState = oscState;
              evt.msg.osc.chan = chan;
              n_alEvtqPostEvent(&seqp->evtq, &evt, delta);
              break;
                
	  case (AL_SEQP_MIDI_EVT):
              __n_HandleMIDIMsg(seqp, &seqp->nextEvent);
              break;

	  case (AL_SEQP_META_EVT):
              __n_handleMetaMsg(seqp, &seqp->nextEvent);
              break;
              
	  case (AL_SEQP_PLAY_EVT):
	      if (seqp->state != AL_PLAYING)
	      {
	          seqp->state = AL_PLAYING;
	          __n_postNextSeqEvent(seqp);	/* seqp must be AL_PLAYING before we call this routine. */
	      }
	      break;

	  case (AL_SEQP_STOP_EVT):
              if ( seqp->state == AL_STOPPING )
	      {
                  for (vs = seqp->vAllocHead; vs != 0; vs = seqp->vAllocHead)
                  {
                      n_alSynStopVoice(&vs->voice);
                      n_alSynFreeVoice(&vs->voice);
                      if(vs->flags)
                          __n_seqpStopOsc((N_ALSeqPlayer*)seqp,vs);
                      __n_unmapVoice((N_ALSeqPlayer*)seqp, &vs->voice); 
                  }

                  seqp->curTime = 0;
                  seqp->state = AL_STOPPED;

                  /* alEvtqFlush(&seqp->evtq); - Don't flush event
                     queue anymore. */
		  /* sct 1/3/96 - Don't overwrite nextEvent with
		     AL_SEQP_API_EVT or set nextDelta to
		     AL_USEC_PER_FRAME since we're not stopping event
		     processing. */
		  /* sct 1/3/96 - Don't return here since we keep
                     processing events as usual. */
              }
              break;

	  case (AL_SEQP_STOPPING_EVT):
              if (seqp->state == AL_PLAYING)
	      {
		  /*
                   * sct 12/29/95 - Remove events associated with the
                   * stopping sequence.  Note that flushing
                   * AL_SEQP_MIDI_EVTs may flush events that were
                   * posted after the call to alSeqpStop, so the
                   * application must queue these events either when
                   * the player is fully stopped, or when it is
                   * playing.
                   */
		  n_alEvtqFlushType(&seqp->evtq, AL_SEQ_REF_EVT);
		  n_alEvtqFlushType(&seqp->evtq, AL_SEQP_MIDI_EVT);	

		  /*
                   * sct 1/3/96 - Check to see which voices need to be
                   * killed and release them.  Unkilled voices should
                   * have note end events occurring prior to
                   * KILL_TIME.
                   */
                  for (vs = seqp->vAllocHead; vs != 0; vs = vs->next)
		  {
		      if (__n_voiceNeedsNoteKill (seqp, &vs->voice, KILL_TIME))
			  __n_seqpReleaseVoice(seqp, &vs->voice, KILL_TIME);
		  }

                  seqp->state = AL_STOPPING;
                  evt.type = AL_SEQP_STOP_EVT;
                  n_alEvtqPostEvent(&seqp->evtq, &evt, AL_EVTQ_END);
              }
              break;

	  case (AL_SEQP_VOL_EVT):
              seqp->vol = seqp->nextEvent.msg.spvol.vol;
              for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                  n_alSynSetVol(&vs->voice, __n_vsVol(vs, seqp),
                              __n_vsDelta(vs, seqp->curTime));
              }
              break;
            
	  case (AL_SEQP_LOOP_EVT):
              seqp->loopStart = seqp->nextEvent.msg.loop.start;
              seqp->loopEnd   = seqp->nextEvent.msg.loop.end;
              seqp->loopCount = seqp->nextEvent.msg.loop.count;
              break;

	  case (AL_SEQP_PRIORITY_EVT):
	      chan = seqp->nextEvent.msg.sppriority.chan;
	      seqp->chanState[chan].priority = seqp->nextEvent.msg.sppriority.priority;
	      break;

	  case (AL_SEQP_SEQ_EVT):

#ifdef _DEBUG
	      assert(seqp->state != AL_PLAYING);	/* Must be done playing to change sequences. */
#endif
    
	      seqp->target = seqp->nextEvent.msg.spseq.seq;
	      __n_setUsptFromTempo (seqp, 500000.0);
	      if (seqp->bank)
		  __n_initFromBank(seqp, seqp->bank);
	      break;

	  case (AL_SEQP_BANK_EVT):

#ifdef _DEBUG
	      assert(seqp->state == AL_STOPPED);	/* Must be fully stopped to change banks. */
#endif
    
	      seqp->bank = seqp->nextEvent.msg.spbank.bank;
	      __n_initFromBank(seqp, seqp->bank);
	      break;

	  /* sct 11/6/95 - these events should now be handled by __n_handleNextSeqEvent */
	  case (AL_SEQ_END_EVT):
	  case (AL_TEMPO_EVT):
          case (AL_SEQ_MIDI_EVT):

#ifdef _DEBUG
	      assert(FALSE);	
#endif

              break;
        }

	seqp->nextDelta = n_alEvtqNextEvent (&seqp->evtq, &seqp->nextEvent);
	
    } while (seqp->nextDelta == 0);

    /*
     * assume that next callback won't be more than half an hour away
     */
    seqp->curTime += seqp->nextDelta;	/* Update the player's current time. */
    return seqp->nextDelta;
}

/*
  Call this routine to handle the next event in the sequence.
  Assumes that the next sequence event is scheduled to be processed
  immediately since it does not check the event's tick time.

  sct 11/7/95
*/
static void
__n_handleNextSeqEvent(N_ALSeqPlayer *seqp)
{
    N_ALEvent	evt;

    /* sct 1/5/96 - Do nothing if we don't have a target sequence. */
    if (seqp->target == NULL)
	return;

    n_alSeqNextEvent(seqp->target, &evt);

    switch (evt.type)
    {
    case AL_SEQ_MIDI_EVT:
	__n_HandleMIDIMsg(seqp, &evt);
	__n_postNextSeqEvent(seqp);
	break;

    case AL_TEMPO_EVT:
	__n_handleMetaMsg(seqp, &evt);
	__n_postNextSeqEvent(seqp);
	break;

    case AL_SEQ_END_EVT:
	seqp->state = AL_STOPPING;      
	evt.type    = AL_SEQP_STOP_EVT;
	n_alEvtqPostEvent(&seqp->evtq, &evt, AL_EVTQ_END);
	break;

    default:

#ifdef _DEBUG
	assert(FALSE);	/* Sequence event type not supported. */
#endif

	break;
    }
}

// 0x8002E11C
char __n_voiceNeedsNoteKill (N_ALSeqPlayer *seqp, N_ALVoice *voice, ALMicroTime killTime)
{
    ALLink              *thisNode;
    ALLink              *nextNode;
    N_ALEventListItem     *thisItem;
    ALMicroTime		    itemTime = 0;
    char		        needsNoteKill = TRUE;

#if VOICENEEDSNOTEKILL_DEBUG
    alEvtqPrintAllocEvts (&seqp->evtq);
#endif

    for (thisNode = seqp->evtq.allocList.next; thisNode != 0; thisNode = nextNode)
    {
    	nextNode = thisNode->next;
    	thisItem = (N_ALEventListItem *)thisNode;
    	itemTime += thisItem->delta;
    
    	if (thisItem->evt.type == AL_NOTE_END_EVT)
    	{
    	    if (thisItem->evt.msg.note.voice == voice)
    	    {
        		if (itemTime > killTime)
        		{
        		    if ((N_ALEventListItem *)nextNode)
        			((N_ALEventListItem *)nextNode)->delta += thisItem->delta;
        		    alUnlink(thisNode);
        		    alLink(thisNode, &seqp->evtq.freeList);
        		}
        		else
        		    needsNoteKill = FALSE;
        		break;
    	    }
    	}
    }

#if VOICENEEDSNOTEKILL_DEBUG
    if (thisNode)
	osSyncPrintf("vox 0x%0x: end time %d  kill time %d\n\n", voice, itemTime, killTime);
    else
	osSyncPrintf("vox 0x%0x: not found\n\n", voice);

    alEvtqPrintAllocEvts (&seqp->evtq);
#endif

   return needsNoteKill;
}

// 0x8002F39C
void __n_seqpStopOsc(N_ALSeqPlayer *seqp, N_ALVoiceState *vs)
{
	N_ALEventListItem *thisNode, *nextNode;
	s16 evtType;

	thisNode = (N_ALEventListItem*)seqp->evtq.allocList.next;
	while (thisNode)
	{
		nextNode = (N_ALEventListItem*)thisNode->node.next;
		evtType = thisNode->evt.type;
		if (evtType == AL_TREM_OSC_EVT || evtType == AL_VIB_OSC_EVT)
		{
			if (thisNode->evt.msg.osc.vs == vs)
			{
				(*seqp->stopOsc)(thisNode->evt.msg.osc.oscState);
				alUnlink((ALLink*)thisNode);
				if (nextNode)
					nextNode->delta += thisNode->delta;
				alLink((ALLink*)thisNode, &seqp->evtq.freeList);
				if (evtType == AL_TREM_OSC_EVT)
					vs->flags = vs->flags & 0xFE;
				else /* must be a AL_VIB_OSC_EVT */
					vs->flags = vs->flags & 0xFD;
				if (!vs->flags)
					return;  /* there should be no more events */
			}
		}

		thisNode = nextNode;
	}
}

// 0x8002DE68
void __n_initFromBank(N_ALSeqPlayer* seqp, ALBank *b)
{
	/*
	 * init the chanState with the default instrument
	 */
	s32 i;
	ALInstrument* inst = 0;

	/* set to the first available instrument. */
	for(i = 0; !inst ; i++)
		inst = b->instArray[i];

	/* sct 11/6/95 - Setup the channel state for the given instrument. */
	/* There is some wasted effort here since both calls the same state vars */
	/* but it's safer. */
	for (i = 0; i < seqp->maxChannels; i++)
	{
		__n_resetPerfChanState(seqp, i);
		__n_setInstChanState(seqp, inst, i);
	}

	if (b->percussion)
	{
		__n_resetPerfChanState(seqp, i);
		__n_setInstChanState(seqp, b->percussion, 9);
	}
}

/*
  sct 11/6/95 -- Call this whenever a new sequence is to be played or when
  initializing a sequence player.
*/
// 0x8002DDB8
void __n_resetPerfChanState(N_ALSeqPlayer *seqp, s32 chan)
{
  seqp->chanState[chan].fxId = AL_FX_NONE;
  seqp->chanState[chan].fxmix = AL_DEFAULT_FXMIX;
  seqp->chanState[chan].pan = AL_PAN_CENTER;
  seqp->chanState[chan].vol = 127;
  seqp->chanState[chan].priority = AL_DEFAULT_PRIORITY;
  seqp->chanState[chan].sustain = 0;
  seqp->chanState[chan].bendRange = 200;
  seqp->chanState[chan].pitchBend = 1.0F;
  seqp->chanState[chan].unk_0x12 = 0;
  seqp->chanState[chan].unk_0x13 = 95;
  seqp->chanState[chan].unk_0x14 = 0;
}

/*
  sct 11/6/95 - Call this whenever a new instrument gets assigned to a channel
  such as when changing banks or in response to a MIDI program change event.
  Currently also gets called when changing sequences.
*/
// 0x8002DD4C
void __n_setInstChanState(N_ALSeqPlayer *seqp, ALInstrument *inst, s32 chan)
{
    seqp->chanState[chan].instrument = inst;
    seqp->chanState[chan].pan = inst->pan;
    seqp->chanState[chan].vol = 127;
    seqp->chanState[chan].priority = inst->priority;
    seqp->chanState[chan].bendRange = inst->bendRange;
    seqp->chanState[chan].vol2 = inst->volume;
}

/*
 Calculates the delta time in ticks until the next sequence
 event taking into account loop points, and posts a
 sequence reference event with the time in usecs.

 Does nothing if the sequence player is not playing or there
 is no target sequence.

 sct 11/7/95
*/
// 0x8002DC70
void __n_postNextSeqEvent(N_ALSeqPlayer *seqp)
{
	N_ALEvent evt;
	s32 deltaTicks;
	ALSeq *seq = seqp->target;

	/* sct 1/5/96 - Do nothing if we're not playing or don't have a target sequence. */
	if ((seqp->state != AL_PLAYING) || (seq == NULL))
		return;

	/* Get the next event time in ticks. */
	/* If false is returned, then there is no next delta (ie. end of sequence reached). */
	if (!__alSeqNextDelta(seq, &deltaTicks))
		return;

	/* Handle loops. */
	if (seqp->loopCount)
	{
		/* Assume that the loop end falls on a MIDI event. Delta time
		   will be correct even if we loop */
		if (alSeqGetTicks(seq) + deltaTicks >= seqp->loopEnd->curTicks)
		{
			alSeqSetLoc(seq, seqp->loopStart);

			if (seqp->loopCount != -1)
			seqp->loopCount--;
		}
	}

	evt.type = AL_SEQ_REF_EVT;
	n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTicks * seqp->uspt);
}

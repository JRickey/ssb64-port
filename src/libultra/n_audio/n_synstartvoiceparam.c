#include "common.h"
#include <n_audio/n_synthInternals.h>

void n_alSynStartVoiceParams(N_ALVoice *v, ALWaveTable *w,
                           f32 pitch, s16 vol, ALPan pan, u8 fxmix,
                           ALMicroTime t)
{
    ALStartParamAlt  *update;
    

    
    if ( v->pvoice) {
        /*
         * get new update struct from the free list
         */
        update = (ALStartParamAlt *)__n_allocParam();

        if (fxmix < 0)
            fxmix = -fxmix;

        ALFailIf(update == 0, ERR_ALSYN_NO_UPDATE);
        
        /*
         * set offset and fxmix data
         */
        update->delta  = n_syn->paramSamples + v->pvoice->offset;
        update->next   = 0;
        update->type   = AL_FILTER_START_VOICE_ALT;

        update->unity  = v->unityPitch;
        update->pan    = pan;
        update->volume = vol;
        update->fxMix  = fxmix;
        update->unk1C = 0;
        update->unk1D = 0x5F;
        update->pitch  = pitch;
        update->samples = _n_timeToSamples( t);
        update->wave    = w;
        
        n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);        
    }
    
}

void n_alSynStartVoiceParams_Alt(N_ALVoice *v, ALWaveTable *w,
                           f32 pitch, s16 vol, ALPan pan, u8 fxmix,
                           ALMicroTime t, u8 arg7, u8 arg8)
{
    ALStartParamAlt  *update;
    

    
    if ( v->pvoice) {
        /*
         * get new update struct from the free list
         */
        update = (ALStartParamAlt *)__n_allocParam();

        if (fxmix < 0)
            fxmix = -fxmix;

        ALFailIf(update == 0, ERR_ALSYN_NO_UPDATE);
        
        /*
         * set offset and fxmix data
         */
        update->delta  = n_syn->paramSamples + v->pvoice->offset;
        update->next   = 0;
        update->type   = AL_FILTER_START_VOICE_ALT;

        update->unity  = v->unityPitch;
        update->pan    = pan;
        update->volume = vol;
        update->fxMix  = fxmix;
        update->unk1C = arg7;
        update->unk1D = arg8;
        update->pitch  = pitch;
        update->samples = _n_timeToSamples( t);
        update->wave    = w;
        
        n_alEnvmixerParam(v->pvoice, AL_FILTER_ADD_UPDATE, update);        
    }
    
}

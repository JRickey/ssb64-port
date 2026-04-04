#include "common.h"
#include <n_audio/n_synthInternals.h>
#include <n_audio/n_seqp.h>

#define KILL_TIME	50000	/* 50 ms */

typedef struct ALWhatever80026094_sub
{
    u8 filler_0x0[0x38];
    u8 unk_alwhatev80026094_sub_0x38;
    u8 unk_alwhatev80026094_sub_0x39;
    u8 unk_alwhatev80026094_sub_0x3A;
    u8 unk_alwhatev80026094_sub_0x3B;
    u8 unk_alwhatev80026094_sub_0x3C;

} ALWhatever80026094_sub;

typedef struct ALWhatever80026094
{
    struct ALWhatever80026094 *next;
    struct ALWhatever80026094 *unk_alwhatev80026094_0x4;
    u8 filler_0x8[0x22 - 0x8];
    u8 unk_alwhatev80026094_0x22;
    s32 unk_alwhatev80026094_0x24;
    ALWhatever80026094_sub *unk_alwhatev80026094_0x28;
    u8 unk_alwhatev80026094_0x2C;
    u8 unk_alwhatev80026094_0x2D;
    u8 unk_alwhatev80026094_0x2E;
    u8 unk_alwhatev80026094_0x2F;
    u8 unk_alwhatev80026094_0x30;
    
} ALWhatever80026094;

typedef struct N_ALUnk80026204
{
    u16 unk_80026204_0x0;
    u16 unk_80026204_0x2;
    u16 unk_80026204_0x4;
    u16 unk_80026204_0x6;
    u16 fgm_ucode_count;
    u16 fgm_table_count;
    u16 unk_80026204_0xC;
    void *unk_80026204_0x10;
    void *fgm_ucode_data;
    void *fgm_table_data;
    s32 unk_80026204_0x1C;
    ALHeap *heap;
    u8 unk_80026204_0x24;
    u16 unk_80026204_0x26;
    u16 unk_80026204_0x28;
    u16 unk_80026204_0x2A;
    u16 unk_80026204_0x2C;
    u16 unk_80026204_0x2E;
    u16 unk_80026204_0x30;

} N_ALUnk80026204;

typedef struct alSoundEffect
{
    struct alSoundEffect *next;
    void *unk_0x4;
    void *unk_0x8;
    void *unk_0xC;
    u16 unk_0x10;
    u16 unk_0x12;
    u16 unk_0x14;
    u16 unk_0x16;
    u16 unk_0x18;
    u16 unk_0x1A;
    u16 unk_0x1C;
    u8 unk_0x1E;
    u8 unk_0x1F;
    u16 unk_0x20;
    u16 unk_0x22;
    u16 unk_0x24;
    u16 sfx_id;
    u16 sfx_max;
    u16 unk_alsound_0x2A;
    u8 filler_0x2C[0x2F - 0x2C];
    u8 balance;
    
} alSoundEffect;

typedef struct ALWhatever8009EDD0_off5C_offC
{
    u8 filler_0x0[0x84];
    s32 unkALWhatever8009EDD0_off5C_offC_0x84;

} ALWhatever8009EDD0_off5C_offC;

typedef struct ALWhatever8009EDD0_siz24
{
    struct ALWhatever8009EDD0_siz24 *next;
    u8 unkALWhatever8009EDD0_siz24_0x4;
    u8 unkALWhatever8009EDD0_siz24_0x5;
    u8 unkALWhatever8009EDD0_siz24_0x6;
    u8 unkALWhatever8009EDD0_siz24_0x7;
    f32 unkALWhatever8009EDD0_siz24_0x8;
    f32 unkALWhatever8009EDD0_siz24_0xC;
    f32 unkALWhatever8009EDD0_siz24_0x10;
    f32 unkALWhatever8009EDD0_siz24_0x14;
    f32 unkALWhatever8009EDD0_siz24_0x18;
    f32 unkALWhatever8009EDD0_siz24_0x1C;
    f32 unkALWhatever8009EDD0_siz24_0x20;
} ALWhatever8009EDD0_siz24;

typedef struct ALWhatever8009EE0C
{
	struct ALWhatever8009EE0C *next;
    N_ALVoice* voice;
	u8 pad8[0xC - 0x8];
    ALWhatever8009EDD0_off5C_offC *unkC;
    u8 pad10[0x20 - 0x10];
    s32 unk20;
    s32 unk24;
	u16 unk28;
	u8 unk2A;
	u8 unk2B;
    s16 unk2C;
    u16 unk2E;
    u16 unk30;
    u8 unk32;
    u8 unk33;
    u8 unk34;
    u8 unk35;
    u8 unk36;
    u8 unk37;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    u8 unk3B;
    u8 unk3C;
    s32 unk40;
    ALWhatever8009EDD0_siz24 *unk44;
	u16 unk48;
} ALWhatever8009EE0C;

typedef struct ALWhatever8009EE0C_2 ALWhatever8009EE0C_2;
struct ALWhatever8009EE0C_2
{
	u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    N_ALVoice voice;
    u8 *unk20;
    u8 *unk24;
	u16 unk28;
	u8 unk2A;
	u8 unk2B;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    u8 unk32;
    u8 unk33;
    u8 unk34;
    u8 unk35;
    u8 unk36;
    u8 unk37;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    u8 unk3B;
    u8 unk3C;
    u8 unk3D;
    s32 unk40;
    void *unk44;
	u16 unk48;
};

typedef struct ALWhatever8009EE0C_3
{
	u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    f32 unk4;
	f32 unk8;
    f32 unkC;

} ALWhatever8009EE0C_3;

typedef struct ALWhatever8009EDD0_siz34
{
    struct ALWhatever8009EDD0_siz34 *next;
    struct ALWhatever8009EDD0_siz34 *unkALWhatever8009EDD0_siz34_0x4;
    u8 *unkALWhatever8009EDD0_siz34_0x8;
    u8 *unkALWhatever8009EDD0_siz34_0xC;
    u16 unkALWhatever8009EDD0_siz34_0x10;
    u16 unkALWhatever8009EDD0_siz34_0x12[6];
    u8 unkALWhatever8009EDD0_siz34_0x1E;
    u8 unkALWhatever8009EDD0_siz34_0x1F;
    u8 unkALWhatever8009EDD0_siz34_0x20;
    u8 unkALWhatever8009EDD0_siz34_0x21;
    u8 unkALWhatever8009EDD0_siz34_0x22;
    u8 unkALWhatever8009EDD0_siz34_0x23;
    u16 unkALWhatever8009EDD0_siz34_0x24;
    u16 unkALWhatever8009EDD0_siz34_0x26;
    ALWhatever8009EE0C *unkALWhatever8009EDD0_siz34_0x28;
    u8 unkALWhatever8009EDD0_siz34_0x2C;
    u8 unkALWhatever8009EDD0_siz34_0x2D;
    u8 unkALWhatever8009EDD0_siz34_0x2E;
    u8 unkALWhatever8009EDD0_siz34_0x2F;
    u8 unkALWhatever8009EDD0_siz34_0x30;

} ALWhatever8009EDD0_siz34;

typedef struct ALWhatever8009EDD0_off18
{
    u8 filler_0x8[0x8];
    u32 unkALWhatever8009EDD0_off18_0x8;

} ALWhatever8009EDD0_off18;


typedef struct ALWhatever8009EDD0
{
    ALPlayer node;
    u16 unk_alsound_0x14;
    ALWhatever8009EDD0_off18 **unk_alsound_0x18;
    u8 **fgm_ucode_data;
    u8 **fgm_table_data;
    ALWhatever8009EE0C_3 *unk_alsound_0x24;
    u16 fgm_ucode_count;
    u16 fgm_table_count;
    u16 unk_alsound_0x2C;
    ALWhatever8009EDD0_siz24 *unk_alsound_0x30;
    ALWhatever8009EE0C *unk_alsound_0x34;            // 0x34
    ALWhatever8009EDD0_siz34 *unk_alsound_0x38;
    ALWhatever8009EE0C *unk_alsound_0x3C;
    ALWhatever8009EDD0_siz34 *unk_alsound_0x40;
    s32 unk_alsound_0x44;
    u16 unk_alsound_0x48;
    u16 unk_alsound_0x4A;
    u8 unk_alsound_0x4C;
    u16 unk_alsound_0x4E[6];
    u8 unk_alsound_0x5A;
    ALWhatever8009EE0C *unk_alsound_0x5C;
    ALWhatever8009EDD0_siz34 *unk_alsound_0x60;
    s32 unk_alwhatev8009EDD0_0x64;

} ALWhatever8009EDD0;

ALWhatever8009EDD0 D_8009EDD0_406D0;
f32 randFloat1(void);
f32 randFloat2(void);

void __n_CSPPostNextSeqEvent(N_ALCSPlayer *seqp);
char __alCSeqNextDelta(ALCSeq *seq, s32 *pDeltaTicks);
static s32 _n_timeToSamplesNoRound(s32 micros);
static s32 __n_nextSampleTime(ALPlayer **client);

// They DID make this a macro and it's used in n_alSynNew
#define alLinkMacro(ln, to)       \
{                                 \
	ALLink *_ln = (ALLink *)(ln); \
	ALLink *_to = (ALLink *)(to); \
	_ln->next = _to->next;        \
	_ln->prev = _to;              \
									\
	if (_to->next) {              \
		_to->next->prev = _ln;    \
	}                             \
									\
	_to->next = _ln;              \
}

/////////////////////////////////////////////////////////////
// n_drvrNew.c

/*
 * WARNING: THE FOLLOWING CONSTANT MUST BE KEPT IN SYNC
 * WITH SCALING IN MICROCODE!!!
 */
#define	SCALE 16384.0f

/*
 * the following arrays contain default parameters for
 * a few hopefully useful effects.
 */
#define ms *(((s32)((f32)44.1))&~0x7)
 
s32 SMALLROOM_PARAMS_N[26] = {
    /* sections	   length */
          3,        100 ms,
                                      /*       chorus  chorus   filter
       input  output  fbcoef  ffcoef   gain     rate   depth     coef  */
           0,   54 ms,  9830,  -9830,      0,      0,      0,      0,
       19 ms,   38 ms,  3276,  -3276, 0x3fff,      0,      0,      0,
           0,   60 ms,  5000,      0,      0,      0,      0, 0x5000
};

s32 BIGROOM_PARAMS_N[34] = {
    /* sections	   length */
          4,        100 ms,
                                      /*       chorus  chorus   filter
       input  output  fbcoef  ffcoef   gain     rate   depth     coef  */
           0,   66 ms,  9830,  -9830,      0,      0,      0,      0,
       22 ms,   54 ms,  3276,  -3276, 0x3fff,      0,      0,      0,
       66 ms,   91 ms,  3276,  -3276, 0x3fff,      0,      0,      0,
           0,   94 ms,  8000,      0,      0,      0,      0, 0x5000
};

s32 ECHO_PARAMS_N[10] = {
    /* sections	   length */
          1,       200 ms,
                                      /*       chorus  chorus   filter
       input  output  fbcoef  ffcoef   gain     rate   depth     coef   */
           0,  179 ms, 12000,      0, 0x7fff,      0,      0,      0
};

s32 CHORUS_PARAMS_N[10] = {
    /* sections	   length */
          1,        20 ms,
                                      /*       chorus  chorus   filter
       input  output  fbcoef  ffcoef   gain     rate   depth     coef   */
	  0,   5 ms, 0x4000,      0,  0x7fff,   7600,   700,      0
};

s32 FLANGE_PARAMS_N[10] = {
    /* sections	   length */
          1,        20 ms,
                                      /*       chorus  chorus   filter
       input  output  fbcoef  ffcoef   gain     rate   depth     coef   */
	   0,   5 ms,      0, 0x5fff, 0x7fff,    380,   500,      0
};

s32 NULL_PARAMS_N[10] = {
    0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};


void _init_lpfilter(ALLowPass *lp)
{
	s32		i, temp;
	s16		fc;
	f64		ffc, fcoef;

	temp = lp->fc * SCALE;
	fc = temp >> 15;
	lp->fgain = SCALE - fc;

	lp->first = 1;
	for (i=0; i<8; i++)
	lp->fcvec.fccoef[i] = 0;

	lp->fcvec.fccoef[i++] = fc;
	fcoef = ffc = (f64)fc/SCALE;

	for (; i<16; i++){
	fcoef *= ffc;
	lp->fcvec.fccoef[i] = (s16)(fcoef * SCALE);
	}
}

// 0x8002D218
void n_alFxNew(ALFx **fx_ar, ALSynConfig *c, ALHeap *hp)
{
	u16		i, j, k;
	s32		*param = 0;
	ALDelay	*d;
	ALFx *r;

	*fx_ar = r = (ALFx *)alHeapAlloc(hp, 1, sizeof(ALFx));

	switch (c->fxType) {
		case AL_FX_SMALLROOM:	param = SMALLROOM_PARAMS_N;	break;
		case AL_FX_BIGROOM:	param = BIGROOM_PARAMS_N;	break;
		case AL_FX_ECHO:		param = ECHO_PARAMS_N;		break;
		case AL_FX_CHORUS:	param = CHORUS_PARAMS_N;	break;
		case AL_FX_FLANGE:	param = FLANGE_PARAMS_N;	break;
		case AL_FX_CUSTOM:	param = c->params;		break;
		default:			    param = NULL_PARAMS_N;		break;
	}


	j = 0;

	r->section_count = param[j++];
	r->length 	     = param[j++];

	r->delay = alHeapAlloc(hp, r->section_count, sizeof(ALDelay));
	r->base = alHeapAlloc(hp, r->length, sizeof(s16));
	r->input = r->base;

	for ( k=0; k < r->length; k++)
	r->base[k] = 0;

	for ( i=0; i<r->section_count; i++ ){
	d = &r->delay[i];
	d->input  = param[j++];
	d->output = param[j++];
	d->fbcoef = param[j++];
	d->ffcoef = param[j++];
	d->gain   = param[j++];

	if (param[j]) {
#define RANGE 2.0f
/*	    d->rsinc     = ((f32) param[j++])/0xffffff; */
		d->rsinc = ((((f32)param[j++])/1000) * RANGE)/c->outputRate;

		/*
		 * the following constant is derived from:
		 *
		 *		ratio = 2^(cents/1200)
		 *
		 * and therefore for hundredths of a cent
		 *			           x
		 *		ln(ratio) = ---------------
		 *			    (120,000)/ln(2)
		 * where
		 *		120,000/ln(2) = 173123.40...
		 */
#define CONVERT 173123.404906676f
#define LENGTH	(d->output - d->input)
		d->rsgain 	 = (((f32) param[j++])/CONVERT) * LENGTH;
		d->rsval	 = 1.0;
		d->rsdelta	 = 0.0;
		d->rs 	 = alHeapAlloc(hp, 1, sizeof(ALResampler));
		d->rs->state = alHeapAlloc(hp, 1, sizeof(RESAMPLE_STATE));
		d->rs->delta = 0.0;
		d->rs->first = 1;
	} else {
		d->rs = 0;
		j++;
		j++;
	}

	if (param[j]) {
		d->lp = alHeapAlloc(hp, 1, sizeof(ALLowPass));
		d->lp->fstate = alHeapAlloc(hp, 1, sizeof(POLEF_STATE));
		d->lp->fc = param[j++];
		_init_lpfilter(d->lp);
	} else {
		d->lp = 0;
		j++;
	}
	}
}

// 0x8002CA20
void alN_PVoiceNew(N_PVoice *mv, ALDMANew dmaNew, ALHeap *hp)
{
	mv->dc_state = alHeapAlloc(hp, 1, sizeof(ADPCM_STATE));
	mv->dc_lstate = alHeapAlloc(hp, 1, sizeof(ADPCM_STATE));
	mv->dc_dma = dmaNew(&mv->dc_dmaState);
	mv->dc_lastsam = 0;
	mv->dc_first = 1;
	mv->dc_memin = 0;

	mv->rs_state = alHeapAlloc(hp, 1, sizeof(RESAMPLE_STATE));
	mv->rs_delta  = 0.0;
	mv->rs_first  = 1;
	mv->rs_ratio = 1.0;
	mv->rs_upitch = 0;

	mv->em_state = alHeapAlloc(hp, 1, sizeof(ENVMIX_STATE));
	mv->em_first = 1;
	mv->em_motion = AL_STOPPED;
	mv->em_volume = 1;
	mv->em_ltgt = 1;
	mv->em_rtgt = 1;
	mv->em_cvolL = 1;
	mv->em_cvolR = 1;
	mv->em_dryamt = 0;
	mv->em_wetamt = 0;
	mv->em_lratm = 1;
	mv->em_lratl = 0;
	mv->em_lratm = 1;
	mv->em_lratl = 0;
	mv->em_delta = 0;
	mv->em_segEnd = 0;
	mv->em_pan = 0;
	mv->em_ctrlList = 0;
	mv->em_ctrlTail = 0;
}

/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// n_load.c
#define ADPCMFBYTES      9
#define LFSAMPLES        4

static
  Acmd *_decodeChunk(Acmd *ptr, N_PVoice *f, s32 tsam, s32 nbytes, s16 outp, s16 inp, u32 flags);

Acmd *n_alAdpcmPull(N_PVoice *filter, s16 *outp, s32 outCount, Acmd *p) 
{
  Acmd        *ptr = p;
  s16         inp;
  s32         tsam;
  s32         nframes;
  s32         nbytes;
  s32         overFlow;
  s32         startZero;
  s32         nOver;
  s32         nSam;
  s32         op;
  s32         nLeft;
  s32         bEnd;
  s32         decoded = 0;
  s32         looped = 0;
  
  N_PVoice *f = filter;
  
#ifdef AUD_PROFILE
  lastCnt[++cnt_index] = osGetCount();
#endif
  
  if (outCount == 0)
    return ptr;

#ifndef N_MICRO  
  inp = AL_DECODER_IN;
#else
  inp = N_AL_DECODER_IN;
#endif

  aLoadADPCM(ptr++, f->dc_bookSize,
	     K0_TO_PHYS(f->dc_table->waveInfo.adpcmWave.book->book));
  
  looped = (outCount + f->dc_sample > f->dc_loop.end) && (f->dc_loop.count != 0);

  if (looped)
    nSam = f->dc_loop.end - f->dc_sample;
  else
    nSam = outCount;
  
  if (f->dc_lastsam)
    nLeft = ADPCMFSIZE - f->dc_lastsam;
  else
    nLeft = 0;
  tsam = nSam - nLeft;
  if (tsam<0) tsam = 0;
  
  nframes = (tsam+ADPCMFSIZE-1)>>LFSAMPLES;
  nbytes =  nframes*ADPCMFBYTES;
  
  if (looped){
    ptr = _decodeChunk(ptr, f, tsam, nbytes, *outp, inp, f->dc_first);
    
    /*
     * Fix up output pointer, which will be used as the input pointer
     * by the following module.
     */
    if (f->dc_lastsam)
      *outp += (f->dc_lastsam<<1);
    else
      *outp += (ADPCMFSIZE<<1);
    
    /*
     * Now fix up state info to reflect the loop start point
     */
    f->dc_lastsam = f->dc_loop.start &0xf;
    f->dc_memin = (s32) f->dc_table->base + ADPCMFBYTES *
      ((s32) (f->dc_loop.start>>LFSAMPLES) + 1);
    f->dc_sample = f->dc_loop.start;
    
    bEnd = *outp;
    while (outCount > nSam){

      outCount -= nSam;
      
      /*
       * Put next one after the end of the last lot - on the
       * frame boundary (32 byte) after the end.
       */
      op = (bEnd + ((nframes+1)<<(LFSAMPLES+1)) + 16) & ~0x1f;
      
      /*
       * The actual end of data
       */
      bEnd += (nSam<<1);

      /*
       * -1 is loop forever - the loop count is not exact now
       * for small loops!
       */
      if ((f->dc_loop.count != -1) && (f->dc_loop.count != 0))
	f->dc_loop.count--;
      
      /*
       * What's left to compute.
       */
      nSam = MIN(outCount, f->dc_loop.end - f->dc_loop.start);
      tsam = nSam - ADPCMFSIZE + f->dc_lastsam;  
      if (tsam<0) tsam = 0;
      nframes = (tsam+ADPCMFSIZE-1)>>LFSAMPLES;
      nbytes =  nframes*ADPCMFBYTES;
      ptr = _decodeChunk(ptr, f, tsam, nbytes, op, inp, f->dc_first | A_LOOP);

      /*
       * Merge the two sections in DMEM.
       */
      aDMEMMove(ptr++, op+(f->dc_lastsam<<1), bEnd, nSam<<1);
    }
    
    f->dc_lastsam = (outCount + f->dc_lastsam) & 0xf;
    f->dc_sample += outCount;
    f->dc_memin += ADPCMFBYTES*nframes;    
#ifdef AUD_PROFILE
    PROFILE_AUD(adpcm_num, adpcm_cnt, adpcm_max, adpcm_min);
#endif
    return ptr;
  }
  
  /*
   * The unlooped case, which is executed most of the time
   */
  
  nSam = nframes<<LFSAMPLES;
  
  /*
   * overFlow is the number of bytes past the end
   * of the bitstream I try to generate
   */
  overFlow = f->dc_memin + nbytes - ((s32) f->dc_table->base + f->dc_table->len);
  if (overFlow < 0)
    overFlow = 0;
  nOver = (overFlow/ADPCMFBYTES)<<LFSAMPLES;
  if (nOver > nSam + nLeft)
    nOver = nSam + nLeft;
  
  nbytes -= overFlow;
  
  if ((nOver - (nOver & 0xf))< outCount){
    decoded = 1;
    ptr = _decodeChunk(ptr, f, nSam - nOver, nbytes, *outp, inp, f->dc_first);
    
    if (f->dc_lastsam)
      *outp += (f->dc_lastsam<<1);
    else
      *outp += (ADPCMFSIZE<<1);
    
    f->dc_lastsam = (outCount + f->dc_lastsam) & 0xf;
    f->dc_sample += outCount;
    f->dc_memin += ADPCMFBYTES*nframes;    
  } else {        
    f->dc_lastsam = 0;
    f->dc_memin += ADPCMFBYTES*nframes;    
  }
  
  /*
   * Put zeros in if necessary
   */
  if (nOver){
    f->dc_lastsam = 0;
    if (decoded)
      startZero = (nLeft + nSam - nOver)<<1;
    else
      startZero = 0;
    aClearBuffer(ptr++, startZero + *outp, nOver<<1);
  }
#ifdef AUD_PROFILE
  PROFILE_AUD(adpcm_num, adpcm_cnt, adpcm_max, adpcm_min);
#endif
  
  return ptr;
}

s32
  n_alLoadParam(N_PVoice *filter, s32 paramID, void *param)
{
  N_PVoice *a = filter;
#if 0
  ALFilter *f = (ALFilter *) filter;
#endif
  
  switch (paramID) {
  case (AL_FILTER_SET_WAVETABLE):
    a->dc_table = (ALWaveTable *) param;
    a->dc_memin = (s32) a->dc_table->base;
    a->dc_sample = 0;
    switch (a->dc_table->type){
    case (AL_ADPCM_WAVE):
      
      a->dc_table->len = ADPCMFBYTES *
	((s32) (a->dc_table->len/ADPCMFBYTES));
      
      a->dc_bookSize = 2*a->dc_table->waveInfo.adpcmWave.book->order*
	a->dc_table->waveInfo.adpcmWave.book->npredictors*ADPCMVSIZE;
      if (a->dc_table->waveInfo.adpcmWave.loop) {
	a->dc_loop.start = a->dc_table->waveInfo.adpcmWave.loop->start;
	a->dc_loop.end = a->dc_table->waveInfo.adpcmWave.loop->end;
	a->dc_loop.count = a->dc_table->waveInfo.adpcmWave.loop->count;
	alCopy(a->dc_table->waveInfo.adpcmWave.loop->state,
	       a->dc_lstate, sizeof(ADPCM_STATE));
      } else {
	a->dc_loop.start = a->dc_loop.end = a->dc_loop.count = 0;
      }
      break;
      
    case (AL_RAW16_WAVE):
      if (a->dc_table->waveInfo.rawWave.loop) {
	a->dc_loop.start = a->dc_table->waveInfo.rawWave.loop->start;
	a->dc_loop.end = a->dc_table->waveInfo.rawWave.loop->end;
	a->dc_loop.count = a->dc_table->waveInfo.rawWave.loop->count;
      } else {
	a->dc_loop.start = a->dc_loop.end = a->dc_loop.count = 0;
      }
      break;
      
    default:
      break;
      
    }
    break;
    
  case (AL_FILTER_RESET):
    a->dc_lastsam = 0;
    a->dc_first   = 1;
    a->dc_sample = 0;
    
    /* sct 2/14/96 - Check table since it is initialized to null and */
    /* Get loop info according to table type. */
    if (a->dc_table)
      {
	a->dc_memin  = (s32) a->dc_table->base;
	if (a->dc_table->type == AL_ADPCM_WAVE)
	  {
	    if (a->dc_table->waveInfo.adpcmWave.loop)
	      a->dc_loop.count = a->dc_table->waveInfo.adpcmWave.loop->count;
	  }
	else if (a->dc_table->type == AL_RAW16_WAVE)
	  {
	    if (a->dc_table->waveInfo.rawWave.loop)
	      a->dc_loop.count = a->dc_table->waveInfo.rawWave.loop->count;
	  }
      }
    
    break;
    
  default:
    break;
  }
}

Acmd *_decodeChunk(Acmd *ptr, N_PVoice *f, s32 tsam,
		   s32 nbytes, s16 outp, s16 inp, u32 flags)
{
  s32 dramAlign, dramLoc;
  
  if (nbytes > 0){
    dramLoc = (f->dc_dma)(f->dc_memin, nbytes, f->dc_dmaState);
    /*
     * Make sure enough is loaded into DMEM to take care
     * of 8 byte alignment
     */
    dramAlign = dramLoc & 0x7;
    nbytes += dramAlign;
#ifndef N_MICRO
    aSetBuffer(ptr++, 0, inp, 0, nbytes + 8 - (nbytes & 0x7));
    aLoadBuffer(ptr++, dramLoc - dramAlign);
#else
    n_aLoadBuffer(ptr++, nbytes + 8 - (nbytes & 0x7), inp, dramLoc - dramAlign);
#endif
  } else
    dramAlign = 0;

  if (flags & A_LOOP){
    aSetLoop(ptr++, K0_TO_PHYS(f->dc_lstate));
  }

#ifndef N_MICRO
  aSetBuffer(ptr++, 0, inp + dramAlign, outp, tsam<<1);
  aADPCMdec(ptr++, flags, K0_TO_PHYS(f->dc_state));
#else
  n_aADPCMdec(ptr++, K0_TO_PHYS(f->dc_state), flags, tsam<<1, dramAlign, outp);
#endif
  f->dc_first = 0;
  return ptr;
}
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// n_env.c

#ifdef AUD_PROFILE
extern u32 cnt_index, env_num, env_cnt, env_max, env_min, lastCnt[];
extern u32 rate_num, rate_cnt, rate_max, rate_min;
extern u32 vol_num, vol_cnt, vol_max, vol_min;
#endif

#define N_EQPOWER_LENGTH 128

s16 n_eqpower[ N_EQPOWER_LENGTH ] = {
    32767,  32764,  32757,  32744,  32727,  32704,
    32677,  32644,  32607,  32564,  32517,  32464,
    32407,  32344,  32277,  32205,  32127,  32045,
    31958,  31866,  31770,  31668,  31561,  31450,
    31334,  31213,  31087,  30957,  30822,  30682,
    30537,  30388,  30234,  30075,  29912,  29744,
    29572,  29395,  29214,  29028,  28838,  28643,
    28444,  28241,  28033,  27821,  27605,  27385,
    27160,  26931,  26698,  26461,  26220,  25975,
    25726,  25473,  25216,  24956,  24691,  24423,
    24151,  23875,  23596,  23313,  23026,  22736,
    22442,  22145,  21845,  21541,  21234,  20924,
    20610,  20294,  19974,  19651,  19325,  18997,
    18665,  18331,  17993,  17653,  17310,  16965,
    16617,  16266,  15913,  15558,  15200,  14840,
    14477,  14113,  13746,  13377,  13006,  12633,
    12258,  11881,  11503,  11122,  10740,  10357,
    9971,   9584,   9196,   8806,   8415,   8023,
    7630,   7235,   6839,   6442,   6044,   5646,
    5246,   4845,   4444,   4042,   3640,   3237,
    2833,   2429,   2025,   1620,   1216,    810,
    405,      0
};

#ifndef N_MICRO
extern	f64	__pow(f64, f64);
extern f64 _frexpf(f64 value, s32 *eptr);
extern f64 _ldexpf(f64 in, s32 ex);
#endif

/*
 * prototypes for private enveloper functions
 */
static  Acmd *_pullSubFrame(N_PVoice *pv, s16 *inp, s16 *outp, s32 outCount,
                             Acmd *p) ;

static  s16 _getRate(f32 vol, f32 tgt, s32 count, u16* ratel);
     
     
#ifndef N_MICRO
static  f32 _getVol(f32 ivol, s32 samples, s16 ratem, u16 ratel);
#else
static  s16 _getVol(s16 ivol, s32 samples, s16 ratem, u16 ratel);
#endif
     
/***********************************************************************
  * Enveloper filter public interfaces
 ***********************************************************************/
Acmd *n_alEnvmixerPull(N_PVoice *filter, s32 sampleOffset, Acmd *p) 
{
  Acmd        *ptr = p;

  
  N_PVoice	*e = (N_PVoice *)filter;
  
  s16         inp;
  s32         lastOffset;
  s32         thisOffset = sampleOffset;
  s32         samples;
  s16         loutp = 0;
  s32         fVol;
  ALParam     *thisParam;
  s32 outCount = FIXED_SAMPLE;
  
#ifdef AUD_PROFILE
  lastCnt[++cnt_index] = osGetCount();
#endif
  
  /*
   * Force the input to be the resampler output
   */
#ifndef N_MICRO
  inp = AL_RESAMPLER_OUT;
#else
  inp = N_AL_RESAMPLER_OUT;
#endif
  
  while (e->em_ctrlList != 0) {

    // Also different...
    lastOffset = thisOffset;
    thisOffset = (((e->em_ctrlList->delta + (SAMPLES / 2)) / SAMPLES) * SAMPLES);
    samples = thisOffset - lastOffset;

    if (samples > outCount)
      break;
    
#ifdef _DEBUG
    assert(samples >= 0);
#ifndef N_MICRO
    assert(samples <= AL_MAX_RSP_SAMPLES);
#else
    assert(samples <= FIXED_SAMPLE);
#endif
#endif

    switch (e->em_ctrlList->type) {
    case (AL_FILTER_START_VOICE_ALT):
      {                  
	ALStartParamAlt *param = (ALStartParamAlt *)e->em_ctrlList;
	ALFilter     *f = (ALFilter *) e;
	s32 tmp;
	
	if (param->unity) {
	  e->rs_upitch = 1;
	}
	
	n_alLoadParam(e, AL_FILTER_SET_WAVETABLE, param->wave);
	e->em_motion = AL_PLAYING;
	e->em_first  = 1;
	e->em_delta  = 0;

#ifdef FINAL_ROUND
	e->em_segEnd = ((param->samples + (SAMPLES / 2)) / SAMPLES) * (u32) SAMPLES; // This is different...
#else
	e->em_segEnd = param->samples;
#endif

	tmp = ((s32)param->volume * (s32)param->volume) >> 15;


	e->em_volume = (s16) tmp;
	e->em_pan    = param->pan;

    if (param->unk1C != 0 || param->unk1D != 0x5F) {
        e->em_wetamt = n_eqpower[N_EQPOWER_LENGTH - param->unk1C - 1];
	    e->em_dryamt = n_eqpower[N_EQPOWER_LENGTH - param->unk1D - 1];
    }
    else {
        e->em_dryamt = n_eqpower[param->fxMix];
	    e->em_wetamt = n_eqpower[N_EQPOWER_LENGTH - param->fxMix - 1];
    }
	
	if (param->samples) {
	  e->em_cvolL  = 1;
	  e->em_cvolR  = 1;
	} else {
	  /*
	   * Attack time is zero. Simply set the
	   * volume. We don't want an attack segment.
	   */
	  e->em_cvolL = (e->em_volume * n_eqpower[e->em_pan]) >> 15;
	  e->em_cvolR = (e->em_volume *
			 n_eqpower[N_EQPOWER_LENGTH - e->em_pan - 1]) >> 15;
	}
	e->rs_ratio = param->pitch;
      }
      
      break;
      
    case (AL_FILTER_SET_FXAMT):
    case (AL_FILTER_SET_PAN):
    case (AL_FILTER_SET_VOLUME):
    case (AL_FILTER_SET_FXAMT_ALT):
      ptr = _pullSubFrame(e, &inp, &loutp, samples, ptr);
      
      if (e->em_delta >= e->em_segEnd){
	/*
	 * We should have reached our target, calculate
	 * target in case e->em_segEnd was 0
	 */
	e->em_ltgt = (e->em_volume * n_eqpower[e->em_pan]) >> 15;
	e->em_rtgt = (e->em_volume *
		      n_eqpower[N_EQPOWER_LENGTH - e->em_pan - 1]) >> 15;
	e->em_delta = e->em_segEnd;   /* To prevent overflow */
	e->em_cvolL = e->em_ltgt;
	e->em_cvolR = e->em_rtgt;
      } else {
	/* 
	 * Estimate the current volume
	 */
	e->em_cvolL = _getVol(e->em_cvolL, e->em_delta, e->em_lratm, e->em_lratl);
	e->em_cvolR = _getVol(e->em_cvolR, e->em_delta, e->em_rratm, e->em_rratl);
      }
      
      /*
       * We can't have volume of zero, because the envelope
       * would never go anywhere from there
       */
      if( e->em_cvolL == 0 ) e->em_cvolL = 1;
      if( e->em_cvolR == 0 ) e->em_cvolR = 1;
      
      if (e->em_ctrlList->type == AL_FILTER_SET_PAN)
	
	/*
	 * This should result in a change to the current
	 * segment rate and target
	 */
	e->em_pan = (s16) e->em_ctrlList->data.i;

      if (e->em_ctrlList->type == AL_FILTER_SET_FXAMT_ALT){
          
      }
      
      if (e->em_ctrlList->type == AL_FILTER_SET_VOLUME){
	
	/*
	 * Switching to a new segment
	 */
	e->em_delta = 0;
	
	/*
	 * Map volume non-linearly to give something close to
	 * loudness
	 */
	fVol = (e->em_ctrlList->data.i);
	fVol = (fVol*fVol)>>15;
	e->em_volume = (s16) fVol;
	
#ifdef FINAL_ROUND
	e->em_segEnd = SAMPLE184(e->em_ctrlList->moredata.i);
#else
	e->em_segEnd = e->em_ctrlList->moredata.i;
#endif
	
      }
      
      if (e->em_ctrlList->type == AL_FILTER_SET_FXAMT){
	e->em_dryamt = n_eqpower[e->em_ctrlList->data.i];
	e->em_wetamt = n_eqpower[N_EQPOWER_LENGTH - e->em_ctrlList->data.i - 1];
      }

      if (e->em_ctrlList->type == AL_FILTER_SET_FXAMT_ALT){
	e->em_dryamt = n_eqpower[N_EQPOWER_LENGTH - e->em_ctrlList->moredata.i];
	e->em_wetamt = n_eqpower[N_EQPOWER_LENGTH - e->em_ctrlList->data.i - 1];
      }
      
      /*
       * Force a volume update
       */
      e->em_first = 1;
      break;
      
    case (AL_FILTER_START_VOICE):
      {
	ALStartParam *p = (ALStartParam *)e->em_ctrlList;
	
	/*
	 * Changing to PLAYING (since the previous state was
	 * persumable STOPPED, we'll just bump the output
	 * pointer rather than pull a subframe of zeros).
	 */
	if (p->unity) {
	  e->rs_upitch = 1;
	}
	
	n_alLoadParam(e, AL_FILTER_SET_WAVETABLE, p->wave);
	e->em_motion = AL_PLAYING;
      }
      break;
    case (AL_FILTER_STOP_VOICE):
      {
	/*
	 * Changing to STOPPED and reset the filter
	 */
	ptr = _pullSubFrame(e, &inp, &loutp, samples, ptr);
	n_alEnvmixerParam(e, AL_FILTER_RESET, 0);
      }
      break;
      
    case (AL_FILTER_FREE_VOICE):
      {                  
	N_ALFreeParam *param = (N_ALFreeParam *)e->em_ctrlList;
	param->pvoice->offset = 0;
	_n_freePVoice( (N_PVoice *)param->pvoice);
      }
      break;
#if 1
    case (AL_FILTER_SET_PITCH):
      ptr = _pullSubFrame(e, &inp, &loutp, samples, ptr);
      e->rs_ratio = e->em_ctrlList->data.f;
      break;
    case (AL_FILTER_SET_UNITY_PITCH):
      ptr = _pullSubFrame(e, &inp, &loutp, samples, ptr);
      e->rs_upitch = 1;
      break;
    case (AL_FILTER_SET_WAVETABLE):
      ptr = _pullSubFrame(e, &inp, &loutp, samples, ptr);
      n_alLoadParam(e, AL_FILTER_SET_WAVETABLE,
		       (void *)e->em_ctrlList->data.i);
      break;
#endif
    default:
      /*
       * Pull the reuired number of samples and then pass the message
       * on down the chain
       */
      ptr = _pullSubFrame(e, &inp, &loutp, samples, ptr);
      n_alEnvmixerParam(e, e->em_ctrlList->type,
		      (void *) e->em_ctrlList->data.i);
      break;
    }
    loutp  += (samples<<1);
    outCount -= samples;
    
    /*
     * put the param record back on the free list
     */
    thisParam = e->em_ctrlList;
    e->em_ctrlList = e->em_ctrlList->next;
    if (e->em_ctrlList == 0)
      e->em_ctrlTail = 0;
    
    _n_freeParam(thisParam);
    
  }
  
  ptr = _pullSubFrame(e, &inp, &loutp, outCount, ptr);
  
  /*
   * Prevent overflow in e->em_delta
   */
  if (e->em_delta > e->em_segEnd)
    e->em_delta = e->em_segEnd;
  
#ifdef AUD_PROFILE
  PROFILE_AUD(env_num, env_cnt, env_max, env_min);
#endif
  return ptr;
}

s32
  n_alEnvmixerParam(N_PVoice *filter, s32 paramID, void *param)
{
  N_PVoice	*e =  filter;
  
  switch (paramID) {
  case (AL_FILTER_ADD_UPDATE):
    if (e->em_ctrlTail) {
      e->em_ctrlTail->next = (ALParam *)param;
    } else {
      e->em_ctrlList = (ALParam *)param;
    }            
    e->em_ctrlTail = (ALParam *)param;
    break;
  case (AL_FILTER_RESET):
    e->em_first = 1;
    e->em_motion = AL_STOPPED;
    e->em_volume = 1;
    // e->em_segEnd = 0;
    e->rs_delta  = 0.0;
    e->rs_first  = 1;
    e->rs_upitch = 0;
    n_alLoadParam(e, AL_FILTER_RESET, param);
    break;
  case (AL_FILTER_START):
    e->em_motion = AL_PLAYING;
    break;
  default:
#if 1
    n_alLoadParam(e, paramID, param);
#else
    n_alResampleParam(e, paramID, param);
#endif
    break;
  }
  return 0;
}

static
  Acmd* _pullSubFrame(N_PVoice *filter, s16 *inp, s16 *outp, s32 outCount,
		      Acmd *p) 
{
  Acmd        *ptr = p;
  N_PVoice	*e = filter;
#ifndef N_MICRO
  s16 *inpp = AL_RESAMPLER_OUT;
#else
  s16 *inpp = N_AL_RESAMPLER_OUT;
#endif

  /* filter must be playing and request non-zero output samples to pull. */
  if (e->em_motion != AL_PLAYING || !outCount)
    return ptr;

  /*
   * ask all filters upstream from us to build their command
   * lists.
   */
  
  ptr = n_alResamplePull(e,inp, p);
  
  /*
   * construct our portion of the command list
   */
#ifndef N_MICRO
  aSetBuffer(ptr++, A_MAIN, *inp, AL_MAIN_L_OUT, FIXED_SAMPLE<<1);
  aSetBuffer(ptr++, A_AUX, AL_MAIN_R_OUT , AL_AUX_L_OUT ,
	     AL_AUX_R_OUT );
#endif
  
  if (e->em_first){
    e->em_first = 0;
    
    /*
     * Calculate derived parameters
     */
    e->em_ltgt = (e->em_volume * n_eqpower[e->em_pan]) >> 15;
    e->em_lratm = _getRate(e->em_cvolL, e->em_ltgt,
			   e->em_segEnd, &(e->em_lratl));
    e->em_rtgt = (e->em_volume *
		  n_eqpower[N_EQPOWER_LENGTH - e->em_pan - 1]) >> 15;
    e->em_rratm = _getRate(e->em_cvolR, e->em_rtgt, e->em_segEnd,
			   &(e->em_rratl));

#ifndef N_MICRO
    aSetVolume(ptr++, A_LEFT | A_VOL, e->em_cvolL, 0, 0);
    aSetVolume(ptr++, A_RIGHT | A_VOL, e->em_cvolR, 0, 0);
    aSetVolume(ptr++, A_LEFT  | A_RATE, e->em_ltgt, e->em_lratm, e->em_lratl);
    aSetVolume(ptr++, A_RIGHT | A_RATE, e->em_rtgt, e->em_rratm, e->em_rratl);
    aSetVolume(ptr++, A_AUX, e->em_dryamt, 0, e->em_wetamt);
    aEnvMixer (ptr++, A_INIT | A_AUX, osVirtualToPhysical(e->em_state));
  }
  else
    aEnvMixer(ptr++, A_CONTINUE | A_AUX, osVirtualToPhysical(e->em_state));
#else
    n_aSetVolume(ptr++, A_RATE, e->em_ltgt, e->em_lratm, e->em_lratl);\
    n_aSetVolume(ptr++, A_LEFT  | A_VOL, e->em_cvolL, e->em_dryamt, e->em_wetamt);\
    n_aSetVolume(ptr++, A_RIGHT | A_VOL, e->em_rtgt, e->em_rratm,  e->em_rratl);\
    n_aEnvMixer (ptr++, A_INIT, e->em_cvolR, osVirtualToPhysical(e->em_state));
}
  else
    n_aEnvMixer(ptr++, A_CONTINUE, 0, osVirtualToPhysical(e->em_state));

#endif  
  /*
   * bump the input buffer pointer
   */
  
  *inp += (FIXED_SAMPLE<<1);
  e->em_delta += FIXED_SAMPLE;
  
  return ptr;
}

#ifndef N_MICRO
#define EXP_MASK  0x7f800000
#define MANT_MASK 0x807fffff

static
  s16 _getRate(f64 vol, f64 tgt, s32 count, u16* ratel)
{
  s16         s;
  
  f64         invn = 1.0/count, eps, a, fs, mant;
  s32         i_invn, ex, indx;
  
#ifdef AUD_PROFILE
  lastCnt[++cnt_index] = osGetCount();
#endif
  
  if (count == 0){
    if (tgt >= vol){
      *ratel = 0xffff;
      return 0x7fff;
    }
    else{
      *ratel = 0;
      return 0;
    }
  }
  
  if (tgt < 1.0)
    tgt = 1.0;
  if (vol <= 0) vol = 1;	/* zero and neg values not allowed */
  
#define NBITS (3)
#define NPOS  (1<<NBITS)
#define NFRACBITS (30)
#define M_LN2		0.69314718055994530942
  /*
   * rww's parametric pow()
   Goal: compute a = (tgt/vol)^(1/count)
   
   Approach:
   (tgt/vol)^(1/count) =
   ((tgt/vol)^(1/2^30))^(2^30*1/count)
   
   (tgt/vol)^(1/2^30) ~= 1 + eps
   
   where
   
   eps ~= ln(tgt/vol)/2^30
   
   ln(tgt/vol) = ln2(tgt/vol) * ln(2)
   
   ln2(tgt/vol) = fp_exponent( tgt/vol ) +
   ln2( fp_mantissa( tgt/vol ) )
   
   fp_mantissa() and fp_exponent() are
   calculated via tricky bit manipulations of
   the floating point number. ln2() is
   approximated by a look up table.
   
   Note that this final (1+eps) value needs
   to be raised to the 2^30/count power. This
   is done by operating on the binary representaion
   of this number in the final while loop.
   
   Enjoy!
   */
  {
    f64 logtab[] = { -0.912537, -0.752072, -0.607683, -0.476438,
		       -0.356144, -0.245112, -0.142019, -0.045804  };
    
    i_invn = (s32) _ldexpf( invn, NFRACBITS );
    mant = _frexpf( tgt/vol, &ex );
    indx = (s32) (_ldexpf( mant, NBITS+1 ) ); /* NPOS <= indx < 2*NPOS */
    eps = (logtab[indx - NPOS] + ex) * M_LN2;
    eps /= _ldexpf( 1, NFRACBITS ); /* eps / 2^NFRACBITS */
    fs = (1.0 + eps);
    a = 1.0;
    while( i_invn ) {
      if( i_invn & 1 )
	a = a * fs;
      fs *= fs;
      i_invn >>= 1;
    }
  }
  
  a *= (a *= (a *= a));
  s = (s16) a;
  *ratel = (s16)(0xffff * (a - (f32) s));
  
#ifdef AUD_PROFILE
  PROFILE_AUD(rate_num, rate_cnt, rate_max, rate_min);
#endif
  return (s16)a;
  
}
#else
static
s16 _getRate(f32 vol, f32 tgt, s32 count, u16* ratel)
{
    s16         s, tmp;
    f32         invn, a, f;

#ifdef AUD_PROFILE
    lastCnt[++cnt_index] = osGetCount();
#endif
    
    if (count == 0){
        if (tgt >= vol){
            *ratel = 0xffff;
            return 0x7fff;
        }
        else{
            *ratel = 0;
            return 0;
        }
    }

    invn = 1.0F / count;

    if (tgt < 1.0F)
        tgt = 1.0F;
    if (vol <= 0.0F) vol = 1.0F;	/* zero and neg values not allowed */

    a = (tgt - vol) * invn * 8.0F;
    s = (s16)a;
    f = a - (f32)s;
    s -= 1;
    f += 1.0F;
    tmp = (s16)f;
    s += tmp;
    f -= (f32)tmp;

#ifdef AUD_PROFILE
	PROFILE_AUD( rate_num, rate_cnt, rate_max, rate_min);
#endif

    *ratel = (u16)(0xffff * f);
    return s;
}
#endif

#ifndef N_MICRO
static
  f32 _getVol(f32 ivol, s32 samples, s16 ratem, u16 ratel)
{
  f32	        r, a;
  s32	      	i;
  
#ifdef AUD_PROFILE
  lastCnt[++cnt_index] = osGetCount();
#endif
  
  /*
   * Rate values are actually rate^8
   */
  samples >>=3;
  if (samples == 0){
    return ivol;
  }
  r = ((f32) (ratem<<16) + (f32) ratel)/65536;
  
  a = 1.0;
  for (i=0; i<32; i++){
    if( samples & 1 )
      a *= r;
    samples >>= 1;
    if (samples == 0)
      break;
    r *= r;
  }
  ivol *= a;
#ifdef AUD_PROFILE
  PROFILE_AUD(vol_num, vol_cnt, vol_max, vol_min);
#endif
  return ivol;
}
#else
static
s16 _getVol(s16 ivol, s32 samples, s16 ratem, u16 ratel)
{
    s32 tmp1;
#ifdef AUD_PROFILE
    lastCnt[++cnt_index] = osGetCount();
#endif
    
    samples >>= 3;
    if (samples == 0){
        return ivol;
    }

    tmp1 = ratel * samples;
    tmp1 >>= 16;
    tmp1 += ratem * samples;
    ivol += tmp1;

#ifdef AUD_PROFILE
    PROFILE_AUD(vol_num, vol_cnt, vol_max, vol_min);
#endif
    return ivol;
}
#endif

/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// n_auxbus.c

Acmd *n_alAuxBusPull(s32 sampleOffset, Acmd *p) 
{
  Acmd        *ptr = p;
  N_ALAuxBus   *m = (N_ALAuxBus *)n_syn->auxBus;
  N_PVoice    **sources = m->sources;
  s32         i;

#ifndef N_MICRO
  aClearBuffer(ptr++, AL_AUX_L_OUT, FIXED_SAMPLE<<1);
  aClearBuffer(ptr++, AL_AUX_R_OUT, FIXED_SAMPLE<<1);
#else
  aClearBuffer(ptr++, N_AL_AUX_L_OUT, N_AL_DIVIDED<<1);
#endif

  for (i = 0; i < m->sourceCount; i++)
    ptr = n_alEnvmixerPull(sources[i],sampleOffset,ptr);
  return ptr;
}

/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// n_resample.c

/***********************************************************************
 * Resampler filter public interfaces
 ***********************************************************************/
Acmd *n_alResamplePull(N_PVoice *e, s16 *outp, Acmd *p) 
{
    Acmd        *ptr = p;
    s16         inp;
    s32         inCount;
    s32		incr;
    f32         finCount;
    
#ifdef AUD_PROFILE
    lastCnt[++cnt_index] = osGetCount();
#endif

#ifndef N_MICRO
    inp = AL_DECODER_OUT;
#else
    inp = N_AL_DECODER_OUT;
#endif
    
    /*
     * check if resampler is required
     */
    if (e->rs_upitch) {

	ptr = n_alAdpcmPull(e, &inp, FIXED_SAMPLE, p);
	aDMEMMove(ptr++, inp, *outp , FIXED_SAMPLE<<1);

    } else {

        /*
	 * clip to maximum allowable pitch
	 * FIXME: should we check for some minimum as well?
	 */
	if (e->rs_ratio > MAX_RATIO) e->rs_ratio = MAX_RATIO;

	/*
	 * quantize the pitch
	 */
	e->rs_ratio = (s32)(e->rs_ratio * UNITY_PITCH);
	e->rs_ratio = e->rs_ratio / UNITY_PITCH;

	/*
	 * determine how many samples to generate
	 */
	finCount = e->rs_delta + (e->rs_ratio * (f32)FIXED_SAMPLE);
	inCount = (s32) finCount;
	e->rs_delta = finCount - (f32)inCount;

	/*
	 * ask all filters upstream from us to build their command
	 * lists.
	 */
	ptr = n_alAdpcmPull(e, &inp,inCount, p);

	/*
	 * construct our portion of the command list
	 */
	incr = (s32)(e->rs_ratio * UNITY_PITCH);
#ifndef N_MICRO
	aSetBuffer(ptr++, 0, inp , *outp,  FIXED_SAMPLE<<1);
	aResample(ptr++, e->rs_first, incr, osVirtualToPhysical(e->rs_state));
#else
       n_aResample(ptr++, osVirtualToPhysical(e->rs_state), e->rs_first, incr, inp, 0);
#endif
	e->rs_first = 0;
    }
    
#ifdef AUD_PROFILE
    PROFILE_AUD(resampler_num, resampler_cnt, resampler_max, resampler_min);
#endif
    return ptr;
}

s32 n_alResampleParam(N_PVoice *filter, s32 paramID, void *param)
{
    N_PVoice   *r = filter;
    union {
        f32             f;
        s32             i;
    } data;
    
    switch (paramID) {
#if 0
        case (AL_FILTER_RESET):
            r->rs_delta  = 0.0;
            r->rs_first  = 1;
            r->rs_upitch = 0;
	    n_alLoadParam(filter, AL_FILTER_RESET, 0);
            break;
        case (AL_FILTER_START):
	    n_alLoadParam(filter, AL_FILTER_START, 0);
            break;
        case (AL_FILTER_SET_PITCH):
            data.i = (s32) param;
            r->rs_ratio = data.f;
            break;
	case (AL_FILTER_SET_UNITY_PITCH):
	    r->rs_upitch = 1;
            break;
#endif            
        default:
	    n_alLoadParam(filter,  paramID, param);
            break;
    }
    return 0;
}

/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
// n_reverb.c

#define RANGE 2

#ifdef AUD_PROFILE
extern u32 cnt_index, reverb_num, reverb_cnt, reverb_max, reverb_min, lastCnt[];
extern u32 load_num, load_cnt, load_max, load_min, save_num, save_cnt, save_max, save_min;
#endif

/*
 * macros
 */
#define SWAP(in, out)	\
{			\
			  s16 t = out;	\
			    out = in;		\
			      in = t;		\
			      }

Acmd *_n_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, Acmd *p);
Acmd *_n_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);
Acmd *_n_saveBuffer(ALFx *r, s16 *curr_ptr, s32 buff, Acmd *p);
Acmd *_n_filterBuffer(ALLowPass *lp, s32 buff, Acmd *p);

extern f32  _doModFunc(ALDelay *d, s32 count);
extern s32 L_INC[];

/***********************************************************************
 * Reverb filter public interfaces
 ***********************************************************************/
Acmd *n_alFxPull(s32 sampleOffset,
	       Acmd *p) 
{
  Acmd        *ptr = p;
  ALFx	*r = (ALFx *)n_syn->auxBus->fx;
  s16		i, buff1, buff2, input, output;
  s16		*in_ptr, *out_ptr, gain, *prev_out_ptr = 0;
  ALDelay	*d, *pd;
  
#ifdef AUD_PROFILE
  lastCnt[++cnt_index] = osGetCount();
#endif

  /*
   * pull channels going into this effect first
   */
  ptr = n_alAuxBusPull(sampleOffset, p);
  
#ifndef N_MICRO
  input  = AL_AUX_L_OUT;
  output = AL_AUX_R_OUT;
  buff1  = AL_TEMP_0;
  buff2  = AL_TEMP_1;
#else
  input  = N_AL_AUX_L_OUT;
  output = N_AL_AUX_R_OUT;
  buff1  = N_AL_TEMP_0;
  buff2  = N_AL_TEMP_1;
#endif

#ifndef N_MICRO
  aSetBuffer(ptr++, 0, 0, 0, FIXED_SAMPLE<<1);  /* set the buffer size */
  aMix(ptr++, 0, 0xda83, AL_AUX_L_OUT, input); /* .707L = L - .293L */
  aMix(ptr++, 0, 0x5a82, AL_AUX_R_OUT, input); /* mix the AuxL and AuxR into the AuxL */
#else
  aMix(ptr++, 0, 0xda83, N_AL_AUX_L_OUT, input);
  aMix(ptr++, 0, 0x5a82, N_AL_AUX_R_OUT, input);
#endif
  /* and write the mixed value to the delay line at r->input */
  ptr = _n_saveBuffer(r, r->input, input, ptr);
  
  aClearBuffer(ptr++, output, FIXED_SAMPLE<<1); /* clear the AL_AUX_R_OUT */
  
  for (i = 0; i < r->section_count; i++) {
    d  = &r->delay[i];  /* get the ALDelay structure */
    in_ptr  = &r->input[-d->input];
    out_ptr = &r->input[-d->output];
    
    if (in_ptr == prev_out_ptr) {
      SWAP(buff1, buff2);
    } else {  /* load data at in_ptr into buff1 */
      ptr = _n_loadBuffer(r, in_ptr, buff1, FIXED_SAMPLE, ptr);
    }
    ptr = _n_loadOutputBuffer(r, d, buff2, ptr);
    
    if (d->ffcoef) {
      aMix(ptr++, 0, (u16)d->ffcoef, buff1, buff2);
      if (!d->rs && !d->lp) {
	ptr = _n_saveBuffer(r, out_ptr, buff2, ptr);
      }
    }
    
    if (d->fbcoef) {
      aMix(ptr++, 0, (u16)d->fbcoef, buff2, buff1);
      ptr = _n_saveBuffer(r, in_ptr, buff1, ptr);
    }
    
    if (d->lp)
      ptr = _n_filterBuffer(d->lp, buff2, ptr);
    
    if (!d->rs)
      ptr = _n_saveBuffer(r, out_ptr, buff2, ptr);
    
    if (d->gain)
      aMix(ptr++, 0, (u16)d->gain, buff2, output);
    
    prev_out_ptr = &r->input[d->output];
  }
  
  /*
   * bump the master delay line input pointer
   * modulo the length
   */
  r->input += FIXED_SAMPLE;
  if (r->input > &r->base[r->length])
    r->input -= r->length;
  
  /*
   * output already in AL_AUX_R_OUT
   *      just copy to AL_AUX_L_OUT
   */
#ifndef N_MICRO
  aDMEMMove(ptr++, output, AL_AUX_L_OUT, FIXED_SAMPLE<<1);
#else
  aDMEMMove(ptr++, output, N_AL_AUX_L_OUT, FIXED_SAMPLE<<1);
#endif
  
#ifdef AUD_PROFILE
  PROFILE_AUD(reverb_num, reverb_cnt, reverb_max, reverb_min);
#endif
  return ptr;
}

/*
 * This routine gets called by alSynSetFXParam. No checking takes place to
 * verify the validity of the paramID or the param value. input and output
 * values must be 8 byte aligned, so round down any param passed.
 */
s32 n_alFxParamHdl(void *filter, s32 paramID, void *param)
{
	ALFx   *f = (ALFx *) filter;
	s32    p = (paramID - 2) % 8;
	s32    s = (paramID - 2) / 8;
	s32    val = *(s32*)param;

#define INPUT_PARAM         0
#define OUTPUT_PARAM        1
#define FBCOEF_PARAM        2
#define FFCOEF_PARAM        3
#define GAIN_PARAM          4
#define CHORUSRATE_PARAM    5
#define CHORUSDEPTH_PARAM   6
#define LPFILT_PARAM        7

	switch(p)
	{
		case INPUT_PARAM:
			f->delay[s].input = (u32)val & 0xFFFFFFF8;
			break;
		case OUTPUT_PARAM:
			f->delay[s].output = (u32)val & 0xFFFFFFF8;
			break;
		case FFCOEF_PARAM:
			f->delay[s].ffcoef = (s16)val;
			break;
		case FBCOEF_PARAM:
			f->delay[s].fbcoef = (s16)val;
			break;
		case GAIN_PARAM:
			f->delay[s].gain = (s16)val;
			break;
		case CHORUSRATE_PARAM:
			// /* f->delay[s].rsinc = ((f32)val)/0xffffff; */
			f->delay[s].rsinc = ((((f32)val)/1000) * RANGE)/n_syn->outputRate;
			break;

/*
 * the following constant is derived from:
 *
 *      ratio = 2^(cents/1200)
 *
 * and therefore for hundredths of a cent
 *                     x
 *      ln(ratio) = ---------------
 *              (120,000)/ln(2)
 * where
 *      120,000/ln(2) = 173123.40...
 */
#define CONVERT 173123.404906676f
#define LENGTH  (f->delay[s].output - f->delay[s].input)

		case CHORUSDEPTH_PARAM:
			/*f->delay[s].rsgain = (((f32)val) / CONVERT) * LENGTH; */
			f->delay[s].rsgain = (((f32)val) / CONVERT) * LENGTH;
			break;
		case LPFILT_PARAM:
			if(f->delay[s].lp)
			{
				f->delay[s].lp->fc = (s16)val;
				_init_lpfilter(f->delay[s].lp);
			}
			break;
	}
	return 0;
}

Acmd *_n_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, Acmd *p)
{
  Acmd        *ptr = p;
#ifndef N_MICRO
  s32         ratio, count, rbuff = AL_TEMP_2;
#else
  s32         ratio, count, rbuff = N_AL_TEMP_2;
#endif
  s16         *out_ptr;
  f32         fincount, fratio, delta;
  s32         ramalign = 0, length;
  static f32  val=0.0, lastval=-10.0;
  static f32  blob=0;
  s32 incount = FIXED_SAMPLE;

  if (d->rs) {
    length = d->output - d->input;
    delta = _doModFunc(d, incount);
    delta /= length;
    delta = (s32)(delta * UNITY_PITCH);
    delta = delta / UNITY_PITCH;
    fratio = 1.0F - delta;
    fincount = d->rs->delta + (fratio * (f32)incount);
    count = (s32) fincount;
    d->rs->delta = fincount - (f32)count;
    out_ptr = &r->input[-(d->output - d->rsdelta)];
    ramalign = ((s32)out_ptr & 0x7) >> 1;
    ptr = _n_loadBuffer(r, out_ptr - ramalign, rbuff, count + ramalign, ptr);
    
    ratio = (s32)(fratio * UNITY_PITCH);
#ifndef N_MICRO
    aSetBuffer(ptr++, 0, rbuff + (ramalign<<1), buff, incount<<1);
    aResample(ptr++, d->rs->first, ratio, osVirtualToPhysical(d->rs->state));
#else
    {
        s16 tmp;

        tmp = buff >> 8;
        n_aResample(ptr++, osVirtualToPhysical(d->rs->state), d->rs->first, ratio, rbuff + (ramalign<<1), tmp);
      }
#endif    
    d->rs->first = 0;
    d->rsdelta += count - incount;
  } else {
    out_ptr = &r->input[-d->output];
    ptr = _n_loadBuffer(r, out_ptr, buff, FIXED_SAMPLE, ptr);
  }
  
  return ptr;
}

Acmd *_n_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff,s32 count, Acmd *p)
{
    Acmd    *ptr = p;
    s32     after_end, before_end;
    s16     *updated_ptr, *delay_end;

#ifdef AUD_PROFILE
    lastCnt[++cnt_index] = osGetCount();
#endif

    delay_end = &r->base[r->length];

#ifdef _DEBUG
    if(curr_ptr > delay_end)
        __osError(ERR_ALMODDELAYOVERFLOW, 1, delay_end - curr_ptr);
#endif

    if (curr_ptr < r->base)
    curr_ptr += r->length;
    updated_ptr = curr_ptr + count;
    
    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        
#ifndef N_MICRO
        aSetBuffer(ptr++, 0, buff, 0, before_end<<1);
        aLoadBuffer(ptr++, osVirtualToPhysical(curr_ptr));
        aSetBuffer(ptr++, 0, buff+(before_end<<1), 0, after_end<<1);
        aLoadBuffer(ptr++, osVirtualToPhysical(r->base));
    } else {
        aSetBuffer(ptr++, 0, buff, 0, count<<1);
        aLoadBuffer(ptr++, osVirtualToPhysical(curr_ptr));
    }

    aSetBuffer(ptr++, 0, 0, 0, count<<1);
#else
      n_aLoadBuffer(ptr++, before_end<<1, buff, osVirtualToPhysical(curr_ptr));
        n_aLoadBuffer(ptr++, after_end<<1, buff+(before_end<<1), osVirtualToPhysical(r->base));
   } else {
        n_aLoadBuffer(ptr++, count<<1, buff, osVirtualToPhysical(curr_ptr));
   }
#endif

#ifdef AUD_PROFILE
    PROFILE_AUD(load_num, load_cnt, load_max, load_min);
#endif
    return ptr;
}

Acmd *_n_saveBuffer(ALFx *r, s16 *curr_ptr, s32 buff, Acmd *p)
{
    Acmd    *ptr = p;
    s32     after_end, before_end;
    s16     *updated_ptr, *delay_end;
#ifdef AUD_PROFILE
    lastCnt[++cnt_index] = osGetCount();
#endif

    delay_end = &r->base[r->length];
    if (curr_ptr < r->base)      /* probably just security */
        curr_ptr += r->length;   /* shouldn't occur */
    updated_ptr = curr_ptr + FIXED_SAMPLE;

    if (updated_ptr > delay_end) { /* if the data wraps past end of r->base */
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;

#ifndef N_MICRO
        aSetBuffer(ptr++, 0, 0, buff, before_end<<1);
        aSaveBuffer(ptr++, osVirtualToPhysical(curr_ptr));
        aSetBuffer(ptr++, 0, 0, buff+(before_end<<1), after_end<<1);
        aSaveBuffer(ptr++, osVirtualToPhysical(r->base));
        aSetBuffer(ptr++, 0, 0, 0, FIXED_SAMPLE<<1);
    } else {
        aSetBuffer(ptr++, 0, 0, buff, FIXED_SAMPLE<<1);
        aSaveBuffer(ptr++, osVirtualToPhysical(curr_ptr));
    }
#else
        n_aSaveBuffer(ptr++, before_end<<1, buff, osVirtualToPhysical(curr_ptr));
        n_aSaveBuffer(ptr++, after_end<<1, buff+(before_end<<1), osVirtualToPhysical(r->base));
   } else {
        n_aSaveBuffer(ptr++, FIXED_SAMPLE<<1, buff, osVirtualToPhysical(curr_ptr));
   }

#endif

#ifdef AUD_PROFILE
    PROFILE_AUD(save_num, save_cnt, save_max, save_min);
#endif
    return ptr;

}

Acmd *_n_filterBuffer(ALLowPass *lp, s32 buff, Acmd *p)
{
    Acmd	*ptr = p;
#ifndef N_MICRO
    aSetBuffer(ptr++, 0, buff, buff, FIXED_SAMPLE<<1);
    aLoadADPCM(ptr++, 32, osVirtualToPhysical(lp->fcvec.fccoef));
    aPoleFilter(ptr++, lp->first, lp->fgain, osVirtualToPhysical(lp->fstate));
#else
{
  s16  tmp;

  tmp = buff >> 8;
  n_aLoadADPCM(ptr++, 32, osVirtualToPhysical(lp->fcvec.fccoef));
  n_aPoleFilter(ptr++, lp->first, lp->fgain, tmp, osVirtualToPhysical(lp->fstate));
}
#endif

    lp->first = 0;

    return ptr;
}

/*
 * Generate a triangle wave from -1 to 1, and find the current position
 * in the wave. (Rate of the wave is controlled by d->rsinc, which is chorus
 * rate) Multiply the current triangle wave value by d->rsgain, (chorus depth)
 * which is expressed in number of samples back from output pointer the chorus
 * should go at it's full chorus. In otherwords, this function returns a number
 * of samples the output pointer should modulate backwards.
 */
f32 _doModFunc(ALDelay *d, s32 count)
{
  f32 val;

  /*
   * generate bipolar sawtooth
   * from -RANGE to +RANGE
   */
  d->rsval += d->rsinc * count;
  d->rsval = (d->rsval > RANGE) ? d->rsval-(RANGE*2) : d->rsval;

  /*
   * convert to monopolar triangle
   * from 0 to RANGE
   */
  val = d->rsval;
  val = (val < 0) ? -val : val;

  /*
   * convert to bipolar triangle 
   * from -1 to 1
   */
  val -= RANGE/2;

  return(d->rsgain * val);
}

#undef RANGE
#undef CONVERT
#undef LENGTH

/////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// n_sl.c

N_ALGlobals *n_alGlobals=0;
N_ALSynth *n_syn=0;

// 0x8002CE88
void n_alInit(N_ALGlobals *g, ALSynConfig *c)
{
	if (!n_alGlobals) {
		n_alGlobals = g;

		if (!n_syn) {
			n_syn = &n_alGlobals->drvr;
			n_alSynNew(c);
		}
	}
}

// 0x0x8002CE48
void n_alClose(N_ALGlobals *g)
{
	if (n_alGlobals) {
		n_alSynDelete();
		n_alGlobals = NULL;
		n_syn = NULL;
	}
}


////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// n_synthesizer.c

#ifdef AUD_PROFILE
#include <os.h>
extern u32 cnt_index, drvr_num, drvr_cnt, drvr_max, drvr_min, lastCnt[];
extern u32 client_num, client_cnt, client_max, client_min;
#endif

#ifndef MIN
#   define MIN(a,b) (((a)<(b))?(a):(b))
#endif

static s32 __n_nextSampleTime(ALPlayer **client);
static s32 _n_timeToSamplesNoRound( s32 micros);

/***********************************************************************
 * Synthesis driver public interfaces
 ***********************************************************************/
// 0x8002CB48
void n_alSynNew(ALSynConfig *c)
{
	s32         i;
	N_ALVoice  *vv;
	N_PVoice   *pv;
	N_ALVoice  *vvoices;
	N_PVoice   *pvoices;
	ALHeap      *hp = c->heap;
	ALSave      *save;
	ALFilter    *sources;
	N_PVoice   *m_sources;
	ALParam     *params;
	ALParam     *paramPtr;

	n_syn->head            = NULL;
	n_syn->n_seqp1         = NULL;
	n_syn->n_seqp2         = NULL;
	n_syn->n_sndp          = NULL;
	n_syn->numPVoices      = c->maxPVoices;
	n_syn->curSamples      = 0;
	n_syn->paramSamples    = 0;
	n_syn->outputRate      = c->outputRate;
#ifndef N_MICRO
	n_syn->maxOutSamples   = AL_MAX_RSP_SAMPLES;
#else
	n_syn->maxOutSamples   = FIXED_SAMPLE;
#endif
	n_syn->dma             = (ALDMANew) c->dmaproc;

/******* save new *******************************/
	n_syn->sv_dramout = 0;
	n_syn->sv_first = 1;

/******* aux new *******************************/
	n_syn->auxBus = (N_ALAuxBus *)alHeapAlloc(hp, 1, sizeof(N_ALAuxBus));

	n_syn->auxBus->sourceCount = 0;
	n_syn->auxBus->maxSources = c->maxPVoices;
	n_syn->auxBus->sources = (N_PVoice **)
		alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice *));

/******* main new *******************************/
	n_syn->mainBus = (N_ALMainBus *)alHeapAlloc(hp, 1, sizeof(N_ALMainBus));

/******* fx new *******************************/

	if (c->fxType != AL_FX_NONE){
		n_syn->auxBus->fx = n_alSynAllocFX(0, c, hp);
		n_syn->mainBus->filter.handler = (N_ALCmdHandler)n_alFxPull;
	} else {
		n_syn->mainBus->filter.handler = (N_ALCmdHandler)n_alAuxBusPull;
	}

	n_syn->pFreeList.next = 0;
	n_syn->pFreeList.prev = 0;
	n_syn->pLameList.next = 0;
	n_syn->pLameList.prev = 0;
	n_syn->pAllocList.next = 0;
	n_syn->pAllocList.prev = 0;

	pvoices = alHeapAlloc(hp, c->maxPVoices, sizeof(N_PVoice));
	for (i = 0; i < c->maxPVoices; i++) {
		pv = &pvoices[i];
		alLinkMacro((ALLink *)pv, &n_syn->pFreeList);
		pv->vvoice = 0;
		alN_PVoiceNew(pv, n_syn->dma, hp);
		n_syn->auxBus->sources[n_syn->auxBus->sourceCount++] = pv;
	}

	params = alHeapAlloc(hp, c->maxUpdates, sizeof(ALParam));
	n_syn->paramList = 0;
	for (i = 0; i < c->maxUpdates; i++) {
		paramPtr= &params[i];
		paramPtr->next = n_syn->paramList;
		n_syn->paramList = paramPtr;
	}
	n_syn->heap = hp;
}

// TODO
Acmd *n_alAudioFrame(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen)
{
  ALPlayer    *client;
  s16         tmp = 0;        /* Starting buffer in DMEM */
  Acmd        *cmdlEnd = cmdList;
  Acmd        *cmdPtr;
  s32         nOut;
  s16         *lOutBuf = outBuf;
  
#ifdef AUD_PROFILE
  lastCnt[++cnt_index] = osGetCount();
#endif
  
  if (n_syn->head == 0) {
    *cmdLen = 0;
    return cmdList;         /* nothing to do */
  }    

  /*
   * run down list of clients and execute callback if needed this
   * subframe. Here we do all the work for the frame at the
   * start. Time offsets that occur before the next frame are
   * executed "early".
   */
  
#ifdef AUD_PROFILE
  lastCnt[++cnt_index] = osGetCount();
#endif
  
  /*
   * paramSamples = time of next parameter change.
   * curSamples = current sample time.
   * so paramSamples - curSamples is the time until the next parameter change.
   * if the next parameter change occurs within this frame time (outLen),
   * then call back the client that contains the parameter change.
   * Note, paramSamples must be rounded down to 16 sample boundary for use
   * during the client handler.
   */
  
  for (n_syn->paramSamples = __n_nextSampleTime( &client);
       n_syn->paramSamples - n_syn->curSamples < outLen;
       n_syn->paramSamples = __n_nextSampleTime( &client))
    {
      n_syn->paramSamples &= ~0xf;
      client->samplesLeft += _n_timeToSamplesNoRound((*client->handler)(client));
    }
  
  /* for safety's sake, always store paramSamples aligned to 16 sample
     boundary.
   * this way, if an voice handler routine gets called outside
   the N_ALVoiceHandler
   * routine (alSynAllocVoice) it will get timestamped with an
   aligned value and
   * will be processed immediately next audio frame.
   */
  n_syn->paramSamples &= ~0xf;
  
#ifdef AUD_PROFILE
  PROFILE_AUD(client_num, client_cnt, client_max, client_min);
#endif
  
  /*
   * Now build the command list in small chunks
   */
  while (outLen > 0){
    nOut = MIN(n_syn->maxOutSamples, outLen);
    
    /*
     * construct the command list for each physical voice by calling
     * the head of the filter chain.
     */
    cmdPtr = cmdlEnd;
#ifndef N_MICRO
    aSegment(cmdPtr++, 0, 0);
#endif
    n_syn->sv_dramout = (s32)lOutBuf;
    cmdlEnd = n_alSavePull(n_syn->curSamples, cmdPtr);
    
    outLen -= nOut;
    lOutBuf += nOut<<1;     /* For Stereo */
    
    // This is different...
    if (n_syn->curSamples < 0x7FFFFF47) {
        n_syn->curSamples += nOut;
    } else {
        n_syn->curSamples = 0x80000090;
    }
  }
  *cmdLen = (s32) (cmdlEnd - cmdList);
  
  _n_collectPVoices(); /* collect free physical voices */
  
#ifdef AUD_PROFILE
  PROFILE_AUD(drvr_num, drvr_cnt, drvr_max, drvr_min);
#endif
  return cmdlEnd;
}

////////////////////////////////////////

////////////////////////////////////////
// n_save.c

// 0x2D2BC
Acmd *n_alSavePull( s32 sampleOffset, Acmd *p)
{
	Acmd        *ptr = p;

	ptr = n_alMainBusPull(sampleOffset, ptr);

#ifndef N_MICRO
	aSetBuffer (ptr++, 0, 0, 0, FIXED_SAMPLE<<1);
	aInterleave(ptr++, AL_MAIN_L_OUT, AL_MAIN_R_OUT);
	aSetBuffer (ptr++, 0, 0, 0, FIXED_SAMPLE<<2);
	aSaveBuffer(ptr++, n_syn->sv_dramout);
#else
	// Oneliner
	n_aInterleave(ptr++); \
	n_aSaveBuffer(ptr++, FIXED_SAMPLE<<2, 0, n_syn->sv_dramout);
#endif
	return ptr;
}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// n_mainbus.c

//0x2D248
Acmd *n_alMainBusPull( s32 sampleOffset, Acmd *p)
{
	Acmd        *ptr = p;

#ifndef N_MICRO
	aClearBuffer(ptr++, AL_MAIN_L_OUT, FIXED_SAMPLE<<1);
	aClearBuffer(ptr++, AL_MAIN_R_OUT, FIXED_SAMPLE<<1);
#else
	aClearBuffer(ptr++, N_AL_MAIN_L_OUT, N_AL_DIVIDED<<1);
#endif

	ptr = (n_syn->mainBus->filter.handler)(sampleOffset,ptr);

#ifndef N_MICRO
	aSetBuffer(ptr++, 0, 0, 0, FIXED_SAMPLE<<1);
	aMix(ptr++, 0, 0x7fff, AL_AUX_L_OUT, AL_MAIN_L_OUT);
	aMix(ptr++, 0, 0x7fff, AL_AUX_R_OUT, AL_MAIN_R_OUT);
#else
	aMix(ptr++, 0, 0x7fff, N_AL_AUX_L_OUT, N_AL_MAIN_L_OUT);
	aMix(ptr++, 0, 0x7fff, N_AL_AUX_R_OUT, N_AL_MAIN_R_OUT);
#endif

	return ptr;
}

////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// n_synthesizer.c

static s32 _n_timeToSamplesNoRound(s32 micros);



/***********************************************************************
 * Synthesis driver private interfaces
 ***********************************************************************/

ALParam *__n_allocParam() 
{
  ALParam *update = 0;
  
  if (n_syn->paramList) {        
    update = n_syn->paramList;
    n_syn->paramList = n_syn->paramList->next;
    update->next = 0;
  }
  return update;
}

void _n_freeParam(ALParam *param) 
{
  param->next = n_syn->paramList;
  n_syn->paramList = param;
}

void _n_collectPVoices(void)
{
  ALLink       *dl;
  N_PVoice      *pv;
  
  while ((dl = n_syn->pLameList.next) != 0) {
    pv = (N_PVoice *)dl;
    
    /* ### remove from mixer */
    
    alUnlink(dl);
    alLink(dl, &n_syn->pFreeList);        
  }
}

void _n_freePVoice(N_PVoice *pvoice) 
{
  /*
   * move the voice from the allocated list to the lame list
   */
  alUnlink((ALLink *)pvoice);
  alLink((ALLink *)pvoice, &n_syn->pLameList);
}

/*
	Add 0.5 to adjust the average affect of
	the truncation error produced by casting
	a float to an int.
*/
s32 _n_timeToSamplesNoRound(s32 micros)
{
	f32 tmp = ((f32)micros) * n_syn->outputRate / 1000000.0f + 0.5f;

	return (s32)tmp;
}

s32 _n_timeToSamples(s32 micros) {
	return _n_timeToSamplesNoRound(micros) & ~0xF;
}

static s32 __n_nextSampleTime(ALPlayer **client) 
{
  ALMicroTime temp,delta = 0x7fffffff;     /* max delta for s32 */
  ALPlayer *cl;
  
  *client = 0;

#if 0  
  for (cl = n_syn->head; cl != 0; cl = cl->next) {
    if ((cl->samplesLeft - n_syn->curSamples) < delta) {
      *client = cl;
      delta = cl->samplesLeft - n_syn->curSamples;
    }
  }
#endif

  if( n_syn->n_sndp )
    if( (temp = n_syn->n_sndp->samplesLeft - n_syn->curSamples) < delta ) {
      *client = n_syn->n_sndp;
      delta = temp;
    }
  
  if( n_syn->n_seqp1 )
    if( (temp = n_syn->n_seqp1->samplesLeft - n_syn->curSamples) < delta ) {
      *client = n_syn->n_seqp1;
      delta = temp;
    }
  
  if( n_syn->n_seqp2 )
    if( (n_syn->n_seqp2->samplesLeft - n_syn->curSamples) < delta ) {
      *client = n_syn->n_seqp2;
    }
  
  return (*client)->samplesLeft;
}

///////////////////////////////////////////////////////

f32 D_8003D318_3DF18 = 1.0F;
u16 D_8003D31C_3DF1C = 0xFFFF;
s32 sRandomSeed2 = 1;
s32 sRandomSeed1 = 1;

/////////////////////////////////////////////////////////
// n_csplayer.c

static ALMicroTime      __n_CSPVoiceHandler(void *node);
static void		__n_CSPHandleNextSeqEvent(N_ALCSPlayer *seqp);
static void             __n_CSPHandleMIDIMsg(N_ALCSPlayer *seqp, N_ALEvent *event);
static void             __n_CSPHandleMetaMsg(N_ALCSPlayer *seqp, N_ALEvent *event);
static void             __n_CSPRepostEvent(ALEventQueue *evtq, N_ALEventListItem *item);
static void		__n_setUsptFromTempo(N_ALCSPlayer *seqp, f32 tempo);

/*
 * Sequence Player public functions
 */
void n_alCSPNew(N_ALCSPlayer *seqp, ALSeqpConfig *c)
{
  s32                 i;
  N_ALEventListItem  *items;
  N_ALVoiceState     *vs;
  N_ALVoiceState     *voices;
  
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
  seqp->curTime       = 0;
  seqp->initOsc       = c->initOsc;
  seqp->updateOsc     = c->updateOsc;
  seqp->stopOsc       = c->stopOsc;
  
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
  
  /*
   * init the event queue
   */
  items = alHeapAlloc(hp, c->maxEvents, sizeof(N_ALEventListItem));
  n_alEvtqNew(&seqp->evtq, items, c->maxEvents);
  
  
  /*
   * add ourselves to the driver
   */
  seqp->node.next       = NULL;
  seqp->node.handler    = __n_CSPVoiceHandler;
  seqp->node.clientData = seqp;
  n_alSynAddSeqPlayer( &seqp->node);  
}

/*************************************************************
 * private routines or driver callback routines
 *************************************************************/
static ALMicroTime __n_CSPVoiceHandler(void *node)
{
  N_ALCSPlayer    *seqp = (N_ALCSPlayer *) node;
  N_ALEvent        evt;
  N_ALVoice       *voice;
  ALMicroTime      delta;
  N_ALVoiceState  *vs;
  void            *oscState;
  f32              oscValue;
  u8               chan;
  
  do {
    switch (seqp->nextEvent.type) {
    case (AL_SEQ_REF_EVT):
      __n_CSPHandleNextSeqEvent(seqp);
      break;
      
    case (AL_SEQP_API_EVT):
      evt.type = AL_SEQP_API_EVT;
      n_alEvtqPostEvent(&seqp->evtq, (N_ALEvent *)&evt, seqp->frameTime);
      break;
      
    case (AL_NOTE_END_EVT):
      voice = seqp->nextEvent.msg.note.voice;
      
      n_alSynStopVoice( voice);
      n_alSynFreeVoice( voice);
      vs = (N_ALVoiceState *)voice->clientPrivate;
      if(vs->flags)
	__n_seqpStopOsc((N_ALSeqPlayer*)seqp,vs);
      __n_unmapVoice((N_ALSeqPlayer*)seqp, voice); 
      break;
      
    case (AL_SEQP_ENV_EVT):
      voice = seqp->nextEvent.msg.vol.voice;
      vs = (N_ALVoiceState *)voice->clientPrivate;
      
      if(vs->envPhase == AL_PHASE_ATTACK)
	vs->envPhase = AL_PHASE_DECAY;
      
      delta = seqp->nextEvent.msg.vol.delta;
      vs->envEndTime = seqp->curTime + delta;
      vs->envGain = seqp->nextEvent.msg.vol.vol;
      n_alSynSetVol( voice, __n_vsVol(vs, (N_ALSeqPlayer*)seqp), delta);
      break;
      
    case (AL_TREM_OSC_EVT):
      vs = seqp->nextEvent.msg.osc.vs;
      oscState = seqp->nextEvent.msg.osc.oscState;
      delta = (*seqp->updateOsc)(oscState,&oscValue);
      vs->tremelo = (u8)oscValue;
      n_alSynSetVol( &vs->voice, __n_vsVol(vs,(N_ALSeqPlayer*)seqp),
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
      n_alSynSetPitch(&vs->voice, (vs->pitch * ((((vs->vibrato - 1.0F) * seqp->chanState[chan].unk_0x10) / 127.0F) + 1.0F)) * seqp->chanState[chan].pitchBend);
      evt.type = AL_VIB_OSC_EVT;
      evt.msg.osc.vs = vs;
      evt.msg.osc.oscState = oscState;
      evt.msg.osc.chan = chan;
      n_alEvtqPostEvent(&seqp->evtq, &evt, delta);
      break;
      
    case (AL_SEQP_MIDI_EVT):
    case (AL_CSP_NOTEOFF_EVT):	/* nextEvent is a note off midi message */
      __n_CSPHandleMIDIMsg(seqp, &seqp->nextEvent);
      break;
      
    case (AL_SEQP_META_EVT):
      __n_CSPHandleMetaMsg(seqp, &seqp->nextEvent);
      break;
      
    case (AL_SEQP_VOL_EVT):
      seqp->vol =  seqp->nextEvent.msg.spvol.vol;
      for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
	n_alSynSetVol( &vs->voice,
		      __n_vsVol(vs, (N_ALSeqPlayer*)seqp),
		      __n_vsDelta(vs, seqp->curTime));
      }
      break;
      
    case (AL_SEQP_PLAY_EVT):
      seqp->masterVol = 0x64; // This is new...
      if (seqp->state != AL_PLAYING) {
	seqp->state = AL_PLAYING;
    
	__n_CSPPostNextSeqEvent(seqp);
	/* seqp must be AL_PLAYING before we call this routine. */
      }
      break;
      
    case (AL_SEQP_STOP_EVT):
      if ( seqp->state == AL_STOPPING ) {
	for (vs = seqp->vAllocHead; vs != 0; vs = seqp->vAllocHead) {
#ifdef _DEBUG
	  __osError(ERR_ALCSPVNOTFREE, 2, vs->channel, vs->key);
#endif                    
	  n_alSynStopVoice( &vs->voice);
	  n_alSynFreeVoice( &vs->voice);
	  if(vs->flags)
	    __n_seqpStopOsc((N_ALSeqPlayer*)seqp,vs);
	  __n_unmapVoice((N_ALSeqPlayer*)seqp, &vs->voice);
	}
    __n_initChanState((N_ALSeqPlayer*)seqp); // This is new...
	seqp->state = AL_STOPPED;
	
	/* alEvtqFlush(&seqp->evtq); - Don't flush event queue
	   anymore. */
	/* sct 1/3/96 - Don't overwrite nextEvent with
	   AL_SEQP_API_EVT or set nextDelta to
	   AL_USEC_PER_FRAME since we're not stopping event
	   processing. */
	/* sct 1/3/96 - Don't return here since we keep
	   processing events as usual. */
      }
      break;
      
    case (AL_SEQP_STOPPING_EVT):
      if (seqp->state == AL_PLAYING) {
	/* sct 12/29/95 - Remove events associated with the
	 * stopping sequence.  For compact sequence player,
	 * also remove all queued note off events since they
	 * are not contained in a compact sequence but are
	 * generated in response to note ons.  Note that
	 * flushing AL_SEQP_MIDI_EVTs may flush events that
	 * were posted after the call to alSeqpStop, so the
	 * application must queue these events either when
	 * the player is fully stopped, or when it is
	 * playing. */
	n_alEvtqFlushType(&seqp->evtq, AL_SEQ_REF_EVT);
	n_alEvtqFlushType(&seqp->evtq, AL_CSP_NOTEOFF_EVT);
	n_alEvtqFlushType(&seqp->evtq, AL_SEQP_MIDI_EVT);
	
	/* sct 1/3/96 - Check to see which voices need to be
	   killed and release them. */
	/* Unkilled voices should have note end events
	   occurring prior to KILL_TIME. */
	for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
	  if (__n_voiceNeedsNoteKill ((N_ALSeqPlayer*)seqp, &vs->voice, KILL_TIME))
	    __n_seqpReleaseVoice((N_ALSeqPlayer*)seqp, &vs->voice, KILL_TIME);
	}
	
	seqp->state = AL_STOPPING;
	evt.type = AL_SEQP_STOP_EVT;
	n_alEvtqPostEvent(&seqp->evtq, &evt, AL_EVTQ_END);
      }
      break;
      
    case (AL_SEQP_PRIORITY_EVT):
      chan = seqp->nextEvent.msg.sppriority.chan;
      seqp->chanState[chan].priority = seqp->nextEvent.msg.sppriority.priority;
      break;
      
    case (AL_SEQP_SEQ_EVT):

#ifdef _DEBUG
      assert(seqp->state != AL_PLAYING);
#endif

      /* Must be done playing to change sequences. */
      
      seqp->target = seqp->nextEvent.msg.spseq.seq;
      __n_setUsptFromTempo (seqp, 500000.0);
      if (seqp->bank)
	__n_initFromBank((N_ALSeqPlayer *)seqp, seqp->bank);
      break;
      
    case (AL_SEQP_BANK_EVT):

#ifdef _DEBUG
      assert(seqp->state == AL_STOPPED);
#endif

      /* Must be fully stopped to change banks. */
      
      seqp->bank = seqp->nextEvent.msg.spbank.bank;
      __n_initFromBank((N_ALSeqPlayer *)seqp, seqp->bank);
      break;
      
      /* sct 11/6/95 - these events should
	 now be handled by __n_CSPHandleNextSeqEvent */

    case (AL_SEQ_END_EVT+20):
        // TODO: may be wrong case, may be wrong RHS
        seqp->unknown0 = seqp->nextEvent.msg.spseq.seq;
        __n_initFromBank((N_ALSeqPlayer *)seqp, seqp->nextEvent.msg.spseq.seq);
        break;
    case (AL_SEQ_END_EVT+21):
        // TODO: may be wrong case, may be wrong RHS
        seqp->unknown1 = seqp->nextEvent.msg.spseq.seq;
        __n_initFromBank((N_ALSeqPlayer *)seqp, seqp->nextEvent.msg.spseq.seq);
        break;
    case (AL_SEQ_END_EVT):
    case (AL_TEMPO_EVT):
    case (AL_SEQ_MIDI_EVT):

#ifdef _DEBUG
      assert(FALSE);		
#endif
      break;
    }
    seqp->nextDelta = n_alEvtqNextEvent(&seqp->evtq, &seqp->nextEvent); 
    
  } while (seqp->nextDelta == 0);
  
  /*
   * assume that next callback won't be more than half an
   * hour away
   */
  seqp->curTime += seqp->nextDelta;		/* sct 11/7/95 */
  return seqp->nextDelta/D_8003D318_3DF18; // this is different
}

/*
  Call this routine to handle the next event in the sequence.
  Assumes that the next sequence event is scheduled to be processed
  immediately since it does not check the event's tick time.
  
  sct 11/7/95
  */
static void
  __n_CSPHandleNextSeqEvent(N_ALCSPlayer *seqp)
{
  N_ALEvent	evt;
  
  /* sct 1/5/96 - Do nothing if we don't have a target sequence. */
  if (seqp->target == NULL)
    return;
  
  n_alCSeqNextEvent(seqp->target, &evt);
  
  switch (evt.type) {
  case AL_SEQ_MIDI_EVT:
    __n_CSPHandleMIDIMsg(seqp, &evt);
    __n_CSPPostNextSeqEvent(seqp);
    break;
    
  case AL_TEMPO_EVT:
    __n_CSPHandleMetaMsg(seqp, &evt);
    __n_CSPPostNextSeqEvent(seqp);
    break;
    
  case AL_SEQ_END_EVT:
    seqp->state = AL_STOPPING;      
    evt.type    = AL_SEQP_STOP_EVT;
    n_alEvtqPostEvent(&seqp->evtq, &evt, AL_EVTQ_END);
    break;
    
  case AL_TRACK_END:
  case AL_CSP_LOOPSTART:
  case AL_CSP_LOOPEND:
    __n_CSPPostNextSeqEvent(seqp);
    break;
    
  default:

#ifdef _DEBUG
    assert(FALSE);	/* Sequence event type not supported. */
#endif

    break;
  }
}


static void __n_CSPHandleMIDIMsg(N_ALCSPlayer *seqp, N_ALEvent *event)
{
  N_ALVoice          *voice;
  N_ALVoiceState     *vs;
  s32                 status;
  u8                  chan;
  u8                  key;
  u8                  vel;
  u8                  byte1;
  u8                  byte2;
  ALMIDIEvent         *midi = &event->msg.midi;
  s16                 vol;
  N_ALEvent           evt;
  ALMicroTime         deltaTime;
  N_ALVoiceState     *vstate;
  ALPan   		    pan;
  ALFxRef		        fxref;
  
  
  status = midi->status & AL_MIDI_StatusMask;
  chan = midi->status & AL_MIDI_ChannelMask;
  byte1 = key  = midi->byte1;
  byte2 = vel  = midi->byte2;
  
  switch (status) {
  case (AL_MIDI_NoteOn):
    
    if (vel != 0) /* a real note on */ {
      ALVoiceConfig   config;
      ALSound        *sound;
      s16             cents;
      f32             pitch,oscValue;
      u8              fxmix;
      u8 unk_0x12;
      u8 unk_0x13;
      void           *oscState;
      ALInstrument   *inst;
        
      /* If we're not playing, don't process note ons. */
      if (seqp->state != AL_PLAYING) break;

      if (!(D_8003D31C_3DF1C & 1<<chan)) break;
      
      sound = __n_lookupSoundQuick((N_ALSeqPlayer*)seqp, key, vel, chan);
      ALFlagFailIf(!sound, seqp->debugFlags & NO_SOUND_ERR_MASK,
		   ERR_ALSEQP_NO_SOUND); 
      
      config.priority = seqp->chanState[chan].priority;
      config.fxBus    = 0;
      config.unityPitch = 0;
      
      vstate = __n_mapVoice((N_ALSeqPlayer*)seqp, key, vel, chan);
      ALFlagFailIf(!vstate, seqp->debugFlags & NO_VOICE_ERR_MASK,
		   ERR_ALSEQP_NO_VOICE );
      
      voice = &vstate->voice;
      
      n_alSynAllocVoice( voice, &config);
      
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
      if(inst->tremType) {
	if(seqp->initOsc) {
	  deltaTime = (*seqp->initOsc)(&oscState,&oscValue,inst->tremType,
				       inst->tremRate,inst->tremDepth,inst->tremDelay);
	  
	  if(deltaTime) /* a deltaTime of zero means don't run osc */ {
	    evt.type = AL_TREM_OSC_EVT;
	    evt.msg.osc.vs = vstate;
	    evt.msg.osc.oscState = oscState;
	    n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime);
	    vstate->flags |= 0x01; /* set tremelo flag bit */
	  }
	}
      }
      vstate->tremelo = (u8)oscValue;
      /* will default if not changed by initOsc */
      
      oscValue = 1; /* set this as a default */
      if(inst->vibType) {
	if(seqp->initOsc) {
	  deltaTime = (*seqp->initOsc)(&oscState,&oscValue,inst->vibType,
				       inst->vibRate,inst->vibDepth,inst->vibDelay);
	  
	  if(deltaTime)  /* a deltaTime of zero means don't run osc. */ {
	    evt.type = AL_VIB_OSC_EVT;
	    evt.msg.osc.vs = vstate;
	    evt.msg.osc.oscState = oscState;
	    evt.msg.osc.chan = chan;
	    n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime);
	    vstate->flags |= 0x02; /* set the vibrato flag bit */
	  }
	}
      }
      vstate->vibrato = oscValue; /* will default if not changed by initOsc */
      
      /*
       * calculate the note on parameters
       */
      pitch = vstate->pitch * seqp->chanState[chan].pitchBend *
	vstate->vibrato;
      fxmix = seqp->chanState[chan].fxmix;
      unk_0x12 = seqp->chanState[chan].unk_0x12; 
      unk_0x13 = seqp->chanState[chan].unk_0x13; 
      pan   = __n_vsPan(vstate, (N_ALSeqPlayer*)seqp);
      vol   = __n_vsVol(vstate, (N_ALSeqPlayer*)seqp);
      deltaTime  = sound->envelope->attackTime;
      
      n_alSynStartVoiceParams_Alt( voice, sound->wavetable,
			      pitch, vol, pan, fxmix, deltaTime, unk_0x12, unk_0x13);
      /*
       * set up callbacks for envelope
       */
      evt.type          = AL_SEQP_ENV_EVT;
      evt.msg.vol.voice = voice;
      evt.msg.vol.vol   = sound->envelope->decayVolume;
      evt.msg.vol.delta = sound->envelope->decayTime;
      
      n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime);
      
      if(midi->duration) {
	/*
	 * set up note off evt. if no duration don't do this
	 */
	evt.type            = AL_CSP_NOTEOFF_EVT;
	evt.msg.midi.status = chan | AL_MIDI_NoteOff;
	evt.msg.midi.byte1  = key;
	evt.msg.midi.byte2  = 0;   /* not needed ? */
	deltaTime = seqp->uspt * midi->duration;
	
	/* max time would be about one hour ten minutes */
	n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime);
      }
      
      break;
    }
    
    /*
     * NOTE: intentional fall-through for note on with zero
     * velocity (Should never happen with compact midi sequence,
     * but could happen with real time midi.)
     */
    
  case (AL_MIDI_NoteOff):
    vstate = __n_lookupVoice((N_ALSeqPlayer*)seqp, key, chan);
    ALFlagFailIf(!vstate, seqp->debugFlags & NOTE_OFF_ERR_MASK,
		 ERR_ALSEQP_OFF_VOICE );
    
    if (vstate->phase == AL_PHASE_SUSTAIN)
      vstate->phase = AL_PHASE_SUSTREL;
    else {
      vstate->phase = AL_PHASE_RELEASE;
      __n_seqpReleaseVoice((N_ALSeqPlayer*)seqp, &vstate->voice,
			   vstate->sound->envelope->releaseTime);
    }
    
    break;
    
  case (AL_MIDI_PolyKeyPressure):
    /*
     * Aftertouch per key (hardwired to volume). Note that
     * aftertouch affects only notes that are already
     * sounding.
     */
    vstate = __n_lookupVoice((N_ALSeqPlayer*)seqp, key, chan);
    ALFailIf(!vstate,  ERR_ALSEQP_POLY_VOICE );
    
    vstate->velocity = byte2;
    n_alSynSetVol( &vstate->voice,
		  __n_vsVol(vstate, (N_ALSeqPlayer*)seqp), 
		  __n_vsDelta(vstate,seqp->curTime));
    break;
    
  case (AL_MIDI_ChannelPressure):
	seqp->chanState[chan].unk_0x10 = key;
    /*
     * Aftertouch per channel (hardwired to volume). Note that
     * aftertouch affects only notes that are already
     * sounding.
     */
 //    for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
 //      if (vs->channel == chan) {
	// vs->velocity = byte1;
	// n_alSynSetVol( &vs->voice,
	// 	      __n_vsVol(vs, (N_ALSeqPlayer*)seqp),
	// 	      __n_vsDelta(vs,seqp->curTime));
 //      }
 //    }
    break;
    
  case (AL_MIDI_ControlChange):
    switch (byte1)
      {
      case (AL_MIDI_PAN_CTRL):
	seqp->chanState[chan].pan = byte2;
	for (vs = seqp->vAllocHead; vs != 0; vs = vs->next)
	  {
	    if (vs->channel == chan)
	      {
		pan = __n_vsPan(vs, (N_ALSeqPlayer*)seqp);
		n_alSynSetPan( &vs->voice, pan);
	      }
	  }
	break;
      case (AL_MIDI_VOLUME_CTRL):
	seqp->chanState[chan].vol = byte2;
	for (vs = seqp->vAllocHead; vs != 0; vs = vs->next)
	  {
	    if ((vs->channel == chan) && (vs->envPhase != AL_PHASE_RELEASE))
	      {
		vol = __n_vsVol(vs, (N_ALSeqPlayer*)seqp);
		n_alSynSetVol(&vs->voice, vol,
			      __n_vsDelta(vs,seqp->curTime));
	      }
	  }
	break;
      case (AL_MIDI_PRIORITY_CTRL):
	/* leave current voices where they are */
	seqp->chanState[chan].priority = byte2;
	break;
      case (AL_MIDI_SUSTAIN_CTRL):
	seqp->chanState[chan].sustain = byte2;
	for (vs = seqp->vAllocHead; vs != 0; vs = vs->next)
	  {
	    if ((vs->channel == chan) && (vs->phase != AL_PHASE_RELEASE))
	      {
		if ( byte2 > AL_SUSTAIN )
		  {
		    /*
		     * sustain pedal down
		     */
		    if (vs->phase == AL_PHASE_NOTEON)
		      vs->phase = AL_PHASE_SUSTAIN;
		  }
		else
		  {
		    /*
		     * sustain pedal up
		     */
		    if (vs->phase == AL_PHASE_SUSTAIN)
		      vs->phase = AL_PHASE_NOTEON;
		    
		    else if(vs->phase == AL_PHASE_SUSTREL)
		      {
			vs->phase = AL_PHASE_RELEASE;
			__n_seqpReleaseVoice((N_ALSeqPlayer*)seqp,
					     &vs->voice,
					     vs->sound->envelope->releaseTime);
		      }
		  }
	      }
	  }
	break;
      case (AL_MIDI_FX1_CTRL):
	seqp->chanState[chan].fxmix = byte2;
	for (vs = seqp->vAllocHead; vs != 0; vs = vs->next)
	  {
	    if (vs->channel == chan)
	      n_alSynSetFXMix( &vs->voice, byte2);
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
              seqp->chanState[chan].unk_0x12 = byte2;
              for (vs = seqp->vAllocHead; vs != 0; vs = vs->next)
        	  {
        	      if (vs->channel == chan)
                      n_alSynSetFXMix_Alt(&vs->voice, byte2, seqp->chanState[chan].unk_0x13);
              }
              break;
	    case (AL_MIDI_FX_CTRL_3):
              seqp->chanState[chan].unk_0x13 = byte2;
              for (vs = seqp->vAllocHead; vs != 0; vs = vs->next)
        	  {
        	      if (vs->channel == chan)
                      n_alSynSetFXMix_Alt(&vs->voice, seqp->chanState[chan].unk_0x12, byte2);
              }
              break;
	    case (AL_MIDI_FX_CTRL_4):
            if (byte2 > 2) break;
            
            if ((ALBank*)*(&seqp->bank + byte2) != NULL)
              seqp->chanState[chan].unk_0x14 = byte2;
              break;
	    case (AL_MIDI_FX_CTRL_5):
              seqp->chanState[chan].priority = byte2;
              break;
	    // case (AL_MIDI_FX_CTRL_6):
	    // case (AL_MIDI_FX_CTRL_7):
#if 0	/* fx control not implemented */      
		fxref = n_alSynGetFXRef(0, 0);
		if (fxref)
		    n_alSynSetFXParam(fxref, (s16)byte1, (void *)byte2);
		break;
#endif
	    // case (AL_MIDI_FX3_CTRL):
      default:
	break;
      }
    break;
  case (AL_MIDI_ProgramChange):
    /* sct 1/16/96 - We must have a valid bank in order to process the program change. */

#ifdef _DEBUG
    assert(seqp->bank != NULL);
#endif
    
    if (key < ((ALBank*)*(&seqp->bank + seqp->chanState[chan].unk_0x14))->instCount)
      {
	ALInstrument *inst = ((ALBank*)*(&seqp->bank + seqp->chanState[chan].unk_0x14))->instArray[key];
	__n_setInstChanState_Alt((N_ALSeqPlayer*)seqp, inst, chan);	/* sct 11/6/95 */               
      }
    else
      {
#ifdef _DEBUG
	__osError(ERR_ALSEQPINVALIDPROG, 2, key, seqp->bank->instCount);
#endif                
      }
    break;            
  case (AL_MIDI_PitchBendChange):
    {
      s32 bendVal;
      f32 bendRatio;
      s32 cents;
      
      /* get 14-bit unsigned midi value */
      bendVal = ( (byte2 << 7) + byte1) - 8192;
      
      /* calculate pitch bend in cents */
      cents = (seqp->chanState[chan].bendRange * bendVal)/8192;
      
      /* calculate the corresponding ratio  */
      bendRatio = alCents2Ratio(cents);
      seqp->chanState[chan].pitchBend = bendRatio;
      
      for (vs = seqp->vAllocHead; vs != NULL; vs = vs->next)
      {
          if (vs->channel == chan) {
            n_alSynSetPitch(&vs->voice, (((seqp->chanState[chan].unk_0x10 * (vs->vibrato - 1.0F)) / 127) + 1.0F) * (vs->pitch * bendRatio));
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

static void __n_CSPHandleMetaMsg(N_ALCSPlayer *seqp, N_ALEvent *event)
{
  ALTempoEvent    *tevt = &event->msg.tempo;
  N_ALEvent       evt;
  s32             tempo;
  s32             oldUspt;
  u32             ticks;
  ALMicroTime         tempDelta,curDelta = 0;
  N_ALEventListItem     *thisNode,*nextNode,*firstTemp = 0;
  
  if (event->msg.tempo.status == AL_MIDI_Meta) {
    if (event->msg.tempo.type == AL_MIDI_META_TEMPO) {
      oldUspt = seqp->uspt;
      tempo = (tevt->byte1 << 16) | (tevt->byte2 <<  8) | (tevt->byte3 <<  0);
      __n_setUsptFromTempo (seqp, (f32)tempo);	/* sct 1/8/96 */
      
      thisNode = (N_ALEventListItem*)seqp->evtq.allocList.next;
      while(thisNode) {
	curDelta += thisNode->delta;
	nextNode = (N_ALEventListItem*)thisNode->node.next;
	if(thisNode->evt.type == AL_CSP_NOTEOFF_EVT) {
	  alUnlink((ALLink*)thisNode);
	  
	  if(firstTemp)
	    alLink((ALLink*)thisNode,(ALLink*)firstTemp);
	  else {
	    thisNode->node.next = 0;
	    thisNode->node.prev = 0;
	    firstTemp = thisNode;
	  }
	  tempDelta = curDelta;         /* record the current delta */
	  if(nextNode) /* don't do this if no nextNode */ {
	    curDelta -= thisNode->delta;  /* subtract out this delta */
	    nextNode->delta += thisNode->delta; /* add it to next event */
	  }
	  thisNode->delta = tempDelta; /* set this event delta from current */
	}
	thisNode = nextNode;
      }
      
      thisNode = firstTemp;
      while(thisNode) {
	nextNode = (N_ALEventListItem*)thisNode->node.next;
	ticks = thisNode->delta/oldUspt;
	thisNode->delta = ticks * seqp->uspt;
	__n_CSPRepostEvent(&seqp->evtq,thisNode);
	thisNode = nextNode;
      }
    }
  }
}

static void  __n_CSPRepostEvent(ALEventQueue *evtq, N_ALEventListItem *item)
{
  OSIntMask           mask;
  ALLink             *node;
  N_ALEventListItem  *nextItem;
  
  mask = osSetIntMask(OS_IM_NONE);
  
  for (node = &evtq->allocList; node != 0; node = node->next) {
    if (!node->next)  /* end of the list */ {
      alLink((ALLink *)item, node);
      break;
    }
    else {
      nextItem = (N_ALEventListItem *)node->next;
      if (item->delta < nextItem->delta) {
	nextItem->delta -= item->delta;
	alLink((ALLink *)item, node);
	break;
      }
      item->delta -= nextItem->delta;
    }
  }
  osSetIntMask(mask);
}

/*
  This routine safely calculates the sequence player's
  uspt value based on the given tempo.  It does this safely
  by making sure that the player has a target sequence and
  therefore a qnpt value which is needed for the calculation.
  
  Compact sequence player needs its own version of this routine
  since the ALCSeq's qnpt field is at a different offset.
  */
static void __n_setUsptFromTempo (N_ALCSPlayer *seqp, f32 tempo)
{
  if (seqp->target)
    seqp->uspt = (s32)((f32)tempo * seqp->target->qnpt);
  else
    seqp->uspt = 488;		/* This is the initial value set by n_alSeqpNew. */
}


/*
  Calculates the delta time in ticks until the next sequence
  event and posts a sequence reference event with the time in usecs.
  Loops are handled automatically by the compact sequence.
  
  Does nothing if the sequence player is not playing or if there
  is no target sequence.
  
  sct 11/7/95
  */
void __n_CSPPostNextSeqEvent(N_ALCSPlayer *seqp) 
{
  N_ALEvent   evt;
  s32		deltaTicks;
  
  if (seqp->state != AL_PLAYING || seqp->target == NULL)
    return;
  
  /* Get the next event time in ticks. */
  /* If false is returned, then there is no next delta (ie. end of sequence reached). */
  if (!__alCSeqNextDelta(seqp->target, &deltaTicks))
    return;
  
  evt.type = AL_SEQ_REF_EVT;
  n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTicks * seqp->uspt);                
}

/////////////////////////////////////////////////////////

void func_800290AC_29CAC(f32 arg0)
{
	if (arg0 <= 0.0f)
		arg0 = 1.0f;
	else if (arg0 > 10.0f)
		arg0 = 1.0f;
	D_8003D318_3DF18 = arg0;
}

void func_8002909C_29C9C(u16 arg0) {
	D_8003D31C_3DF1C = arg0;
}

//////////////////////////////////////////////////////
// n_event.c

void n_alEvtqNew(ALEventQueue *evtq, N_ALEventListItem *items, s32 itemCount)
{
    s32 i;
    
    evtq->eventCount     = 0;
    evtq->allocList.next = 0;
    evtq->allocList.prev = 0;
    evtq->freeList.next  = 0;
    evtq->freeList.prev  = 0;

    for (i = 0; i < itemCount; i++) {
        alLink((ALLink *)&items[i], &evtq->freeList);
    }
}

ALMicroTime n_alEvtqNextEvent(ALEventQueue *evtq, N_ALEvent *evt)
{
	ALEventListItem *item;
	ALMicroTime delta;
	OSIntMask mask;

	mask = osSetIntMask(OS_IM_NONE);

	item = (ALEventListItem *)evtq->allocList.next;

	if (item)
	{
		alUnlink((ALLink *)item);
		alCopy(&item->evt, evt, sizeof(*evt));
		alLinkMacro((ALLink *)item, &evtq->freeList);
	delta = item->delta;
	}
	else
	{
		/* sct 11/28/95 - If we get here, most like we overflowed the event queue */
	/* with non-self-perpetuating events.  Eg. if we filled the evtq with volume */
	/* events, then when the seqp is told to play it will handle all the events */
	/* at once completely emptying out the queue.  At this point this problem */
	/* must be treated as an out of resource error and the evtq should be increased. */
	evt->type = -1;
	delta = 0;
	}

	osSetIntMask(mask);

	return delta;
}

void n_alEvtqPostEvent(ALEventQueue *evtq, N_ALEvent *evt, ALMicroTime delta)
{
    N_ALEventListItem  *item;
    N_ALEventListItem  *nextItem;
    ALLink             *node;
    s32                 postAtEnd=0;
    OSIntMask           mask;

    mask = osSetIntMask(OS_IM_NONE);

    item = (N_ALEventListItem *)evtq->freeList.next;
    if (!item) {
        osSetIntMask(mask);
#ifdef _DEBUG
        __osError(ERR_ALEVENTNOFREE, 0);
#endif        
        return;
    }
    
    alUnlink((ALLink *)item);
    alCopy(evt, &item->evt, sizeof(*evt));

    if (delta == AL_EVTQ_END)
        postAtEnd = -1;
    
    for (node = &evtq->allocList; node != 0; node = node->next) {
        if (!node->next) { /* end of the list */
            if (postAtEnd)
                item->delta = 0;
            else
                item->delta = delta;
            alLink((ALLink *)item, node);
            break;
        } else {
            nextItem = (N_ALEventListItem *)node->next;

            if (delta < nextItem->delta) {
                item->delta = delta;
                nextItem->delta -= delta;
                    
                alLink((ALLink *)item, node);
                break;
            }
                
            delta -= nextItem->delta;

        }
    }

    osSetIntMask(mask);
    
}

//0x298B4
/*
	This routine flushes events according their type.
*/
void n_alEvtqFlushType(ALEventQueue *evtq, s16 type)
{
	ALLink      	*thisNode;
	ALLink      	*nextNode;
	ALEventListItem     *thisItem, *nextItem;
	OSIntMask   	mask;

	mask = osSetIntMask(OS_IM_NONE);

	thisNode = evtq->allocList.next;
	while( thisNode != 0 )
	{
	nextNode = thisNode->next;
	thisItem = (ALEventListItem *)thisNode;
	nextItem = (ALEventListItem *)nextNode;
	if (thisItem->evt.type == type)
	{
		if (nextItem)
		nextItem->delta += thisItem->delta;
		alUnlink(thisNode);
		alLinkMacro(thisNode, &evtq->freeList);
	}
	thisNode = nextNode;
	}

	osSetIntMask(mask);
}

//////////////////////////////////////////////////////


void alUnlink(ALLink *ln)			
{					
    if (ln->next)                   
        ln->next->prev = ln->prev;  
    if (ln->prev)                   
        ln->prev->next = ln->next;  
}

/* might want to make these macros */
void alLink(ALLink *ln, ALLink *to)
{					
    ln->next = to->next;     
    ln->prev = to;           
    if (to->next)            
        to->next->prev = ln; 
    to->next = ln;           
}

//////////////////////////////////////////////////////////////
// n_csq.c

static u32 __readVarLen(ALCSeq *s,u32 track);
static u8  __getTrackByte(ALCSeq *s,u32 track);
static u32 __n_alCSeqGetTrackEvent(ALCSeq *seq, u32 track, N_ALEvent *event);

void n_alCSeqNew(ALCSeq *seq, u8 *ptr)
{
	u32         i,tmpOff,flagTmp;

	/* load the seqence pointed to by ptr   */
	seq->base = (ALCMidiHdr*)ptr;
	seq->validTracks = 0;
	seq->lastDeltaTicks = 0;
	seq->lastTicks = 0;
	seq->deltaFlag = 1;

	for(i = 0; i < 16; i++)
	{
		seq->lastStatus[i] = 0;
		seq->curBUPtr[i] = 0;
		seq->curBULen[i] = 0;
		tmpOff = seq->base->trackOffset[i];
		if(tmpOff) /* if the track is valid */
		{
			flagTmp = 1 << i;
			seq->validTracks |= flagTmp;
			seq->curLoc[i] = (u8*)((u32)ptr + tmpOff);
			seq->evtDeltaTicks[i] = __readVarLen(seq,i);
			/*__n_alCSeqGetTrackEvent(seq,i); prime the event buffers  */
		}
		else
			seq->curLoc[i] = 0;
	}

	seq->qnpt = 1.0f/(f32)seq->base->division;
}

void n_alCSeqNextEvent(ALCSeq *seq,N_ALEvent *evt)
{
	u32     i;
	u32     firstTime = 0xFFFFFFFF;
	u32     firstTrack;
	u32     lastTicks = seq->lastDeltaTicks;

#ifdef _DEBUG
	/* sct 1/17/96 - Warn if we are beyond the end of sequence. */
	if (!seq->validTracks)
	__osError(ERR_ALSEQOVERRUN, 0);
#endif


	for(i = 0; i < 16 ; i++)
	{
	if((seq->validTracks >> i) & 1)
		{
		if(seq->deltaFlag)
		seq->evtDeltaTicks[i] -= lastTicks;
		if(seq->evtDeltaTicks[i] < firstTime)
			{
		firstTime = seq->evtDeltaTicks[i];
		firstTrack = i;
			}
		}
	}

	__n_alCSeqGetTrackEvent(seq,firstTrack,evt);

	evt->msg.midi.ticks = firstTime;
	seq->lastTicks += firstTime;
	seq->lastDeltaTicks = firstTime;
	if(evt->type != AL_TRACK_END)
	seq->evtDeltaTicks[firstTrack] += __readVarLen(seq,firstTrack);
	seq->deltaFlag = 1;

}

char __alCSeqNextDelta(ALCSeq *seq, s32 *pDeltaTicks)
{
	u32     i;
	u32	    firstTime = 0xFFFFFFFF;
	u32     lastTicks = seq->lastDeltaTicks;

	if (!seq->validTracks)
	return FALSE;

	for(i = 0; i < 16 ; i++)
	{
	if((seq->validTracks >> i) & 1)
		{
		if(seq->deltaFlag)
		seq->evtDeltaTicks[i] -= lastTicks;

		if(seq->evtDeltaTicks[i] < firstTime)
		firstTime = seq->evtDeltaTicks[i];
		}
	}

	seq->deltaFlag = 0;
	*pDeltaTicks = firstTime;

	return TRUE;
}


/* only call n_alCSeqGetTrackEvent with a valid track !! */
static u32 __n_alCSeqGetTrackEvent(ALCSeq *seq, u32 track, N_ALEvent *event)
{
	u32     offset;
	u8      status, loopCt, curLpCt, *tmpPtr;


	status = __getTrackByte(seq,track);     /* read the status byte */

	if (status == AL_MIDI_Meta) /* running status not allowed on meta events!! */
	{
		u8 type = __getTrackByte(seq,track);

		if (type == AL_MIDI_META_TEMPO)
		{
			event->type = AL_TEMPO_EVT;
			event->msg.tempo.status = status;
			event->msg.tempo.type = type;
			event->msg.tempo.byte1 = __getTrackByte(seq,track);
			event->msg.tempo.byte2 = __getTrackByte(seq,track);
			event->msg.tempo.byte3 = __getTrackByte(seq,track);
			seq->lastStatus[track] = 0;  /* lastStatus not supported after meta */
		}
		else if (type == AL_MIDI_META_EOT)
		{
			u32     flagMask;

			flagMask = 0x01 << track;
			seq->validTracks = seq->validTracks ^ flagMask;

			if(seq->validTracks) /* there is music left don't end */
				event->type = AL_TRACK_END;
			else         /* no more music send AL_SEQ_END_EVT msg */
				event->type = AL_SEQ_END_EVT;
		}
		else if (type == AL_CMIDI_LOOPSTART_CODE)
		{
			status = __getTrackByte(seq,track); /* get next two bytes, ignore them */
			status = __getTrackByte(seq,track);
			seq->lastStatus[track] = 0;
			event->type = AL_CSP_LOOPSTART;
		}
		else if (type == AL_CMIDI_LOOPEND_CODE)
		{
			tmpPtr = seq->curLoc[track];
			loopCt = *tmpPtr++;
			curLpCt = *tmpPtr;
			if(curLpCt == 0) /* done looping */
			{
				*tmpPtr = loopCt; /* reset current loop count */
				seq->curLoc[track] = tmpPtr + 5; /* move pointer to end of event */
			}
			else
			{
				if(curLpCt != 0xFF) /* not a loop forever */
					*tmpPtr = curLpCt - 1;   /* decrement current loop count */
				tmpPtr++;                    /* get offset from end of event */
				offset = (*tmpPtr++) << 24;
				offset += (*tmpPtr++) << 16;
				offset += (*tmpPtr++) << 8;
				offset += *tmpPtr++;
				seq->curLoc[track] = tmpPtr - offset;
			}
			seq->lastStatus[track] = 0;
			event->type = AL_CSP_LOOPEND;
		}

#ifdef _DEBUG
		else
			__osError(ERR_ALSEQMETA, 1, type);
#endif

	}
	else
	{
		event->type = AL_SEQ_MIDI_EVT;
		if (status & 0x80)  /* if high bit is set, then new status */
		{
			event->msg.midi.status = status;
			event->msg.midi.byte1 = __getTrackByte(seq,track);
			seq->lastStatus[track] = status;
		}
		else     /* running status */
		{
#ifdef _DEBUG
			if(seq->lastStatus[track] == 0)
				__osError(ERR_ALCSEQZEROSTATUS, 1, track);
#endif
			event->msg.midi.status = seq->lastStatus[track];
			event->msg.midi.byte1 = status;
		}

		if (((event->msg.midi.status & 0xf0) != AL_MIDI_ProgramChange) &&
			((event->msg.midi.status & 0xf0) != AL_MIDI_ChannelPressure))
		{
			event->msg.midi.byte2 = __getTrackByte(seq,track);
			if((event->msg.midi.status & 0xf0) == AL_MIDI_NoteOn)
			{
				event->msg.midi.duration = __readVarLen(seq,track);
#ifdef _DEBUG
				if(event->msg.midi.byte2 == 0)
					__osError( ERR_ALCSEQZEROVEL, 1, track);
#endif
			}
		}
		else
			event->msg.midi.byte2 = 0;
	}
	return TRUE;
}


void n_alCSeqNewMarker(ALCSeq *seq, ALCSeqMarker *m, u32 ticks)
{
	N_ALEvent     evt;
	ALCSeq      tempSeq;
	s32         i;


	n_alCSeqNew(&tempSeq, (u8*)seq->base);

	do {
		m->validTracks    = tempSeq.validTracks;
		m->lastTicks      = tempSeq.lastTicks;
		m->lastDeltaTicks = tempSeq.lastDeltaTicks;

		for(i=0;i<16;i++)
		{
			m->curLoc[i]        = tempSeq.curLoc[i];
			m->curBUPtr[i]      = tempSeq.curBUPtr[i];
			m->curBULen[i]      = tempSeq.curBULen[i];
			m->lastStatus[i]    = tempSeq.lastStatus[i];
			m->evtDeltaTicks[i] = tempSeq.evtDeltaTicks[i];
		}

		n_alCSeqNextEvent(&tempSeq, &evt);

		if (evt.type == AL_SEQ_END_EVT)
			break;

	} while (tempSeq.lastTicks < ticks);

}


/* non-aligned byte reading routines */
static u8 __getTrackByte(ALCSeq *seq,u32 track)
{
	u8      theByte;


	if(seq->curBULen[track])
	{
		theByte = *seq->curBUPtr[track];
		seq->curBUPtr[track]++;
		seq->curBULen[track]--;
	}
	else  /* need to handle backup mode */
	{
		theByte = *seq->curLoc[track];
		seq->curLoc[track]++;
		if(theByte == AL_CMIDI_BLOCK_CODE)
		{
			u8   loBackUp,hiBackUp,theLen,nextByte;
			u32  backup;

			nextByte = *seq->curLoc[track];
			seq->curLoc[track]++;
			if(nextByte != AL_CMIDI_BLOCK_CODE)
			{
				/* if here, then got a backup section. get the amount of
					 backup, and the len of the section. Subtract the amount of
					 backup from the curLoc ptr, and subtract four more, since
					 curLoc has been advanced by four while reading the codes. */
				hiBackUp = nextByte;
				loBackUp = *seq->curLoc[track];
				seq->curLoc[track]++;
				theLen = *seq->curLoc[track];
				seq->curLoc[track]++;
				backup = (u32)hiBackUp;
				backup = backup << 8;
				backup += loBackUp;
				seq->curBUPtr[track] = seq->curLoc[track] - (backup + 4);
				seq->curBULen[track] = (u32)theLen;

				/* now get the byte */
				theByte = *seq->curBUPtr[track];
				seq->curBUPtr[track]++;
				seq->curBULen[track]--;
			}
		}
	}

	return theByte;
}

static u32 __readVarLen(ALCSeq *seq,u32 track)
{
	u32 value;
	u32 c;

	value = (u32)__getTrackByte(seq,track);
	if ( value & 0x00000080 )
	{
		value &= 0x7f;
		do
		{
			c = (u32)__getTrackByte(seq,track);
			value = (value << 7) + (c & 0x7f);
		} while (c & 0x80);
	}
	return (value);
}

//////////////////////////////////////////////////////////////

extern u16 gSYSinTable[0x800];

void func_80027460_28060(ALWhatever8009EE0C_2 *arg0)
{
    ALWhatever8009EDD0_siz24 *var_s0_6, *var_v1_2;
    ALWhatever8009EDD0_siz24 *var_a1;
    f32 temp_f0, temp_f0_2;
    f32 temp1, temp2;
    s32 temp3;
    f32 spC0[6];
    s32 temp4, temp5;
    f32 var_f12;
    s32 temp_a1;
    s32 temp_a0;
    s32 temp_v0;
    u16 sintable_id;
    u8 *ucode;
    f32 sp9C;
    ALVoiceConfig voice_config;
    ALWhatever8009EDD0_siz24 *var_v1;
    f32 angle;
    u16 timer;
    s16 param;
    u16 param2;
    s16 param3;
    u16 id;
    s32 temp_a3;
    u16 instr;
    
    if (arg0->unk28 != 0)
    {
        arg0->unk28--;
        
        if (arg0->unk28 == 0)
        {
            ucode = arg0->unk20;
            
            do
            {
                instr = *ucode++;
                timer = instr & 0xF;
                
                if (timer & 0x8)
                {
                    param2 = *ucode++;
                    timer &= 0x7;
                    timer = (timer << 7) + (param2 & 0x7F);

                    if (param2 & 0x80)
                    {
                        timer = (timer << 8) + *ucode++;
                    }
                }
                switch (instr & 0xF0)
                {
                case 0x0:
                    param2 = *ucode++;
                    
                    if (param2 <= 127)
                    {
                        arg0->unk32 = param2;
                    }
                    else
                    {
                        // Might need separate var (var_s0)
                        param = param2;
                        param -= 192;
                        param += arg0->unk32;
                        
                        if (param < 0)
                        {
                            param = 0;
                        }
                        if (param > 127)
                        {
                            param = 127;
                        }
                        arg0->unk32 = param;
                    }
                    break;
                    
                case 0x10:
                    param2 = *ucode++;
                    
                    if (param2 <= 127)
                    {
                        arg0->unk34 = param2;
                    }
                    else
                    {
                        // Might need separate var (var_s0)
                        param = param2;
                        param -= 192;
                        param += arg0->unk34;
                        
                        if (param < 0)
                        {
                            param = 0;
                        }
                        if (param > 127)
                        {
                            param = 127;
                        }
                        arg0->unk34 = param;
                    }
                    break;
                    
                case 0x20:
                    param2 = *ucode++ << 8;
                    param2 |= *ucode++;
                    param = param2;
                    if (param <= 0x4B0)
                    {
                        if (param < -0x4B0)
                        {
                            param = -0x4B0;
                        }
                        arg0->unk2C = param;
                    }
                    else
                    {
                        param -= 0x960;
                        param += arg0->unk2C;
                        
                        if (param < -0x4B0)
                        {
                            param = -0x4B0;
                        }
                        if (param > 0x4B0)
                        {
                            param = 0x4B0;
                        }
                        arg0->unk2C = param;
                    }
                    break;
                    
                case 0x30:
                    param2 = *ucode++;
                    
                    if (param2 <= 127)
                    {
                        arg0->unk36 = param2;
                    }
                    else
                    {
                        // Might need separate var (var_s0)
                        param = param2;
                        param -= 192;
                        param += arg0->unk36;
                        
                        if (param < 0)
                        {
                            param = 0;
                        }
                        if (param > 127)
                        {
                            param = 127;
                        }
                        arg0->unk36 = param;
                    }
                    break;
                    
                case 0x40:
                    id = *ucode++;
                    param2 = *ucode++;

                    if (param2 & 0x80)
                    {
                        param2 = *ucode++ + ((param2 & 0x7F) << 8);
                    }
                    if (param2 < D_8009EDD0_406D0.unk_alsound_0x2C)
                    {
                        temp_a3 = D_8009EDD0_406D0.unk_alsound_0x24[param2].unk0;
                        temp3 = D_8009EDD0_406D0.unk_alsound_0x24[param2].unk1;
                        temp4 = D_8009EDD0_406D0.unk_alsound_0x24[param2].unk2;
                        sp9C = D_8009EDD0_406D0.unk_alsound_0x24[param2].unk3;
                        temp_f0 = D_8009EDD0_406D0.unk_alsound_0x24[param2].unk4;
                        temp1 = D_8009EDD0_406D0.unk_alsound_0x24[param2].unk8;
                        temp2 = D_8009EDD0_406D0.unk_alsound_0x24[param2].unkC;
                        
                        var_v1 = arg0->unk44;
                        var_a1 = NULL;

                        while (TRUE)
                        {
                            if (var_v1 == NULL)
                            {
                                var_s0_6 = D_8009EDD0_406D0.unk_alsound_0x30;
                            
                                if (var_s0_6 != NULL)
                                {
                                    D_8009EDD0_406D0.unk_alsound_0x30 = var_s0_6->next;
                                    var_s0_6->next = NULL;
                                
                                    if (var_a1 != NULL)
                                    {
                                        var_a1->next = var_s0_6;
                                    }
                                    else arg0->unk44 = var_s0_6;
                                }
                                break;
                            }

                            temp_a0 = var_v1->unkALWhatever8009EDD0_siz24_0x4;
                            if (id == temp_a0)
                            {
                                var_s0_6 = var_v1;
                                break;
                            }

                            if (id < temp_a0)
                            {
                                var_s0_6 = D_8009EDD0_406D0.unk_alsound_0x30;
                                
                                if (var_s0_6 != NULL)
                                {
                                    D_8009EDD0_406D0.unk_alsound_0x30 = var_s0_6->next;
                                    
                                    if (var_a1 != NULL)
                                    {
                                        var_s0_6->next = var_a1->next;
                                        var_a1->next = var_s0_6;
                                    }
                                    else
                                    {
                                        var_s0_6->next = arg0->unk44;
                                        arg0->unk44 = var_s0_6;
                                    }    
                                }
                                break;
                            }

                            var_a1 = var_v1;
                            var_v1 = var_v1->next;
                        }
                        if (var_s0_6 == NULL)
                        {
                            break;
                        }
                        var_s0_6->unkALWhatever8009EDD0_siz24_0x14 = temp_f0 * sp9C * (1.0F / 256.0F);
                        var_s0_6->unkALWhatever8009EDD0_siz24_0x4 = id;
                        var_s0_6->unkALWhatever8009EDD0_siz24_0x5 = temp_a3;
                        var_s0_6->unkALWhatever8009EDD0_siz24_0x6 = temp3;
                        var_s0_6->unkALWhatever8009EDD0_siz24_0x7 = temp4;
                        var_s0_6->unkALWhatever8009EDD0_siz24_0x8 = temp_f0;
                        var_s0_6->unkALWhatever8009EDD0_siz24_0xC = temp1;
                        var_s0_6->unkALWhatever8009EDD0_siz24_0x10 = temp2;
                        
                        switch (temp_a3)
                        {
                        case 4:
                            var_s0_6->unkALWhatever8009EDD0_siz24_0x20 = (randFloat1() * var_s0_6->unkALWhatever8009EDD0_siz24_0xC) + var_s0_6->unkALWhatever8009EDD0_siz24_0x10;
                            var_s0_6->unkALWhatever8009EDD0_siz24_0x18 = randFloat1() * var_s0_6->unkALWhatever8009EDD0_siz24_0x8;
                            var_s0_6->unkALWhatever8009EDD0_siz24_0x14 = var_s0_6->unkALWhatever8009EDD0_siz24_0x18 * sp9C * (1.0F / 256.0F);
                            break;
                            
                        case 5:
                            var_s0_6->unkALWhatever8009EDD0_siz24_0x1C = 0.0F;
                            var_s0_6->unkALWhatever8009EDD0_siz24_0x20 = (randFloat1() * var_s0_6->unkALWhatever8009EDD0_siz24_0xC) + var_s0_6->unkALWhatever8009EDD0_siz24_0x10;
                            var_s0_6->unkALWhatever8009EDD0_siz24_0x18 = (randFloat1() * var_s0_6->unkALWhatever8009EDD0_siz24_0x8) + 0.5F;
                            var_s0_6->unkALWhatever8009EDD0_siz24_0x14 = var_s0_6->unkALWhatever8009EDD0_siz24_0x18 * sp9C * (1.0F / 256.0F);
                            break;
                            
                        case 8:
                            var_s0_6->unkALWhatever8009EDD0_siz24_0x20 = (randFloat2() * var_s0_6->unkALWhatever8009EDD0_siz24_0xC) + var_s0_6->unkALWhatever8009EDD0_siz24_0x10;
                            var_s0_6->unkALWhatever8009EDD0_siz24_0x18 = randFloat2() * var_s0_6->unkALWhatever8009EDD0_siz24_0x8;
                            var_s0_6->unkALWhatever8009EDD0_siz24_0x14 = var_s0_6->unkALWhatever8009EDD0_siz24_0x18 * sp9C * (1.0F / 256.0F);
                            break;
                        }
                    }
                    break;
                    
                case 0x50:
                    id = *ucode++;
                    var_v1 = arg0->unk44;
                    var_a1 = NULL;
                    
                    while (var_v1 != NULL)
                    {
                        if (id == var_v1->unkALWhatever8009EDD0_siz24_0x4)
                        {
                            if (var_a1 == NULL)
                            {
                                arg0->unk44 = var_v1->next;
                            }
                            else var_a1->next = var_v1->next;
                            
                            var_v1->next = D_8009EDD0_406D0.unk_alsound_0x30;
                            D_8009EDD0_406D0.unk_alsound_0x30 = var_v1;
                            break;
                        }

                        var_a1 = var_v1;
                        var_v1 = var_v1->next;
                    }
                    break;
                    
                case 0x60:
                    param2 = *ucode++;

                    if (param2 & 0x80)
                    {
                        param2 = *ucode++ + ((param2 & 0x7F) << 8);
                    }
                    if (param2 < D_8009EDD0_406D0.unk_alsound_0x14)
                    {
                        // This is probably an actual struct?
                        arg0->unk40 = D_8009EDD0_406D0.unk_alsound_0x18[param2]->unkALWhatever8009EDD0_off18_0x8;
                        arg0->unk2A = 3;
                    }
                    break;
                    
                case 0x70:
                    arg0->unk2A = 2;
                    timer = 10000;
                    break;
                    
                case 0x80:
                    arg0->unk24 = ucode;
                    break;
                    
                case 0x90:
                    ucode = arg0->unk24;
                    break;
                }
            }
            while (timer == 0);
            
            arg0->unk20 = ucode;
            arg0->unk28 = timer;
        }
    }
    var_s0_6 = arg0->unk44;
    
    while (var_s0_6 != NULL)
    {
        if (var_s0_6->unkALWhatever8009EDD0_siz24_0x5 < 4)
        {
            var_s0_6->unkALWhatever8009EDD0_siz24_0x14 += 1.0F;
                
            if (var_s0_6->unkALWhatever8009EDD0_siz24_0x8 < var_s0_6->unkALWhatever8009EDD0_siz24_0x14)
            {
                var_s0_6->unkALWhatever8009EDD0_siz24_0x14 -= var_s0_6->unkALWhatever8009EDD0_siz24_0x8;
            }
        }
        switch (var_s0_6->unkALWhatever8009EDD0_siz24_0x5)
        {
        case 0:
            temp_f0_2 = (var_s0_6->unkALWhatever8009EDD0_siz24_0x14 / var_s0_6->unkALWhatever8009EDD0_siz24_0x8);
            sintable_id = ((s32) (temp_f0_2 * 4096.0F)) & 0xFFF;
                
            angle = gSYSinTable[sintable_id & 0x7FF] / 65536.0F;

            var_f12 = (((sintable_id & 0x800) ? -angle : angle) * var_s0_6->unkALWhatever8009EDD0_siz24_0xC) + var_s0_6->unkALWhatever8009EDD0_siz24_0x10;
            break;
                
        case 1:
            if ((var_s0_6->unkALWhatever8009EDD0_siz24_0x8 / 2) < var_s0_6->unkALWhatever8009EDD0_siz24_0x14)
            {
                var_f12 = var_s0_6->unkALWhatever8009EDD0_siz24_0xC;
            }
            else var_f12 = var_s0_6->unkALWhatever8009EDD0_siz24_0x10;  
            break;
            
            case 2:                                 /* switch 2 */
            var_f12 =
            (
                (var_s0_6->unkALWhatever8009EDD0_siz24_0xC * var_s0_6->unkALWhatever8009EDD0_siz24_0x14) /
                var_s0_6->unkALWhatever8009EDD0_siz24_0x8
            ) + var_s0_6->unkALWhatever8009EDD0_siz24_0x10;
            break;
                
        case 3:
            var_f12 =
            (
                (var_s0_6->unkALWhatever8009EDD0_siz24_0xC * (var_s0_6->unkALWhatever8009EDD0_siz24_0x8 - var_s0_6->unkALWhatever8009EDD0_siz24_0x14)) /
                var_s0_6->unkALWhatever8009EDD0_siz24_0x8
            ) + var_s0_6->unkALWhatever8009EDD0_siz24_0x10;
            break;
                
        case 4:
            var_s0_6->unkALWhatever8009EDD0_siz24_0x14 += 1.0F;
                
            if (var_s0_6->unkALWhatever8009EDD0_siz24_0x18 < var_s0_6->unkALWhatever8009EDD0_siz24_0x14)
            {
                var_s0_6->unkALWhatever8009EDD0_siz24_0x20 =
                ((randFloat1() * var_s0_6->unkALWhatever8009EDD0_siz24_0xC) + var_s0_6->unkALWhatever8009EDD0_siz24_0x10);
                    
                var_s0_6->unkALWhatever8009EDD0_siz24_0x18 = randFloat1() * var_s0_6->unkALWhatever8009EDD0_siz24_0x8;
                var_s0_6->unkALWhatever8009EDD0_siz24_0x14 = 0;
            }
            var_f12 = var_s0_6->unkALWhatever8009EDD0_siz24_0x20;
            break;
                
        case 5:
            var_s0_6->unkALWhatever8009EDD0_siz24_0x14 += 1.0F;
                
            if (var_s0_6->unkALWhatever8009EDD0_siz24_0x18 < var_s0_6->unkALWhatever8009EDD0_siz24_0x14)
            {
                var_s0_6->unkALWhatever8009EDD0_siz24_0x1C = var_s0_6->unkALWhatever8009EDD0_siz24_0x20;
                var_s0_6->unkALWhatever8009EDD0_siz24_0x20 = ((randFloat1() * var_s0_6->unkALWhatever8009EDD0_siz24_0xC) + var_s0_6->unkALWhatever8009EDD0_siz24_0x10);
                    
                var_s0_6->unkALWhatever8009EDD0_siz24_0x18 = randFloat1() * var_s0_6->unkALWhatever8009EDD0_siz24_0x8 + 0.5F;
                var_s0_6->unkALWhatever8009EDD0_siz24_0x14 = 0;
            }
            var_f12 =
            (
                (
                    (var_s0_6->unkALWhatever8009EDD0_siz24_0x20 - var_s0_6->unkALWhatever8009EDD0_siz24_0x1C) * 
                    var_s0_6->unkALWhatever8009EDD0_siz24_0x14
                ) / var_s0_6->unkALWhatever8009EDD0_siz24_0x18
            ) + var_s0_6->unkALWhatever8009EDD0_siz24_0x1C;
            break;
                
        case 6:
            var_s0_6->unkALWhatever8009EDD0_siz24_0x14 += 1.0F;
                
            if (var_s0_6->unkALWhatever8009EDD0_siz24_0x8 < var_s0_6->unkALWhatever8009EDD0_siz24_0x14)
            {
                var_s0_6->unkALWhatever8009EDD0_siz24_0x14 = var_s0_6->unkALWhatever8009EDD0_siz24_0x8;
            }
            var_f12 =
            (
                (var_s0_6->unkALWhatever8009EDD0_siz24_0xC * var_s0_6->unkALWhatever8009EDD0_siz24_0x14) /
                var_s0_6->unkALWhatever8009EDD0_siz24_0x8
            ) + var_s0_6->unkALWhatever8009EDD0_siz24_0x10;
            break;
                
        case 7:
            var_s0_6->unkALWhatever8009EDD0_siz24_0x14 += 1.0F;
                
            if (var_s0_6->unkALWhatever8009EDD0_siz24_0x8 < var_s0_6->unkALWhatever8009EDD0_siz24_0x14)
            {
                var_s0_6->unkALWhatever8009EDD0_siz24_0x14 = var_s0_6->unkALWhatever8009EDD0_siz24_0x8;
            }
            var_f12 =
            (
                (var_s0_6->unkALWhatever8009EDD0_siz24_0xC * (var_s0_6->unkALWhatever8009EDD0_siz24_0x8 - var_s0_6->unkALWhatever8009EDD0_siz24_0x14))
                / var_s0_6->unkALWhatever8009EDD0_siz24_0x8
            ) + var_s0_6->unkALWhatever8009EDD0_siz24_0x10;
            break;
                
        case 8:
            var_s0_6->unkALWhatever8009EDD0_siz24_0x14 += 1.0F;
                
            if (var_s0_6->unkALWhatever8009EDD0_siz24_0x18 < var_s0_6->unkALWhatever8009EDD0_siz24_0x14)
            {
                var_s0_6->unkALWhatever8009EDD0_siz24_0x20 = (randFloat2() * var_s0_6->unkALWhatever8009EDD0_siz24_0xC) + var_s0_6->unkALWhatever8009EDD0_siz24_0x10;
                var_s0_6->unkALWhatever8009EDD0_siz24_0x18 = randFloat2() * var_s0_6->unkALWhatever8009EDD0_siz24_0x8;
                var_s0_6->unkALWhatever8009EDD0_siz24_0x14 = 0;
            }
            var_f12 = var_s0_6->unkALWhatever8009EDD0_siz24_0x20;
            break;
                
        default:
            var_f12 = 0;
            break;
        }
        switch (var_s0_6->unkALWhatever8009EDD0_siz24_0x7 >> 4)
        {
        case 1:
            var_f12 += spC0[var_s0_6->unkALWhatever8009EDD0_siz24_0x7 & 0xF];
            break;
                
        case 2:
            var_f12 *= spC0[var_s0_6->unkALWhatever8009EDD0_siz24_0x7 & 0xF];
            break;
        }
        
        temp_a0 = var_s0_6->unkALWhatever8009EDD0_siz24_0x6;
        
        switch (temp_a0)
        {
        case 11:
            var_f12 += arg0->unk32;
                /* fallthrough */
        case 10:                                /* switch 3 */
            if (var_f12 < 0)
            {
                var_f12 = 0;
            }
            if (var_f12 > 127)
            {
                var_f12 = 127;
            }
            arg0->unk32 = var_f12;
            break;
                
        case 13:
            var_f12 += arg0->unk2C;
                /* fallthrough */
        case 12:
            if (var_f12 < -1200)
            {
                var_f12 = -1200;
            }
            if (var_f12 > 1200)
            {
                var_f12 = 1200;
            }
            arg0->unk2C = var_f12;
            break;
                
        case 15:
            var_f12 += arg0->unk34;
                /* fallthrough */
        case 14:
            if (var_f12 < 0)
            {
                var_f12 = 0;
            }
            if (var_f12 > 127)
            {
                var_f12 = 127;
            }
            arg0->unk34 = var_f12;
            break;
                
        default:
            if (temp_a0 < 10)
            {
                spC0[temp_a0] = var_f12;
            }
            else
            {
                temp_a1 = temp_a0 - 24;
                    
                if (temp_a0 < 24)
                {
                    var_v1 = var_s0_6;
                    temp_a1 = temp_a0;
                }
                else
                {
                    var_v1 = arg0->unk44;

                    temp_v0 = temp_a1 / 8;
                    // FAKE but ill take it
                    temp_a0 = 8;
                    temp_a1 %= temp_a0;
                    temp_a1 += 16;
                        
                    while (var_v1 != 0)
                    {
                        if (temp_v0 != var_v1->unkALWhatever8009EDD0_siz24_0x4)
                        {
                            var_v1 = var_v1->next;
                        }
                        else break;
                    }
                    if (var_v1 == NULL)
                    {
                        break;
                    }
                }
                switch (temp_a1)
                {
                case 17:
                    var_f12 += var_v1->unkALWhatever8009EDD0_siz24_0x8;
                    /* fallthrough */
                case 16:
                    var_v1->unkALWhatever8009EDD0_siz24_0x8 = var_f12;
                    break;
                    
                case 19:
                    var_f12 += var_v1->unkALWhatever8009EDD0_siz24_0xC;
                    /* fallthrough */
                case 18:
                        var_v1->unkALWhatever8009EDD0_siz24_0xC = var_f12;
                        break;
                        
                case 21:
                    var_f12 += var_v1->unkALWhatever8009EDD0_siz24_0x10;
                    /* fallthrough */
                case 20:
                    var_v1->unkALWhatever8009EDD0_siz24_0x10 = var_f12;
                    break;
                        
                case 23:
                    var_f12 += var_v1->unkALWhatever8009EDD0_siz24_0x14;
                    /* fallthrough */
                case 22:
                    var_v1->unkALWhatever8009EDD0_siz24_0x14 = var_f12;
                    break;
                }
            }
            break;
        }
        var_s0_6 = var_s0_6->next;
    }
    switch (arg0->unk2A)
    {
    case 0:
        break;
        
    case 1:
        if (arg0->unk2C != arg0->unk2E)
        {
            n_alSynSetPitch(&arg0->voice, alCents2Ratio(arg0->unk2C + arg0->unk30));
        }
        if ((arg0->unk32 != arg0->unk33) || (arg0->unk38 != arg0->unk39))
        {
            n_alSynSetVol(&arg0->voice, ((arg0->unk32 * arg0->unk38 * D_8009EDD0_406D0.unk_alsound_0x5A) >> 7), D_8009EDD0_406D0.unk_alsound_0x44);
        }
        if ((arg0->unk34 != arg0->unk35) || (arg0->unk3A != arg0->unk3B))
        {
            param = ((s16)arg0->unk34 + arg0->unk3A) - 64;
            
            if (param < 0)
            {
                param = 0;
            }
            if (param > 0x7F)
            {
                param = 0x7F;
            }
            n_alSynSetPan(&arg0->voice, param);
        }
        if ((arg0->unk36 != arg0->unk37) || (arg0->unk3C != arg0->unk3D))
        {
            param = (arg0->unk36 * (arg0->unk3C >> 1)) >> 7;
            
            if (param < 0)
            {
                param = 0;
            }
            if (param > 0x7F)
            {
                param = 0x7F;
            }
            n_alSynSetFXMix(&arg0->voice, param);
        }
        break;
        
    case 2:
        n_alSynSetVol(&arg0->voice, 0, D_8009EDD0_406D0.unk_alsound_0x44);
        arg0->unk2A = 0;
        break;

    case 3:
        voice_config.fxBus = 0;
        voice_config.priority = arg0->unk2B & 0x7F;
        voice_config.unityPitch = 0;
            
        if (n_alSynAllocVoice(&arg0->voice, &voice_config) != 0)
        {
            param = (arg0->unk34 + arg0->unk3A) - 0x40;
                
            if (param < 0)
            {
                param = 0;
            }
            if (param > 0x7F)
            {
                param = 0x7F;
            }
            param3 = (arg0->unk36 * (arg0->unk3C >> 1)) >> 7;
                
            if (param3 < 0)
            {
                param3 = 0;
            }
            if (param3 > 0x7F)
            {
                param3 = 0x7F;
            }
            n_alSynStartVoiceParams(&arg0->voice, arg0->unk40, alCents2Ratio(arg0->unk2C + arg0->unk30), (arg0->unk32 * arg0->unk38 * D_8009EDD0_406D0.unk_alsound_0x5A) >> 7, param, param3, 0);
            arg0->unk2A = 1;
        }
        else arg0->unk2A = 0;
        break;
    }
    arg0->unk2E = arg0->unk2C;
    arg0->unk33 = arg0->unk32;
    arg0->unk35 = arg0->unk34;
    arg0->unk37 = arg0->unk36;
    arg0->unk39 = arg0->unk38;
    arg0->unk3B = arg0->unk3A;
    arg0->unk3D = arg0->unk3C;
}


void func_80027458_28058(void)
{

}

s32 func_800264A4_270A4();
void func_8002668C_2728C(ALWhatever8009EE0C*);
ALWhatever8009EDD0_siz34* func_80026844_27444(void *id);
ALWhatever8009EDD0_siz34* func_80026958_27558(void *id);
ALWhatever8009EE0C* func_80026A6C_2766C(void *arg0);
ALWhatever8009EE0C* func_80026B40_27740(u16 id);
f32 randFloat1();

void func_80026B90_27790(ALWhatever8009EDD0_siz34 *arg0);

// TODO: where does this go?
static s32 func_800293A8_29FA8(s32 arg0) 
{
    ALWhatever8009EDD0_siz24* temp_a0;
    ALWhatever8009EE0C* next_node;
    ALWhatever8009EDD0_siz34* temp_v1;
    ALWhatever8009EDD0_siz34* var_s0;
    ALWhatever8009EE0C* this_node;
    ALWhatever8009EE0C* var_s4;
    ALWhatever8009EDD0_siz24* var_v0;
    ALWhatever8009EDD0_siz24* var_v1;
    N_ALVoice* temp_s0;

    this_node = D_8009EDD0_406D0.unk_alsound_0x3C;
    var_s4 = NULL;
    
    while (this_node != NULL) 
    {
        next_node = (ALWhatever8009EE0C*)this_node->next;
        temp_s0 = &this_node->voice;
        
        if (this_node->unk2A == 0) 
        {
            n_alSynStopVoice(temp_s0);
            n_alSynFreeVoice(temp_s0);
            
            var_s0 = D_8009EDD0_406D0.unk_alsound_0x40;
            
            while (var_s0 != NULL) 
            {
                if (this_node == var_s0->unkALWhatever8009EDD0_siz34_0x28) 
                {
                    var_s0->unkALWhatever8009EDD0_siz34_0x28 = NULL;
                }
                
                var_s0 = var_s0->next;
            }
            
            temp_a0 = this_node->unk44;
            
            if (temp_a0 != NULL) 
            {
                var_v0 = temp_a0->next;
                var_v1 = temp_a0;
                
                while (var_v0 != NULL)
                {
                    var_v1 = var_v0;
                    var_v0 = var_v0->next;
                }
                
                var_v1->next = D_8009EDD0_406D0.unk_alsound_0x30;
                D_8009EDD0_406D0.unk_alsound_0x30 = this_node->unk44;
                this_node->unk44 = NULL;
            }
            
            this_node->unk48 = 0;
            
            if (var_s4 != NULL) 
            {
                var_s4->next = this_node->next;
            } 
            else 
            {
                D_8009EDD0_406D0.unk_alsound_0x3C = (ALWhatever8009EE0C* ) this_node->next;
            }
            
            this_node->next = D_8009EDD0_406D0.unk_alsound_0x34;
            D_8009EDD0_406D0.unk_alsound_0x34 = this_node;
        } 
        else 
        {
            var_s4 = this_node;
        }
        
        this_node = next_node;
    }
    
    var_s0 = D_8009EDD0_406D0.unk_alsound_0x40;
    
    while (var_s0 != NULL) 
    {
        func_80026B90_27790(var_s0);
        var_s0 = var_s0->next;
    }
    
    this_node = D_8009EDD0_406D0.unk_alsound_0x3C;
    
    while (this_node != NULL) 
    {
        func_80027460_28060(this_node);
        this_node = this_node->next;
    }
    
    var_s0 = D_8009EDD0_406D0.unk_alsound_0x40;
    var_v0 = NULL;
    
    while (var_s0 != NULL) 
    {
        temp_v1 = var_s0->next;
        
        if (var_s0->unkALWhatever8009EDD0_siz34_0x10 == 0xFFFF) 
        {
            var_s0->unkALWhatever8009EDD0_siz34_0x10 = 0U;
            var_s0->unkALWhatever8009EDD0_siz34_0x26 = 0;
            
            if (var_v0 != NULL) 
            {
                var_v0->next = var_s0->next;
            } 
            else 
            {
                D_8009EDD0_406D0.unk_alsound_0x40 = (ALWhatever8009EDD0_siz34* ) var_s0->next;
            }
            
            var_s0->next = D_8009EDD0_406D0.unk_alsound_0x38;
            D_8009EDD0_406D0.unk_alsound_0x38 = var_s0;
        } 
        else 
        {
            var_v0 = var_s0;
        }
        var_s0 = temp_v1;
    }
    randFloat2();
    
    return D_8009EDD0_406D0.unk_alsound_0x44;
}

f32 randFloat1(void)
{
	sRandomSeed2 = (sRandomSeed2 * 0x343FD) + 0x269EC3;
	return ((sRandomSeed2 >> 16) & 0xFFFF) / 65536.0f;
}

f32 randFloat2(void)
{
	sRandomSeed1 = (sRandomSeed1 * 0x343FD) + 0x269EC3;
	return ((sRandomSeed1 >> 16) & 0xFFFF) / 65536.0f;
}

void func_80027338(void)
{
    OSIntMask temp_a0;
    ALWhatever8009EE0C *var_v1;

    temp_a0 = osSetIntMask(1);
    var_v1 = D_8009EDD0_406D0.unk_alsound_0x3C;
    
    while (var_v1 != NULL)
    {
        var_v1->unk28 = 0;
        var_v1->unk2A = 2;
        var_v1->unk48 = 0;
        var_v1 = var_v1->next;
    }
    osSetIntMask(temp_a0);
}

void func_80026B90_27790(ALWhatever8009EDD0_siz34 *arg0)
{
	u8 *ucode;	 // ucode (sp6C)
    s16 var_v0;
    u16 timer;	 // timer (sp68)
    s32 i;
    ALWhatever8009EDD0_siz34 *var_a2;
	ALWhatever8009EDD0_siz34 *var_v1_2;
	u16 param;
	u8 instr;	// instr (sp20)
	s32 var_v1; // var_v1 (sp18)
    s16 var_t5; // var_t5 (sp52)
    u16 var_v0_2;

	var_t5 = 0;

	if (arg0->unkALWhatever8009EDD0_siz34_0x10 != 0xFFFF)
	{
		if (arg0->unkALWhatever8009EDD0_siz34_0x10 != 0)
		{
			arg0->unkALWhatever8009EDD0_siz34_0x10--;

			if (arg0->unkALWhatever8009EDD0_siz34_0x10 == 0)
			{
				if (arg0->unkALWhatever8009EDD0_siz34_0x21 != 0)
				{
					arg0->unkALWhatever8009EDD0_siz34_0x21 = 0;

					if (arg0->unkALWhatever8009EDD0_siz34_0x28 != NULL)
					{
						func_8002668C_2728C(arg0->unkALWhatever8009EDD0_siz34_0x28);

						arg0->unkALWhatever8009EDD0_siz34_0x28 = NULL;
					}
					arg0->unkALWhatever8009EDD0_siz34_0x10 = 1;

					return;
				}
				ucode = arg0->unkALWhatever8009EDD0_siz34_0x8;

				do
				{
					instr = *ucode++;

					if ((instr & 0xF8) >= 0xD0)
					{
						timer = 0;

						switch (instr)
						{
						case 0xD0:
							timer = -1;
                            
							if (arg0->unkALWhatever8009EDD0_siz34_0x28 != NULL)
							{
								func_8002668C_2728C(arg0->unkALWhatever8009EDD0_siz34_0x28);

								arg0->unkALWhatever8009EDD0_siz34_0x28 = NULL;
							}
							break;

						case 0xD1:
							param = *ucode++;

							if (param & 0x80)
							{
								param = *ucode++ + ((param & 0x7F) << 8);
							}
							arg0->unkALWhatever8009EDD0_siz34_0x24 = param;
							break;

						case 0xD2:
							param = *ucode++;

							arg0->unkALWhatever8009EDD0_siz34_0x1E = param & 0x7F;
							arg0->unkALWhatever8009EDD0_siz34_0x20 = (param & 0x80) ? 1 : 0;
							break;

						case 0xD3:
                            arg0->unkALWhatever8009EDD0_siz34_0x1F = *ucode++;
                            break;

						case 0xD4:
							for (i = 0; i < ARRAY_COUNT(arg0->unkALWhatever8009EDD0_siz34_0x12); i++)
							{
								param = *ucode++;

								if (param & 0x80)
								{
									param = *ucode++ + ((param & 0x7F) << 8);
								}
								arg0->unkALWhatever8009EDD0_siz34_0x12[i] = param;
							}
							break;

						case 0xD5:
                            arg0->unkALWhatever8009EDD0_siz34_0x22 = *ucode++;
                            break;

						case 0xD6:
							var_v0 = ((s8)*ucode++) + arg0->unkALWhatever8009EDD0_siz34_0x22;

							if (var_v0 < 0)
							{
								var_v0 = 0;
							}
							if (var_v0 > 0xFF)
							{
								var_v0 = 0xFF;
							}
							arg0->unkALWhatever8009EDD0_siz34_0x22 = var_v0;
							break;

						case 0xD7:
                            arg0->unkALWhatever8009EDD0_siz34_0x23 = *ucode++;
                            break;

						case 0xD8:
							var_v0 = ((s8)*ucode++) + arg0->unkALWhatever8009EDD0_siz34_0x23;

							if (var_v0 < 0)
							{
								var_v0 = 0;
							}
							if (var_v0 > 0x7F)
							{
								var_v0 = 0x7F;
							}
							arg0->unkALWhatever8009EDD0_siz34_0x23 = var_v0;
							break;

						case 0xD9:
							param = *ucode++;

							if (param & 0x80)
							{
								param = *ucode++ + ((param & 0x7F) << 8);
							}
							if (param >= D_8009EDD0_406D0.fgm_ucode_count)
							{
								var_a2 = NULL;
							}
                            else var_a2 = D_8009EDD0_406D0.unk_alsound_0x38;

							if (var_a2 != NULL)
							{
								D_8009EDD0_406D0.unk_alsound_0x38 = var_a2->next;
								*var_a2 = *arg0;

								var_v1_2 = arg0;
								var_a2->next = NULL;

								while (var_v1_2->next != NULL)
								{
									var_v1_2 = var_v1_2->next;
								}
								var_v1_2->next = var_a2;
								var_a2->unkALWhatever8009EDD0_siz34_0x10 = 1;

                                var_a2->unkALWhatever8009EDD0_siz34_0x8 = D_8009EDD0_406D0.fgm_ucode_data[param];
                                var_a2->unkALWhatever8009EDD0_siz34_0xC = var_a2->unkALWhatever8009EDD0_siz34_0x8;
                                
								if (++D_8009EDD0_406D0.unk_alsound_0x4A == 0)
								{
									D_8009EDD0_406D0.unk_alsound_0x4A++;
								}
								var_a2->unkALWhatever8009EDD0_siz34_0x26 = D_8009EDD0_406D0.unk_alsound_0x4A;
								var_a2->unkALWhatever8009EDD0_siz34_0x28 = NULL;

								if (var_a2->unkALWhatever8009EDD0_siz34_0x4 == NULL)
								{
									var_a2->unkALWhatever8009EDD0_siz34_0x4 = arg0;
								}
							}
							break;

						case 0xDA:
                            arg0->unkALWhatever8009EDD0_siz34_0xC = ucode;
                            break;

						case 0xDB:
                            ucode = arg0->unkALWhatever8009EDD0_siz34_0xC;
                            break;

						case 0xDC:
                            arg0->unkALWhatever8009EDD0_siz34_0x2C = *ucode++;
                            break;

						case 0xDD:
							var_v0 = ((s8)*ucode++) + arg0->unkALWhatever8009EDD0_siz34_0x2C;

							if (var_v0 < 0)
							{
								var_v0 = 0;
							}
							if (var_v0 > 0x7F)
							{
								var_v0 = 0x7F;
							}
							arg0->unkALWhatever8009EDD0_siz34_0x2C = var_v0;
							break;

						case 0xDE:
							arg0->unkALWhatever8009EDD0_siz34_0x2D = *ucode++;

							if (arg0->unkALWhatever8009EDD0_siz34_0x2D != 0)
							{
								var_v1_2 = D_8009EDD0_406D0.unk_alsound_0x40;

								while (var_v1_2 != NULL)
								{
									if ((var_v1_2 != arg0) && (arg0->unkALWhatever8009EDD0_siz34_0x2D == var_v1_2->unkALWhatever8009EDD0_siz34_0x2D))
									{
										if ((arg0->unkALWhatever8009EDD0_siz34_0x1F & 0x7F) >= (var_v1_2->unkALWhatever8009EDD0_siz34_0x1F & 0x7F))
										{
											var_v1_2->unkALWhatever8009EDD0_siz34_0x10 = -1;

											if (var_v1_2->unkALWhatever8009EDD0_siz34_0x28 != NULL)
											{
												func_8002668C_2728C(var_v1_2->unkALWhatever8009EDD0_siz34_0x28);

												var_v1_2->unkALWhatever8009EDD0_siz34_0x28 = NULL;
											}
										}
										else
										{
											timer = -1;

											if (arg0->unkALWhatever8009EDD0_siz34_0x28 != NULL)
											{
												func_8002668C_2728C(arg0->unkALWhatever8009EDD0_siz34_0x28);

												arg0->unkALWhatever8009EDD0_siz34_0x28 = NULL;
											}
										}
									}
									var_v1_2 = var_v1_2->next;
								}
							}
							break;

						case 0xDF:
                            var_t5 = -0x960;
                            break;

						case 0xE0:
                            var_t5 = -0x12C0;
                            break;
						}
					}
					else
					{
						timer = (instr & 7);

						switch (timer)
						{
						case 0:
                            timer = 0;
                            break;

						case 1:
						case 2:
						case 3:
						case 4:
						case 5:
						case 6:
                            timer = arg0->unkALWhatever8009EDD0_siz34_0x12[timer - 1];
                            break;

						case 7:
							timer = *ucode++;

							if (timer & 0x80)
							{
								timer = *ucode++ + ((timer & 0x7F) << 8);
							}
							break;
						}
						if (!(instr & 0xF8))
						{
							if (arg0->unkALWhatever8009EDD0_siz34_0x28 != NULL)
							{
								func_8002668C_2728C(arg0->unkALWhatever8009EDD0_siz34_0x28);

								arg0->unkALWhatever8009EDD0_siz34_0x28 = NULL;
							}
						}
						else
						{
							if (arg0->unkALWhatever8009EDD0_siz34_0x28 != NULL)
							{
								arg0->unkALWhatever8009EDD0_siz34_0x28->unk30 = (s16)(((instr >> 3) * 100) - 1300) + var_t5;
								var_t5 = 0;
								arg0->unkALWhatever8009EDD0_siz34_0x28->unk2E = arg0->unkALWhatever8009EDD0_siz34_0x28->unk2C + 1;
							}
							else
							{
								arg0->unkALWhatever8009EDD0_siz34_0x28 = func_80026B40_27740(arg0->unkALWhatever8009EDD0_siz34_0x24);

								if (arg0->unkALWhatever8009EDD0_siz34_0x28 != NULL)
								{
									arg0->unkALWhatever8009EDD0_siz34_0x28->unk2B = arg0->unkALWhatever8009EDD0_siz34_0x1F;
									arg0->unkALWhatever8009EDD0_siz34_0x28->unk30 = (s16)(((instr >> 3) * 100) - 1300) + var_t5;
									var_t5 = 0;
									arg0->unkALWhatever8009EDD0_siz34_0x28->unk38 = (arg0->unkALWhatever8009EDD0_siz34_0x22 * arg0->unkALWhatever8009EDD0_siz34_0x2E) >> 7;

									if (arg0->unkALWhatever8009EDD0_siz34_0x2F != 0x80)
									{
										arg0->unkALWhatever8009EDD0_siz34_0x28->unk3A = arg0->unkALWhatever8009EDD0_siz34_0x2F;
									}
                                    else arg0->unkALWhatever8009EDD0_siz34_0x28->unk3A = arg0->unkALWhatever8009EDD0_siz34_0x23;

									if (arg0->unkALWhatever8009EDD0_siz34_0x30 != 0x80)
									{
										arg0->unkALWhatever8009EDD0_siz34_0x28->unk3C = arg0->unkALWhatever8009EDD0_siz34_0x30 * 2;
									}
                                    else arg0->unkALWhatever8009EDD0_siz34_0x28->unk3C = arg0->unkALWhatever8009EDD0_siz34_0x2C;
								}
							}
							if ((timer > 1) && (arg0->unkALWhatever8009EDD0_siz34_0x20 != 0))
							{
								timer--;
								arg0->unkALWhatever8009EDD0_siz34_0x21 = 1;
							}
						}
					}
				}
                while (timer == 0);

				arg0->unkALWhatever8009EDD0_siz34_0x8 = ucode;
				arg0->unkALWhatever8009EDD0_siz34_0x10 = timer;
			}
		}
	}
}

ALWhatever8009EE0C* func_80026B40_27740(u16 id)
{
    if (id >= D_8009EDD0_406D0.fgm_table_count)
    {
        return NULL;
    }
    else return func_80026A6C_2766C(D_8009EDD0_406D0.fgm_table_data[id]);
}

ALWhatever8009EE0C* func_80026A6C_2766C(void *arg0)
{
    ALWhatever8009EE0C *temp_v1;
    OSIntMask temp_a1;
    
    temp_a1 = osSetIntMask(1);
    temp_v1 = D_8009EDD0_406D0.unk_alsound_0x34;
    
    if (temp_v1 != NULL)
    {
        D_8009EDD0_406D0.unk_alsound_0x34 = temp_v1->next;
        temp_v1->next = D_8009EDD0_406D0.unk_alsound_0x3C;
        D_8009EDD0_406D0.unk_alsound_0x3C = temp_v1;
        
        temp_v1->unk28 = 1;
        temp_v1->unk20 = arg0;
        temp_v1->unk24 = arg0;
        temp_v1->unk2A = 3;
        temp_v1->unk32 = 0x7F;
        temp_v1->unk34 = 0x40;
        temp_v1->unk36 = 0;
        temp_v1->unk2C = 0;
        temp_v1->unk2B = D_8009EDD0_406D0.unk_alsound_0x4C;
        temp_v1->unk44 = 0;
        temp_v1->unk30 = 0;
        temp_v1->unk38 = 0xFF;
        temp_v1->unk3A = 0x40;
        temp_v1->unk3C = 0;
        
        D_8009EDD0_406D0.unk_alsound_0x48++;

        if (D_8009EDD0_406D0.unk_alsound_0x48 == 0)
        {
            D_8009EDD0_406D0.unk_alsound_0x48++;
        }
        temp_v1->unk48 = D_8009EDD0_406D0.unk_alsound_0x48;
    }
    osSetIntMask(temp_a1);
    
    return temp_v1;
}

ALWhatever8009EDD0_siz34* func_80026A10_27610(u16 id)
{
	if (id >= D_8009EDD0_406D0.fgm_ucode_count)
	{
		return NULL;
	}
	else return func_80026844_27444(D_8009EDD0_406D0.fgm_ucode_data[id]);
}

ALWhatever8009EDD0_siz34* func_800269C0_275C0(u16 id)
{
	if (id >= D_8009EDD0_406D0.fgm_ucode_count)
	{
		return NULL;
	}
	else return func_80026958_27558(D_8009EDD0_406D0.fgm_ucode_data[id]);
}

ALWhatever8009EDD0_siz34* func_80026958_27558(void *id)
{
    ALWhatever8009EDD0_siz34 *temp_v0;
    s32 unused;
    OSIntMask sp24 = osSetIntMask(1);

    temp_v0 = func_80026844_27444(id);
    
    if (temp_v0 != NULL)
    {
        temp_v0->next = D_8009EDD0_406D0.unk_alsound_0x40;
        D_8009EDD0_406D0.unk_alsound_0x40 = temp_v0;
    }
    osSetIntMask(sp24);

    return temp_v0;
}

static ALWhatever8009EDD0_siz34* func_80026844_27444(void *id)
{
    ALWhatever8009EDD0_siz34* temp_s0;
    OSIntMask temp_a3;
    s32 i;
    
    temp_a3 = osSetIntMask(1);
    temp_s0 = D_8009EDD0_406D0.unk_alsound_0x38;

    if (temp_s0 != NULL)
    {
        D_8009EDD0_406D0.unk_alsound_0x38 = temp_s0->next;
        temp_s0->unkALWhatever8009EDD0_siz34_0x10 = 1;
        temp_s0->unkALWhatever8009EDD0_siz34_0x8 = id;
        temp_s0->unkALWhatever8009EDD0_siz34_0xC = id;
        temp_s0->unkALWhatever8009EDD0_siz34_0x1E = 0x30;
        temp_s0->unkALWhatever8009EDD0_siz34_0x1F = (u8) D_8009EDD0_406D0.unk_alsound_0x4C;

        for (i = 0; i < (ARRAY_COUNT(temp_s0->unkALWhatever8009EDD0_siz34_0x12) + ARRAY_COUNT(D_8009EDD0_406D0.unk_alsound_0x4E)) / 2; i++)
        {
            temp_s0->unkALWhatever8009EDD0_siz34_0x12[i] = D_8009EDD0_406D0.unk_alsound_0x4E[i];
        }
        temp_s0->unkALWhatever8009EDD0_siz34_0x20 = 
        temp_s0->unkALWhatever8009EDD0_siz34_0x21 = 0;
        temp_s0->unkALWhatever8009EDD0_siz34_0x28 = NULL;
        temp_s0->unkALWhatever8009EDD0_siz34_0x4 = NULL;
        temp_s0->unkALWhatever8009EDD0_siz34_0x22 = 0xFF;
        temp_s0->unkALWhatever8009EDD0_siz34_0x23 = 0x40;
        temp_s0->unkALWhatever8009EDD0_siz34_0x2C = 0x40;
        temp_s0->unkALWhatever8009EDD0_siz34_0x2E = 0x7F;
        temp_s0->unkALWhatever8009EDD0_siz34_0x2F = 0x80;
        temp_s0->unkALWhatever8009EDD0_siz34_0x30 = 0x80;
        
        D_8009EDD0_406D0.unk_alsound_0x4A++;
        
        if (D_8009EDD0_406D0.unk_alsound_0x4A == 0)
        {
            D_8009EDD0_406D0.unk_alsound_0x4A++;
        }
        temp_s0->unkALWhatever8009EDD0_siz34_0x26 = D_8009EDD0_406D0.unk_alsound_0x4A;
        temp_s0->unkALWhatever8009EDD0_siz34_0x2D = 0;
    }
    osSetIntMask(temp_a3);
    
    return temp_s0;
}

void func_800267F4_273F4(ALWhatever8009EDD0_siz34 *arg0)
{
    OSIntMask temp_a0 = osSetIntMask(1);

    if (arg0 != NULL)
    {
        arg0->next = D_8009EDD0_406D0.unk_alsound_0x40;
        D_8009EDD0_406D0.unk_alsound_0x40 = arg0;
    }
    osSetIntMask(temp_a0);
}

void func_80026738_27338(ALWhatever8009EDD0_siz34 *arg0)
{
    ALWhatever8009EDD0_siz34 *var_v1;
    ALWhatever8009EDD0_siz34 *temp_v0;
    ALWhatever8009EDD0_siz34 *temp_a2;
    u32 sp1C;
    ALWhatever8009EDD0_siz34 *var_a1;
    ALWhatever8009EE0C *temp_a0;

    sp1C = osSetIntMask(1);
    var_v1 = D_8009EDD0_406D0.unk_alsound_0x40;
    var_a1 = NULL;
    
    while (var_v1 != NULL)
    {
        temp_a2 = var_v1->next;
        
        if ((var_v1 == arg0) || (var_v1->unkALWhatever8009EDD0_siz34_0x4 == arg0))
        {
            var_v1->unkALWhatever8009EDD0_siz34_0x10 = 0;
            var_v1->unkALWhatever8009EDD0_siz34_0x26 = 0;

            if (var_v1->unkALWhatever8009EDD0_siz34_0x28 != NULL)
            {
                temp_a0 = var_v1->unkALWhatever8009EDD0_siz34_0x28;
            
                temp_a0->unk28 = 0;
                temp_a0->unk2A = 2;
                temp_a0->unk48 = 0;
            }
            if (var_a1 != NULL)
            {
                var_a1->next = var_v1->next;
            }
            else D_8009EDD0_406D0.unk_alsound_0x40 = var_v1->next;
            
            var_v1->next = D_8009EDD0_406D0.unk_alsound_0x38;
            D_8009EDD0_406D0.unk_alsound_0x38 = var_v1;
        }
        else var_a1 = var_v1;

        var_v1 = temp_a2;
    }
    osSetIntMask(sp1C);
}

void func_800266A0_272A0(void)
{
    ALWhatever8009EDD0_siz34 *var_v1;
    ALWhatever8009EDD0_siz34 *temp_v0;
    u32 sp1C;
    ALWhatever8009EE0C *temp_a0;

    sp1C = osSetIntMask(1);
    func_800264A4_270A4();
    
    var_v1 = D_8009EDD0_406D0.unk_alsound_0x40;
    
    while (var_v1 != NULL)
    {
        var_v1->unkALWhatever8009EDD0_siz34_0x10 = 0;
        var_v1->unkALWhatever8009EDD0_siz34_0x26 = 0;

        if (var_v1->unkALWhatever8009EDD0_siz34_0x28 != NULL)
        {
            temp_a0 = var_v1->unkALWhatever8009EDD0_siz34_0x28;
            
            temp_a0->unk28 = 0;
            temp_a0->unk2A = 2;
            temp_a0->unk48 = 0;
        }
        temp_v0 = var_v1;
        var_v1 = var_v1->next;
    }
    if (D_8009EDD0_406D0.unk_alsound_0x40 != NULL)
    {
        temp_v0->next = D_8009EDD0_406D0.unk_alsound_0x38;
        D_8009EDD0_406D0.unk_alsound_0x38 = D_8009EDD0_406D0.unk_alsound_0x40;
        D_8009EDD0_406D0.unk_alsound_0x40 = NULL;
    }
    osSetIntMask(sp1C);
}

void func_8002668C_2728C(ALWhatever8009EE0C *arg0)
{
    arg0->unk28 = 0;
    arg0->unk2A = 2;
    arg0->unk48 = 0;
}

s32 func_80026594_27194(void)
{
    ALWhatever8009EE0C *var_v0;
    ALWhatever8009EDD0_siz34 *var_v0_2;
    ALWhatever8009EE0C *var_a0;
    ALWhatever8009EDD0_siz34 *var_a0_2;
    ALWhatever8009EE0C *var_v1;
    ALWhatever8009EDD0_siz34 *var_v1_2;
    u32 im = osSetIntMask(1);
    
    var_v1 = D_8009EDD0_406D0.unk_alsound_0x3C;
    var_a0 = NULL;
    
    while (var_v1 != NULL)
    {
        var_v0 = var_v1->next;
      
        if (var_v1->unk2B & 0x80)
        {
            if (var_a0 != NULL)
            {
                var_a0->next = var_v1->next;
            }
            else D_8009EDD0_406D0.unk_alsound_0x3C = var_v1->next;
            
            if (var_v1->unk2A == 1)
            {
                if (var_v1->unkC != NULL)
                {
                    var_v1->unkC->unkALWhatever8009EDD0_off5C_offC_0x84 = 0; // 0xC = ALSeqPlayer? or N_ALSeqPlayer?
                }
            }
            var_v1->next = D_8009EDD0_406D0.unk_alsound_0x5C;
            D_8009EDD0_406D0.unk_alsound_0x5C = var_v1;
        }
        else var_a0 = var_v1;

        var_v1 = var_v0;
    }
    var_v1_2 = D_8009EDD0_406D0.unk_alsound_0x40;
    var_a0_2 = NULL;
    
    while (var_v1_2 != NULL)
    {
        var_v0_2 = var_v1_2->next;
        
        if (var_v1_2->unkALWhatever8009EDD0_siz34_0x1F & 0x80)
        {
            if (var_a0_2 != NULL)
            {
                var_a0_2->next = var_v1_2->next;
            }
            else D_8009EDD0_406D0.unk_alsound_0x40 = var_v1_2->next;

            var_v1_2->next = D_8009EDD0_406D0.unk_alsound_0x60;
            D_8009EDD0_406D0.unk_alsound_0x60 = var_v1_2;
        }
        else var_a0_2 = var_v1_2;
        
        var_v1_2 = var_v0_2;
    }
    osSetIntMask(im);
}

s32 func_800264A4_270A4(void)
{
  ALWhatever8009EE0C *var_v0;
  ALWhatever8009EDD0_siz34 *var_v0_2;
  int new_var;
  ALWhatever8009EE0C *var_v1;
  ALWhatever8009EDD0_siz34 *var_v1_2;
  u32 im = osSetIntMask(1);
    
    var_v1 = D_8009EDD0_406D0.unk_alsound_0x5C;
    
    if (var_v1 != NULL)
    {
        var_v0 = var_v1->next;
      
        while (var_v0 != NULL)
        {
            if (var_v1->unk2A == 1)
            {
                if (var_v1->unkC != NULL)
                {
                    var_v1->unkC->unkALWhatever8009EDD0_off5C_offC_0x84 = 1; // 0xC = ALSeqPlayer? or N_ALSeqPlayer?
                    var_v0 = var_v1->next;
                }
            }
            new_var = !(&D_8009EDD0_406D0);
            var_v1 = var_v0;
        
            if (new_var && new_var)
            {
            }
            var_v0 = var_v1->next;
        }
        if (var_v1->unk2A == 1)
        {
            if (var_v1->unkC != NULL)
            {
                var_v1->unkC->unkALWhatever8009EDD0_off5C_offC_0x84 = 1;
            }
        }
        var_v1->next = D_8009EDD0_406D0.unk_alsound_0x3C;
        D_8009EDD0_406D0.unk_alsound_0x3C = D_8009EDD0_406D0.unk_alsound_0x5C;
        D_8009EDD0_406D0.unk_alsound_0x5C = NULL;
    }
    var_v1_2 = D_8009EDD0_406D0.unk_alsound_0x60;
    
    if (var_v1_2 != NULL)
    {
        var_v0_2 = var_v1_2->next;
        
        while (var_v0_2 != NULL)
        {
            var_v1_2 = var_v0_2;
            var_v0_2 = var_v1_2->next;
        }
        var_v1_2->next = D_8009EDD0_406D0.unk_alsound_0x40;
        D_8009EDD0_406D0.unk_alsound_0x40 = D_8009EDD0_406D0.unk_alsound_0x60;
        D_8009EDD0_406D0.unk_alsound_0x60 = NULL;
    }
    osSetIntMask(im);
}

void func_80026204_26E04(N_ALUnk80026204 *arg0)
{
    s32 i, j;
    ALPlayer *client;

    D_8009EDD0_406D0.unk_alsound_0x30 = alHeapAlloc(arg0->heap, sizeof(*D_8009EDD0_406D0.unk_alsound_0x30), arg0->unk_80026204_0x0);

    for (i = 0; i < arg0->unk_80026204_0x0 - 1; i++)
    {
        D_8009EDD0_406D0.unk_alsound_0x30[i].next = &D_8009EDD0_406D0.unk_alsound_0x30[i + 1];
    }
    D_8009EDD0_406D0.unk_alsound_0x30[i].next = NULL;
    
    D_8009EDD0_406D0.unk_alsound_0x34 = alHeapAlloc(arg0->heap, sizeof(*D_8009EDD0_406D0.unk_alsound_0x34), arg0->unk_80026204_0x2);
    D_8009EDD0_406D0.unk_alsound_0x3C = 0;
    
    for (i = 0; i < arg0->unk_80026204_0x2 - 1; i++)
    {
        D_8009EDD0_406D0.unk_alsound_0x34[i].next = &D_8009EDD0_406D0.unk_alsound_0x34[i + 1];
    }
    D_8009EDD0_406D0.unk_alsound_0x34[i].next = NULL;
    D_8009EDD0_406D0.unk_alsound_0x48 = 0;

    D_8009EDD0_406D0.unk_alsound_0x38 = alHeapAlloc(arg0->heap, sizeof(*D_8009EDD0_406D0.unk_alsound_0x38), arg0->unk_80026204_0x4);
    D_8009EDD0_406D0.unk_alsound_0x40 = 0;
    
    for (i = 0; i < arg0->unk_80026204_0x4 - 1; i++)
    {
        D_8009EDD0_406D0.unk_alsound_0x38[i].unkALWhatever8009EDD0_siz34_0x10 = 0;
        D_8009EDD0_406D0.unk_alsound_0x38[i].next = &D_8009EDD0_406D0.unk_alsound_0x38[i + 1];
    }
    D_8009EDD0_406D0.unk_alsound_0x38[i].next = NULL;
    D_8009EDD0_406D0.unk_alsound_0x4A = 0;
    
    D_8009EDD0_406D0.fgm_ucode_data = arg0->fgm_ucode_data;
    D_8009EDD0_406D0.fgm_ucode_count = arg0->fgm_ucode_count;
    D_8009EDD0_406D0.fgm_table_data = arg0->fgm_table_data;
    D_8009EDD0_406D0.fgm_table_count = arg0->fgm_table_count;
    D_8009EDD0_406D0.unk_alsound_0x24 = arg0->unk_80026204_0x1C;
    D_8009EDD0_406D0.unk_alsound_0x2C = arg0->unk_80026204_0xC;
    D_8009EDD0_406D0.unk_alsound_0x14 = arg0->unk_80026204_0x6;
    D_8009EDD0_406D0.unk_alsound_0x18 =  arg0->unk_80026204_0x10;
    D_8009EDD0_406D0.unk_alsound_0x44 = (184000000 / n_syn->outputRate);
    D_8009EDD0_406D0.unk_alsound_0x5A = 127;

    client = &D_8009EDD0_406D0.node;
    
    client->next = NULL;
    client->handler = func_800293A8_29FA8;
    client->clientData = NULL;
    
    n_alSynAddSndPlayer(client);
    sRandomSeed2 = 1;
}

void func_80026174_26D74(ALWhatever8009EDD0_siz34 *arg0, u8 arg1)
{
    ALWhatever8009EDD0_siz34 *temp_v1, *var_v0;

    if (arg1 > 127)
    {
        arg1 = 127;
    }
    arg0->unkALWhatever8009EDD0_siz34_0x2E = arg1;
    
    if (arg0->unkALWhatever8009EDD0_siz34_0x28 != NULL)
    {
        arg0->unkALWhatever8009EDD0_siz34_0x28->unk38 = (arg0->unkALWhatever8009EDD0_siz34_0x22 * arg1) >> 7;
    }
    var_v0 = D_8009EDD0_406D0.unk_alsound_0x40;
    
    while (var_v0 != NULL)
    {
        temp_v1 = var_v0->next;
        
        if (arg0 == var_v0->unkALWhatever8009EDD0_siz34_0x4)
        {
            var_v0->unkALWhatever8009EDD0_siz34_0x2E = arg1;
            
            if (var_v0->unkALWhatever8009EDD0_siz34_0x28 != NULL)
            {
                var_v0->unkALWhatever8009EDD0_siz34_0x28->unk38 = (arg0->unkALWhatever8009EDD0_siz34_0x22 * arg1) >> 7;
            }
        }
        var_v0 = temp_v1;
    }
}

void func_80026104_26D04(ALWhatever8009EDD0_siz34 *arg0, u8 arg1)
{
    ALWhatever8009EDD0_siz34 *temp_v1, *var_v0;

    if (arg1 > 127)
    {
        arg1 = 127;
    }
    arg0->unkALWhatever8009EDD0_siz34_0x2F = arg1;
    
    if (arg0->unkALWhatever8009EDD0_siz34_0x28 != NULL)
    {
        arg0->unkALWhatever8009EDD0_siz34_0x28->unk3A = arg1;
    }
    var_v0 = D_8009EDD0_406D0.unk_alsound_0x40;
    
    while (var_v0 != NULL)
    {
        temp_v1 = (ALWhatever8009EDD0_siz34*) (u64) var_v0->next;
        
        if (arg0 == var_v0->unkALWhatever8009EDD0_siz34_0x4)
        {
            var_v0->unkALWhatever8009EDD0_siz34_0x2F = arg1;
            
            if (var_v0->unkALWhatever8009EDD0_siz34_0x28 != NULL)
            {
                var_v0->unkALWhatever8009EDD0_siz34_0x28->unk3A = arg1;
            }
        }
        var_v0 = temp_v1;
    }
}

void func_80026094_26C94(ALWhatever8009EDD0_siz34 *arg0, u8 arg1)
{
    ALWhatever8009EDD0_siz34 *temp_v1, *var_v0;

    if (arg1 > 127)
    {
        arg1 = 127;
    }
    arg0->unkALWhatever8009EDD0_siz34_0x30 = arg1;
    
    if (arg0->unkALWhatever8009EDD0_siz34_0x28 != NULL)
    {
        arg0->unkALWhatever8009EDD0_siz34_0x28->unk3C = arg1;
    }
    var_v0 = D_8009EDD0_406D0.unk_alsound_0x40;
    
    while (var_v0 != NULL)
    {
        temp_v1 = (ALWhatever8009EDD0_siz34*) (u64) var_v0->next;
        
        if (arg0 == var_v0->unkALWhatever8009EDD0_siz34_0x4)
        {
            var_v0->unkALWhatever8009EDD0_siz34_0x30 = arg1;
            
            if (var_v0->unkALWhatever8009EDD0_siz34_0x28 != NULL)
            {
                var_v0->unkALWhatever8009EDD0_siz34_0x28->unk3C = arg1;
            }
        }
        var_v0 = temp_v1;
    }
}

void func_80026070_26C70(u8 arg0)
{
	if (arg0 > 127)
	{
		arg0 = 127;
	}
	D_8009EDD0_406D0.unk_alsound_0x5A = arg0;
}
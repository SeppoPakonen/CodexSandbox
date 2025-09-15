#ifndef __AXART_H__
#define __AXART_H__

#if 0
extern "C" {
#endif


/*---------------------------------------------------------------------------*
    AXART_ART generic articulator header     
 *---------------------------------------------------------------------------*/
typedef struct
{

    void        *next;
    u32         type;

} AXART_ART;


// articulator types
#define AXART_TYPE_3D               1
#define AXART_TYPE_PANNING          2
#define AXART_TYPE_ITD              3
#define AXART_TYPE_SRCTYPE          4
#define AXART_TYPE_PITCH            5
#define AXART_TYPE_PITCH_ENV        6
#define AXART_TYPE_PITCH_MOD        7
#define AXART_TYPE_VOLUME           8
#define AXART_TYPE_AUXA_VOLUME      9
#define AXART_TYPE_AUXB_VOLUME      10
#define AXART_TYPE_VOLUME_ENV       11
#define AXART_TYPE_AUXA_VOLUME_ENV  12
#define AXART_TYPE_AUXB_VOLUME_ENV  13
#define AXART_TYPE_VOLUME_MOD       14
#define AXART_TYPE_AUXA_VOLUME_MOD  15
#define AXART_TYPE_AUXB_VOLUME_MOD  16


/*---------------------------------------------------------------------------*
    LFO used with modulation articulators     
 *---------------------------------------------------------------------------*/
typedef struct
{
    
    f32         *lfo;           // lfo samples in RAM
    u32         length;         // lfo samples for 1 period
    f32         delta;          // lfo delta per 5ms 
    
    u32         sampleIndex;    // lfo sample index      
    f32         counter;        // lfo counter
    f32         sample1;        // lfo last sample for interpolation
    f32         sample;         // lfo sample
    f32         output;         // lfo output

} AXART_LFO;

#define AXART_SINE_SAMPLES          64
#define AXART_SQUARE_SAMPLES        64
#define AXART_SAW_SAMPLES           64
#define AXART_REVERSESAW_SAMPLES    64
#define AXART_TRIANGLE_SAMPLES      64
#define AXART_NOISE_SAMPLES         64


extern f32 AXARTSine[AXART_SINE_SAMPLES];
extern f32 AXARTSquare[AXART_SQUARE_SAMPLES];
extern f32 AXARTSaw[AXART_SAW_SAMPLES];
extern f32 AXARTReverseSaw[AXART_REVERSESAW_SAMPLES];
extern f32 AXARTTriangle[AXART_TRIANGLE_SAMPLES];
extern f32 AXARTNoise[AXART_NOISE_SAMPLES];


/*---------------------------------------------------------------------------*
    articulators      
 *---------------------------------------------------------------------------*/
typedef struct
{
    AXART_ART   art;

    // runtime user params

    f32         hAngle;         // horzontal angle
    f32         vAngle;         // vertical angle
    f32         dist;           // distance from listener
    f32         closingSpeed;   // for doppler
    u32         update;         // set to true after changing params

    // do not write to these params

    u8          pan;
    u8          span;
    u8          src;
    u16         itdL;
    u16         itdR;
    f32         pitch;
    s32         attenuation;

} AXART_3D;

typedef struct
{
    AXART_ART   art;

    // runtime user params

    u8          pan;            // left - right 0 - 127, 64 is center
    u8          span;           // rear - front 0 - 127, 127 is front

} AXART_PANNING;

typedef struct
{
    AXART_ART   art;

    // runtime user params

    u16         itdL;           // sample shift left, 0 - 31
    u16         itdR;           // sample shift right, 0 - 31

} AXART_ITD;

typedef struct
{

    AXART_ART   art;
    
    // runtime user params

    u8          src;            // use one of the following
                                // AX_SRC_TYPE_NONE            
                                // AX_SRC_TYPE_LINEAR          
                                // AX_SRC_TYPE_4TAP_8K         
                                // AX_SRC_TYPE_4TAP_12K        
                                // AX_SRC_TYPE_4TAP_16K        


} AXART_SRCTYPE;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         cents;

} AXART_PITCH;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         delta;
    s32         target;
    s32         cents;

} AXART_PITCH_ENV;

typedef struct
{
    AXART_ART   art;
    AXART_LFO   lfo;

    // runtime user params
    
    s32         cents;

} AXART_PITCH_MOD;

typedef struct
{
    AXART_ART   art;
    
    // runtime user params

    s32         attenuation;

} AXART_VOLUME;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         attenuation;

} AXART_AUXA_VOLUME;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         attenuation;

} AXART_AUXB_VOLUME;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         delta;
    s32         target;
    s32         attenuation;

} AXART_VOLUME_ENV;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         delta;
    s32         target;
    s32         attenuation;

} AXART_AUXA_VOLUME_ENV;

typedef struct
{
    AXART_ART   art;

    // runtime user params
    
    s32         delta;
    s32         target;
    s32         attenuation;

} AXART_AUXB_VOLUME_ENV;

typedef struct
{
    AXART_ART   art;
    AXART_LFO   lfo;

    // runtime user params
    
    s32         attenuation;

} AXART_VOLUME_MOD;

typedef struct
{
    AXART_ART   art;
    AXART_LFO   lfo;
    
    // runtime user params

    s32         attenuation;

} AXART_AUXA_VOLUME_MOD;

typedef struct
{
    AXART_ART   art;
    AXART_LFO   lfo;

    // runtime user params
    
    s32         attenuation;

} AXART_AUXB_VOLUME_MOD;


/*---------------------------------------------------------------------------*
    AXART_SOUND struct used per sound in sound list to articulate     
 *---------------------------------------------------------------------------*/
typedef struct
{

    void        *next;          // next sound in list
    void        *prev;          // prev sound in list

    AXVPB       *axvpb;         // user acquired voice
    f32         sampleRate;     // normal sample rate

    AXART_ART   *articulators;  // list of articulators

} AXART_SOUND;



/*---------------------------------------------------------------------------*
    function prototypes
 *---------------------------------------------------------------------------*/

void AXARTInit                  (void);
void AXARTQuit                  (void);
void AXARTServiceSounds         (void);
void AXARTAddSound              (AXART_SOUND *sound);
void AXARTRemoveSound           (AXART_SOUND *sound);
void AXARTSet3DDistanceScale    (f32 scale);
void AXARTSet3DDopplerScale     (f32 scale);
void AXARTAddArticulator        (AXART_SOUND *sound, AXART_ART *articulator);

void AXARTInitLfo               (AXART_LFO *lfo, f32 *samples, u32 length, f32 delta);
void AXARTInitArt3D             (AXART_3D *articulator);
void AXARTInitArtPanning        (AXART_PANNING *articulator);
void AXARTInitArtItd            (AXART_ITD *articulator);
void AXARTInitArtSrctype        (AXART_SRCTYPE *articulator);
void AXARTInitArtPitch          (AXART_PITCH *articulator);
void AXARTInitArtPitchEnv       (AXART_PITCH_ENV *articulator);
void AXARTInitArtPitchMod       (AXART_PITCH_MOD *articulator);
void AXARTInitArtVolume         (AXART_VOLUME *articulator);
void AXARTInitArtAuxAVolume     (AXART_AUXA_VOLUME *articulator);
void AXARTInitArtAuxBVolume     (AXART_AUXB_VOLUME *articulator);
void AXARTInitArtVolumeEnv      (AXART_VOLUME_ENV *articulator);
void AXARTInitArtAuxAVolumeEnv  (AXART_AUXA_VOLUME_ENV *articulator);
void AXARTInitArtAuxBVolumeEnv  (AXART_AUXB_VOLUME_ENV *articulator);
void AXARTInitArtVolumeMod      (AXART_VOLUME_MOD *articulator);
void AXARTInitArtAuxAVolumeMod  (AXART_AUXA_VOLUME_MOD *articulator);
void AXARTInitArtAuxBVolumeMod  (AXART_AUXB_VOLUME_MOD *articulator);

#if 0
}
#endif

#endif // __AXART_H__ 

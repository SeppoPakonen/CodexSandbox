#pragma once



struct ColorChannel {
    GXColor mat_clr;
    GXColor amb_clr;
    GXBool enabled[4];
    GXColorSrc amb_src;
    GXColorSrc mat_src;
    uint light_mask;
    GXDiffuseFn diff_fn;
    GXAttnFn attn_fn;
};



extern LightState		GXLightState[8];
extern ColorChannel		ColorChan[6];
extern uint				GXNumChans;
extern u32				GXActiveLights;
extern GXColor			VtxColor;
extern GXColor			AccumColor;
extern Vec				FinalPosition;
extern Vec				FinalNormal;
extern RGBAf			Illum;
extern GXColor			FinalColors[2];



void GetLightNormal(LightState *state);

void DoColorLighting(ColorChannel *chan);
void DoAlphaLighting(ColorChannel *clr);
void GXColor2Float(RGBAf *vec,GXColor clr);
void GXColor2FloatNoAlpha(RGBAf *vec,GXColor clr);
void LightColorChannel(ColorChannel *chan, LightState *state);
void LightAlphaChannel(ColorChannel *clr, LightState *state);


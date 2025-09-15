#include "DolphinRE.h"

LightState		GXLightState[8];
ColorChannel	ColorChan[6];
uint			GXNumChans;
u32				GXActiveLights;
GXColor			VtxColor;
GXColor			AccumColor;
Vec				FinalNormal;
RGBAf			Illum;
GXColor			FinalColors[2];

void GXInitLightAttnA(GXLightObj *lt_obj, float a0, float a1, float a2) {
	lt_obj->spotlight_type.x = a0;
	lt_obj->spotlight_type.y = a1;
	lt_obj->spotlight_type.z = a2;
}


void GXGetLightAttnA(GXLightObj *lt_obj, float *a0, float *a1, float *a2) {
	*a0 = lt_obj->spotlight_type.x;
	*a1 = lt_obj->spotlight_type.y;
	*a2 = lt_obj->spotlight_type.z;
}


void GXInitLightAttnK(GXLightObj *lt_obj, float k0, float k1, float k2) {
	lt_obj->lt_dist_attn.x = k0;
	lt_obj->lt_dist_attn.y = k1;
	lt_obj->lt_dist_attn.z = k2;
}


void GXGetLightAttnK(GXLightObj *lt_obj, float *k0, float *k1, float *k2) {
	*k0 = lt_obj->lt_dist_attn.x;
	*k1 = lt_obj->lt_dist_attn.y;
	*k2 = lt_obj->lt_dist_attn.z;
}


void GXInitLightAttn(GXLightObj *lt_obj, float a0, float a1, float a2, float k0, float k1, float k2) {
	GXInitLightAttnA(lt_obj, a0, a1, a2);
	GXInitLightAttnK(lt_obj, k0, k1, k2);
}


void GXInitLightPos(GXLightObj *lt_obj, float x, float y, float z) {
	lt_obj->normal.x = x;
	lt_obj->normal.y = y;
	lt_obj->normal.z = z;
}


void GXGetLightPos(GXLightObj *lt_obj, float *x, float *y, float *z) {
	*x = lt_obj->normal.x;
	*y = lt_obj->normal.y;
	*z = lt_obj->normal.z;
}


void GXInitLightDir(GXLightObj *lt_obj, float nx, float ny, float nz) {
	Vec in;
	in.x = nx;
	in.y = ny;
	in.z = nz;
	
	float dot = VECDotProduct(&in, &in);
	if (dot != 0.0f)
		VECNormalize(&in, &in);
	
	lt_obj->halfangle.x = in.x;
	lt_obj->halfangle.y = in.y;
	lt_obj->halfangle.z = in.z;
}


void GXGetLightDir(GXLightObj *lt_obj, float *nx, float *ny, float *nz) {
	*nx = lt_obj->halfangle.x;
	*ny = lt_obj->halfangle.y;
	*nz = lt_obj->halfangle.z;
}


void GXInitSpecularDir(GXLightObj *lt_obj, float nx, float ny, float nz) {
	double x = -nx;
	double y = -ny;
	double z = 1.0 - nz;
	double vec_len = sqrt(z * z + y * y + x * x);
	float unit_mul = 1.0 / vec_len;
	lt_obj->halfangle.x = x * unit_mul;
	lt_obj->halfangle.y = y * unit_mul;
	lt_obj->halfangle.z = z * unit_mul;
	lt_obj->normal.x = -nx * POW2_1024;
	lt_obj->normal.y = -ny * POW2_1024;
	lt_obj->normal.z = -nz * POW2_1024;
}


void GXInitSpecularDirHA(GXLightObj *lt_obj, float nx, float ny, float nz, float hx, float hy, float hz) {
	lt_obj->halfangle.x = hx;
	lt_obj->halfangle.y = hy;
	lt_obj->halfangle.z = hz;
	
	lt_obj->normal.x = -nx * POW2_1024;
	lt_obj->normal.y = -ny * POW2_1024;
	lt_obj->normal.z = -nz * POW2_1024;
}


void GXInitLightColor(GXLightObj *lt_obj, GXColor color) {
	lt_obj->color = color;
}


void GXGetLightColor(GXLightObj *lt_obj, GXColor *color) {
	*color = lt_obj->color;
}


void GXInitLightSpot(GXLightObj *lt_obj, f32 cutoff, GXSpotFn spot_func) {
	float neg_cutoff_x;
	float cutoff_x;
	float a0, a1, a2;
	
	if (cutoff <= 0.0 || cutoff > 90.0)
		spot_func = GX_SP_OFF;
	
	cutoff_x = cos((cutoff * M_PI) / 180.0);
	
	switch (spot_func) {
		
	case GX_SP_FLAT:
		a0 = -1000.0 * cutoff_x;
		a1 = 1000.0;
		a2 = 0.0;
		break;
		
	case GX_SP_COS:
		neg_cutoff_x = 1.0 - cutoff_x;
		a0 = -cutoff_x / neg_cutoff_x;
		a1 = 1.0 / neg_cutoff_x;
		a2 = 0.0;
		break;
		
	case GX_SP_COS2:
		neg_cutoff_x = 1.0 - cutoff_x;
		a0 = 0.0;
		a1 = -cutoff_x / neg_cutoff_x;
		a2 = 1.0 / neg_cutoff_x;
		break;
		
	case GX_SP_SHARP:
		neg_cutoff_x = (1.0 - cutoff_x) * (1.0 - cutoff_x);
		a0 = ((cutoff_x - 2.0) * cutoff_x) / neg_cutoff_x;
		a1 = 2.0 / neg_cutoff_x;
		a2 = -1.0 / neg_cutoff_x;
		break;
		
	case GX_SP_RING1:
		neg_cutoff_x = (1.0 - cutoff_x) * (1.0 - cutoff_x);
		a0 = (-4.0 * cutoff_x) / neg_cutoff_x;
		a1 = ((1.0 + cutoff_x) * 4.0) / neg_cutoff_x;
		a2 = -4.0 / neg_cutoff_x;
		break;
		
	case GX_SP_RING2:
		neg_cutoff_x = (1.0 - cutoff_x) * (1.0 - cutoff_x);
		a0 = 1.0 - (2.0 * cutoff_x * cutoff_x) / neg_cutoff_x;
		a1 = (4.0 * cutoff_x) / neg_cutoff_x;
		a2 = -2.0 / neg_cutoff_x;
		break;
		
	default:
		a0 = 1.0;
		a1 = 0.0;
		a2 = 0.0;
	}
	lt_obj->spotlight_type.x = a0;
	lt_obj->spotlight_type.y = a1;
	lt_obj->spotlight_type.z = a2;
}


void GXInitLightDistAttn(GXLightObj *lt_obj, float ref_distance, float ref_brightness, GXDistAttnFn dist_func) {
	float k2, k1;
	
	if (ref_distance < 0.0)
		dist_func = GX_DA_OFF;
	
	if (ref_brightness <= 0.0 || 1.0 <= ref_brightness)
		dist_func = GX_DA_OFF;
	
	if (dist_func == GX_DA_GENTLE) {
		k1 = (1.0 - ref_brightness) / (ref_brightness * ref_distance);
		k2 = 0.0;
	}
	else if (dist_func == GX_DA_MEDIUM) {
		k1 = ((1.0 - ref_brightness) * 0.5) / (ref_brightness * ref_distance);
		k2 = ((1.0 - ref_brightness) * 0.5) / (ref_brightness * (ref_distance * ref_distance));
	}
	else if (dist_func == GX_DA_STEEP) {
		k1 = 0.0;
		k2 = (1.0 - ref_brightness) / (ref_brightness * (ref_distance * ref_distance));
	}
	else {
		k1 = 0.0;
		k2 = 0.0;
	}
	
	lt_obj->lt_dist_attn.x = 1.0;
	lt_obj->lt_dist_attn.y = k1;
	lt_obj->lt_dist_attn.z = k2;
}


void GXLoadLightObjImm(const GXLightObj *lt_obj, GXLightID light) {
	int light_id = TranslateGXLightID(light);
	ASSERTMSG(light_id >= 0 && light_id < 8, "Invalid light id");
	
	memcpy(&GXLightState[light_id].lt, lt_obj, sizeof(GXLightObj));
}



void GXSetChanAmbColor(GXChannelID chan, GXColor amb_color) {
	int chan_id;
	
	if (chan == GX_COLOR0A0) {
		chan_id = TranslateGXChannelID(GX_COLOR0);
		ASSERTMSG(chan_id >= 0 && chan_id < 6, "Invalid channel id");
		ColorChan[chan_id].amb_clr = amb_color;
		
		chan_id = TranslateGXChannelID(GX_ALPHA0);
		ASSERTMSG(chan_id >= 0 && chan_id < 6, "Invalid channel id");
		ColorChan[chan_id].amb_clr = amb_color;
	}
	else if (chan == GX_COLOR1A1) {
		chan_id = TranslateGXChannelID(GX_COLOR1);
		ASSERTMSG(chan_id >= 0 && chan_id < 6, "Invalid channel id");
		ColorChan[chan_id].amb_clr = amb_color;
		
		chan_id = TranslateGXChannelID(GX_ALPHA1);
		ASSERTMSG(chan_id >= 0 && chan_id < 6, "Invalid channel id");
		ColorChan[chan_id].amb_clr = amb_color;
	}
	else {
		chan_id = TranslateGXChannelID(chan);
		ASSERTMSG(chan_id >= 0 && chan_id < 6, "Invalid channel id");
		ColorChan[chan_id].amb_clr = amb_color;
	}
}


void GXSetChanMatColor(GXChannelID chan, GXColor mat_color) {
	int chan_id;
	
	if (chan == GX_COLOR0A0) {
		chan_id = TranslateGXChannelID(GX_COLOR0);
		ASSERTMSG(chan_id >= 0 && chan_id < 6, "Invalid channel id");
		ColorChan[chan_id].mat_clr = mat_color;
		
		chan_id = TranslateGXChannelID(GX_ALPHA0);
		ASSERTMSG(chan_id >= 0 && chan_id < 6, "Invalid channel id");
		ColorChan[chan_id].mat_clr = mat_color;
	}
	else if (chan == GX_COLOR1A1) {
		chan_id = TranslateGXChannelID(GX_COLOR1);
		ASSERTMSG(chan_id >= 0 && chan_id < 6, "Invalid channel id");
		ColorChan[chan_id].mat_clr = mat_color;
		
		chan_id = TranslateGXChannelID(GX_ALPHA1);
		ASSERTMSG(chan_id >= 0 && chan_id < 6, "Invalid channel id");
		ColorChan[chan_id].mat_clr = mat_color;
	}
	else {
		chan_id = TranslateGXChannelID(chan);
		ASSERTMSG(chan_id >= 0 && chan_id < 6, "Invalid channel id");
		ColorChan[chan_id].mat_clr = mat_color;
	}
}


void GXSetNumChans(u8 nChans) {
	ASSERTMSG(nChans <= 2, "Invalid count of channels");
	GXNumChans = nChans;
}


void GXSetChanCtrl(
		GXChannelID chan,
		GXBool enable,
		GXColorSrc amb_src,
		GXColorSrc mat_src,
		uint light_mask,
		GXDiffuseFn diff_fn,
		GXAttnFn attn_fn) {
	if (chan == GX_COLOR0A0) {
		GXSetChanCtrl(GX_COLOR0, enable, amb_src, mat_src, light_mask, diff_fn, attn_fn);
		GXSetChanCtrl(GX_ALPHA0, enable, amb_src, mat_src, light_mask, diff_fn, attn_fn);
	}
	else if (chan == GX_COLOR1A1) {
		GXSetChanCtrl(GX_COLOR1, enable, amb_src, mat_src, light_mask, diff_fn, attn_fn);
		GXSetChanCtrl(GX_ALPHA1, enable, amb_src, mat_src, light_mask, diff_fn, attn_fn);
	}
	else {
		int trans_chan = TranslateGXChannelID(chan);
		ASSERTMSG(trans_chan >= 0, "Invalid channel id");
		
		ColorChan[trans_chan].enabled[0]	= enable;
		ColorChan[trans_chan].amb_src		= amb_src;
		ColorChan[trans_chan].mat_src		= mat_src;
		ColorChan[trans_chan].light_mask	= light_mask;
		ColorChan[trans_chan].diff_fn		= diff_fn;
		ColorChan[trans_chan].attn_fn		= attn_fn;
		
		GXActiveLights = GX_LIGHT_NULL;
		for(int i = 0; i < 4; i++)
			if (ColorChan[i].enabled[0])
				GXActiveLights |= ColorChan[i].light_mask;
	}
}



void GXPerformLighting() {
	int chan_id;
	
	for(int i = 0; i < 8; i++) {
		u32 light_id = UntranslateGXLightID((GXLightID)i);
		if ((u32)GXActiveLights & light_id)
			GetLightNormal(&GXLightState[i]);
	}
	
	if (GXNumChans != 1) {
		if (GXNumChans != 2)
			return;
		VtxColor = CurrentColors[1];
		
		chan_id = TranslateGXChannelID(GX_COLOR1);
		ASSERTMSG(chan_id >= 0 && chan_id < 6, "Invalid channel id");
		DoColorLighting(&ColorChan[chan_id]);
		
		chan_id = TranslateGXChannelID(GX_ALPHA1);
		ASSERTMSG(chan_id >= 0 && chan_id < 6, "Invalid channel id");
		DoAlphaLighting(&ColorChan[chan_id]);
		
		FinalColors[1] = AccumColor;
	}
	VtxColor = CurrentColors[0];
	
	chan_id = TranslateGXChannelID(GX_COLOR0);
	ASSERTMSG(chan_id >= 0 && chan_id < 6, "Invalid channel id");
	DoColorLighting(&ColorChan[chan_id]);
	
	chan_id = TranslateGXChannelID(GX_ALPHA0);
	ASSERTMSG(chan_id >= 0 && chan_id < 6, "Invalid channel id");
	DoAlphaLighting(&ColorChan[chan_id]);
	
	FinalColors[0] = AccumColor;
}



void GetLightNormal(LightState *state) {
	VECSubtract(&state->lt.normal, &FinalPosition, &state->unit);
	
	state->vec_len =
		(state->unit.x * state->unit.x) +
		(state->unit.y * state->unit.y) +
		(state->unit.z * state->unit.z);
	
	state->el_pow = sqrt((double)state->vec_len);
	
	double mul = 1.0 / state->el_pow;
	state->unit.x *= mul;
	state->unit.y *= mul;
	state->unit.z *= mul;
}


void DoColorLighting(ColorChannel *chan) {
	GXColor clr{0, 0, 0, 0};
	
	if (chan->enabled[0] == false) {
		if (chan->mat_src == GX_SRC_REG)
			AccumColor = chan->mat_clr;
		else
			AccumColor = VtxColor;
	}
	else {
		if (chan->amb_src == GX_SRC_REG)
			GXColor2FloatNoAlpha(&Illum, chan->amb_clr);
		else
			GXColor2FloatNoAlpha(&Illum, VtxColor);
		
		if (chan->mat_src == GX_SRC_REG)
			clr = chan->mat_clr;
		else if (chan->mat_src == GX_SRC_VTX)
			clr = VtxColor;
		else {
			ASSERTMSG(false, "Never here");
		}
		
		
		for(int i = 0; i < 8; i++) {
			byte light_id = UntranslateGXLightID((GXLightID)i);
			if (chan->light_mask & light_id)
				LightColorChannel(chan, &GXLightState[i]);
		}
		
		Illum.r = std::max(0.0f, std::min(1.0f, Illum.r));
		Illum.g = std::max(0.0f, std::min(1.0f, Illum.g));
		Illum.b = std::max(0.0f, std::min(1.0f, Illum.b));
		
		AccumColor.r = clr.r;
		AccumColor.g = clr.g;
		AccumColor.b = clr.b;
	}
}


void DoAlphaLighting(ColorChannel *clr) {
	float alpha_illum;
	uint alpha;
	
	if (clr->enabled[0] == false) {
		if (clr->mat_src == GX_SRC_REG)
			AccumColor.a = clr->mat_clr.a;
		else
			AccumColor.a = VtxColor.a;
	}
	else {
		if (clr->amb_src == GX_SRC_REG)
			alpha = clr->amb_clr.a;
		else
			alpha = VtxColor.a;
		
		Illum.a = (float)alpha / 255.0;
		
		if (clr->mat_src == GX_SRC_REG)
			alpha = clr->mat_clr.a;
		if (clr->mat_src == GX_SRC_VTX)
			alpha = VtxColor.a;
		
		for(int i = 0; i < 8; i++) {
			byte light_id = UntranslateGXLightID((GXLightID)i);
			if (clr->light_mask & light_id)
				LightAlphaChannel(clr, &GXLightState[i]);
		}
		
		Illum.a = std::max(0.0f, std::min(1.0f, Illum.a));
		AccumColor.a = alpha;
	}
}


void LightColorChannel(ColorChannel *chan, LightState *light) {
	float dot;
	float blue;
	float green;
	float red;
	float spot;
	float attn;
	
	
	if (chan->diff_fn == GX_DF_NONE) {
		dot = 1.0;
	}
	else if (chan->diff_fn == GX_DF_CLAMP) {
		dot = VECDotProduct(&FinalNormal, &light->unit);
		if (dot < 0.0)
			dot = 0.0;
	}
	else if (chan->diff_fn == GX_DF_SIGN) {
		dot = VECDotProduct(&FinalNormal, &light->unit);
	}
	
	
	if (chan->attn_fn == GX_AF_SPOT) {
		float d = VECDotProduct(&light->lt.halfangle, &light->unit);
		if (0.0 < d)
			d = 0.0;
		d = -d;
		
		spot =	d *			light->lt.spotlight_type.y +
				d * d *		light->lt.spotlight_type.z +
							light->lt.spotlight_type.x;
		
		if (spot < 0.0)
			spot = 0.0;
		
		attn =	light->lt.lt_dist_attn.y * light->el_pow +
				light->lt.lt_dist_attn.z * light->vec_len +
				light->lt.lt_dist_attn.x;
		
		if (attn == 0.0)
			attn = 1.0;
		else
			attn = spot / attn;
	} else if (chan->attn_fn == GX_AF_SPEC) {
		float d = VECDotProduct(&light->lt.normal, &FinalNormal);
		if (d > 0.0)
			d = VECDotProduct(&light->lt.halfangle, &FinalNormal);
		else
			d = 0.0;
		
		spot =	d *			light->lt.spotlight_type.y +
				d * d *		light->lt.spotlight_type.z +
							light->lt.spotlight_type.x;
							
		if (spot < 0.0)
			spot = 0.0;
		
		attn =	d *		light->lt.lt_dist_attn.y +
				d * d *	light->lt.lt_dist_attn.z +
						light->lt.lt_dist_attn.x;
		
		if (attn == 0.0)
			attn = 1.0;
		else
			attn = spot / attn;
	}
	else if (chan->attn_fn == GX_AF_NONE)
		attn = 1.0;
	else {
		attn = 0.0;
		ASSERTMSG(false, "Never here");
	}
	
	RGBAf clr;
	GXColor2Float(&clr, light->lt.color);
	
	red = attn * dot * clr.r;
	red = std::max(-1.0f, std::min(+1.0f, red));
	
	green = attn * dot * clr.g;
	green = std::max(-1.0f, std::min(+1.0f, green));
	
	blue = attn * dot * clr.b;
	blue = std::max(-1.0f, std::min(+1.0f, blue));
	
	Illum.r += red;
	Illum.g += green;
	Illum.b += blue;
}


void LightAlphaChannel(ColorChannel *chan, LightState *state) {
	float dot   = 0;
	float spot  = 0;
	float attn  = 0;
	float alpha = 0;
	RGBAf clr;
	
	if (chan->diff_fn == GX_DF_NONE)
		dot = 1.0;
	else if (chan->diff_fn == GX_DF_CLAMP) {
		dot = VECDotProduct(&FinalNormal, &state->unit);
		if (dot < 0.0)
			dot = 0.0;
	}
	else if (chan->diff_fn == GX_DF_SIGN) {
		dot = VECDotProduct(&FinalNormal, &state->unit);
	}
	
	if (chan->attn_fn == GX_AF_SPOT) {
		float d = VECDotProduct(&state->lt.halfangle, &state->unit);
		if (0.0 <= d)
			d = 0.0;
		
		d = -d;
		
		spot =	d *		state->lt.spotlight_type.y +
				d * d *	state->lt.spotlight_type.z +
						state->lt.spotlight_type.x;
		
		if (spot < 0.0)
			spot = 0.0;
		
		attn =			state->lt.lt_dist_attn.y * state->el_pow +
						state->lt.lt_dist_attn.z * state->vec_len +
						state->lt.lt_dist_attn.x;
						
		if (attn == 0.0)
			attn = 1.0;
		else
			attn = spot / attn;
	}
	else if (chan->attn_fn == GX_AF_SPEC) {
		float d = VECDotProduct(&state->lt.normal, &FinalNormal);
		if (d > 0.0)
			d = VECDotProduct(&state->lt.halfangle, &FinalNormal);
		else
			d = 0.0;
		
		spot =	d *		state->lt.spotlight_type.y +
				d * d *	state->lt.spotlight_type.z +
						state->lt.spotlight_type.x;
		
		if (spot < 0.0)
			spot = 0.0;
		
		attn =	d *		state->lt.lt_dist_attn.y +
				d * d *	state->lt.lt_dist_attn.z +
						state->lt.lt_dist_attn.x;
		
		if (attn == 0.0)
			attn = 1.0;
		else
			attn = spot / attn;
	}
	else if (chan->attn_fn == GX_AF_NONE)
		attn = 1.0;
	else {
		ASSERTMSG(false, "Never here");
	}
	
	GXColor2Float(&clr, state->lt.color);
	
	clr.a *= attn * dot;
	if (-1.0 <= clr.a) {
		alpha = clr.a;
		if (clr.a > 1.0)
			alpha = 1.0;
	}
	else
		alpha = -1.0;
	
	Illum.a += alpha;
}



void GXColor2Float(RGBAf *vec, GXColor clr) {
	vec->r = clr.r / 255.0;
	vec->g = clr.g / 255.0;
	vec->b = clr.b / 255.0;
	vec->a = clr.a / 255.0;
}

void GXColor2FloatNoAlpha(RGBAf *vec, GXColor clr) {
	vec->r = clr.r / 255.0;
	vec->g = clr.g / 255.0;
	vec->b = clr.b / 255.0;
}



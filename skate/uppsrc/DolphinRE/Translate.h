#pragma once




int TranslateGXAttr_VCD(GXAttr attr);
GXAttr UntranslateGXAttr_VCD(int vcd);
int UntranslateApiID(int i);
int TranslateGXAttr_VAT(GXAttr attr);
GXAttr UntranslateGXAttr_VAT(int vat);
int TranslateGXAttr_Array(GXAttr attr);
int TranslateGXAttr_TexMtxIndex(GXAttr attr);
GXAttr UntranslateGXAttr_TexMtxIndex(int i);
int TranslateGXVtxFmt(GXVtxFmt fmt);
int TranslateGXLightID(GXLightID i);
byte UntranslateGXLightID(GXLightID lid);
int TranslateGXChannelID(GXChannelID chan);
int TranslateGXTexMapID(GXTexMapID map);
int TranslateGXTexCoord(GXTexCoordID coord);
int TranslateGXTexFormat(GXTexFmt fmt);
uint TranslateGXTlutSize(uint lut);
uint TranslateGXTexCacheSize(GXTexCacheSize i);
int TranslateGXTevStageID(GXTevStageID i);
int TranslateGXTexGenSrc_TexCoord(GXTexGenSrc in);
int TranslateGXTexGenSrc_BumpTexCoord(GXTexGenSrc i);
const char* TranslateGXAttr_Name(GXAttr attr);
const char* TranslateGXAttrType_Name(GXAttrType type);
const char* TranslateGXApiID_Name(ApiID api);
const char* TranslateGXCompCnt_Name(GXAttr attr,GXCompCnt cnt);
const char* TranslateGXCompType_Name(GXChannelID chan,GXCompType type);
